import FWCore.ParameterSet.Config as cms

rpcChamberQuality = cms.EDProducer("RPCChamberQuality",
                                   OfflineDQM = cms.untracked.bool(True),
                                   PrescaleFactor  = cms.untracked.int32(5),
                                   NumberOfEndcapDisks  = cms.untracked.int32(4),
                                   MinimumRPCEvents = cms.untracked.int32(10000),
                                   RecHitTypeFolder = cms.untracked.string("AllHits")
                                   )


rpcMuonChamberQuality = cms.EDProducer("RPCChamberQuality",
                                       OfflineDQM = cms.untracked.bool(True),
                                       PrescaleFactor  = cms.untracked.int32(5),
                                       NumberOfEndcapDisks  = cms.untracked.int32(4),
                                       MinimumRPCEvents = cms.untracked.int32(10000),
                                       RecHitTypeFolder = cms.untracked.string("Muon")
                                       )
