from WMCore.Configuration import Configuration

day = "01082019"
version = "v4"

config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'SIMSexaq_trial13_test_'+day+'_'+version 

config.section_('JobType') 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'sexaq_cfg_original_withGrandDaughterpdgIdCuts_for_CRAB.py' 

config.section_('Data') 
config.Data.unitsPerJob = 1 
config.Data.totalUnits = 10 
config.Data.publication = False 
config.Data.splitting = 'FileBased' 
config.Data.outLFNDirBase = '/store/user/jdeclerc/crmc_Sexaq/GENSIM' 
config.Data.userInputFiles = open('/user/jdeclerc/CMSSW_7_1_20_patch2/src/runGENSIM_Sexaq_withCuts/crab2/inputFilesTrial13Test.txt').readlines() 
config.Data.outputPrimaryDataset = "CRAB_SimSexaq_trial13"

config.section_('User') 
config.User.voGroup = 'becms'

config.section_('Site') 
config.Site.whitelist = ['T2_BE_IIHE'] 
config.Site.storageSite = 'T2_BE_IIHE'
