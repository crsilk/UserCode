#!/usr/bin/env python

import os
import re

from Demo_cfg import *

#########################################
def processSingleSample(paths, sampleName, sampleWeight):
	process.source.fileNames = cms.untracked.vstring()
	print "Will use following input files: "
	for path in paths:
		for fname in os.listdir(path):
			print "Path: ",path
			if re.search(".root",fname)!=None:
			#if re.search("pat",fname)!=None:
				print "       "+fname
				process.source.fileNames.append('file:'+path+"/"+fname)
				process.configurationMetadata.name=sampleName
				process.configurationMetadata.preselectionEff=sampleWeight
				out = open('tmpConfig_'+sampleName+'.py','w')
				out.write(process.dumpPython())
				out.close()
	print "For following output file: PFAnalysis_"+sampleName+".root with weight = ",sampleWeight
	os.system("chrisAnalysis tmpConfig_"+sampleName+".py >&"+sampleName+".out &")
	#os.system("testAnalysis tmpConfig_"+sampleName+".py")
	#os.system("rm -f tmpConfig_"+sampleName+".py")
#########################################
# 
# processSingleSample(paths, sampleName, sampleWeight):
#  *paths - vector of directories where the data files are.
#  *sampleName - name of the sample, included in the output file name
#  *sampleWeight - weight of the event sample. All histograms will be scaled
#                  by sampleWeight/Nevents
#                  if sampleWeight<0 scaling is not performed
#
# The test files can be found here:
# 1. lxplus309 (until deleted automaticaly) under /tmp/akalinow,
#     so if you login there you can run this file as it is
# 2. CASTOR to be copied with:
#
# > mkdir /tmp/$USER/Wmunu  /tmp/$USER/Soup
# > rfcp /castor/cern.ch/user/a/akalinow/CMS/PFTestData/Soup/Merged_SD_Mu9.root /tmp/$USER/Soup
# > rfcp /castor/cern.ch/user/a/akalinow/CMS/PFTestData/Wmunu/Merged_WmunuMC.root /tmp/$USER/Wmunu 
#
#
#########################################
user = os.getenv('USER')

##Wmunu
paths = ["/tmp/%s/Wmunu/" % user ]
processSingleSample(paths,"WmunuMC",89629*0.1057*0.742*0.735)

##Soup
paths = ["/tmp/%s/Soup/" % user ]
processSingleSample(paths,"Soup",-1)
