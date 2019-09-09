#!/usr/bin/env cmsRun

## Original Author: Andrea Carlo Marini
## Porting to 92X HepMC 2 Gen 
## Date of porting: Mon Jul  3 11:52:22 CEST 2017
## Example of hepmc -> gen file
import os,sys
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
#options.inputFiles = 'file:/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Sexaquark_13TeV_trial2/crmc_Sexaq_100.hepmc', 'file:/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Sexaquark_13TeV_trial2/crmc_Sexaq_102.hepmc'
options.parseArguments()


if True:
    print '-> You are using a 2 process file to unzip/untar events on the fly'
    #cmd = "mkfifo /tmp/amarini/hepmc10K.dat"
    #cmd = "cat hepmc10K.dat.gz | gunzip -c > /tmp/amarini/hepmc10K.dat"
    from subprocess import call, check_output
    import threading
    import time
    def call_exe(cmd):
        print "-> Executing cmd: '"+cmd+"'"
        st=call(cmd,shell=True)
        print "-> End cmd: status=",st
        return
    cmd="rm /tmp/"+os.environ['USER']+"/hepmc10K.dat"
    call(cmd,shell=True)
    cmd="mkfifo /tmp/"+os.environ['USER']+"/hepmc10K.dat"
    call(cmd,shell=True)
    exe="cat /tmp/"+os.environ['USER']+"/hepmc.dat.tgz | gunzip -c > /tmp/"+os.environ['USER']+"/hepmc10K.dat &"
    t = threading.Thread(target=call_exe, args= ( [exe] )  )
    t.start()
    print "(sleep 1s to allow start of pipes)"
    time.sleep(1)


import FWCore.ParameterSet.Config as cms

process = cms.Process("GEN")


process.source = cms.Source("MCFileSource",
		    #fileNames = cms.untracked.vstring('file:hepmc100.dat'),
			#fileNames = cms.untracked.vstring('file:../../../crmc_eposlhc_347761894_p_p_6500.hepmc'),
			fileNames = cms.untracked.vstring(options.inputFiles),
			)


maxEvents=options.maxEvents
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvents))


process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(500)

process.GEN = cms.OutputModule("PoolOutputModule",
		#fileName = cms.untracked.string("HepMC_Gen.root"),
		fileName = cms.untracked.string(options.outputFile),
        	SelectEvents = cms.untracked.PSet(
                SelectEvents = cms.vstring('p')
                )
	)



process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.genParticles.src= cms.InputTag("source")


#	process.antisexaqev = cms.EDFilter("PdgIdCandViewSelector",
#	    src = cms.InputTag("genParticles"),
#	    pdgId = cms.vint32(-1020000020)  # replaced in loop
#	)
#
#	process.NumAntiSexaqevFilter = cms.EDFilter("CandViewCountFilter",
#	    src = cms.InputTag("antisexaqev"),
#	    minNumber = cms.uint32(1),
#	  )
#


process.fiducial = cms.EDFilter("EtaPtMinPdgIdCandSelector",
  src = cms.InputTag("genParticles","","GEN"),
  ptMin = cms.double(0.),
  etaMin = cms.double(-999),
  etaMax = cms.double(999),
  pdgId = cms.vint32(-1020000020)
)

process.NumAntiSexaqevFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("fiducial"),
    minNumber = cms.uint32(1),
  )


######### Smearing Vertex example
#stanadard configuration
#from IOMC.EventVertexGenerators.VtxSmearedParameters_cfi import GaussVtxSmearingParameters,VtxSmearedCommon
#VtxSmearedCommon.src=cms.InputTag("source")
#process.generatorSmeared = cms.EDProducer("GaussEvtVtxGenerator",
#    GaussVtxSmearingParameters,
#    VtxSmearedCommon
#    )

#adaped by Jarne:
from IOMC.EventVertexGenerators.VtxSmearedParameters_cfi import Realistic50ns13TeVCollisionVtxSmearingParameters,VtxSmearedCommon
VtxSmearedCommon.src=cms.InputTag("source")
process.generatorSmeared = cms.EDProducer("BetafuncEvtVtxGenerator",
    Realistic50ns13TeVCollisionVtxSmearingParameters,
    VtxSmearedCommon
    )

process.load('Configuration.StandardSequences.Services_cff')
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
        generatorSmeared  = cms.PSet( initialSeed = cms.untracked.uint32(1243987),
            engineName = cms.untracked.string('TRandom3'),
            ),
        )


###################
process.p = cms.Path(process.genParticles * process.generatorSmeared * process.fiducial * process.NumAntiSexaqevFilter )
process.outpath = cms.EndPath(process.GEN)

### TO DO: add the following
# (amarini/hepmc_portTo9X)
# add the following line after the sim and digi loading
# generator needs to be smeared if you want vertex smearing, you'll have:
#       Type                                  Module               Label         Process   
#       -----------------------------------------------------------------------------------
#       GenEventInfoProduct                   "source"             "generator"   "GEN"     
#       edm::HepMCProduct                     "generatorSmeared"   ""            "GEN"     
#       edm::HepMCProduct                     "source"             "generator"   "GEN"   
# NOT needed to be changed if you smear the generator
#process.g4SimHits.HepMCProductLabel = cms.InputTag("source")
#process.g4SimHits.Generator.HepMCProductLabel = cms.InputTag("source")
#process.genParticles.src=  cms.InputTag("source","generator","GEN")


### ADD in the different step the following  (always!)
#
#process.AODSIMoutput.outputCommands.extend([
#		'keep GenRunInfoProduct_*_*_*',
#        	'keep GenLumiInfoProduct_*_*_*',
#		'keep GenEventInfoProduct_*_*_*',
#		])
#
#process.MINIAODSIMoutput.outputcommands.extend([
#       'keep GenRunInfoProduct_*_*_*',
#       'keep GenLumiInfoProduct_*_*_*',
#       'keep GenEventInfoProduct_*_*_*',
#	])
#
# and finally in the ntuples
#process.myanalyzer.generator = cms.InputTag("source","generator")
