#include "model.H"

int
main(int argc, char *argv[])
{
  if (argc != 7)
    {
      cout
          << "modelFileName blockSize textFileName textType activeFileName probFileName"
          << endl;
      cout
          << "textType: n:normal(text), l:list of ngram (words), id:list of ngram (ids), r: rescore type"
          << endl;
      return 0;
    }
  else
    {
      time_t start, end;
      char* modelFileName = argv[1];
      int blockSize = atoi(argv[2]);
      char* textFileName = argv[3];
      string textType = argv[4];

      if (textType != "n" && textType != "l" && textType != "id" && textType
          != "r")
        {
          cerr << "What is textType?" << endl;
          return 0;
        }
      char* activeFileName = argv[5];
      char* probFileName = argv[6];
      ioFile iofC;
      if (!iofC.check(modelFileName, 1))
        {
          return 1;
        }
      if (!iofC.check(textFileName, 1))
        {
          return 1;
        }
      if (iofC.check(activeFileName, 0))
        {
          cerr << "active file exists" << endl;
          return 1;
        }

      if (iofC.check(probFileName, 0))
        {
          cerr << "prob file exists" << endl;
          return 1;
        }

      Model* model;
      READMODEL(model, blockSize, modelFileName);
      if (model->name != MAXOVN && model->name != DWTMAXOVN)
        {
          cerr << "Model is not in max family" << endl;
          return 1;
        }
      model->baseNetwork->modules[0]->infoIof.takeWriteFile(activeFileName);
      model->dataSet->groupContext = 0;
      time(&start);
      realTensor probTensor;
      probTensor = model->computeProbability(textFileName, textType);
      if (textType != "r")
        {
          ioFile iof;
          iof.format = TEXT;
          iof.takeWriteFile(probFileName);
          for (int i = 0; i < probTensor.length; i++)
            {
              iof.writeReal(probTensor(i));
            }
        }
      else
        {
          ioFile iof;
          iof.format = BINARY;
          iof.takeWriteFile(probFileName);
          iof.writeRealArray(probTensor.data, probTensor.length);
        }
      time(&end);
      cout << "Finish after " << difftime(end, start) / 60 << " minutes"
          << endl;
      model->baseNetwork->modules[0]->infoIof.freeWriteFile();
      delete model;
    }
  return 0;
}

