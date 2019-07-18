#!/bin/bash

basedir=/user/jdeclerc/CMSSW_7_1_20_patch2/src/runGENSIM_Sexaq_withCuts

source $VO_CMS_SW_DIR/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc481
cd /user/jdeclerc/CMSSW_7_1_20_patch2/src ; eval `scram runtime -sh` ; cd - >/dev/null
export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)
voms-proxy-init -pwstdin < /user/jdeclerc/CMSSW_8_0_30/src/STEP1_Sexaq/password

tmpdir=$TMPDIR
mkdir -p $tmpdir
cp $basedir/SUS-RunIISummer15GS-00146_1_100_cfg.py $tmpdir
cp $basedir/../crmc_Sexaq_combined_test.root $tmpdir
cd $tmpdir
cmsRun ./SUS-RunIISummer15GS-00146_1_100_cfg.py > Job_1_100.log 2>&1

# move job log back
mv Job_1_100.log $basedir
# make sure folder on storage element exists /pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq
gfal-mkdir srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/GENSIM/`basename $basedir` 2>/dev/null
# put root output on storage element
for f in `ls *root` ; do
  gfal-copy file://$tmpdir/$f srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/jdeclerc/crmc_Sexaq/GENSIM/`basename $basedir`/$f
  #rm $f
done
