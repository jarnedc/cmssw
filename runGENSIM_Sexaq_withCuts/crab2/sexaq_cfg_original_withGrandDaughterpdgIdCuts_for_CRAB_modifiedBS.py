# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/SUS-RunIISummer15GS-00146-fragment.py --filein=file:./SIMP_M100.lhe --fileout file:SIMP100_GENSIM.root --mc --eventcontent RAWSIM --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM --conditions MCRUN2_71_V1::All --beamspot Realistic50ns13TeVCollision --step GEN,SIM --magField 38T_PostLS1 --python_filename SUS-RunIISummer15GS-00146_1_cfg.py --no_exec -n 481
import FWCore.ParameterSet.Config as cms
import sys
from FWCore.ParameterSet.VarParsing import VarParsing

process = cms.Process('SIM')
options = VarParsing ('analysis')
options.parseArguments()

#file_number = (sys.argv[1])


# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
#process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.Geometry.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi_Jarne2')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/crmc/Sexaquark_13TeV_trial10_scalingNumAntiS/crmc_Sexaq_1.root'),
    fileNames = cms.untracked.vstring(options.inputFiles),
    skipEvents = cms.untracked.uint32(0),
    duplicateCheckMode = cms.untracked.string ("noDuplicateCheck")
)



# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('Configuration/GenProduction/python/SUS-RunIISummer15GS-00146-fragment.py nevts:481'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
 #   fileName = cms.untracked.string('file:/user/jdeclerc/CMSSW_7_1_20_patch2/src/runGENSIM_Sexaq_withCuts/crab/OutputRootFiles/sexaq_GENSIM_1.root'),
    fileName = cms.untracked.string(options.outputFile),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('masterSequence_step')
    )
)

# Additional output definition
process.RAWSIMoutput.outputCommands += ("keep *_genParticlesPlusGEANT_*_*",)

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_71_V1::All', '')

#process.generator = cms.EDFilter("Pythia8HadronizerFilter",
#    pythiaPylistVerbosity = cms.untracked.int32(1),
#    filterEfficiency = cms.untracked.double(1.0),
#    pythiaHepMCVerbosity = cms.untracked.bool(False),
#    comEnergy = cms.double(13000.0),
#    maxEventsToPrint = cms.untracked.int32(1),
#    PythiaParameters = cms.PSet(
#        pythia8CommonSettings = cms.vstring('Tune:preferLHAPDF = 2', 
#            'Main:timesAllowErrors = 10000', 
#            'Check:epTolErr = 0.01', 
#            'Beams:setProductionScalesFromLHEF = off', 
#            'SLHA:keepSM = on', 
#            'SLHA:minMassSM = 1000.', 
#            'ParticleDecays:limitTau0 = on', 
#            'ParticleDecays:tau0Max = 10', 
#            'ParticleDecays:allowPhotonRadiation = on'),
#        pythia8CUEP8M1Settings = cms.vstring('Tune:pp 14', 
#            'Tune:ee 7', 
#            'MultipartonInteractions:pT0Ref=2.4024', 
#            'MultipartonInteractions:ecmPow=0.25208', 
#            'MultipartonInteractions:expPow=1.6'),
#        processParameters = cms.vstring('JetMatching:setMad = off', 
#            'JetMatching:scheme = 1', 
#            'JetMatching:merge = off',
#            'JetMatching:jetAlgorithm = 2', 
#            'JetMatching:etaJetMax = 5.', 
#            'JetMatching:coneRadius = 1.', 
#            'JetMatching:slowJetPower = 1', 
#            'JetMatching:qCut = 14.', 
#            'JetMatching:nQmatch = 5', 
#            'JetMatching:nJetMax = 4', 
#            'JetMatching:doShowerKt = off'),
#        parameterSets = cms.vstring('pythia8CommonSettings', 
#            'pythia8CUEP8M1Settings', 
#            'processParameters')
#    )
#)

process.genParticlesPlusGEANT = cms.EDProducer("GenPlusSimParticleProducer",
  src           = cms.InputTag("g4SimHits"),
  setStatus     = cms.int32(8),                # set status = 8 for GEANT GPs
  particleTypes = cms.vstring(),
  filter        = cms.vstring(),
  genParticles  = cms.InputTag("genParticles") # original genParticle list
)


process.g4SimHits.Physics.type = cms.string('SimG4Core/Physics/CustomPhysics')
process.g4SimHits.Physics.Verbosity = 1
process.g4SimHits.Physics = cms.PSet(
  process.g4SimHits.Physics, #keep all default value and add others
  particlesDef = cms.FileInPath('SimG4Core/CustomPhysics/data/particles_sexaq_1p8_GeV.txt'),
)
#process.g4SimHits.Generator.ApplyEtaCuts = cms.bool(False)


process.g4SimHits.HepMCProductLabel = cms.InputTag("source")
process.g4SimHits.Generator.HepMCProductLabel = cms.string("source")
#process.VtxSmeared.src = cms.InputTag("source")
process.VtxSmeared = cms.EDProducer("BetafuncEvtVtxGenerator",
    Phi = cms.double(0.0),
    BetaStar = cms.double(65.0),
    Emittance = cms.double(5.411e-08),
    Alpha = cms.double(0.0),
    SigmaZ = cms.double(4.145),
    TimeOffset = cms.double(0.0),
    X0 = cms.double(-0.06597),
    Y0 = cms.double(0.0805),
    Z0 = cms.double(0.07968),
    src = cms.InputTag("source","unsmeared"),
    readDB = cms.bool(False)
)



process.genParticles.src = cms.InputTag("source")


#temporary cut which asks for exactly one antiS to be present in the event:
#process.antiSeventFilter = cms.EDFilter("CandViewSelector",
#     src = cms.InputTag("genParticles"),
#     cut = cms.string("pdgId == -1020000020")
#     )
#process.NumantiSeventFilter = cms.EDFilter("CandViewCountFilter",
#    src = cms.InputTag("antiSeventFilter"),
#    minNumber = cms.uint32(1),
#    maxNumber = cms.uint32(1) ####THIS MAX NUMBER DOES NOT DO ANYTHING!!!!!!!!!! YOU WOULD HAVE TO ADAPTH THE FILTER YOURSELF TO MAKE THIS WORK
#  )

#make a cut on the daughters of the antiS: V0s which are created larger than 20cm should be cut (normally you could put the interaction cross section high, so they would all interact)
process.genAntiSGranddaughterFilterKs = cms.EDFilter("CandViewSelector",
     src = cms.InputTag("genParticlesPlusGEANT"),
     cut = cms.string("abs(pdgId) == 310 && mother(0).pdgId == -1020000020 && sqrt(vx*vx+vy*vy) < 20")
     )
process.NumgenAntiSGranddaughterFilterKs = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("genAntiSGranddaughterFilterKs"),
    minNumber = cms.uint32(1),
  )

process.genAntiSGranddaughterFilterAntiLambda = cms.EDFilter("CandViewSelector",
     src = cms.InputTag("genParticlesPlusGEANT"),
     cut = cms.string("pdgId == -3122 && mother(0).pdgId == -1020000020 && sqrt(vx*vx+vy*vy) < 20")
     )
process.NumgenAntiSGranddaughterFilterAntiLambda = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("genAntiSGranddaughterFilterAntiLambda"),
    minNumber = cms.uint32(1),
  )

#cut on the precense of the two daughters
process.genAntiSGranddaughterFilterKsAntiLambdaPresent = cms.EDFilter("CandViewSelector",
     src = cms.InputTag("genParticlesPlusGEANT"),
     cut = cms.string(" pdgId == -1020000020 && numberOfDaughters() == 2 ")
     )
process.NumgenAntiSGranddaughterFilterKsAntiLambdaPresent = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("genAntiSGranddaughterFilterKsAntiLambdaPresent"),
    minNumber = cms.uint32(1),
  )

#cut on the angular separation between the two daughters: if deltaR(Ks,L) is larger than 3 it looks for sure like background
#process.genAntiSGranddaughterFilterDeltaRKsAntiLambda = cms.EDFilter("CandViewSelector",
#     src = cms.InputTag("genParticlesPlusGEANT"),
#     cut = cms.string("pdgId == -1020000020 && deltaR( daughter(0).eta(),daughter(0).phi(),daughter(1).eta(),daughter(1).phi() ) < 3")
#     )
#process.NumgenAntiSGranddaughterFilterDeltaRKsAntiLambda = cms.EDFilter("CandViewCountFilter",
#    src = cms.InputTag("genAntiSGranddaughterFilterDeltaRKsAntiLambda"),
#    minNumber = cms.uint32(1),
#  )


#make a cut on the granddaughters of the antiS: particles with too low momenta anyway have a reco efficiency close to 0 and also too large displaced particles have a reco efficiency of 0
process.genAntiSGranddaughterFilterKsPiMin = cms.EDFilter("CandViewSelector",
     src = cms.InputTag("genParticlesPlusGEANT"),
     cut = cms.string("pdgId == -211 &&  abs(mother(0).pdgId) == 310 && mother(0).mother(0).pdgId == -1020000020 ")
     )
process.NumgenAntiSGranddaughterFilterKsPiMin = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("genAntiSGranddaughterFilterKsPiMin"),
    minNumber = cms.uint32(1),
  )

process.genAntiSGranddaughterFilterKsPiPlus = cms.EDFilter("CandViewSelector",
     src = cms.InputTag("genParticlesPlusGEANT"),
     cut = cms.string("pdgId == 211 &&  abs(mother(0).pdgId) == 310 && mother(0).mother(0).pdgId == -1020000020 ")
     )
process.NumgenAntiSGranddaughterFilterKsPiPlus = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("genAntiSGranddaughterFilterKsPiPlus"),
    minNumber = cms.uint32(1),
  )

process.genAntiSGranddaughterFilterAntiLambdaPiPlus = cms.EDFilter("CandViewSelector",
     src = cms.InputTag("genParticlesPlusGEANT"),
     cut = cms.string("pdgId == 211 &&  mother(0).pdgId == -3122 && mother(0).mother(0).pdgId == -1020000020 ")
     )
process.NumgenAntiSGranddaughterFilterAntiLambdaPiPlus = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("genAntiSGranddaughterFilterAntiLambdaPiPlus"),
    minNumber = cms.uint32(1),
  )

process.genAntiSGranddaughterFilterAntiLambdaAntiProton = cms.EDFilter("CandViewSelector",
     src = cms.InputTag("genParticlesPlusGEANT"),
     cut = cms.string("pdgId == -2212 &&  mother(0).pdgId == -3122 && mother(0).mother(0).pdgId == -1020000020 ")
     )
process.NumgenAntiSGranddaughterFilterAntiLambdaAntiProton = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("genAntiSGranddaughterFilterAntiLambdaAntiProton"),
    minNumber = cms.uint32(1),
  )

# end of: make a cut on the granddaughters of the antiS: particles with too low momenta anyway have a reco efficiency close to 0 and also too large displaced particles have a reco efficiency of 1



################################################################
process.printTree = cms.EDAnalyzer("ParticleListDrawer",
  maxEventsToPrint = cms.untracked.int32(1),
  printVertex = cms.untracked.bool(True),
  printOnlyHardInteraction = cms.untracked.bool(False), # Print only status=3 particles. This will not work for Pythia8, which does not have any such particles.
#  src = cms.InputTag("genParticlesPlusGEANT","","SIM")
  src = cms.InputTag("genParticlesPlusGEANT")
)
process.printTree_step = cms.Path(process.printTree)
###############################################################



process.masterSequence = cms.Sequence(
process.pgen*
process.psim*process.genParticlesPlusGEANT*
process.printTree

#process.genAntiSGranddaughterFilterKsPiMin*process.NumgenAntiSGranddaughterFilterKsPiMin*
#process.genAntiSGranddaughterFilterKsPiPlus*process.NumgenAntiSGranddaughterFilterKsPiPlus*
#process.genAntiSGranddaughterFilterAntiLambdaPiPlus*process.NumgenAntiSGranddaughterFilterAntiLambdaPiPlus*
#process.genAntiSGranddaughterFilterAntiLambdaAntiProton*process.NumgenAntiSGranddaughterFilterAntiLambdaAntiProton
)
process.masterSequence_step = cms.Path(process.masterSequence)



process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)


process.schedule = cms.Schedule(process.masterSequence_step,process.endjob_step,process.RAWSIMoutput_step)


# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1(process)

# End of customisation functions

process.options = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)
