#!/bin/bash
date
source /uscmst1/prod/sw/cms/shrc prod

CMSSW=/uscms_data/d2/csilkw2/CMSSW_4_2_3

#basedir=$CMSSW/src/UserCode/SusyAnalysis/SLHAFILES/mSugraScan/m0_m12_10_0_1
basedir=$CMSSW/src/$SCANDIR

slhas=$basedir/*.slha

i=$OFFSET
t=""
for file in $slhas
do
 if [ $i == $1 ]
 then
   t=$file
   break
 fi
 let 'i = i + 1'
done

INPUTFILE=$t

workerspace=$PWD

cd $CMSSW/src
eval `scramv1 runtime -sh`
cd $workerspace


slhasForProcessing=`python sorter_msugra.py $1 $FILESPERPOINT $slhas`

python make_msugra_lhes.py $NEVENTS $SCANDIR $slhasForProcessing


lhes=*.lhe
for lhe in $lhes
do
  cp $lhe $OUTPUTDIRECTORY
  #srmcp "file:///`pwd`/$lhe srm://cmssrm.fnal.gov:8443/srm/managerv1?SFN=/resilient/crsilk/msugraScan/lhe/$lhe"
done
rm *.py
rm *.lhe
rm fort.69.bak

date
pwd
#---------------------------------------------------------------------------------
