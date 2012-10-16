dlvl = ../../.
include $(dlvl)/Makefile.in
all:
	$(CC) $(FLAGS) createPrototype.cc -o createPrototype.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) resamplingData.cc -o resamplingData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) sequenceTrain.cc -o sequenceTrain.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
		
	$(CC) $(FLAGS) growPredictionSpace.cc -o growPredictionSpace.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) growOutPredictionSpace.cc -o growOutPredictionSpace.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) growContext.cc -o growContext.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) growVocOutPredictionSpace.cc -o growVocOutPredictionSpace.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	
	$(CC) $(FLAGS) text2Perplexity.cc -o text2Perplexity.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)	
	$(CC) $(FLAGS) text2Prob.cc -o text2Prob.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) text2ProbAll.cc -o text2ProbAll.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)	
	#$(CC) $(FLAGS) text2ProbKen.cc -o text2ProbKen.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lbincode -lmodel  $(LLIB)

	$(CC) $(FLAGS) infoNgram.cc -o infoNgram.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) infoModel.cc -o infoModel.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	
	$(CC) $(FLAGS) addNULL.cc -o addNULL.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) disWordTupleMono2Model.cc -o disWordTupleMono2Model.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	
	$(CC) $(FLAGS) pushAllParameter.cc -o pushAllParameter.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) takeLookupTable.cc -o takeLookupTable.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) takeBaseParameter.cc -o takeBaseParameter.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) takeParameter.cc -o takeParameter.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	
	$(CC) $(FLAGS) text2Index.cc -o text2Index.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) mapVoc.cc -o mapVoc.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) uniqVoc.cc -o uniqVoc.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)	
	
	$(CC) $(FLAGS) text2ProbActiveMax.cc -o text2ProbActiveMax.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) text2ProbContext.cc -o text2ProbContext.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)	
	
	$(CC) $(FLAGS) resamplingIdData.cc -o resamplingIdData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) resamplingNgramData.cc -o resamplingNgramData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)			
	$(CC) $(FLAGS) resamplingSentData.cc -o resamplingSentData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) resamplingTypeData.cc -o resamplingTypeData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)	
	
	$(CC) $(FLAGS) disWordTupleGrowContext.cc -o disWordTupleGrowContext.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) disWordTupleResamplingData.cc -o disWordTupleResamplingData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) disWordTupleCreatePrototype.cc -o disWordTupleCreatePrototype.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	
	$(CC) $(FLAGS) wordAlignMono2Model.cc -o wordAlignMono2Model.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)	
	$(CC) $(FLAGS) wordAlign1SideText2Ngram.cc -o wordAlign1SideText2Ngram.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) wordAlignText2Ngram.cc -o wordAlignText2Ngram.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)	
	
	$(CC) $(FLAGS) resamplingMidData.cc -o resamplingMidData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) createMidPrototype.cc -o createMidPrototype.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) inverseModel.cc -o inverseModel.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	
	$(CC) $(FLAGS) fResamplingData.cc -o fResamplingData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) fSequenceTrain.cc -o fSequenceTrain.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) fCreatePrototype.cc -o fCreatePrototype.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) fText2Prob.cc -o fText2Prob.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)	
	
	$(CC) $(FLAGS) jkResamplingData.cc -o jkResamplingData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) jkText2Ngram.cc -o jkText2Ngram.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) jkAsrText2Ngram.cc -o jkAsrText2Ngram.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) jkGrowPredictionSpace.cc -o jkGrowPredictionSpace.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) outMarcoModel.cc -o outMarcoModel.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	
	$(CC) $(FLAGS) rankCreatePrototype.cc -o rankCreatePrototype.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) rankResamplingData.cc -o rankResamplingData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	
	$(CC) $(FLAGS) rovn2rModel.cc -o rovn2rModel.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)	
	$(CC) $(FLAGS) rCreatePrototype.cc -o rCreatePrototype.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) rResamplingData.cc -o rResamplingData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) rInfoNgram.cc -o rInfoNgram.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	
	$(CC) $(FLAGS) disTupleCreatePrototype.cc -o disTupleCreatePrototype.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	$(CC) $(FLAGS) disTupleResamplingData.cc -o disTupleResamplingData.exe $(CLIB_INCLUDE) -ltensor -lioFile -ltext -lmodule -lmodel $(LLIB)
	

	cp *.exe $(CBIN_PATH)

