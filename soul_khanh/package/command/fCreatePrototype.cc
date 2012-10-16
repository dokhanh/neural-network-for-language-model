#include "model.H"
int
getHiddenCode(char* input, intTensor& outputTensor)
{
  char hidden[260];
  strcpy(hidden, input);
  int hiddenNumber = 1;
  for (int i = 0; i < strlen(input); i++)
    {
      if (hidden[i] == '_')
        {
          hidden[i] = ' ';
          hiddenNumber++;
        }
    }
  string strHidden = hidden;
  istringstream streamHidden(strHidden);
  string word;
  outputTensor.resize(hiddenNumber, 1);
  hiddenNumber = 0;
  while (streamHidden >> word)
    {
      outputTensor(hiddenNumber) = atoi(word.c_str());
      hiddenNumber++;
    }
}
int
main(int argc, char *argv[])
{
  //Create model
  if (argc != 6)
    {
      cout
          << "dim classNumber nonLinearType hiddenLayerSizeCode outputFileName"
          << endl;

      return 1;
    }
  else
    {
      srand48(time(NULL));
      srand(time(NULL));
      FunctionModel* modelPrototype;
      int dim = atoi(argv[1]);
      int classNumber = atoi(argv[2]);
      string nonLinearType = argv[3];
      if (nonLinearType != "t" && nonLinearType != "s" && nonLinearType != "l")
        {
          cerr << "Which activation do you want?" << endl;
          return 1;
        }
      char* hiddenLayerSizeCode = argv[4];
      char* outputFileName = argv[5];
      int blockSize = 1;
      ioFile iofC;
      if (iofC.check(outputFileName, 0))
        {
          cerr << "Prototype exists" << endl;
          return 1;
        }
      intTensor hiddenLayerSizeArray;
      getHiddenCode(hiddenLayerSizeCode, hiddenLayerSizeArray);
      modelPrototype = new FunctionModel(dim, classNumber, blockSize,
          nonLinearType, hiddenLayerSizeArray);
      modelPrototype->name = "func";

      ioFile mIof;
      mIof.takeWriteFile(outputFileName);
      modelPrototype->write(&mIof);
      delete modelPrototype;
      return 0;
    }
}
