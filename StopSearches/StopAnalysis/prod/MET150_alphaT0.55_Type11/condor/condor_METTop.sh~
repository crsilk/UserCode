#!/bin/bash
date

source /uscmst1/prod/sw/cms/shrc prod

workerspace=$PWD
cd $CMSSW/src/
eval `scramv1 runtime -sh`
cd $workerspace


cp $CMSSW/src/Analysis/StopAnalysis/prod/MET150/$CFG .
cmsRun $CFG 

cp *root $CMSSW/src/Analysis/StopAnalysis/data/MET150/

rm *root
rm *py


date
pwd
#---------------------------------------------------------------------------------
