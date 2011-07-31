#!/bin/bash
date
source /uscmst1/prod/sw/cms/shrc prod
echo 1
CMSSW=/uscms_data/d2/csilkw2/CMSSW_4_2_3
echo 2
#basedir=$CMSSW/src/UserCode/SusyAnalysis/SLHAFILES/mSugraScan/m0_m12_10_0_1
basedir=$CMSSW/src/$SCANDIR
echo 3
slhas=$basedir/*.slha
echo 4
i=`expr $OFFSET + $1`
t=""

echo 5
INPUTFILE=$t

workerspace=$PWD
echo 6
cd $CMSSW/src
eval `scramv1 runtime -sh`
cd $workerspace
echo 7


slhasForProcessing=`python sorter_SMS.py $i $FILESPERPOINT $slhas`
echo 8
python make_SMS_lhes.py $NEVENTS $SCANDIR $slhasForProcessing
echo 9

lhes=*.lhe
for lhe in $lhes
do
  cp $lhe $OUTPUTDIRECTORY
done
rm *.py
rm *.lhe
rm fort.69.bak
echo 10
date
pwd
#---------------------------------------------------------------------------------
