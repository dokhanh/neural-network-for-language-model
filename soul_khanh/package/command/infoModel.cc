#include "model.H"
int
main(int argc, char *argv[])
{
  if (argc != 2)
    {
      cout << "modelFileName" << endl;
      return 0;
    }
  char* modelFileName = argv[1];
  ioFile iof;
  if (!iof.check(modelFileName, 1))
    {
      return 1;
    }

  Model* model;
  READMODEL(model, 0, modelFileName);

  cout << "Model: " << model->name << endl;
  cout << "ngramType: " << model->ngramType << endl;
  cout << "voc size: " << model->inputVoc->wordNumber << ", "
      << model->outputVoc->wordNumber << endl;
  cout << "map: " << model->mapIUnk << ", " << model->mapOUnk << endl;
  cout << "n (BPTT + 1): " << model->n << endl;
  cout << model->nonLinearType << endl;

  cout << "LookupTable dimension: " << model->baseNetwork->lkt->dimensionSize
      << endl;
  cout << "hiddenLayerSize, " << model->hiddenLayerSizeArray.length
      << " layers: ";
  for (int i = 0; i < model->hiddenLayerSizeArray.length; i++)
    {
      cout << model->hiddenLayerSizeArray(i) << " ";
    }
  cout << endl;
  /*
   model->outputNetwork[0]->weight.info();
   model->outputNetwork[0]->bias.info();
   int id = model->inputVoc->index("NULL");
   int i;
   for (i = 0; i < model->dimensionSize; i ++)
   {
   cout << model->baseNetwork->lkt->weight(i, id) << " ";
   }
   cout << endl;
   id = model->outputVoc->index("NULL");
   for (i = 0; i < model->codeWord.size[1]; i++)
   {
   cout << model->codeWord(id, i) << " ";
   }
   cout << endl;

   id = model->outputVoc->index("src.NULL");
   for (i = 0; i < model->dimensionSize; i ++)
   {
   cout << model->baseNetwork->lkt->weight(i, id) << " ";
   }
   cout << endl;
   */
  delete model;
}

