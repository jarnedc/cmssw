#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("GEN")

process.source = cms.Source("MCFileSource",
	fileNames = cms.untracked.vstring('file:../../../crmc_eposlhc_347761894_p_p_6500.hepmc')
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))


process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

process.GEN = cms.OutputModule("PoolOutputModule",
	fileName = cms.untracked.string('HepMC_GEN.root')
)

process.outpath = cms.EndPath(process.GEN)
