#include "model.H"
#include "time.h"
#include<algorithm>
#include <iterator>
int
main(int argc, char *argv[])
{
  if (argc != 6)
    {
      cout
          << "textFileName inputVocFileName outputVocFileName n outputFileName"
          << endl;
      return 0;
    }
  else
    {
      char* textFileName = argv[1];
      char* inputVocFileName = argv[2];
      char* outputVocFileName = argv[3];
      int n = atoi(argv[4]);
      int mapIUnk = 1;
      int mapOUnk = 1;
      int cont;
      int blockSize;
      int BOS = n - 1;
      int times = 10;
      char* outputFileName = argv[5];
      ioFile iofC;
      if (!iofC.check(textFileName, 1))
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
      if (iofC.check(outputFileName, 0))
        {
          cout << "file: " << outputFileName << " exists" << endl;
          return 1;
        }

      SoulVocab* inputVoc = new SoulVocab(inputVocFileName);
      SoulVocab* outputVoc = new SoulVocab(outputVocFileName);
      JKNgramDataSet* dataSet;
      char * maxNgramNumberEnv;
      maxNgramNumberEnv = getenv("RESAMPLING_NGRAM_NUMBER");
      if (maxNgramNumberEnv != NULL)
        {
          dataSet = new JKNgramDataSet(n, BOS, inputVoc, outputVoc, mapIUnk,
              mapOUnk, atoi(maxNgramNumberEnv));
        }
      else
        {
          dataSet = new JKNgramDataSet(n, BOS, inputVoc, outputVoc, mapIUnk,
              mapOUnk, RESAMPLING_NGRAM_NUMBER);
        }
      if (dataSet->data == NULL)
        {
          return 1;
        }
      ioFile iofI;
      ioFile iofO;
      iofI.format = TEXT;
      iofI.takeReadFile(textFileName);
      dataSet->readText(&iofI);
      cout << "write to file : " << outputFileName << " with "
          << dataSet->ngramNumber << " ngrams" << endl;

      //iofO.format = TEXT;
      iofO.takeWriteFile(outputFileName);
      dataSet->writeCoBiNgram(&iofO);
      delete dataSet;
      delete inputVoc;
      delete outputVoc;
      return 0;
    }
}
