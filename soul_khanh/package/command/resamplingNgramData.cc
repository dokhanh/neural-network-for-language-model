#include "model.H"
#include "time.h"
#include<algorithm>
#include <iterator>
int
main(int argc, char *argv[])
{
  if (argc != 10)
    {
      cout
          << "ngramFileName inputVocFileName outputVocFileName n mapIUnk mapOUnk prefixOutputFileName minEpochs maxEpochs"
          << endl;
      return 0;
    }
  else
    {
      srand48(time(NULL));
      srand(time(NULL));
      int type = 0;
      char* ngramFileName = argv[1];
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
      if (!iofC.check(ngramFileName, 1))
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
      maxNgramNumberEnv = getenv("RESAMPLING_NGRAM_NUMBER");
      if (maxNgramNumberEnv != NULL)
        {
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
      ioFile iofI;
      iofI.format = TEXT;
      iofI.takeReadFile(ngramFileName);
      dataSet->readTextNgram(&iofI);

      ioFile iofO;
      for (int iter = minEpochs; iter <= maxEpochs; iter++)
        {
          strcpy(outputFileName, prefixOutputFileName);
          ostringstream iConvert;
          iConvert << iter;
          strcat(outputFileName, iConvert.str().c_str());
          if (iofC.check(outputFileName, 0))
            {
              cout << "file: " << outputFileName << " exists" << endl;
              continue;
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
