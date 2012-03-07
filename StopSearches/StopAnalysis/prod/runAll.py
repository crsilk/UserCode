import os, sys, glob, subprocess

directories = glob.glob("*")
files =[]

for directory in directories:
	if directory == "CVS" or directory.find(".py")> -1: continue

	files  = glob.glob(directory + "/condor/*submit")

	print "cd " + directory + "/condor/"
	
	for file in files:
		print "condor_submit " + file.split('/')[-1]

	print "cd ../../"
	
