#!/usr/bin/env python

import os
import re

from Demo_cfg import *

#########################################
def processSingleSample(paths, sampleName, sampleWeight):
	process.source.fileNames = cms.untracked.vstring()
	for path in paths:
		for fname in os.listdir(path):
			if re.search(".root",fname)!=None:
				process.source.fileNames.append('file:'+path+"/"+fname)
				process.configurationMetadata.name=sampleName
				process.configurationMetadata.preselectionEff=sampleWeight
				out = open('tmpConfig.py','w')
				out.write(process.dumpPython())
				out.close()
	os.system("root -q -b runProof.C")	
	os.system("rm -f tmpConfig.py")
#########################################
#########################################
# 
# processSingleSample(paths, sampleName, sampleWeight):
#  *paths - vector of directories where the data files are.
#  *sampleName - name of the sample, included in the output file name
#  *sampleWeight - weight of the event sample. All histograms will be scaled
#                  by sampleWeight/Nevents
#                  if sampleWeight<0 scaling is not performed
#########################################
##Wmunu
paths = ["/tmp/akalinow/Wmunu/"]
processSingleSample(paths,"WmunuMC",89629*0.1057*0.742*0.735)

##Soup
paths = ["/tmp/akalinow/Soup/"]
processSingleSample(paths,"Soup",-1)
