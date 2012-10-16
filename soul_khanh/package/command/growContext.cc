#include "model.H"
/* Only with NgramModel and not with irovn
 Not true with recurrent case: Only increase the context, copy weights, not add zeros.
 */
int
growContextSizeZero(char* modelFileName, int newn, char* newInputVocFileName,
    char* outputModelFileName)
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
      newLkTable
          = new LookupTable(model->baseNetwork->lkt->multi, model->inputVoc->wordNumber,
              model->dimensionSize, newn - 1, model->blockSize, 1,
              model->otl);
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
      newLkTable = new LookupTable(model->baseNetwork->lkt->multi, newInputVoc->wordNumber,
          model->dimensionSize, newn - 1, model->blockSize, initClass,
          model->otl);
      int i, oldIndex;
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
              if (oldIndex != ID_UNK)
                {
                  selectOld.select(model->baseNetwork->lkt->weight, 1, oldIndex);
                  selectNew.select(newLkTable->weight, 1, run->index);
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
  int hiddenLayerSize = model->hiddenLayerSizeArray(0);
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
  if (model->name == DTOVN)
    {
      if (oldn < model->n)
        {
          for (k = 0; k < oldn / 2; k++)
            {
              subOldWeight.sub(oldLinear->weight,
                  model->dimensionSize * k,
                  model->dimensionSize * (k + 1) - 1, 0,
                  hiddenLayerSize - 1);
              subWeight.sub(
                  newLinear->weight,
                  model->dimensionSize * (k + (model->n - oldn) / 2),
                  model->dimensionSize * (k + (model->n - oldn) / 2 + 1)
                      - 1, 0, hiddenLayerSize - 1);
              subWeight.copy(subOldWeight);

              subOldWeight.sub(oldLinear->weight,
                  model->dimensionSize * (k + oldn / 2),
                  model->dimensionSize * (k + oldn / 2 + 1) - 1, 0,
                  hiddenLayerSize - 1);
              subWeight.sub(
                  newLinear->weight,
                  model->dimensionSize * (k + model->n - oldn / 2),
                  model->dimensionSize * (k + model->n - oldn / 2 + 1)
                      - 1, 0, hiddenLayerSize - 1);
              subWeight.copy(subOldWeight);
            }
        }
      else if (oldn > model->n)
        {
          for (k = 0; k < model->n / 2; k++)
            {
              subWeight.sub(newLinear->weight, model->dimensionSize * k,
                  model->dimensionSize * (k + 1) - 1, 0,
                  hiddenLayerSize - 1);
              subOldWeight.sub(
                  oldLinear->weight,
                  model->dimensionSize * (k + (oldn - model->n) / 2),
                  model->dimensionSize * (k + (oldn - model->n) / 2 + 1)
                      - 1, 0, hiddenLayerSize - 1);
              subWeight.copy(subOldWeight);

              subWeight.sub(newLinear->weight,
                  model->dimensionSize * (k + model->n / 2),
                  model->dimensionSize * (k + model->n / 2 + 1) - 1, 0,
                  hiddenLayerSize - 1);
              subOldWeight.sub(
                  oldLinear->weight,
                  model->dimensionSize * (k + oldn - model->n / 2),
                  model->dimensionSize * (k + oldn - model->n / 2 + 1)
                      - 1, 0, hiddenLayerSize - 1);
              subWeight.copy(subOldWeight);
            }
        }
    }
  else
    {
      if (oldn < model->n)
        {
          subWeight.sub(newLinear->weight, 0,
              model->dimensionSize * (model->n - oldn) - 1, 0,
              hiddenLayerSize - 1);
          //subWeight = 0;
          subWeight.sub(newLinear->weight,
              model->dimensionSize * (model->n - oldn),
              model->dimensionSize * (model->n - 1) - 1, 0,
              hiddenLayerSize - 1);
          subWeight.copy(oldLinear->weight);
        }
      else if (oldn > model->n)
        {
          subWeight.sub(oldLinear->weight,
              model->dimensionSize * (oldn - model->n),
              model->dimensionSize * (oldn - 1) - 1, 0,
              hiddenLayerSize - 1);
          newLinear->weight.copy(subWeight);
        }
    }
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
  if (argc != 5)
    {
      cout << "modelFileName newn newInputVocFileName outputModelFileName"
          << endl;
      return 0;
    }
  char* modelFileName = argv[1];
  int newn = atoi(argv[2]);
  char* newInputVocFileName = argv[3];
  char* outputModelFileName = argv[4];
  return growContextSizeZero(modelFileName, newn, newInputVocFileName,
      outputModelFileName);
}

