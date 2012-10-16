#include "model.H"

int
main(int argc, char *argv[])
{
  if (argc != 5)
    {
      cout
          << "modelFileName codeWordFileName outputNetworkSizeFileName outputModelFileName"
          << endl;
      return 0;
    }
  char* modelFileName = argv[1];
  char* codeWordFileName = argv[2];
  char* outputNetworkSizeFileName = argv[3];
  char* outputModelFileName = argv[4];
  ioFile iofC;
  if (!iofC.check(modelFileName, 1))
    {
      return 1;
    }
  if (!iofC.check(codeWordFileName, 1))
    {
      return 1;
    }
  if (!iofC.check(outputNetworkSizeFileName, 1))
    {
      return 1;
    }

  if (iofC.check(outputModelFileName, 0))
    {
      cerr << "Prototype exists" << endl;
      return 1;
    }

  Model* model;
  int i;
  READMODEL(model, 0, modelFileName);

  ioFile readIof;
  readIof.takeReadFile(codeWordFileName);
  intTensor codeWord;
  codeWord.read(&readIof);
  readIof.takeReadFile(outputNetworkSizeFileName);
  intTensor outputNetworkSize;
  outputNetworkSize.read(&readIof);
  realTensor newWeight;

  newWeight.resize(model->baseNetwork->lkt->weight.size[0],
      outputNetworkSize(0));
  newWeight = 0;
  int newId;
  newId = 0;
  SoulVocab* newVoc = new SoulVocab();
  SoulVocab* bkVoc;
  VocNode* run;

  for (i = 0; i < model->inputVoc->tableSize; i++)
    {
      run = model->inputVoc->table[i];
      while (run->next != NULL)
        {
          run = run->next;
          newId = codeWord(run->index, 1);
          newVoc->add(run->word, newId);
        }
    }
  realTensor count(outputNetworkSize(0), 1);
  count = 0;
  realTensor selectO;
  realTensor selectN;
  for (i = 0; i < model->inputVoc->wordNumber; i++)
    {
      newId = codeWord(i, 1);
      selectO.select(model->baseNetwork->lkt->weight, 1, i);
      selectN.select(newWeight, 1, newId);
      // y = 1.0 * x + y
      selectN.axpy(selectO, 1.0);
      count(newId)++;
    }

  for (i = 0; i < outputNetworkSize(0); i++)
    {
      selectN.select(newWeight, 1, i);
      selectN.scal(1 / count(i));
    }

  delete[] model->baseNetwork->lkt->weight.data;
  model->baseNetwork->lkt->weight.data = newWeight.data;
  newWeight.haveMemory = 0;
  ioFile iof;
  iof.takeWriteFile(outputModelFileName);
  bkVoc = model->inputVoc;
  model->inputVoc = newVoc;
  model->write(&iof);
  model->inputVoc = bkVoc;

  delete newVoc;
  delete model;
}

