#include "model.H"
int
main(int argc, char *argv[])
{
  if (argc != 4)
    {
      cout << "modelFileName lookupTableFileName weightFileName" << endl;
      return 0;
    }
  char* modelFileName = argv[1];
  char* lookupTableFileName = argv[2];
  char* weightFileName = argv[3];
  ioFile iof;
  if (!iof.check(modelFileName, 1))
    {
      return 1;
    }

  if (iof.check(lookupTableFileName, 0))
    {
      cerr << "lookupTable File exists" << endl;
      return 1;
    }

  if (iof.check(weightFileName, 0))
    {
      cerr << "weight File exists" << endl;
      return 1;
    }

  Model* model;
  READMODEL(model, 0, modelFileName);
  int i, j;

  model->baseNetwork->lkt->weight;
  iof.takeWriteFile(lookupTableFileName);
  model->baseNetwork->lkt->weight.info();
  for (i = 0; i < model->inputVoc->wordNumber; i++)
    {
      for (j = 0; j < model->dimensionSize; j++)
        {
          iof.writeReal(model->baseNetwork->lkt->weight(j, i));
        }
    }
  iof.takeWriteFile(weightFileName);
  model->baseNetwork->modules[0]->weight.info();
  for (i = 0; i < model->baseNetwork->modules[0]->weight.size[0]; i++)
    {
      for (j = 0; j < model->baseNetwork->modules[0]->weight.size[1]; j++)
        {
          iof.writeReal(model->baseNetwork->modules[0]->weight(i, j));
        }
    }
  delete model;
}

