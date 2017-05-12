import FWCore.ParameterSet.Config as cms

rpcEfficiencyPerRingLayer = cms.EDProducer("RPCEfficiencyPerRingLayer",
                                           GlobalFolder = cms.untracked.string('RPC/RPCEfficiency/'),
                                           NumberOfEndcapDisks  = cms.untracked.int32(4),
                                           NumberOfInnermostEndcapRings  = cms.untracked.int32(2)
                                           )
