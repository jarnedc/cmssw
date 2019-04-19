#!/bin/bash

nrevperjob=300

rm script*.std* 2> /dev/null
rm Job*.log 2> /dev/null

for i in {1..1}; do
  for j in {1..100}; do

#    jobname=Job_${i}\_${j}
    nrevskip=$(((j-1)*nrevperjob))

#    export LS_JOBNAME=${jobname}_`date +"%s"`
    export id=${i}
    export id1=${j}
    export direc=$(pwd)
    if [ "`dirname $X509_USER_PROXY`" != "/user/$USER" ] ; then
      cp $X509_USER_PROXY /user/$USER
      export X509_USER_PROXY=/user/$USER/x509up_u$(id -u $USER)
    fi

    cp sexaq_cfg_original.py ./SUS-RunIISummer15GS-00146_IIDDD_cfg.py
    #sed -i "s/LHENR/$id/g" ./SUS-RunIISummer15GS-00146_IIDDD_cfg.py
    sed -i "s/IIDD/$id\_$id1/g" ./SUS-RunIISummer15GS-00146_IIDDD_cfg.py
    sed -i "s/NREV/$nrevperjob/g" ./SUS-RunIISummer15GS-00146_IIDDD_cfg.py
    sed -i "s/SKIP/$nrevskip/g" ./SUS-RunIISummer15GS-00146_IIDDD_cfg.py
    rename IIDDD $id\_$id1 ./SUS-RunIISummer15GS-00146_IIDDD_cfg.py
    cp ./simp.sh ./simptmp.sh
    sed -i "s/IDDD/$id\_$id1/g" ./simptmp.sh
    sed -i "s@PPWWDD@$direc@g" ./simptmp.sh

    qsub -q localgrid -o script_${i}\_${j}.stdout -e script_${i}\_${j}.stderr < simptmp.sh
#    rm ./simptmp.sh

  done
done


