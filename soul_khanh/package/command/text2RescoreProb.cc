#include "model.H"

int
main(int argc, char *argv[])
{
  if (argc != 6)
    {
      cout << "modelFileName blockSize incrUnk textFileName outputFileName"
          << endl;
      return 0;
    }
  else
    {
      time_t start, end;
      char* modelFileName = argv[1];
      int blockSize = atoi(argv[2]);
      REAL incrUnk = pow(10, atof(argv[3]));
      char* textFileName = argv[4];
      char* outputFileName = argv[5];
      ioFile iofC;
      if (!iofC.check(modelFileName, 1))
        {
          return 1;
        }
      if (!iofC.check(textFileName, 1))
        {
          return 1;
        }
      if (iofC.check(outputFileName, 0))
        {
          cerr << "prob file exists" << endl;
          return 1;
        }

      Model* model;
      READMODEL(model, blockSize, modelFileName);
      model->incrUnk = incrUnk;

      time(&start);
      ioFile iof;
      iof.format = TEXT;
      iof.takeReadFile((char*) textFileName);
      ioFile iofO;
      iofO.format = TEXT;
      iofO.takeWriteFile(outputFileName);
      int readLineNumber = 0;

      char * maxNgramNumberEnv;
      int maxNgramNumber = MODEL_NGRAM_NUMBER;
      maxNgramNumberEnv = getenv("MODEL_NGRAM_NUMBER");
      if (maxNgramNumberEnv != NULL)
        {
          maxNgramNumber = atoi(maxNgramNumberEnv);
        }

      while (!iof.getEOF())
        {
          model->dataSet->addRescoreLine(&iof);
          readLineNumber++;
#if PRINT_DEBUG
          if (readLineNumber % NLINEPRINT == 0 && readLineNumber != 0)
            {
              cout << readLineNumber << " ... " << flush;
            }
#endif
          if (model->dataSet->ngramNumber > maxNgramNumber
              - MAX_WORD_PER_SENTENCE)
            {
              cout << "Compute " << model->dataSet->ngramNumber << " ngrams"
                  << endl;
              model->dataSet->createTensor();
              model->forwardProbability(model->dataSet->dataTensor,
                  model->dataSet->probTensor);
              for (int i = 0; i < model->dataSet->probTensor.length; i++)
                {
                  iofO.writeReal(model->dataSet->probTensor(i));
                }
              model->dataSet->reset();
            }
        }
      if (model->dataSet->ngramNumber != 0)
        {
          cout << "Compute with " << model->dataSet->ngramNumber << " ngrams"
              << endl;
          model->dataSet->createTensor();
          model->forwardProbability(model->dataSet->dataTensor,
              model->dataSet->probTensor);
          for (int i = 0; i < model->dataSet->probTensor.length; i++)
            {
              iofO.writeReal(model->dataSet->probTensor(i));
            }
        }

#if PRINT_DEBUG
      cout << endl;
#endif
      time(&end);
      cout << "Finish after " << difftime(end, start) / 60 << " minutes"
          << endl;
      delete model;
    }
  return 0;
}

