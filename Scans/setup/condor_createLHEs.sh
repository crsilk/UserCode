#!/bin/bash

date
source /uscmst1/prod/sw/cms/shrc prod

workerspace=$PWD
cd $CREATESCANDIR

eval `scramv1 runtime -sh`
cd $workerspace

python createLHEs.py $1

rm *.py

pwd
date

#---------------------------------------------------------------------------------
