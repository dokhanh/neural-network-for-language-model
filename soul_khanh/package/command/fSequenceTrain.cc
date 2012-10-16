#include "model.H"

int
fSequenceTrain(char* prefixModel, char* prefixData, int maxExampleNumber,
    char* validationFileName, string learningRateType, int minIteration,
    int maxIteration)
{
  outils otl;
  char inputModelFileName[260];
  char convertStr[260];
  int iteration;
  int gz;
  for (iteration = maxIteration; iteration >= minIteration - 2; iteration--)
    {
      sprintf(convertStr, "%ld", iteration);
      strcpy(inputModelFileName, prefixModel);
      strcat(inputModelFileName, convertStr);
      ioFile iof;
      if (!iof.check(inputModelFileName, 0))
        {
          strcat(inputModelFileName, ".gz");
          if (iof.check(inputModelFileName, 0))
            {
              gz = 1;
              break;
            }
        }
      else
        {
          gz = 0;
          break;
        }
    }
  if (iteration == minIteration - 3)
    {
      cerr << "Can not find training model " << minIteration - 1 << endl;
      return 1;
    }
  else if (iteration == maxIteration)
    {
      cerr << "All is done" << endl;
      return 1;
    }

  sprintf(convertStr, "%ld", iteration);
  strcpy(inputModelFileName, prefixModel);
  strcat(inputModelFileName, convertStr);
  if (gz)
    {
      strcat(inputModelFileName, ".gz");
    }
  FunctionModel* model = new FunctionModel();
  ioFile iof;
  iof.takeReadFile(inputModelFileName);
  model->read(&iof, 1, 0);

  model->sequenceTrain(prefixModel, gz, prefixData, maxExampleNumber,
      validationFileName, learningRateType, iteration + 1, maxIteration);

  delete model;
  return 0;
}

int
main(int argc, char *argv[])
{
  if (argc != 8)
    {
      cout
          << "prefixModel prefixData maxExampleNumber validationFileName learningRateType minIteration maxIteration"
          << endl;
      return 0;
    }
  char* prefixModel = argv[1];
  char* prefixData = argv[2];
  int maxExampleNumber = atoi(argv[3]);
  char* validationFileName = argv[4];
  string learningRateType = argv[5];
  if (learningRateType != LEARNINGRATE_NORMAL && learningRateType
      != LEARNINGRATE_DOWN)
    {
      cerr << "Which learningRateType do you want?" << endl;
      return 1;
    }
  int minIteration = atoi(argv[6]);
  int maxIteration = atoi(argv[7]);
  srand(time(NULL));
  fSequenceTrain(prefixModel, prefixData, maxExampleNumber, validationFileName,
      learningRateType, minIteration, maxIteration);
  return 0;
}

