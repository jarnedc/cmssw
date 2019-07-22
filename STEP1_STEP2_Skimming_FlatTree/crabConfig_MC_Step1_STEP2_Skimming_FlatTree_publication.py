from WMCore.Configuration import Configuration

day = "20072019"
version = "v1"

config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'Step1_Step2_Skimming_FlatTree_publication_trialPublication_'+day+'_'+version 

config.section_('JobType') 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'master_STEP1_STEP2_Skimming_FlatTree_cfg.py' 

config.section_('Data') 
config.Data.unitsPerJob = 1 
config.Data.totalUnits = 50
config.Data.publication = True 
config.Data.splitting = 'FileBased' 
config.Data.outLFNDirBase = '/store/user/jdeclerc/crmc_Sexaq/Skimmed' 
#config.Data.userInputFiles = open('/storage_mnt/storage/user/jdeclerc/CMSSW_8_0_30/src/STEP2_Sexaq/crabSlimmedFEVT/inputFiles.txt').readlines()
config.Data.inputDataset = '/CRAB_SimSexaq_withSLoopingSpherical_120cmborder_neutronXS_0p1_inputFiles_trial_for_publication/jdeclerc-crab_SIMSexaq_19072019_v1-33065c3a8a979241c5e811e20eaee23d/USER'
config.Data.inputDBS = 'phys03'
#config.Data.outputPrimaryDataset = "crab_Step2SexaqWithPU2016NeutrinoGun_tryToFix_8_trial11"
config.Data.ignoreLocality = True


config.section_('User') 
#config.User.voGroup = 'becms'

config.section_('Site') 
config.Site.whitelist =['T2_AT_Vienna','T2_BE_UCL','T2_CH_CERN_HLT','T2_DE_DESY','T2_DE_RWTH','T2_FR_IPHC'] 
config.Site.storageSite = 'T2_BE_IIHE'
