#include "model.H"

int
main(int argc, char *argv[])
{
  if (argc != 4)
    {
      cout << "textFileName vocFileName indexFileName" << endl;
      return 0;
    }
  else
    {
      time_t start, end;
      char* textFileName = argv[1];
      char* vocFileName = argv[2];
      char* indexFileName = argv[3];
      ioFile iofC;
      if (!iofC.check(textFileName, 1))
        {
          return 1;
        }
      if (!iofC.check(vocFileName, 1))
        {
          return 1;
        }
      if (iofC.check(indexFileName, 0))
        {
          cerr << "index file exists" << endl;
          return 1;
        }
      string line;
      int id;
      int readLineNumber = 0;
      SoulVocab* voc = new SoulVocab(vocFileName);
      cout << "Finish read voc with " << voc->wordNumber << " words" << endl;
      cout << "write to file:" << indexFileName << endl;
      ioFile iof;
      iof.format = TEXT;
      iof.takeReadFile(textFileName);
      iof.takeWriteFile(indexFileName);
      string word;
      string tuple;
      int count;
      while (!iof.getEOF())
        {
          if (iof.getLine(line))
            {
              do
                {
                  istringstream streamLine(line);
                  streamLine >> word; //Skip (1,2,3)
                  streamLine >> word;
                  if (word != "<s>" && word != "</s>")
                    {
                      count = 0;
                      while (word != "|||")
                        {
                          if (count == 0)
                            {
                              tuple = word;
                            }
                          else
                            {
                              tuple = tuple + " " + word;
                            }
                          count = count + 1;
                          streamLine >> word;
                        }
                      tuple = tuple + " |||";
                      streamLine >> word;
                      while (word != "|||")
                        {
                          tuple = tuple + " " + word;
                          streamLine >> word;
                        }
                      id = voc->index(tuple);
                      *(iof.fo) << id << " ";
                    }
                  else if (word == "</s>")
                    {
                      *(iof.fo) << endl;
                    }
                  iof.getLine(line);
                }
              while (line != "EOS");
            }
        }
      readLineNumber++;
#if PRINT_DEBUG
      if (readLineNumber % NLINEPRINT == 0)
        {
          cout << readLineNumber << " ... " << flush;
        }
#endif
#if PRINT_DEBUG
      cout << endl;
#endif
      delete voc;
    }
  return 0;
}

