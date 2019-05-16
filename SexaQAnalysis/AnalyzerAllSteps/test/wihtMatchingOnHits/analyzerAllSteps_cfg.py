import sys
import FWCore.ParameterSet.Config as cms

### CMSSW command line parameter parser
from FWCore.ParameterSet.VarParsing import VarParsing

print sys.argv
options = VarParsing ('analysis')
options.parseArguments()

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







process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(options.inputFiles),
  duplicateCheckMode = cms.untracked.string ("noDuplicateCheck")
)

### standard includes
process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')


### validation-specific includes
#process.load("SimTracker.TrackAssociatorProducers.trackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
process.load("Validation.RecoTrack.cuts_cff")
process.load("Validation.RecoTrack.MultiTrackValidator_cff")
process.load("DQMServices.Components.EDMtoMEConverter_cff")
process.load("Validation.Configuration.postValidation_cff")
process.quickTrackAssociatorByHits.SimToRecoDenominator = 'reco'




########### configuration MultiTrackValidator ########
#process.multiTrackValidator.associators = ['quickTrackAssociatorByHits']

#process.multiTrackValidator.label = ['cutsRecoTracks']
#process.multiTrackValidator.histoProducerAlgoBlock.useLogPt = True
#process.multiTrackValidator.histoProducerAlgoBlock.minPt = 0.1
#process.multiTrackValidator.histoProducerAlgoBlock.maxPt = 3000.0
#process.multiTrackValidator.histoProducerAlgoBlock.nintPt = 40
#process.multiTrackValidator.UseAssociators = True


#process.load("Validation.RecoTrack.cuts_cff")
#process.cutsRecoTracks.quality = ['highPurity']
#process.cutsRecoTracks.ptMin    = 0.5
#process.cutsRecoTracks.minHit   = 10
#process.cutsRecoTracks.minRapidity  = -1.0
#process.cutsRecoTracks.maxRapidity  = 1.0

process.quickTrackAssociatorByHits.useClusterTPAssociation = True
process.load("SimTracker.TrackerHitAssociation.tpClusterProducer_cfi")


#TrackRECO on hits:
process.validation = cms.Sequence(
    process.tpClusterProducer *
    process.quickTrackAssociatorByHits 
#    process.multiTrackValidator
)

process.val = cms.Path(
    #  process.cutsRecoTracks
     process.validation
)


# Analyzer
process.load("SexaQAnalysis.AnalyzerAllSteps.AnalyzerAllSteps_cfi")
process.AnalyzerAllSteps.isData = cms.untracked.bool(True) ##############SET BACK TO TRUE################
process.analyzerallsteps = cms.Path(process.AnalyzerAllSteps)

process.p = cms.Schedule(
  process.val,process.analyzerallsteps
)


# Output
process.TFileService = cms.Service('TFileService',
 #   fileName = cms.string(str(sys.argv[3]))
    fileName = cms.string(options.outputFile)
)


#Keep edm output file --> used in the analyzer
process.out = cms.OutputModule("PoolOutputModule",
  outputCommands = cms.untracked.vstring(
     'keep *'
  ),
   fileName = cms.untracked.string("AOD_test_matchingHits.root"),
 # SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring('p') )
)

process.output_step = cms.EndPath(process.out)

