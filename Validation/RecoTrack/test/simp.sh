#!/bin/bash

basedir=PPWWDD

source $VO_CMS_SW_DIR/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530
cd /user/jdeclerc/CMSSW_8_0_30_bis/CMSSW_8_0_30/src; eval `scram runtime -sh` ; cd - >/dev/null
export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)
voms-proxy-init -pwstdin < /user/jdeclerc/Analysis/SexaQuark/CMSSW_9_4_7/src/SexaQAnalysis/RunManyAnalyzerMC/password
tmpdir=$TMPDIR
mkdir -p $tmpdir
cp $basedir/analyzer_AOD_IIDDD_cfg.py $tmpdir
cd $tmpdir
cmsRun /user/jdeclerc/CMSSW_8_0_30_bis/CMSSW_8_0_30/src/Validation/RecoTrack/test/RunManyMultiTrackValidator/analyzer_AOD_IDDD_cfg.py > JobIDDD.log 2>&1

# move job log back
mv JobIDDD.log $basedir
#mv *.std* $basedir # probably useless and done by job scheduler
# make sure folder on storage element exists
gfal-mkdir srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Analyzed/`basename $basedir` 2>/dev/null
# put root output on storage element
for f in `ls *root` ; do
  gfal-copy file://$tmpdir/$f srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/Analyzed/`basename $basedir`/$f
  #rm $f
done
