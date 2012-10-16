#include "model.H"
#include "ioFile.H"

int
sequenceTrain(char* prefixModel, char* prefixData, int maxExampleNumber,
    char* validationFileName, string validType, string learningRateType,
    int minIteration, int maxIteration, int typeData)
{
  outils otl;
  char inputModelFileName[260];
  char convertStr[260];
  int iteration;
  int gz;
  for (iteration = maxIteration; iteration >= minIteration - 2; iteration--)
    {
	  /*//for test
	  cout << iteration;*/
      sprintf(convertStr, "%ld", iteration);
      strcpy(inputModelFileName, prefixModel);
      strcat(inputModelFileName, convertStr);
      // for test
//      cout << inputModelFileName;
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
  // for test
//  cout << gz;
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
  Model* model;
  READMODEL(model, 0, inputModelFileName);
  model->dataSet->type = typeData;

  //for test
    //cout << "loai data: " << model->dataSet->type << endl;
  model->sequenceTrain(prefixModel, gz, prefixData, maxExampleNumber,
      validationFileName, validType, learningRateType, iteration + 1,
      maxIteration);
  delete model;
  return 0;
}

int
main(int argc, char *argv[])
{
  if (argc != 9)
    {
      cout
          << "prefixModel prefixData maxExampleNumber validationFileName validType learningRateType minIteration maxIteration"
          << endl;
      cout << "validType: n(normal-text), l(ngram list), id (binary id ngram)"
          << endl;
      return 0;
    }
  char* prefixModel = argv[1];
  char* prefixData = argv[2];
  int maxExampleNumber = atoi(argv[3]);
  char* validationFileName = argv[4];
  string validType = argv[5];
  if (validType != "n" && validType != "l" && validType != "id")

    {
      cerr << "Which validType do you want?" << endl;
      return 1;
    }

  string learningRateType = argv[6];
  if (learningRateType != LEARNINGRATE_NORMAL && learningRateType
      != LEARNINGRATE_DOWN)
    {
      cerr << "Which learningRateType do you want?" << endl;
      return 1;
    }
  int minIteration = atoi(argv[7]);
  int maxIteration = atoi(argv[8]);
  srand(time(NULL));
  sequenceTrain(prefixModel, prefixData, maxExampleNumber, validationFileName,
      validType, learningRateType, minIteration, maxIteration, TYPE_DATA);
  return 0;
}

