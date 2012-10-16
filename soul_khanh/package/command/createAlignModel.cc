#include "model.H"

int
main(int argc, char *argv[])
{
  if (argc != 5)
    {
      cout
          << "model1FileName model2FileName  output21ModelFileName output12ModelFileName"
          << endl;
      return 0;
    }
  char* model1FileName = argv[1];
  char* model2FileName = argv[2];
  char* output21ModelFileName = argv[3];
  char* output12ModelFileName = argv[4];
  ioFile iof;
  if (!iof.check(model1FileName, 1))
    {
      return 1;
    }
  if (!iof.check(model2FileName, 1))
    {
      return 1;
    }

  if (iof.check(output12ModelFileName, 0))
    {
      cerr << "Prototype exists" << endl;
      return 1;
    }
  if (iof.check(output21ModelFileName, 0))
    {
      cerr << "Prototype exists" << endl;
      return 1;
    }

  Model* model1;
  READMODEL(model1, 0, model1FileName);
  Model* model2;
  READMODEL(model2, 0, model2FileName);
  LookupTable* tmpLkt;
  SoulVocab* tmpVoc;
  tmpLkt = model1->baseNetwork->lkt;
  model1->baseNetwork->lkt = model2->baseNetwork->lkt;
  model2->baseNetwork->lkt = tmpLkt;
  tmpVoc = model1->inputVoc;
  model1->inputVoc = model2->inputVoc;
  model2->inputVoc = tmpVoc;

  ioFile Oiof;
  Oiof.format = BINARY;
  realTensor subWeight;
  Module* bkLinear;

  Module* newLinear = new Linear(model1->dimensionSize,
      model1->hiddenLayerSizeArray(0), model1->blockSize, model1->otl);

  //12: 1 given 2
  subWeight.sub(model2->baseNetwork->modules[0]->weight,
      model2->dimensionSize * (model2->n - 2),
      model2->dimensionSize * (model2->n - 1) - 1, 0,
      model2->hiddenLayerSizeArray(0) - 1);

  newLinear->weight.copy(subWeight);

  Oiof.takeWriteFile(output12ModelFileName);
  bkLinear = model1->baseNetwork->modules[0];
  model1->baseNetwork->modules[0] = newLinear;
  model1->n = 2;
  model1->write(&Oiof);
  model1->baseNetwork->modules[0] = bkLinear;

  //21: 2 given 1
  subWeight.sub(model1->baseNetwork->modules[0]->weight,
      model1->dimensionSize * (model1->n - 2),
      model1->dimensionSize * (model1->n - 1) - 1, 0,
      model1->hiddenLayerSizeArray(0) - 1);

  newLinear->weight.copy(subWeight);

  Oiof.takeWriteFile(output21ModelFileName);

  bkLinear = model2->baseNetwork->modules[0];
  model2->baseNetwork->modules[0] = newLinear;
  model2->n = 2;
  model2->write(&Oiof);
  model2->baseNetwork->modules[0] = bkLinear;

  delete newLinear;
  delete model1;
  delete model2;
}

