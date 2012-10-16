#include "model.H"
#include "time.h"
#include<algorithm>
#include <iterator>

int
checkBlankString(string line)
{
  for (int i = 0; i < line.length(); i++)
    {
      if (line[i] != ' ')
        {
          return 0;
        }
    }
  return 1;
}

int
resamplingSentence(int totalLineNumber, int resamplingLineNumber,
    int* resamplingLineId)
{
  if (totalLineNumber == resamplingLineNumber)
    {
      int i;
      for (i = 0; i < totalLineNumber; i++)
        {
          resamplingLineId[i] = i;
        }
      return 1;
    }
  else
    {
      int* buff = new int[totalLineNumber];
      int chosenPos;
      int i;
      for (i = 0; i < totalLineNumber; i++)
        {
          buff[i] = i;
        }
      int pos = totalLineNumber;
      for (i = 0; i < resamplingLineNumber; i++)
        {
          chosenPos = rand() % pos;
          resamplingLineId[i] = buff[chosenPos];
          buff[chosenPos] = buff[pos - 1];
          pos--;
        }
      delete[] buff;
      sort(resamplingLineId, resamplingLineId + resamplingLineNumber);
      return 1;
    }
}

int
resamplingSentText(ioFile* iof, int totalLineNumber, int resamplingLineNumber)
{
  int* resamplingLineId = new int[resamplingLineNumber];
  resamplingSentence(totalLineNumber, resamplingLineNumber, resamplingLineId);

  int i = 0;
  string line;
  int readLineNumber = 0;
  int currentId = 0;
  while (!iof->getEOF())
    {
      if (iof->getLine(line))
        {
          if (readLineNumber == resamplingLineId[currentId])
            {
              if (!checkBlankString(line))
                {
                  (*iof->fo) << line << endl;
                }
              currentId++;
            }
          if (currentId == resamplingLineNumber)
            {
              break;
            }
        }

      readLineNumber++;
#if PRINT_DEBUG
      if (readLineNumber % NLINEPRINT == 0)
        {
          cout << readLineNumber << " ... " << flush;
        }
#endif
    }
#if PRINT_DEBUG
  cout << endl;
#endif
  delete[] resamplingLineId;
  return resamplingLineNumber;

}

int
resamplingSentDataDes(char* dataDesFileName, char* outputFileName)
{

  ioFile iofRead;
  iofRead.format = TEXT;
  iofRead.takeReadFile(dataDesFileName);
  int resampling = 0;
  int allLineNumber = 0;
  string line;
  int totalLineNumber = 0;
  float percent;
  char dataFileName[260];
  //Read to see how many lines
  while (!iofRead.getEOF())
    {
      if (iofRead.getLine(line))
        {
          istringstream ostr(line);
          ostr >> dataFileName >> totalLineNumber >> percent;
          if (percent < 1)
            {
              resampling = 1;
            }
          allLineNumber += (int) (totalLineNumber * percent);
        }
    }
  int resamplingLineNumber;
  //Now read
  ioFile iof;
  iof.format = TEXT;
  iof.takeWriteFile(outputFileName);
  iofRead.takeReadFile(dataDesFileName);
  while (!iofRead.getEOF())
    {
      if (iofRead.getLine(line) && line != "")
        {
          istringstream ostr(line);
          ostr >> dataFileName >> totalLineNumber >> percent;
          resamplingLineNumber = (int) (totalLineNumber * percent);
          iof.takeReadFile(dataFileName);
          cout << "read file: " << dataFileName << endl;
          resamplingSentText(&iof, totalLineNumber, resamplingLineNumber);
        }
    }
  return resampling;
}

int
main(int argc, char *argv[])
{
  if (argc != 5)
    {
      cout << "dataDesFileName prefixOutputFileName minEpochs maxEpochs"
          << endl;
      return 0;
    }
  else
    {
      srand48(time(NULL));
      srand(time(NULL));
      char* dataDesFileName = argv[1];
      char* prefixOutputFileName = argv[2];
      int minEpochs = atoi(argv[3]);
      int maxEpochs = atoi(argv[4]);
      char outputFileName[260];
      ioFile iofC;
      if (!iofC.check(dataDesFileName, 1))
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
          strcat(outputFileName, iConvert.str().c_str());
          if (iofC.check(outputFileName, 0))
            {
              cout << "file: " << outputFileName << " exists" << endl;
              continue;
            }
          if (resampling)
            {
              resampling = resamplingSentDataDes(dataDesFileName,
                  outputFileName);
            }
          cout << "write to file : " << outputFileName << endl;
        }
      return 0;
    }
}
