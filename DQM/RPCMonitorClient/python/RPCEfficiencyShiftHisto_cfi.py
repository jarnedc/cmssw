import FWCore.ParameterSet.Config as cms

rpcEfficiencyShiftHisto = cms.EDProducer("RPCEfficiencyShiftHisto",
   EffCut = cms.untracked.int32(90),
   GlobalFolder = cms.untracked.string('RPC/RPCEfficiency/'),
   NumberOfEndcapDisks = cms.untracked.int32(4)
)
