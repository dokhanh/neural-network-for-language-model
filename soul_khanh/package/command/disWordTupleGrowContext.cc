#include "model.H"
/* Only with NgramModel and not with irovn
 Not true with recurrent case: Only increase the context, copy weights, not add zeros.
 */
int
growContextSizeZero(char* modelFileName, char* srcModelFileName, int newn,
    char* newInputVocFileName, char* outputModelFileName)
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

  Model* srcModel;
  READMODEL(srcModel, 0, srcModelFileName);

  int oldn = model->n;
  model->n = newn;
  model->BOS = newn - 1; //Hack
  LookupTable* newLkTable;
  Module* newLinear;
  Module* newNonLinear;
  model->otl = new outils();
  model->otl->sgenrand(time(NULL));
  if (!strcmp(newInputVocFileName, "xxx"))
    {
      newLkTable = new LookupTable(model->baseNetwork->lkt->multi,
          model->inputVoc->wordNumber, model->dimensionSize, newn - 1,
          model->blockSize, 1, model->otl);
      newLkTable->weight.copy(model->baseNetwork->lkt->weight);
    }
  else
    {
      SoulVocab* newInputVoc = new SoulVocab(newInputVocFileName);
      int initClass = 1;
      if (model->name == CN)
        {
          initClass = 0;
        }
      newLkTable = new LookupTable(model->baseNetwork->lkt->multi,
          newInputVoc->wordNumber, model->dimensionSize, newn - 1,
          model->blockSize, initClass, model->otl);
      int i, oldIndex, newIndex;
      realTensor selectOld;
      realTensor selectNew;
      VocNode* run;
      for (i = 0; i < newInputVoc->tableSize; i++)
        {
          run = newInputVoc->table[i];
          while (run->next != NULL)
            {
              run = run->next;
              oldIndex = model->inputVoc->index(run->word);
              newIndex = run->index;
              if (oldIndex != ID_UNK)
                {
                  selectOld.select(model->baseNetwork->lkt->weight, 1, oldIndex);
                  selectNew.select(newLkTable->weight, 1, newIndex);
                  selectNew.copy(selectOld);
                }
            }
        }
      for (i = 0; i < srcModel->inputVoc->tableSize; i++)
        {
          run = srcModel->inputVoc->table[i];
          while (run->next != NULL)
            {
              run = run->next;
              oldIndex = run->index;
              newIndex = newInputVoc->index("src." + run->word);
              if (newIndex != ID_UNK)
                {
                  selectOld.select(srcModel->baseNetwork->lkt->weight, 1,
                      oldIndex);
                  selectNew.select(newLkTable->weight, 1, newIndex);
                  selectNew.copy(selectOld);
                }
            }
        }

      delete model->inputVoc;
      model->inputVoc = newInputVoc;
    }
  newLinear = new Linear((newn - 1) * model->dimensionSize,
      model->hiddenLayerSizeArray(0), model->blockSize, model->otl);
  if (model->nonLinearType == TANH)
    {
      newNonLinear = new Tanh(model->hiddenLayerSizeArray(0), model->blockSize); // non linear
    }
  else if (model->nonLinearType == SIGM)
    {
      newNonLinear = new Sigmoid(model->hiddenLayerSizeArray(0),
          model->blockSize); // non linear
    }
  LookupTable* oldLkTable;
  Module* oldLinear;
  Module* oldNonLinear;
  oldLkTable = model->baseNetwork->lkt;
  model->baseNetwork->lkt = newLkTable;
  oldLinear = model->baseNetwork->modules[0];
  if (model->nonLinearType == TANH)
    {
      oldNonLinear = model->baseNetwork->modules[1];
    }
  else if (model->nonLinearType == SIGM)
    {
      oldNonLinear = model->baseNetwork->modules[1];
    }
  realTensor subWeight;
  realTensor subOldWeight;
  int i, j, k;
  k = 0;
  subWeight.sub(newLinear->weight, 0,
      model->dimensionSize * (model->n - oldn) - 1, 0,
      model->hiddenLayerSize - 1);
  subWeight = 0;
  subWeight.sub(newLinear->weight,
      model->dimensionSize * (model->n - oldn),
      model->dimensionSize * (model->n - 1) - 1, 0,
      model->hiddenLayerSize - 1);
  subWeight.copy(oldLinear->weight);
  newLinear->bias.copy(oldLinear->bias);

  model->baseNetwork->modules[0] = newLinear;
  if (model->nonLinearType == TANH)
    {
      model->baseNetwork->modules[1] = newNonLinear;
    }
  else if (model->nonLinearType == SIGM)
    {
      model->baseNetwork->modules[1] = newNonLinear;
    }

  ioFile oIof;
  oIof.takeWriteFile(outputModelFileName);
  model->write(&oIof);

  delete oldLkTable;
  delete oldLinear;
  if (model->nonLinearType == TANH)
    {
      delete oldNonLinear;
    }
  else if (model->nonLinearType == SIGM)
    {
      delete oldNonLinear;
    }

  delete model;
  return 0;
}

int
main(int argc, char *argv[])
{
  if (argc != 6)
    {
      cout
          << "modelFileName srcModelFileName newn newInputVocFileName outputModelFileName"
          << endl;
      return 0;
    }
  char* modelFileName = argv[1];
  char* srcModelFileName = argv[2];
  int newn = atoi(argv[3]);
  char* newInputVocFileName = argv[4];
  char* outputModelFileName = argv[5];
  return growContextSizeZero(modelFileName, srcModelFileName, newn,
      newInputVocFileName, outputModelFileName);
}

