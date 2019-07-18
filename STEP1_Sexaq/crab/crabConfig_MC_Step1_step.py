from WMCore.Configuration import Configuration

day = "14072019"
version = "v4"

config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'Step1SexaqWithPU2016NeutrinoGun_tryToFix_8_'+day+'_'+version 

config.section_('JobType') 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'step1_noSexaq_bad_efficiency_tryToFix_8.py' 
config.JobType.numCores = 1

config.section_('Data') 
config.Data.unitsPerJob = 1 
config.Data.totalUnits = 10000
config.Data.publication = False 
config.Data.splitting = 'FileBased' 
config.Data.outLFNDirBase = '/store/user/jdeclerc/crmc_Sexaq/Step1' 
config.Data.userInputFiles = open('/user/jdeclerc/CMSSW_8_0_30/src/STEP1_Sexaq/crab/inputFiles_Trial11.txt').readlines() 
config.Data.outputPrimaryDataset = "CRAB_SimSexaqWithPU2016NeutrinoGun_tryToFix_8"

config.section_('User') 
config.User.voGroup = 'becms'

config.section_('Site') 
config.Site.whitelist = ['T2_BE_IIHE'] 
config.Site.storageSite = 'T2_BE_IIHE'
