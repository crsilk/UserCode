import subprocess, sys, os,glob
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-n", "--jobNumber", dest = "jobNumber", default = 1, help = 'Job to be run' , type = "int", metavar = "INT")
parser.add_option("-e", "--eventsPerJob", dest = "eventsPerJob", default = 1, help = 'Events per job.' , type = "int", metavar = "INT")
parser.add_option("-c", "--configParserCommands", dest = "configParserCommands", help='Any additional config parser commands to go along with the file', default = "", metavar="STRING")
parser.add_option("-t", "--fileTag", dest = "fileTag", help='Tag that will be appended to the create files to distinguish them.', default = "", metavar="STRING")
(options, args) = parser.parse_args()
cfgFileName = args[0]

skipEvent = options.jobNumber * options.eventsPerJob 


oldFile = open(cfgFileName, 'r')
newFile = open(cfgFileName.replace('_cfg.py','_tmp_cfg.py'), 'w')

for line in oldFile:
	newFile.write(line)

newFile.write("\n\n")
newFile.write("process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(" + str(options.eventsPerJob) + ") )\n")
newFile.write("process.source.skipEvents = cms.untracked.uint32(" + str(skipEvent) + ")\n")
newFile.close()

print "cmsRun " + cfgFileName.replace('_cfg.py','_tmp_cfg.py') + " " + options.configParserCommands

subprocess.call("cmsRun " + cfgFileName.replace('_cfg.py','_tmp_cfg.py')+ " " + options.configParserCommands,shell=True)

rootFiles = glob.glob('*.root')
for rootFile in rootFiles:
	print 'mv ' + rootFile + " " + options.fileTag + "_" + rootFile
	subprocess.call('mv ' + rootFile + " " + options.fileTag + "_" + rootFile.replace('.root','') + '_' + str(options.jobNumber) + '.root', shell=True)

	
