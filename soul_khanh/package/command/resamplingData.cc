#include "model.H"
#include "time.h"
#include<algorithm>
#include <iterator>
#include "ioFile.H"
int
main(int argc, char *argv[])
{
  if (argc != 10)
    {
      cout
          << "dataDesFileName inputVocFileName outputVocFileName n mapIUnk mapOUnk prefixOutputFileName minEpochs maxEpochs"
          << endl;
      return 0;
    }
  else
    {
      srand48(time(NULL));
      srand(time(NULL));
      int type = TYPE_DATA;
      char* dataDesFileName = argv[1];
      char* inputVocFileName = argv[2];
      char* outputVocFileName = argv[3];
      int n = atoi(argv[4]);
      int mapIUnk = atoi(argv[5]);
      int mapOUnk = atoi(argv[6]);
      int cont;
      int blockSize;
      int BOS = n - 1;
      int times = 10;
      char* prefixOutputFileName = argv[7];
      int minEpochs = atoi(argv[8]);
      int maxEpochs = atoi(argv[9]);
      char outputFileName[260];
      ioFile iofC;
      if (!iofC.check(dataDesFileName, 1))
        {
          return 1;
        }
      if (!iofC.check(inputVocFileName, 1))
        {
          return 1;
        }
      if (!iofC.check(outputVocFileName, 1))
        {
          return 1;
        }
      SoulVocab* inputVoc = new SoulVocab(inputVocFileName);
      SoulVocab* outputVoc = new SoulVocab(outputVocFileName);
      NgramDataSet* dataSet;
      char * maxNgramNumberEnv;

      // get the value from the environment
      maxNgramNumberEnv = getenv("RESAMPLING_NGRAM_NUMBER");
      if (maxNgramNumberEnv != NULL)
        {
    	  // type = 0:
          dataSet = new NgramDataSet(type, n, BOS, inputVoc, outputVoc,
              mapIUnk, mapOUnk, atoi(maxNgramNumberEnv));
        }
      else
        {
          dataSet = new NgramDataSet(type, n, BOS, inputVoc, outputVoc,
              mapIUnk, mapOUnk, RESAMPLING_NGRAM_NUMBER);
        }
      if (dataSet->data == NULL)
        {
          return 1;
        }
      int resampling = 1;
      ioFile iofO;
      for (int iter = minEpochs; iter <= maxEpochs; iter++)
        {
          strcpy(outputFileName, prefixOutputFileName);
          ostringstream iConvert;
          iConvert << iter;
          // for test
          //cout << outputFileName << endl;
          strcat(outputFileName, iConvert.str().c_str());
          // for test
          //cout << outputFileName << endl;
          if (iofC.check(outputFileName, 0))
            {
              cout << "file: " << outputFileName << " exists" << endl;
              continue;
            }
          if (resampling)
            {
              resampling = dataSet->resamplingDataDes(dataDesFileName, type);
            }
          cout << "shuffle epoch: " << iter << " with " << dataSet->ngramNumber
              << " ngrams" << endl;
          dataSet->shuffle(times);

          cout << "write to file : " << outputFileName << endl;
          iofO.takeWriteFile(outputFileName);
          dataSet->writeReBiNgram(&iofO);
        }
      delete dataSet;
      delete inputVoc;
      delete outputVoc;
      return 0;
    }
}
