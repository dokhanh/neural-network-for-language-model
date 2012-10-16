#include "model.H"

int
main(int argc, char *argv[])
{
  if (argc != 6)
    {
      cout
          << "modelSrcFileName modelTrgFileName name n prefixOutputModelFileName"
          << endl;
      cout << "name: dwtovn, dwtmaxovn" << endl;
      cout << "n: new gram order of models, must be impair" << endl;
      return 0;
    }
  char* modelSrcFileName = argv[1];
  char* modelTrgFileName = argv[2];
  string name = argv[3];
  if (name != DWTOVN && name != DWTMAXOVN)
    {
      cerr << "Unknown model name" << endl;
      return 1;
    }
  int n = atoi(argv[4]);
  if ((n % 2) != 1)
    {
      cerr << "n must be impair" << endl;
      return 1;
    }
  char* prefixOutputModelFileName = argv[5];
  ioFile iof;
  if (!iof.check(modelSrcFileName, 1))
    {
      return 1;
    }
  if (!iof.check(modelTrgFileName, 1))
    {
      return 1;
    }
  char fileNameTrg[260];
  strcpy(fileNameTrg, prefixOutputModelFileName);
  strcat(fileNameTrg, "Trg");
  if (iof.check(fileNameTrg, 0))
    {
      cerr << "Prototype exists" << endl;
      return 1;
    }
  char fileNameSrc[260];
  strcpy(fileNameSrc, prefixOutputModelFileName);
  strcat(fileNameSrc, "Src");
  if (iof.check(fileNameSrc, 0))
    {
      cerr << "Prototype exists" << endl;
      return 1;
    }

  Model* modelSrc;
  READMODEL(modelSrc, 0, modelSrcFileName);
  Model* modelTrg;
  READMODEL(modelTrg, 0, modelTrgFileName);

  cout << "output order:" << n << " , source order:" << modelSrc->n
      << " , target order:" << modelTrg->n << endl;
  if (modelSrc->inputVoc->index("NULL") == ID_UNK)
    {
      cerr << "modelSrc inputVoc does not have NULL" << endl;
      return 1;
    }
  if (modelSrc->outputVoc->index("NULL") == ID_UNK)
    {
      cerr << "modelSrc outputVoc does not have NULL" << endl;
      return 1;
    }

  if (modelTrg->inputVoc->index("NULL") == ID_UNK)
    {
      cerr << "modelTrg inputVoc does not have NULL" << endl;
      return 1;
    }
  if (modelTrg->outputVoc->index("NULL") == ID_UNK)
    {
      cerr << "modelTrg outputVoc does not have NULL" << endl;
      return 1;
    }

  LookupTable* newLkt;
  SoulVocab* newVoc = new SoulVocab(modelTrg->inputVoc); //Copy target vocab, need to have NULL modelTrg->inputVoc
  SoulVocab* newSourceOutVoc = new SoulVocab(); //Copy target vocab, need to have NULL modelTrg->inputVoc

  VocNode* run;
  int i;
  int offset;
  offset = modelTrg->inputVoc->wordNumber;
  for (i = 0; i < modelSrc->inputVoc->tableSize; i++)
    {
      run = modelSrc->inputVoc->table[i];
      while (run->next != NULL)
        {
          run = run->next;
          newVoc->add(PREFIX_SOURCE + run->word, offset + run->index);
          if (run->word != SS && run->word != ES && run->word != UNK)
            {
              newSourceOutVoc->add(PREFIX_SOURCE + run->word, run->index);
            }
          else
            {
              newSourceOutVoc->add(run->word, run->index);
            }
        }
    }

  int nm = n * 2;

  newLkt = new LookupTable(0, newVoc->wordNumber, modelSrc->dimensionSize,
      nm - 1, modelSrc->blockSize, 0, modelSrc->otl);
  realTensor subWeight;

  subWeight.sub(newLkt->weight, 0, modelTrg->dimensionSize - 1, 0,
      modelTrg->inputVoc->wordNumber - 1);
  subWeight.copy(modelTrg->baseNetwork->lkt->weight);

  subWeight.sub(newLkt->weight, 0, modelSrc->dimensionSize - 1,
      modelTrg->inputVoc->wordNumber, newVoc->wordNumber - 1);
  subWeight.copy(modelSrc->baseNetwork->lkt->weight);

  Module* newLinear;
  if (name == DWTOVN)
    {
      newLinear
          = new Linear(modelSrc->dimensionSize * (nm - 1),
              modelSrc->hiddenLayerSizeArray(0), modelSrc->blockSize,
              modelSrc->otl);
    }
  else if (name == DWTMAXOVN)
    {
      newLinear
          = new MaxLinear(nm - 1, modelSrc->dimensionSize,
              modelSrc->hiddenLayerSizeArray(0), modelSrc->blockSize,
              modelSrc->otl);
    }
  ioFile Oiof;
  // Src

  modelSrc->name = name;
  modelSrc->n = n;
  LookupTable* bkLkt;
  SoulVocab* bkVoc;
  SoulVocab* bkOutVoc;
  Module* bkLinear;
  bkLkt = modelSrc->baseNetwork->lkt;
  modelSrc->baseNetwork->lkt = newLkt;
  bkVoc = modelSrc->inputVoc;
  modelSrc->inputVoc = newVoc;
  bkLinear = modelSrc->baseNetwork->modules[0];
  modelSrc->baseNetwork->modules[0] = newLinear;

  bkOutVoc = modelSrc->outputVoc;
  modelSrc->outputVoc = newSourceOutVoc;

  Oiof.takeWriteFile(fileNameSrc);
  modelSrc->ngramType = 5;
  modelSrc->write(&Oiof);

  modelSrc->baseNetwork->lkt = bkLkt;
  modelSrc->inputVoc = bkVoc;
  modelSrc->baseNetwork->modules[0] = bkLinear;
  modelSrc->outputVoc = bkOutVoc;

  // Trg

  modelTrg->name = name;
  modelTrg->n = n;

  bkLkt = modelTrg->baseNetwork->lkt;
  modelTrg->baseNetwork->lkt = newLkt;
  bkVoc = modelTrg->inputVoc;
  modelTrg->inputVoc = newVoc;
  bkLinear = modelTrg->baseNetwork->modules[0];
  modelTrg->baseNetwork->modules[0] = newLinear;

  Oiof.takeWriteFile(fileNameTrg);
  modelTrg->ngramType = 4;
  modelTrg->write(&Oiof);

  modelTrg->baseNetwork->lkt = bkLkt;
  modelTrg->inputVoc = bkVoc;
  modelTrg->baseNetwork->modules[0] = bkLinear;

  delete newLkt;
  delete newVoc;
  delete newLinear;
  delete newSourceOutVoc;
  delete modelSrc;
  delete modelTrg;

}

