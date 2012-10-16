#include "LangMods.h"
#include "model.H"

int
main(int argc, char **argv)
{
  if (argc != 5)
    {
      cout << "modelFileName incrUnk textFileName probFileName" << endl;
      cout << "incrUnk: *10^incrUnk for unknown word probs" << endl;
      return 0;
    }
  else
    {
      time_t start, end;
      int narg = 1;
      char* modelFileName = argv[1];
      char* incrUnkStr = argv[2];
      char* textFileName = argv[3];
      char* probFileName = argv[4];

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

      float incrUnk = -atof(incrUnkStr);
      // For me, incrUnk = -6 but for Joseph, it's 6
      string modelFileNameStr = modelFileName;
      LangModel *lm = new LangModel(modelFileNameStr, incrUnk);

      time(&start);
      ioFile* iof = new ioFile();
      iof->format = TEXT;
      iof->takeReadFile(textFileName);
      iof->takeWriteFile(probFileName);
      string line;
      string headline = "<s>";
      string tailline = "</s>";
      string word;

      float oprob;
      int readLineNumber = 0;
      while (!iof->getEOF())
        {
          if (iof->getLine(line))
            {
              vector<string> seq;
              seq.push_back(headline);
              istringstream streamLine(line);

              while (streamLine >> word)
                {
                  seq.push_back(word);
                  if (seq.size() > lm->order)
                    {
                      seq.erase(seq.begin());
                    }
                  oprob = pow(10, -lm->getCost(seq));
                  iof->writeFloat(oprob);
                }
              if (seq.size() != 1) // If line isn't empty
                {
                  seq.push_back(tailline);
                  if (seq.size() > lm->order)
                    {
                      seq.erase(seq.begin());
                    }
                  oprob = pow(10, -lm->getCost(seq));
                  iof->writeFloat(oprob);
                }
              readLineNumber++;
#if PRINT_DEBUG
              if (readLineNumber % NLINEPRINT == 0)
                {
                  cout << readLineNumber << " ... " << flush;
                }
#endif
            }
        }
#if PRINT_DEBUG
      cout << endl;
#endif
      time(&end);
      cout << "Finish after " << difftime(end, start) / 60 << " minutes"
          << endl;
      delete iof;
      return 0;
    }
}

