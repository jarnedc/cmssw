import FWCore.ParameterSet.Config as cms

tree = cms.EDAnalyzer(
  'TreeProducer_AOD',
  TrackCollection       = cms.InputTag("generalTracks"),
  lambdaCollection = cms.InputTag("generalV0Candidates","Lambda"),
  kshortCollection = cms.InputTag("generalV0Candidates","Kshort"),
  offlinePrimaryVerticesCollection = cms.InputTag("offlinePrimaryVertices"),
  ak4PFJetsCollection = cms.InputTag("ak4PFJets"),
  muonsCollection = cms.InputTag("pfIsolatedMuonsEI"),
  electronsCollection = cms.InputTag("pfIsolatedElectronsEI"),
  METCollection = cms.InputTag("pfMet"),
  )

