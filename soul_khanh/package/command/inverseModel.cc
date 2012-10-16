#include "model.H"
/* Only with NgramModel and not with irovn
 Not true with recurrent case: Only increase the context, copy weights, not add zeros.
 */
int
inverseModel(char* modelFileName, int ngramType, char* outputModelFileName)
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

  int oldNgramType = model->ngramType;
  model->ngramType = ngramType;
  if (oldNgramType == ngramType)
    {
      cout << "What new?" << endl;
      return 1;
    }
  else
    {
      /*
       if (model->ngramType + oldNgramType == 1) //Inverse weights
       {
       realTensor weight = model->baseNetwork->modules[0]->weight;
       realTensor buffWeight;
       realTensor subWeight;
       realTensor subBuffWeight;
       int m = model->dimensionSize;
       int H = model->hiddenLayerSizeArray(0);
       int n = model->n;
       buffWeight.copy(weight);
       for (int i = 0; i < n - 1; i++)
       {
       subWeight.sub(weight, i * m, (i + 1) * m - 1, 0, H - 1);
       subBuffWeight.sub(buffWeight, (n - 2 - i) * m,
       (n - 1 - i) * m - 1, 0, H - 1);
       subWeight.copy(subBuffWeight);
       }
       }
       */
      ioFile oIof;
      oIof.takeWriteFile(outputModelFileName);
      model->write(&oIof);

    }
  delete model;
  return 0;
}

int
main(int argc, char *argv[])
{
  if (argc != 4)
    {
      cout << "modelFileName ngramType outputModelFileName" << endl;
      return 0;
    }
  char* modelFileName = argv[1];
  char* ngramTypeStr = argv[2];
  int ngramType;
  if (!strcmp(ngramTypeStr, "n"))
    {
      ngramType = 0;
    }
  else if (!strcmp(ngramTypeStr, "i"))
    {
      ngramType = 1;
    }
  else if (!strcmp(ngramTypeStr, "m"))
    {
      ngramType = 2;
    }
  char* outputModelFileName = argv[3];
  return inverseModel(modelFileName, ngramType, outputModelFileName);
}

