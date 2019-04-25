from WMCore.Configuration import Configuration

day = "24042019"
version = "v1"

config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'SIMBackground_'+day+'_'+version 

config.section_('JobType') 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'sexaq_cfg_original_withoutGrandDaughterpdgIdCuts_for_CRAB.py' 

config.section_('Data') 
config.Data.unitsPerJob = 1 
config.Data.totalUnits = 2450
config.Data.publication = False 
config.Data.splitting = 'FileBased' 
config.Data.outLFNDirBase = '/store/user/jdeclerc/crmc_Sexaq/GENSIM' 
config.Data.userInputFiles = open('/user/jdeclerc/CMSSW_7_1_20_patch2/src/runGENSIM_Sexaq_withCuts/crabBackground/inputFiles.txt').readlines() 
config.Data.outputPrimaryDataset = "CRAB_SimSexaq_Background"

config.section_('User') 
config.User.voGroup = 'becms'

config.section_('Site') 
config.Site.whitelist = ['T2_BE_IIHE'] 
config.Site.storageSite = 'T2_BE_IIHE'
