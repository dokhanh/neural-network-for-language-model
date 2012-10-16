#include "model.H"
#include "time.h"
#include<algorithm>
#include <iterator>
int
main(int argc, char *argv[])
{
  if (argc != 6)
    {
      cout << "dataFileName dim prefixOutputFileName minEpochs maxEpochs"
          << endl;
      return 0;
    }
  else
    {
      srand48(time(NULL));
      srand(time(NULL));
      char* dataFileName = argv[1];
      int dim = atoi(argv[2]);
      char* prefixOutputFileName = argv[3];
      int minEpochs = atoi(argv[4]);
      int maxEpochs = atoi(argv[5]);
      char outputFileName[260];
      int times = 10;
      ioFile iofC;
      if (!iofC.check(dataFileName, 1))
        {
          return 1;
        }
      FunctionDataSet* dataSet;
      int classNumber = 0;
      dataSet = new FunctionDataSet(dim, classNumber);
      if (dataSet->data == NULL)
        {
          return 1;
        }
      ioFile iofI;
      //iofI.format = TEXT;
      iofI.takeReadFile(dataFileName);
      ioFile iofO;
      dataSet->readBiNgram(&iofI);
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
          cout << "shuffle epoch: " << iter << " with " << dataSet->dataNumber
              << " ngrams" << endl;
          dataSet->shuffle(times);

          cout << "write to file : " << outputFileName << endl;
          iofO.takeWriteFile(outputFileName);
          dataSet->writeNgram(&iofO);
        }
      delete dataSet;
      return 0;
    }
}
