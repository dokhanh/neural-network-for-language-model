#include "model.H"

int
main(int argc, char *argv[])
{
  if (argc != 5)
    {
      cout
          << "modelSrcFileName modelTrgFileName name prefixOutputModelFileName"
          << endl;
      cout << "name: dwtovn, dwtmaxovn" << endl;
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
  char* prefixOutputModelFileName = argv[4];
  ioFile iof;
  if (!iof.check(modelSrcFileName, 1))
    {
      return 1;
    }
  if (!iof.check(modelTrgFileName, 1))
    {
      return 1;
    }
  char fileNameTrgSrc[260];

  strcpy(fileNameTrgSrc, prefixOutputModelFileName);
  strcat(fileNameTrgSrc, "TrgSrc");
  if (iof.check(fileNameTrgSrc, 0))
    {
      cerr << "Prototype exists" << endl;
      return 1;
    }
  char fileNameSrcTrg[260];
  strcpy(fileNameSrcTrg, prefixOutputModelFileName);
  strcat(fileNameSrcTrg, "SrcTrg");
  if (iof.check(fileNameSrcTrg, 0))
    {
      cerr << "Prototype exists" << endl;
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

  int n = modelSrc->n;
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

  // SrcTrg, Src

  newLinear->weight = 0;
  newLinear->bias.copy(modelSrc->baseNetwork->modules[0]->bias);

  subWeight.sub(newLinear->weight, modelTrg->dimensionSize,
      modelTrg->dimensionSize * n - 1, 0, modelTrg->hiddenLayerSizeArray(0) - 1);

  subWeight.copy(modelTrg->baseNetwork->modules[0]->weight);

  subWeight.sub(newLinear->weight, modelTrg->dimensionSize * n,
      modelTrg->dimensionSize * (nm - 1) - 1, 0,
      modelTrg->hiddenLayerSizeArray(0) - 1);

  subWeight.copy(modelSrc->baseNetwork->modules[0]->weight);

  modelSrc->name = name;
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

  Oiof.takeWriteFile(fileNameSrcTrg);
  modelSrc->ngramType = 2;
  modelSrc->write(&Oiof);

  Oiof.takeWriteFile(fileNameSrc);
  modelSrc->ngramType = 3;
  modelSrc->write(&Oiof);

  modelSrc->baseNetwork->lkt = bkLkt;
  modelSrc->inputVoc = bkVoc;
  modelSrc->baseNetwork->modules[0] = bkLinear;
  modelSrc->outputVoc = bkOutVoc;

  // TrgSrc, Trg

  newLinear->weight = 0;
  newLinear->bias.copy(modelTrg->baseNetwork->modules[0]->bias);

  subWeight.sub(newLinear->weight, modelSrc->dimensionSize * n,
      modelSrc->dimensionSize * (nm - 1) - 1, 0,
      modelSrc->hiddenLayerSizeArray(0) - 1);

  subWeight.copy(modelTrg->baseNetwork->modules[0]->weight);
  modelTrg->name = name;

  bkLkt = modelTrg->baseNetwork->lkt;
  modelTrg->baseNetwork->lkt = newLkt;
  bkVoc = modelTrg->inputVoc;
  modelTrg->inputVoc = newVoc;
  bkLinear = modelTrg->baseNetwork->modules[0];
  modelTrg->baseNetwork->modules[0] = newLinear;

  Oiof.takeWriteFile(fileNameTrgSrc);
  modelTrg->ngramType = 0;
  modelTrg->write(&Oiof);

  Oiof.takeWriteFile(fileNameTrg);
  modelTrg->ngramType = 1;
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

