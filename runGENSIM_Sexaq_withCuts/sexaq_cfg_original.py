# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/SUS-RunIISummer15GS-00146-fragment.py --filein=file:./SIMP_M100.lhe --fileout file:SIMP100_GENSIM.root --mc --eventcontent RAWSIM --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM --conditions MCRUN2_71_V1::All --beamspot Realistic50ns13TeVCollision --step GEN,SIM --magField 38T_PostLS1 --python_filename SUS-RunIISummer15GS-00146_1_cfg.py --no_exec -n 481
import FWCore.ParameterSet.Config as cms

process = cms.Process('SIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.Geometry.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(NREV)
)

# Input source
#process.source = cms.Source("LHESource",
#    fileNames = cms.untracked.vstring('file:./sexaqtest.lhe'),
#    inputCommands = cms.untracked.vstring('keep *', 
#        'drop LHEXMLStringProduct_*_*_*'),
#    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
#    skipEvents = cms.untracked.uint32(0),
#)
#process.source = cms.Source("MCFileSource",
#    fileNames = cms.untracked.vstring('file:crmc_eposlhc_347761894_p_p_6500.hepmc'),
#)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/GENSIM/Sexaquark_13TeV_trial8/combined/crmc_Sexaq_GENSIM_combined.root'),
    skipEvents = cms.untracked.uint32(SKIP),
    duplicateCheckMode = cms.untracked.string ("noDuplicateCheck")
)


process.options = cms.untracked.PSet(

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
    fileName = cms.untracked.string('file:/user/jdeclerc/CMSSW_7_1_20_patch2/src/runGENSIM_Sexaq/sexaq_GENSIM_IIDD.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition
process.RAWSIMoutput.outputCommands += ("keep *_genParticlesPlusGEANT_*_*",)

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_71_V1::All', '')

process.generator = cms.EDFilter("Pythia8HadronizerFilter",
    pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(1.0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(13000.0),
    maxEventsToPrint = cms.untracked.int32(1),
    PythiaParameters = cms.PSet(
        pythia8CommonSettings = cms.vstring('Tune:preferLHAPDF = 2', 
            'Main:timesAllowErrors = 10000', 
            'Check:epTolErr = 0.01', 
            'Beams:setProductionScalesFromLHEF = off', 
            'SLHA:keepSM = on', 
            'SLHA:minMassSM = 1000.', 
            'ParticleDecays:limitTau0 = on', 
            'ParticleDecays:tau0Max = 10', 
            'ParticleDecays:allowPhotonRadiation = on'),
        pythia8CUEP8M1Settings = cms.vstring('Tune:pp 14', 
            'Tune:ee 7', 
            'MultipartonInteractions:pT0Ref=2.4024', 
            'MultipartonInteractions:ecmPow=0.25208', 
            'MultipartonInteractions:expPow=1.6'),
        processParameters = cms.vstring('JetMatching:setMad = off', 
            'JetMatching:scheme = 1', 
            'JetMatching:merge = off',
            'JetMatching:jetAlgorithm = 2', 
            'JetMatching:etaJetMax = 5.', 
            'JetMatching:coneRadius = 1.', 
            'JetMatching:slowJetPower = 1', 
            'JetMatching:qCut = 14.', 
            'JetMatching:nQmatch = 5', 
            'JetMatching:nJetMax = 4', 
            'JetMatching:doShowerKt = off'),
        parameterSets = cms.vstring('pythia8CommonSettings', 
            'pythia8CUEP8M1Settings', 
            'processParameters')
    )
)

process.genParticlesPlusGEANT = cms.EDProducer("GenPlusSimParticleProducer",
  src           = cms.InputTag("g4SimHits"),
  setStatus     = cms.int32(8),                # set status = 8 for GEANT GPs
  particleTypes = cms.vstring(),
  filter        = cms.vstring(),
  genParticles  = cms.InputTag("genParticles") # original genParticle list
)


process.g4SimHits.Physics.type = cms.string('SimG4Core/Physics/CustomPhysics')
process.g4SimHits.Physics.Verbosity = 1
#process.g4SimHits.G4TrackingManagerVerbosity = 1
#process.g4SimHits.G4EventManagerVerbosity = 1
process.g4SimHits.Physics = cms.PSet(
  process.g4SimHits.Physics, #keep all default value and add others
  particlesDef = cms.FileInPath('SimG4Core/CustomPhysics/data/particles_sexaq_1p8_GeV.txt'),
)


process.g4SimHits.HepMCProductLabel = cms.InputTag("source")
process.g4SimHits.Generator.HepMCProductLabel = cms.string("source")
process.VtxSmeared.src = cms.InputTag("source")
process.genParticles.src = cms.InputTag("source")


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim*process.genParticlesPlusGEANT)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
#process.schedule = cms.Schedule(process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
# filter all path with the production filter sequence
#for path in process.paths:
#	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

# customisation of the process.

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
