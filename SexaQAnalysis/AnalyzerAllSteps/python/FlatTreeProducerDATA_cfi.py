import FWCore.ParameterSet.Config as cms
from Validation.RecoTrack.TrackingParticleSelectionForEfficiency_cfi import * 
from SimTracker.TrackAssociation.LhcParametersDefinerForTP_cfi import * 
FlatTreeProducerDATA = cms.EDAnalyzer('FlatTreeProducerDATA',
    lookAtAntiS = cms.untracked.bool(False),
    beamspot = cms.InputTag("offlineBeamSpot"),
    offlinePV = cms.InputTag("offlinePrimaryVertices","","RECO"),
    sexaqCandidates = cms.InputTag("lambdaKshortVertexFilter", "sParticles","SEXAQ"),
    V0KsCollection = cms.InputTag("generalV0Candidates","Kshort","RECO"),
    V0LCollection = cms.InputTag("generalV0Candidates","Lambda","RECO")
)
