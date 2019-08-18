from WMCore.Configuration import Configuration

day = "18082019"
version = "v6"

config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'SkimmingSexaq_DoubleMuonData_'+day+'_'+version 
#config.General.requestName = 'SkimmingSexaq_ZJetsMC_'+day+'_'+version 

config.section_('JobType') 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'treeproducer_data_cfg.py' 

config.section_('Data') 
config.Data.unitsPerJob = 1
config.Data.totalUnits = 3978
config.Data.publication = False 
config.Data.splitting = 'FileBased' 
config.Data.outLFNDirBase = '/store/user/jdeclerc/crmc_Sexaq/Skimmed/DataMCComparison' 
#config.Data.userInputFiles = open('/user/jdeclerc/CMSSW_8_0_30/src/SexaQAnalysis/RunManySkimming/crab/inputFiles.txt').readlines() 
#config.Data.inputDataset = '/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.inputDataset = '/SingleMuon/Run2016B-07Aug17_ver2-v1/AOD'

#config.Data.inputDataset = '/MinBias_TuneCUETP8M1_13TeV-pythia8/RunIISummer16FSPremix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v4-v1/GEN-SIM-RECO'
#config.Data.inputDataset = '/MinimumBias/PARun2016B-PromptReco-v1/RECO'

#config.Data.inputDataset = '/ttZJets_13TeV_madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
#config.Data.inputDataset = '/ttZJets_13TeV_madgraphMLM/RunIISummer16DR80-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM'
config.Data.inputDataset = '/DoubleMuon/Run2016H-07Aug17-v1/AOD'


config.Data.inputDBS = 'global'
#config.Data.outputPrimaryDataset = "CRAB_SimSexaq_Skimming_trial13_"+day+"_"+version
config.Data.ignoreLocality = True

config.section_('User') 
#config.User.voGroup = 'becms'

config.section_('Site') 
config.Site.whitelist = ['T2_BE_IIHE','T2_AT_Vienna','T2_BE_UCL','T2_CH_CERN_HLT','T2_DE_DESY','T2_DE_RWTH','T2_FR_IPHC','T1_DE_KIT','T1_UK_RAL','T2_HU_Budapest','T2_IT_Bari','T2_IT_Legnaro','T2_IT_Pisa'] 
config.Site.storageSite = 'T2_BE_IIHE'
