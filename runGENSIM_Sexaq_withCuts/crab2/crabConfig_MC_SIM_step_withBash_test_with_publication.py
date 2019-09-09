from WMCore.Configuration import Configuration

day = "07092019"
version = "v1"

config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'SIMSexaq_trial17_'+day+'_'+version 

config.section_('JobType') 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'sexaq_cfg_original_withGrandDaughterpdgIdCuts_for_CRAB.py' 
config.JobType.priority = 100

config.section_('Data') 
config.Data.unitsPerJob = 1 
config.Data.totalUnits = 10000
config.Data.publication = True 
config.Data.splitting = 'FileBased' 
config.Data.outLFNDirBase = '/store/user/jdeclerc/crmc_Sexaq/GENSIM' 
config.Data.userInputFiles = open('/user/jdeclerc/CMSSW_7_1_20_patch2/src/runGENSIM_Sexaq_withCuts/crab2/inputFiles_trial17.txt').readlines() 
config.Data.outputPrimaryDataset = "CRAB_SimSexaq_trial17"
config.Data.ignoreLocality = True


config.section_('User') 
#config.User.voGroup = 'becms'

config.section_('Site') 
config.Site.whitelist =['T2_BE_IIHE','T2_AT_Vienna','T2_BE_UCL','T2_CH_CERN_HLT','T2_DE_DESY','T2_DE_RWTH','T2_FR_IPHC','T1_DE_KIT','T1_UK_RAL','T2_HU_Budapest','T2_IT_Bari','T2_IT_Legnaro','T2_IT_Pisa'] 
config.Site.storageSite = 'T2_BE_IIHE'
