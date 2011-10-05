#!/bin/bash
date
source /uscmst1/prod/sw/cms/shrc prod


CMSSW=/uscms_data/d2/mrenna/devel/CMSSW_4_2_3

workerspace=$PWD

cd $CMSSW/src
eval `scramv1 runtime -sh`
cd $workerspace


jobno=`expr $jobsize \\* $1`

echo $jobno

python make_msugra_slha.py $jobno $jobsize $outputDirectory

rm *.py

date
pwd
#---------------------------------------------------------------------------------
