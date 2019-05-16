import sys
import FWCore.ParameterSet.Config as cms

### CMSSW command line parameter parser
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('python')

print sys.argv

## data or MC options
options.register(
	'isData',True,VarParsing.multiplicity.singleton,VarParsing.varType.bool,
	'flag to indicate data or MC')

options.register(
	'maxEvts',-1,VarParsing.multiplicity.singleton,VarParsing.varType.int,
	'flag to indicate max events to process')
	
	
#options.isData==True


process = cms.Process("SEXAQDATAANA")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/EventContent/EventContent_cff')

from Configuration.AlCa.GlobalTag import GlobalTag

if(options.isData==True):
    process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_2016SeptRepro_v7', '')
else:
    process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_miniAODv2_v1', '')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvts))
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(2000)
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

#Jarne

#SingleMuon
#rangeOfStrings=[str(i) for i in range(1,148)] #1 to 148
#tupleOfFiles=tuple(['file:///pnfs/iihe/cms/store/user/jdeclerc/SingleMuon/SingleMuon_Run2016G/180505_032403/0000/events_skimmed_' + x + '.root' for x in rangeOfStrings])

#zero bias
#rangeOfStrings=[str(i) for i in range(1,500)] #1 to 500
#tupleOfFiles=tuple(['file:///pnfs/iihe/cms/store/user/jdeclerc/ZeroBias/ZeroBias_280318/180505_032510/0000/events_skimmed_' + x + '.root' for x in rangeOfStrings])

#MET
#rangeOfStrings=[str(i) for i in range(1,513)] #1 to 503
#tupleOfFiles=tuple(['file:///pnfs/iihe/cms/store/user/jdeclerc/MET/MET_Run2016G/180505_234637/0000/events_skimmed_' + x + '.root' for x in rangeOfStrings])


#rangeOfStrings=[str(i) for i in range(1,145)] #1 to 145 
#tupleOfFiles=tuple(['file:///pnfs/iihe/cms/store/user/jdeclerc/SingleMuon/SingleMuon_Run2016G/180629_143246/0000/MC_events_skimmed_' + x + '.root' for x in rangeOfStrings])






process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#		*tupleOfFiles #the * is to unpack the tuple of filenames
#      "dccp:///pnfs/iihe/cms/store/user/lowette/SingleMuon/SingleMuon_Run2016G/180205_152747/0000/events_skimmed_100.root"
#	str(sys.argv[2])
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/crmc/Sexaquark_trial10_scalingNAntiS/crmc_Sexaq_1.root',
      #'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/GENSIM/CRAB_SimSexaq_7000GENInputFiles/crab_SIMSexaq_16042019_v1/190416_214431/0000/output_1.root',
      #'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step1/CRAB_SimSexaq_7000Step1InputFiles/crab_Step1Sexaq_19042019_v3/190419_121537/0000/output_1.root',

      #Step2 output, with default V0Fitter:
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_1.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_2.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_3.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_4.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_5.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_6.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_7.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_8.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_9.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_10.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_11.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_12.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_13.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_14.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_15.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_16.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_17.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_18.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_19.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_20042019_v1/190420_063702/0000/output_20.root'

      #Step2 output, with V0Fitter with disabled XYCosTheta cut:
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_1.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_2.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_3.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_4.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_5.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_6.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_7.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_8.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_9.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_10.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_11.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_12.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_13.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_14.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_15.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_16.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_17.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_18.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_19.root',
#      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_7000Step2InputFiles/crab_Step2Sexaq_29042019_v1/190429_084906/0000/output_20.root'

      #Skimming output:
      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Skimmed/CRAB_SimSexaq_7000SkimmedInputFiles/crab_SkimmingSexaq_20042019_v1/190420_150422/0001/output_1208.root',		
      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Skimmed/CRAB_SimSexaq_7000SkimmedInputFiles/crab_SkimmingSexaq_20042019_v1/190420_150422/0005/output_5582.root',		
      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Skimmed/CRAB_SimSexaq_7000SkimmedInputFiles/crab_SkimmingSexaq_20042019_v1/190420_150422/0005/output_5747.root',		
      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Skimmed/CRAB_SimSexaq_7000SkimmedInputFiles/crab_SkimmingSexaq_20042019_v1/190420_150422/0005/output_5516.root',		
      'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Skimmed/CRAB_SimSexaq_7000SkimmedInputFiles/crab_SkimmingSexaq_20042019_v1/190420_150422/0006/output_6445.root',		

	#real data:
	#'file:/pnfs/iihe/cms/store/user/jdeclerc/SinglePhoton/SinglePhoton_Run2016E-07Aug17-v1/181021_132023/0000/events_skimmed_2016_trialD_1.root'
	
	#background ref:
	#'file:/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/crmc/Sexaquark_13TeV_trial10_backgroundReference/crmc_Sexaq_2229.root'

    ),
  duplicateCheckMode = cms.untracked.string ("noDuplicateCheck")
)


# Analyzer
process.load("SexaQAnalysis.AnalyzerAllSteps.AnalyzerAllSteps_cfi")
process.AnalyzerAllSteps.isData = cms.untracked.bool(True) ##############SET BACK TO TRUE################

process.p = cms.Path(
  process.AnalyzerAllSteps
)


# Output
process.TFileService = cms.Service('TFileService',
 #   fileName = cms.string(str(sys.argv[3]))
 #   fileName = cms.string('test_GEN.root')
 #   fileName = cms.string('test_GENSIM.root')
 #   fileName = cms.string('test_Step1.root')
    fileName = cms.string('test_Step2.root')
 #   fileName = cms.string('test_Skimming.root')
 #   fileName = cms.string('test_realDataSinglePhoton.root')
 #   fileName = cms.string('test_backgroundRef.root')
	
)
