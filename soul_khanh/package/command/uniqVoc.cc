#include "model.H"

int
main(int argc, char *argv[])
{

  if (argc != 3)
    {
      cout << "vocFileName outputModelFileName" << endl;
      return 0;
    }

  char* vocFileName = argv[1];
  char* outputModelFileName = argv[2];

  ioFile iof;
  if (!iof.check(vocFileName, 1))
    {
      return 1;
    }
  if (iof.check(outputModelFileName, 0))
    {
      cerr << "output file exists" << endl;
      return 1;
    }

  SoulVocab* baseVoc = new SoulVocab();
  iof.format = TEXT;
  iof.takeReadFile(vocFileName);
  iof.takeWriteFile(outputModelFileName);
  string line;
  int check;
  int mindex = 0;
  while (!iof.getEOF())
    {
      if (iof.getLine(line))
        {
          check = baseVoc->index(line);
          if (check == ID_UNK)
            {
              baseVoc->add(line, mindex);
              mindex++;
              if (mindex % 10000 == 0)
                {
                  cout << mindex << " ... " << flush;
                }
              iof.writeNorString(line.c_str());
            }
        }
    }
  cout << endl;
  delete baseVoc;
}

