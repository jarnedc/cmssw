import sys
import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

runningOnData = True #this flag is used for choosing whether you want to produce trees for data or for MC.  
lookAtAntiS = False  #This flag should be False if you are running on data unless you want to unblind. If you are running on MC it should be True as you want to see the signal.

options = VarParsing ('analysis')
options.parseArguments()
## data or MC options
options.register(
	'isData',True,VarParsing.multiplicity.singleton,VarParsing.varType.bool,
	'flag to indicate data or MC')

options.register(
	'maxEvts',1000000,VarParsing.multiplicity.singleton,VarParsing.varType.int,
	'flag to indicate max events to process')
	
options.isData==True

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

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(options.inputFiles),
  duplicateCheckMode = cms.untracked.string ("noDuplicateCheck")
)




process.load("SexaQAnalysis.FlatTreeProducer.FlatTreeProducerDATA_cfi")
process.FlatTreeProducerDATA.lookAtAntiS = lookAtAntiS
if(runningOnData==True):
	process.FlatTreeProducer = cms.Path(process.FlatTreeProducerDATA)


#process.load("SexaQAnalysis.AnalyzerAllSteps.AnalyzerRECO_cfi")
#process.AnalyzerRECO.lookAtAntiS = lookAtAntiS
#if(runningOnData==True):
#	process.analyzerallsteps = cms.Path(process.AnalyzerRECO)
#
##run this analyer only on GEN:
#process.load("SexaQAnalysis.AnalyzerAllSteps.AnalyzerGEN_cfi")
#if(runningOnData==False):
#	process.analyzerallsteps = cms.Path(process.validation*process.AnalyzerGEN*process.AnalyzerRECO)
#

process.p = cms.Schedule(
  process.FlatTreeProducer
)


# Output
process.TFileService = cms.Service('TFileService',
    fileName = cms.string(options.outputFile)
)


#Keep edm output file --> used in the analyzer
#process.out = cms.OutputModule("PoolOutputModule",
#  outputCommands = cms.untracked.vstring(
#     'keep *'
#  ),
#   fileName = cms.untracked.string("AOD_test_matchingHits.root"),
# # SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring('p') )
#)

#process.output_step = cms.EndPath(process.out)

