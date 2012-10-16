#include "model.H"

int
main(int argc, char *argv[])
{
  if (argc != 6)
    {
      cout << "modelFileName blockSize textFileName type probFileName" << endl;
      return 0;
    }
  else
    {
      time_t start, end;
      char* modelFileName = argv[1];
      int blockSize = atoi(argv[2]);
      char* textFileName = argv[3];
      int type = atoi(argv[4]);
      char* probFileName = argv[5];
      ioFile iofC;
      if (!iofC.check(modelFileName, 1))
        {
          return 1;
        }
      if (!iofC.check(textFileName, 1))
        {
          return 1;
        }
      if (iofC.check(probFileName, 0))
        {
          cerr << "prob file exists" << endl;
          return 1;
        }

      FunctionModel* model = new FunctionModel();

      ioFile iofModel;
      iofModel.takeReadFile(modelFileName);
      model->read(&iofModel, 1, blockSize);

      time(&start);
      cout << "Finish read" << endl;
      model->computeForward(textFileName, type);
      ioFile iof;
      iof.format = TEXT;
      iof.takeWriteFile(probFileName);
      for (int i = 0; i < model->dataSet->probTensor.length; i++)
        {
          iof.writeReal(model->dataSet->probTensor(i));
        }
      time(&end);
      cout << "Finish after " << difftime(end, start) / 60 << " minutes"
          << endl;
      delete model;
    }
  return 0;
}

