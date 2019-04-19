import FWCore.ParameterSet.Config as cms

process = cms.Process("MULTITRACKVALIDATOR")

# message logger
process.MessageLogger = cms.Service("MessageLogger",
     default = cms.untracked.PSet( limit = cms.untracked.int32(10) )
)


#Adding SimpleMemoryCheck service:
process.SimpleMemoryCheck=cms.Service("SimpleMemoryCheck",
                                   ignoreTotal=cms.untracked.int32(1),
                                   oncePerEventMode=cms.untracked.bool(True)
)

process.Timing = cms.Service("Timing"
    ,summaryOnly = cms.untracked.bool(True)
)

# source
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
#source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
source = cms.Source ("PoolSource",fileNames = readFiles,duplicateCheckMode = cms.untracked.string ("noDuplicateCheck"))
readFiles.extend( [

'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_38.root',   
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_127.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_121.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_63.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_12.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_117.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_298.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_4.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_23.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_27.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_166.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_39.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_74.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_58.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_40.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_41.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_16.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_20.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_37.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_283.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_111.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_123.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_107.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_110.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_142.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_139.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_141.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_25.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_49.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_3.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_36.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_102.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_289.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_60.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_197.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_80.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_100.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_45.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_296.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_21.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_6.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_103.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_13.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_44.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_42.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_255.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_33.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_143.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_199.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_122.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_185.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_267.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_280.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_87.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_165.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_46.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_353.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_18.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_9.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_120.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_43.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_189.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_35.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_98.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_118.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_11.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_65.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_34.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_126.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_106.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_56.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_57.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_218.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_156.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_205.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_73.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_343.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_342.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_285.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_71.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_367.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_47.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_198.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_10.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_55.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_76.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_131.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_252.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_187.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_128.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_323.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_208.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_50.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_24.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_191.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_279.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_91.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_159.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_28.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_290.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_79.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_52.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_188.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_368.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_89.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_69.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_8.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_161.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_167.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_362.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_149.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_72.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_324.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_67.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_59.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_124.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_155.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_360.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_147.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_53.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_125.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_7.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_75.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_151.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_153.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_129.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_5.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_68.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_203.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_133.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_254.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_190.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_93.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_183.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_392.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_261.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_88.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_228.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_132.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_70.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_217.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_157.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_97.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_389.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_168.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_250.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_146.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_130.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_278.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_108.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_251.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_315.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_295.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_240.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_245.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_213.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_341.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_301.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_247.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_94.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_86.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_209.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_184.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_105.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_275.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_92.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_77.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_264.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_239.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_300.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_385.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_344.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_288.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_284.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_292.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_358.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_160.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_361.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_210.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_386.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_390.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_412.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_253.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_207.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_214.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_137.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_388.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_268.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_241.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_363.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_318.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_293.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_101.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_222.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_299.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_246.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_230.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_215.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_244.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_176.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_113.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_455.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_282.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_408.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_297.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_262.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_375.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_350.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_352.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_274.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_348.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_393.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_351.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_233.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_281.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_231.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_206.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_152.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_211.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_317.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_333.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_179.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_410.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_186.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_136.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_395.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_236.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_225.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_356.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_326.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_394.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_229.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_365.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_366.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_249.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_354.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_340.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_158.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_456.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_406.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_181.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_177.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_294.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_109.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_192.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_221.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_382.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_175.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_248.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_407.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_115.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_238.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_224.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_138.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_398.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_444.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_446.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_436.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_443.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_409.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_260.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_226.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_223.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_116.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_450.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_99.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_335.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_454.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_322.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_357.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_377.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_134.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_338.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_148.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_399.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_171.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_271.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_193.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_320.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_82.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_272.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_331.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_440.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_135.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_310.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_154.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_339.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_313.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_448.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_78.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_276.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_173.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_15.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_51.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_329.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_172.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_202.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_405.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_387.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_259.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_445.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_372.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_337.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_397.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_401.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_220.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_438.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_402.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_369.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_170.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_404.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_381.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_441.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_307.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_432.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_277.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_84.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_237.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_400.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_383.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_182.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_396.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_376.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_196.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_447.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_83.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_195.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_174.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_269.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_451.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_332.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_439.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_370.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_308.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_379.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_355.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_304.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_347.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_164.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_266.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_273.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_420.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_380.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_219.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_403.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_424.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_319.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_426.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_227.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_421.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_169.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_346.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_430.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_422.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_434.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_425.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_312.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_437.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_419.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_415.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_442.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_413.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_416.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_414.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_423.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_309.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_427.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_180.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_452.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_418.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_311.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_431.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_371.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_303.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_305.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_327.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_306.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_433.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_234.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_428.root',
'dcap://maite.iihe.ac.be/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Step2/Sexaquark_13TeV_trial6_FEVT_GenParticlesPlusGEANTInputTrackingTruthProducer/SUS-RunIISummer16DR80Premix-00068_1_449.root'

                  ] )


secFiles.extend( [
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/041672CC-60AB-E411-B86C-003048FFCC0A.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/06DFB267-64AB-E411-A22D-0025905A60EE.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/0E932222-60AB-E411-952C-0025905A6088.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/1272643C-62AB-E411-969A-0025905A48B2.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/12BCC0BA-60AB-E411-83AA-0025905B85EE.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/2628B0FE-66AB-E411-83C3-0025905A48BC.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/26CA1B5B-61AB-E411-B134-0025905A612A.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/2CDDCDFD-66AB-E411-9ABA-0025905A60AA.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/32E319C0-60AB-E411-B983-0025905B855C.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/349C841F-63AB-E411-8805-0025905A48D8.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/36CBCF5E-61AB-E411-A75C-003048FF86CA.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/36E480F6-5EAB-E411-B272-0025905A48BC.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/4A703881-63AB-E411-A2BA-0025905B858A.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/4C0C75D7-5FAB-E411-9B34-0025905A608E.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/4E0352FF-66AB-E411-8751-0025905B8576.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/4E2491BA-64AB-E411-8F76-0025905B85D8.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/541FAFC2-60AB-E411-8B77-0025905A60A6.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/583B2424-60AB-E411-80B3-0025905B858E.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/58DBD23C-62AB-E411-8F4C-0025905A60EE.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/603F7120-60AB-E411-B0F6-0025905964C4.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/62ECB36D-71AB-E411-8BA6-003048FFD720.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/64BE7CF9-75AB-E411-A588-0025905B85E8.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/68815A64-61AB-E411-99D1-0025905A48BC.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/6EEC89BF-60AB-E411-9D75-0025905B85D8.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/76680CD8-5FAB-E411-99D8-0025905964A2.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/84CE7508-61AB-E411-9124-0025905B85EE.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/84FD8400-76AB-E411-8D13-0025905A612C.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/8670B486-5FAB-E411-A4B1-0025905A60B6.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/88BBBFD3-5FAB-E411-866C-0025905B85B2.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/88CF55D6-62AB-E411-924D-0025905A60B6.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/8A63EA7C-6EAB-E411-A961-0025905B85A2.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/8C717324-60AB-E411-B721-0025905AA9F0.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/9462F7B8-6CAB-E411-A0CC-0025905964B6.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/9E532222-60AB-E411-9353-0025905A60CE.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/A64050C2-60AB-E411-B9B9-0025905A60B0.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/AE05F253-65AB-E411-A528-0025905A60B6.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/AE8613EF-68AB-E411-9283-003048FFD744.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/BC55E7BE-60AB-E411-96A2-0025905A606A.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/CA18EDF3-5EAB-E411-BABF-002618943862.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/D6FF76A7-65AB-E411-8CA0-0025905B8610.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/DCB00F5C-61AB-E411-8A47-003048FFCB96.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/E295E1D2-5FAB-E411-8205-0026189438A9.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/E8821C20-6CAB-E411-8FC1-0025905A6126.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/F27EA4C0-60AB-E411-BD34-003048FFD744.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/F8D4970C-61AB-E411-B866-0025905A48D0.root',
        '/store/relval/CMSSW_7_4_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU25ns_MCRUN2_74_V1-v3/00000/FE40619E-61AB-E411-B453-0025905B858C.root',
        ] )
process.source = source
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

### conditions
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

### standard includes
process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')


### validation-specific includes
#process.load("SimTracker.TrackAssociatorProducers.trackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
process.load("Validation.RecoTrack.cuts_cff")
process.load("Validation.RecoTrack.MultiTrackValidator_cff")
process.load("DQMServices.Components.EDMtoMEConverter_cff")
process.load("Validation.Configuration.postValidation_cff")
process.quickTrackAssociatorByHits.SimToRecoDenominator = 'reco'




########### configuration MultiTrackValidator ########
process.multiTrackValidator.associators = ['quickTrackAssociatorByHits']
#process.cutsRecoTracks.quality = ['','highPurity']
#process.cutsRecoTracks.quality = ['']
#process.multiTrackValidator.label = ['cutsRecoTracks']
process.multiTrackValidator.histoProducerAlgoBlock.useLogPt = True
process.multiTrackValidator.histoProducerAlgoBlock.minPt = 0.1
process.multiTrackValidator.histoProducerAlgoBlock.maxPt = 3000.0
process.multiTrackValidator.histoProducerAlgoBlock.nintPt = 40
process.multiTrackValidator.UseAssociators = True


#process.load("Validation.RecoTrack.cuts_cff")
#process.cutsRecoTracks.quality = ['highPurity']
#process.cutsRecoTracks.ptMin    = 0.5
#process.cutsRecoTracks.minHit   = 10
#process.cutsRecoTracks.minRapidity  = -1.0
#process.cutsRecoTracks.maxRapidity  = 1.0

process.quickTrackAssociatorByHits.useClusterTPAssociation = True
process.load("SimTracker.TrackerHitAssociation.tpClusterProducer_cfi")

process.validation = cms.Sequence(
    process.tpClusterProducer *
    process.quickTrackAssociatorByHits *
    process.multiTrackValidator
)

# paths
process.val = cms.Path(
      process.cutsRecoTracks
    * process.validation
)

# Output definition
process.DQMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.DQMEventContent.outputCommands,
    fileName = cms.untracked.string('file:MTV_inDQM.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('DQM')
    )
)

process.endjob_step = cms.EndPath(process.endOfProcess)
process.DQMoutput_step = cms.EndPath(process.DQMoutput)


process.schedule = cms.Schedule(
      process.val,process.endjob_step,process.DQMoutput_step
)

process.options = cms.untracked.PSet(
    numberOfThreads = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(1),
    wantSummary = cms.untracked.bool(True)
)

process.TFileService = cms.Service('TFileService',
    fileName = cms.string('analyzed_sexaq_Step2_trackAssociator.root')
)

