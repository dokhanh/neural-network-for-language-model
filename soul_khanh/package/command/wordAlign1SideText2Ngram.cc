#include "model.H"

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
// Don't need mapIUnk, mapOUnk
// mapIUnk = 1 always
// mapOUnk = 1 always.
// if shortlist or ooshortlist,
// we don't have UNK in output vocabulary, map or it, unknown word's index is awlays ID_UNK
int
main(int argc, char *argv[])
{
  if (argc != 8)
    {
      cout
          << "sourceVocFileName targetVocFileName sourceTextFileName targetTextFileName alignFileName n prefixOutputFileName"
          << endl;
      cout
          << "n: context length (must be impair), take n foreign words and (n - 1) context words"
          << endl;
      return 0;
    }
  else
    {
      time_t start, end;
      char* sourceVocFileName = argv[1];
      char* targetVocFileName = argv[2];
      char* sourceTextFileName = argv[3];
      char* targetTextFileName = argv[4];
      char* alignFileName = argv[5];
      int n = atoi(argv[6]);
      if ((n % 2) != 1)
        {
          cerr << "n must be impair" << endl;
          return 1;
        }
      int nm = (n - 1) / 2;

      char* prefixOutputFileName = argv[7];
      char norOutputFileName[260];
      char invOutputFileName[260];
      strcpy(norOutputFileName, prefixOutputFileName);
      strcat(norOutputFileName, "Trg");
      strcpy(invOutputFileName, prefixOutputFileName);
      strcat(invOutputFileName, "Src");
      ioFile iofC;
      if (!iofC.check(sourceVocFileName, 1))
        {
          return 1;
        }
      if (!iofC.check(targetVocFileName, 1))
        {
          return 1;
        }
      if (!iofC.check(sourceTextFileName, 1))
        {
          return 1;
        }
      if (!iofC.check(targetTextFileName, 1))
        {
          return 1;
        }
      if (!iofC.check(alignFileName, 1))
        {
          return 1;
        }

      if (iofC.check(norOutputFileName, 0))
        {
          cerr << "output file exists" << endl;
          return 1;
        }
      if (iofC.check(invOutputFileName, 0))
        {
          cerr << "output file exists" << endl;
          return 1;
        }

      string sourceLine;
      string targetLine;
      string alignLine;
      int id;
      int readLineNumber = 0;
      cout << "Read vocabulary" << endl;
      SoulVocab* sourceInVoc = new SoulVocab(sourceVocFileName);
      SoulVocab* targetVoc = new SoulVocab(targetVocFileName);
      int sourceNULL = sourceInVoc->index("NULL");
      int targetNULL = targetVoc->index("NULL");
      if (sourceNULL == ID_UNK)
        {
          sourceNULL = sourceInVoc->wordNumber;
          sourceInVoc->add("NULL", sourceInVoc->wordNumber);
          cerr << "add NULL to source vocabulary " << sourceNULL << endl;
        }
      if (targetNULL == ID_UNK)
        {
          targetNULL = targetVoc->wordNumber;
          targetVoc->add("NULL", targetVoc->wordNumber);
          cerr << "add NULL to target vocabulary " << targetNULL << endl;
        }

      SoulVocab* inVoc = new SoulVocab(targetVoc); //Copy target vocab, need to have NULL modelTrg->inputVoc
      SoulVocab* sourceVoc = new SoulVocab(); //Copy target vocab, need to have NULL modelTrg->inputVoc

      VocNode* run;
      int i;
      int offset;
      offset = targetVoc->wordNumber;
      for (i = 0; i < sourceInVoc->tableSize; i++)
        {
          run = sourceInVoc->table[i];
          while (run->next != NULL)
            {
              run = run->next;
              inVoc->add(PREFIX_SOURCE + run->word, offset + run->index);
              if (run->word != SS && run->word != ES && run->word != UNK)
                {
                  sourceVoc->add(PREFIX_SOURCE + run->word, run->index);
                }
              else
                {
                  sourceVoc->add(run->word, run->index);
                }
            }
        }

      int inSS = inVoc->index("<s>");
      int inES = inVoc->index("</s>");
      int inNULL = inVoc->index("NULL");

      ioFile sourceText;
      sourceText.format = TEXT;
      sourceText.takeReadFile(sourceTextFileName);

      ioFile targetText;
      targetText.format = TEXT;
      targetText.takeReadFile(targetTextFileName);

      ioFile align;
      align.format = TEXT;
      align.takeReadFile(alignFileName);

      ioFile norOutput;
      norOutput.format = BINARY;
      norOutput.takeWriteFile(norOutputFileName);

      ioFile invOutput;
      invOutput.format = BINARY;
      invOutput.takeWriteFile(invOutputFileName);
      norOutput.writeInt(n + 1);
      invOutput.writeInt(n + 1);
      int found;
      int sourceId;
      int targetId;
      int oid;
      int sSourceAlign[MAX_WORD_PER_SENTENCE];
      int sTargetAlign[MAX_WORD_PER_SENTENCE];
      int cSourceAlign[MAX_WORD_PER_SENTENCE];
      int cTargetAlign[MAX_WORD_PER_SENTENCE];

      int sourceWord[MAX_WORD_PER_SENTENCE];
      int targetWord[MAX_WORD_PER_SENTENCE];
      int inSourceWord[MAX_WORD_PER_SENTENCE];
      int sourceSize;
      int targetSize;

      int j;
      cout << "Align" << endl;
      while (!align.getEOF())
        {
          if (align.getLine(alignLine))
            {
              sourceText.getLine(sourceLine);
              targetText.getLine(targetLine);
              if (!checkBlankString(sourceLine)
                  && !checkBlankString(targetLine))
                {
                  sourceSize = 0;
                  targetSize = 0;
                  // add SS
                  for (i = 0; i < nm; i++)
                    {
                      sourceWord[sourceSize] = inSS;
                      inSourceWord[sourceSize] = inSS;
                      sourceSize++;
                      targetWord[targetSize++] = inSS;
                    }
                  string word;
                  // Read source sentence, put source word id into sourceWord
                  istringstream sourceStream(sourceLine);
                  while (sourceStream)
                    {
                      sourceStream >> word;
                      if (!sourceStream)
                        {
                          continue;
                        }
                      oid = sourceVoc->index(PREFIX_SOURCE + word);
                      if (oid == ID_UNK)
                        {
                          oid = sourceVoc->unk;
                        }
                      sourceWord[sourceSize] = oid;
                      oid = inVoc->index(PREFIX_SOURCE + word);
                      if (oid == ID_UNK)
                        {
                          oid = inVoc->unk;
                        }
                      inSourceWord[sourceSize] = oid;
                      sourceSize++;
                    }
                  // Read target sentence, put target word id into sourceWord
                  istringstream targetStream(targetLine);
                  while (targetStream)
                    {
                      targetStream >> word;
                      if (!targetStream)
                        {
                          continue;
                        }
                      oid = targetVoc->index(word);
                      if (oid == ID_UNK)
                        {
                          oid = targetVoc->unk;
                        }
                      targetWord[targetSize++] = oid;
                    }
                  // add ES
                  for (i = 0; i < nm; i++)
                    {
                      sourceWord[sourceSize] = inES;
                      inSourceWord[sourceSize] = inES;
                      sourceSize++;
                      targetWord[targetSize++] = inES;
                    }

                  for (i = 0; i < sourceSize; i++)
                    {
                      sSourceAlign[i] = 0;
                      cSourceAlign[i] = 0;
                    }
                  for (i = 0; i < targetSize; i++)
                    {
                      sTargetAlign[i] = 0;
                      cTargetAlign[i] = 0;
                    }
                  // Read align sentence
                  istringstream alignStream(alignLine);
                  while (alignStream)
                    {
                      alignStream >> word;
                      if (!alignStream)
                        {
                          continue;
                        }
                      found = word.find("-");
                      word.replace(found, 1, " ");
                      istringstream wordStream(word);
                      wordStream >> sourceId;
                      wordStream >> targetId;
                      //Because we add nm SS, so we have to add Ids with nm
                      sourceId += nm;
                      targetId += nm;
                      if (sourceId < sourceSize && targetId < targetSize)
                        {
                          sSourceAlign[sourceId] += targetId;
                          sTargetAlign[targetId] += sourceId;
                          cSourceAlign[sourceId] += 1;
                          cTargetAlign[targetId] += 1;
                        }
                      else
                        {
                          cerr << "id out of range in align file line "
                              << readLineNumber;
                        }
                    }
                  //Compute average Id
                  for (i = nm; i < sourceSize - nm; i++)
                    {
                      if (cSourceAlign[i] != 0)
                        {
                          sSourceAlign[i] /= cSourceAlign[i];
                        }
                    }
                  for (i = nm; i < targetSize - nm; i++)
                    {
                      if (cTargetAlign[i] != 0)
                        {
                          sTargetAlign[i] /= cTargetAlign[i];
                        }
                    }
                  // Normal case
                  for (i = nm; i < targetSize - nm; i++)
                    {
                      if (cTargetAlign[i] != 0)
                        {
                          sourceId = sTargetAlign[i];
                          targetId = i;
                          norOutput.writeIntArray(inSourceWord + sourceId - nm,
                              n);
                          norOutput.writeInt(targetWord[targetId]);
                        }
                    }
                  for (i = nm; i < sourceSize - nm; i++)
                    {
                      if (cSourceAlign[i] != 0)
                        {
                          targetId = sSourceAlign[i];
                          sourceId = i;
                          invOutput.writeIntArray(targetWord + targetId - nm, n);
                          invOutput.writeInt(sourceWord[sourceId]);
                        }

                    }

                  //source word is NULL
                  for (i = nm; i < targetSize - nm; i++)
                    {
                      if (cTargetAlign[i] == 0)
                        {
                          // Find the nearest align word
                          for (j = 1; j < targetSize - nm; j++)
                            {
                              //Use left
                              if (i - j >= nm)
                                {
                                  if (cTargetAlign[i - j] != 0)
                                    {
                                      sTargetAlign[i] = sTargetAlign[i - j];
                                      sourceId = sTargetAlign[i] + 1;
                                      targetId = i;
                                      norOutput.writeIntArray(
                                          inSourceWord + sourceId - nm, nm);
                                      norOutput.writeInt(inNULL);
                                      norOutput.writeIntArray(
                                          inSourceWord + sourceId, nm);
                                      norOutput.writeInt(targetWord[targetId]);

                                      invOutput.writeIntArray(
                                          targetWord + targetId - nm, n);
                                      invOutput.writeInt(sourceNULL);

                                      break;
                                    }
                                }
                              //Use right
                              if (i + j < targetSize - nm)
                                {
                                  if (cTargetAlign[i + j] != 0)
                                    {
                                      sTargetAlign[i] = sTargetAlign[i + j];
                                      sourceId = sTargetAlign[i];
                                      targetId = i;
                                      norOutput.writeIntArray(
                                          inSourceWord + sourceId - nm, nm);
                                      norOutput.writeInt(inNULL);
                                      norOutput.writeIntArray(
                                          inSourceWord + sourceId, nm);
                                      norOutput.writeInt(targetWord[targetId]);

                                      invOutput.writeIntArray(
                                          targetWord + targetId - nm, n);
                                      invOutput.writeInt(sourceNULL);

                                      break;
                                    }
                                }
                            }
                        }
                    }
                  //target word is NULL
                  for (i = nm; i < sourceSize - nm; i++)
                    {
                      if (cSourceAlign[i] == 0)
                        {
                          // Find the nearest align word
                          for (j = 1; j < sourceSize - nm; j++)
                            {
                              //Use left
                              if (i - j >= nm)
                                {
                                  if (cSourceAlign[i - j] != 0)
                                    {
                                      sSourceAlign[i] = sSourceAlign[i - j];
                                      sourceId = i;
                                      targetId = sSourceAlign[i] + 1;
                                      norOutput.writeIntArray(
                                          inSourceWord + sourceId - nm, n);
                                      norOutput.writeInt(targetNULL);

                                      invOutput.writeIntArray(
                                          targetWord + targetId - nm, nm);
                                      invOutput.writeInt(targetNULL);
                                      invOutput.writeIntArray(
                                          targetWord + targetId, nm);
                                      invOutput.writeInt(sourceWord[sourceId]);

                                      break;
                                    }
                                }
                              //Use right
                              if (i + j < sourceSize - nm)
                                {
                                  if (cSourceAlign[i + j] != 0)
                                    {
                                      sSourceAlign[i] = sSourceAlign[i + j];
                                      sourceId = i;
                                      targetId = sSourceAlign[i];
                                      norOutput.writeIntArray(
                                          inSourceWord + sourceId - nm, n);
                                      norOutput.writeInt(targetNULL);

                                      invOutput.writeIntArray(
                                          targetWord + targetId - nm, nm);
                                      invOutput.writeInt(targetNULL);
                                      invOutput.writeIntArray(
                                          targetWord + targetId, nm);
                                      invOutput.writeInt(sourceWord[sourceId]);
                                      break;
                                    }
                                }
                            }
                        }
                    }
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
      delete sourceVoc;
      delete targetVoc;
      delete sourceInVoc;
      delete inVoc;
    }
  return 0;
}

