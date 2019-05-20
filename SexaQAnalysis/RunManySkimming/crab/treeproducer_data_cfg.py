import FWCore.ParameterSet.Config as cms

from RecoVertex.V0Producer.generalV0Candidates_cff import *
### CMSSW command line parameter parser
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('python')

#for MC: you want to add the extra collections so that you will be able to do the trackmatching on hits.
collections_to_keep = cms.untracked.vstring(
    'drop *',
    'keep *_InitialProducer_*_*',
    'keep recoVertexs_offlinePrimaryVertices_*_*',
    'keep recoBeamSpot_offlineBeamSpot_*_*',
    'keep *_genParticles_*_HLT',
    'keep recoVertexCompositeCandidates_generalV0Candidates_*_*',
    'keep recoTracks_lambdaKshortVertexFilter_sParticlesTracks_*',
    'keep recoVertexCompositePtrCandidates_rMassFilter_sVertexCompositePtrCandidate_*',
    'keep recoVertexCompositePtrCandidates_sMassFilter_sVertexCompositePtrCandidate_*',
    'keep *_*_*_SEXAQ',
    "keep *_genParticlesPlusGEANT_*_*",#this is for MC
    "keep *_g4SimHits_*_*",#this is for MC
    "keep *_simSiPixelDigis_*_*",#this is for MC
    "keep *_simMuonRPCDigis_*_*",#this is for MC
    "keep *_simSiStripDigis_*_*",#this is for MC
    "keep *_mix_MergedTrackTruth_*",#this is for MC
    "keep *_siPixelDigis_*_*",#this is for MC
    "keep *_siStripDigis_*_*",#this is for MC
    "keep *_siStripDigis_*_*",#this is for MC
    "keep *_siPixelClusters_*_*",#this is for MC
    "keep *_siStripClusters_*_*",#this is for MC
    "keep *_generalTracks_*_*"#this is for MC
  )

#for data: do not save the extra collections as things will get heavy
#collections_to_keep = cms.untracked.vstring(
#   'drop *',
#   'keep *_InitialProducer_*_*',
#   'keep recoVertexs_offlinePrimaryVertices_*_*',
#   'keep recoBeamSpot_offlineBeamSpot_*_*',
#   'keep *_genParticles_*_HLT',
#   'keep recoVertexCompositeCandidates_generalV0Candidates_*_*',
#   'keep recoTracks_lambdaKshortVertexFilter_sParticlesTracks_*',
#   'keep recoVertexCompositePtrCandidates_rMassFilter_sVertexCompositePtrCandidate_*',
#   'keep recoVertexCompositePtrCandidates_sMassFilter_sVertexCompositePtrCandidate_*',
#   'keep *_*_*_SEXAQ'
# )



## data or MC options
options.register(
	'isData',True,VarParsing.multiplicity.singleton,VarParsing.varType.bool,
	'flag to indicate data or MC')

options.register(
	'maxEvts',-1,VarParsing.multiplicity.singleton,VarParsing.varType.int,
	'flag to indicate max events to process')


process = cms.Process("SEXAQ")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

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

process.source = cms.Source("PoolSource",
   duplicateCheckMode = cms.untracked.string('checkAllFilesOpened'),
   fileNames = cms.untracked.vstring(
   #'file:///user/lowette/SexaQ/RunIISummer16DR80_MinBias_TuneCUETP8M1_13TeV-pythia8_GEN-SIM-RECO_NoPU_RECO_80X_mcRun2_asymptotic_v14-v1_100000_00150044-D075-E611-AAE8-001E67505A2D.root',
   #single muon data example 
   #'file:///user/lowette/SexaQ/Run2016H_SingleMuon_AOD_07Aug17-v1_10000_F068EC7F-FB96-E711-96FF-7845C4FC3BFF.root',
   #Simulated Xi events:
   #file:/user/jdeclerc/CMSSW_8_0_30/src/runSTEP2_Sexaq/SUS-RunIISummer16DR80Premix-00068_IIDD_step2_trial2_noDataMixing.root',
   #'file:///user/jdeclerc/Analysis/ParticleGun/CMSSW_8_0_21/src/XiGun_GENSIM_IIDD_10k_events_decay_length_5cm_no_EvtGen_step2.root',
   #'file:///user/jdeclerc/Analysis/dataSamples/ZeroBiasData/D07C27A2-5690-E711-844F-B083FED429D5.root'
   #'file:///user/jdeclerc/Analysis/ParticleGun/CMSSW_8_0_21/src/f2prime1525_GENSIM_IIDD_0point5to10GeV1k_MODIFIEDBRANCHINGRATIOS_MODPHI_MODBEAMSPOT_0cm_step2.root'
   'file:///pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/CRAB_SimSexaq_cosThetaXYCut_innerHitPosCut/crab_Step2Sexaq_15052019_v1/190515_071013/0000/output_1.root'
   )
)


process.nEvTotal        = cms.EDProducer("EventCountProducer")
process.nEvLambdaKshort = cms.EDProducer("EventCountProducer")
process.nEvLambdaKshortVertex = cms.EDProducer("EventCountProducer")
process.nEvSMass        = cms.EDProducer("EventCountProducer")

process.genParticlePlusGEANT = cms.EDProducer("GenPlusSimParticleProducer",
  src           = cms.InputTag("g4SimHits"),
  setStatus     = cms.int32(8),                # set status = 8 for GEANT GPs
  particleTypes = cms.vstring("Xi-","Xibar+","Lambda0","Lambdabar0","K_S0","K0"),      # also picks pi- (optional)
  filter        = cms.vstring("pt >= 0.0"),     # just for testing
  genParticles  = cms.InputTag("genParticles") # original genParticle list
)

process.load("RecoVertex.V0Producer.generalV0Candidates_cfi")
process.generalV0Candidates.innerHitPosCut = -1
process.generalV0Candidates.cosThetaXYCut = 0

#process.load("SexaQAnalysis.Skimming.InitialProducer_cfi")
#process.InitialProducer = cms.EDProducer('InitialProducer')

process.load("SexaQAnalysis.Skimming.LambdaKshortFilter_cfi")
process.lambdaKshortFilter.genCollection = cms.InputTag("genParticlePlusGEANT")
process.lambdaKshortFilter.isData = True
process.lambdaKshortFilter.minPtLambda = 0. 
process.lambdaKshortFilter.minPtKshort = 0. 
process.lambdaKshortFilter.checkLambdaDaughters = True
process.lambdaKshortFilter.prescaleFalse = 0

process.load("SexaQAnalysis.Skimming.LambdaKshortVertexFilter_cfi")
process.lambdaKshortVertexFilter.lambdaCollection = cms.InputTag("lambdaKshortFilter","lambda")
process.lambdaKshortVertexFilter.kshortCollection = cms.InputTag("lambdaKshortFilter","kshort")
process.lambdaKshortVertexFilter.maxchi2ndofVertexFit = 10.

from SexaQAnalysis.Skimming.MassFilter_cfi import massFilter
massFilter.lambdakshortCollection = cms.InputTag("lambdaKshortVertexFilter","sParticles")
massFilter.minMass = -10000 # effectively no filter
massFilter.maxMass = 10000  # effectively no filter
process.rMassFilter = massFilter.clone()
process.rMassFilter.targetMass = 0
process.sMassFilter = massFilter.clone()
process.sMassFilter.targetMass = 0.939565

process.load("SexaQAnalysis.Skimming.InitialProducer_cff")

process.load("SexaQAnalysis.TreeProducer.Treeproducer_AOD_cfi")
#process.tree.genCollection = cms.InputTag("genParticlePlusGEANT")
#process.tree.sCollection = cms.InputTag("lambdaKshortVertexFilter","sParticles")
#process.tree.sTrackCollection = cms.InputTag("lambdaKshortVertexFilter","sParticlesTracks")
#process.tree.isData = cms.untracked.bool(options.isData)

process.p = cms.Path(
#  process.genParticlePlusGEANT *
  process.generalV0Candidates* 
  process.tree*
  process.nEvTotal *
  process.InitialProducer * 
  process.lambdaKshortFilter *
  process.nEvLambdaKshort *
  process.lambdaKshortVertexFilter *
  process.nEvLambdaKshortVertex *
  process.rMassFilter *
  process.sMassFilter *
  process.nEvSMass 
#  process.tree 
)

# Output --> not used in the analyzer
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('preFilterInfo.root'), 
)

#Keep edm output file --> used in the analyzer
process.out = cms.OutputModule("PoolOutputModule",
  outputCommands = collections_to_keep,
   fileName = cms.untracked.string("events_skimmed.root"),
    SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('p')
  )
)

process.output_step = cms.EndPath(process.out)


#iFileName = "configDump_cfg.py"
#file = open(iFileName,'w')
#file.write(str(process.dumpPython()))
#file.close()
