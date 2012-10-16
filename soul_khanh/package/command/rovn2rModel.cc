#include "model.H"
int
rovn2rModel(char* modelFileName, int n, int cont, char* outputModelFileName)
{
  ioFile iof;
  if (!iof.check(modelFileName, 1))
    {
      return 1;
    }
  if (iof.check(outputModelFileName, 0))
    {
      cerr << "Prototype exists" << endl;
      return 1;
    }
  Model* model;
  READMODEL(model, 0, modelFileName);
  if (model->name != ROVN)
    {
      cerr << "Model is not rovn, don't know what to do" << endl;
      delete model;
      return 1;
    }
  else if (model->nonLinearType == LINEAR)
    {
      cerr << "Model is linear, don't know what to do" << endl;
      delete model;
      return 1;
    }

  Module* newRrl = new RRLinear(model->dimensionSize, model->blockSize,
      model->n, model->nonLinearType, 1, model->otl);
  Module* oldRl = model->baseNetwork->modules[0];
  newRrl->weight.copy(oldRl->weight);
  newRrl->vectorInput.copy(oldRl->vectorInput);

  Model* outModel = new RecurrentModel();
  outModel->recurrent = 1;
  if (cont)
    {
      outModel->name = COVR;
    }
  else
    {
      outModel->name = OVR;
    }
  outModel->inputVoc = model->inputVoc;
  outModel->outputVoc = model->outputVoc;
  outModel->mapIUnk = 1;
  outModel->mapOUnk = 1;
  outModel->blockSize = model->blockSize;
  outModel->n = n;
  outModel->dimensionSize = model->dimensionSize;
  outModel->nonLinearType = model->nonLinearType;
  outModel->hiddenLayerSizeArray = model->hiddenLayerSizeArray;

  outModel->codeWord = model->codeWord;
  outModel->outputNetworkSize = model->outputNetworkSize;
  outModel->hiddenLayerSize = model->hiddenLayerSize;
  outModel->hiddenNumber = model->hiddenNumber;
  outModel->maxCodeWordLength = model->maxCodeWordLength;
  outModel->outputNetworkNumber = model->outputNetworkNumber;
  outModel->outputNetwork = model->outputNetwork;
  outModel->baseNetwork = new Sequential(model->baseNetwork->size - 1);
  outModel->baseNetwork->lkt = model->baseNetwork->lkt;
  outModel->baseNetwork->add(newRrl);
  //ovr don't have first nonlinear as modules[1] (created in RRLinear)
  for (int i = 1; i < model->baseNetwork->size - 1; i++)
    {
      outModel->baseNetwork->add(model->baseNetwork->modules[i + 1]);
    }
  ioFile mIof;
  mIof.takeWriteFile(outputModelFileName);
  outModel->write(&mIof);

  delete newRrl;
  delete model;
  return 0;
}

int
main(int argc, char *argv[])
{
  if (argc != 5)
    {
      cout << "modelFileName n cont outputModelFileName" << endl;
      return 0;
    }
  char* modelFileName = argv[1];
  int n = atoi(argv[2]);
  int cont = atoi(argv[3]);
  char* outputModelFileName = argv[4];
  return rovn2rModel(modelFileName, n, cont, outputModelFileName);
}

