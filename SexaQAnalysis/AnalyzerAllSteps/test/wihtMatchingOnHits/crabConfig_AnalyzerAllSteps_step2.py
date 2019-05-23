from WMCore.Configuration import Configuration

day = "23052019"
version = "v5"

config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'AnalyzerAllStepsDataSkimmed_'+day+'_'+version 

config.section_('JobType') 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'analyzerAllSteps_cfg.py' 

config.section_('Data') 
config.Data.unitsPerJob = 1
config.Data.totalUnits = 306
config.Data.publication = False 
config.Data.splitting = 'FileBased' 
#config.Data.outLFNDirBase = '/store/user/jdeclerc/crmc_Sexaq/Analyzed_Skimmed' 
config.Data.outLFNDirBase = '/store/user/jdeclerc/data_Sexaq/Analyzed/trialG' 
config.Data.userInputFiles = open('/user/jdeclerc/CMSSW_8_0_30/src/SexaQAnalysis/AnalyzerAllSteps/test/wihtMatchingOnHits/inputFiles.txt').readlines() 
#config.Data.outputPrimaryDataset = "CRAB_AnalyzerAllSkimmed_Sexaq_disabled_cosThetaXYCut_innerHitPosCut_"+day+'_'+version
config.Data.outputPrimaryDataset = "CRAB_AnalyzerAllSkimmed_Data_disabled_cosThetaXYCut_innerHitPosCut_"+day+'_'+version

config.section_('User') 
config.User.voGroup = 'becms'

config.section_('Site') 
config.Site.whitelist = ['T2_BE_IIHE'] 
config.Site.storageSite = 'T2_BE_IIHE'
