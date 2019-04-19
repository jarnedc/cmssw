#!/bin/bash

basedir=PPWWDD

source $VO_CMS_SW_DIR/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530
cd /user/jdeclerc/CMSSW_8_0_30_bis/CMSSW_8_0_30/src; eval `scram runtime -sh` ; cd - >/dev/null
export x509_user_proxy=/user/$user/ckValidator_cfg.px509up_u$(id -u $user)
voms-proxy-init -pwstdin < /user/jdeclerc/analysis/sexaquark/cmssw_9_4_7/src/sexaqanalysis/runmanyanalyzermc/password
tmpdir=$tmpdir
mkdir -p $tmpdir
cmsRun /user/jdeclerc/CMSSW_8_0_30_bis/CMSSW_8_0_30/src/Validation/RecoTrack/test/MultiTrackValidator_cfg_run7.py > /user/jdeclerc/CMSSW_8_0_30_bis/CMSSW_8_0_30/src/Validation/RecoTrack/test/MultiTrackValidator7.log > /user/jdeclerc/CMSSW_8_0_30_bis/CMSSW_8_0_30/src/Validation/RecoTrack/test/MultiTrackValidator7Log2
#cmsRun /user/jdeclerc/CMSSW_8_0_30_bis/CMSSW_8_0_30/src/Validation/RecoTrack/test/MultiTrackValidator_cfg.py > /user/jdeclerc/CMSSW_8_0_30_bis/CMSSW_8_0_30/src/Validation/RecoTrack/test/MultiTrackValidator6.log > /user/jdeclerc/CMSSW_8_0_30_bis/CMSSW_8_0_30/src/Validation/RecoTrack/test/MultiTrackValidator6Log2
