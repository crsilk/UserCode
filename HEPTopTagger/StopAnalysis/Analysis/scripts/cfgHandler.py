import subprocess, sys, os,glob
from optparse import OptionParser

def getFileRange(jobNumber, filesPerJob, totalFiles):
	range = []
	if jobNumber*filesPerJob > totalFiles:
		return [0,0]
	if (jobNumber + 1)*filesPerJob > totalFiles:
		range = [jobNumber*filesPerJob, totalFiles]
	else:
		range = [jobNumber*filesPerJob, (jobNumber+1)*filesPerJob]

	return range
parser = OptionParser()
parser.add_option("-j", "--jobNumber", dest = "jobNumber", default = 1, help = 'Job to be run' , type = "int", metavar = "INT")
parser.add_option("-p", "--filesPerJob", dest = "filesPerJob", default = 1, help = 'Files per job.' , type = "int", metavar = "INT")
parser.add_option("-n", "--totalNumberFiles", dest = "totalNumberFiles", default = 1, help = 'Total number of files in the source' , type = "int", metavar = "INT")
parser.add_option("-c", "--configParserCommands", dest = "configParserCommands", help='Any additional config parser commands to go along with the file', default = "", metavar="STRING")
parser.add_option("-t", "--fileTag", dest = "fileTag", help='Tag that will be appended to the create files to distinguish them.', default = "", metavar="STRING")
(options, args) = parser.parse_args()
cfgFileName = args[0]



oldFile = open(cfgFileName, 'r')
newFile = open(cfgFileName.replace('_cfg.py','_tmp_cfg.py'), 'w')

for line in oldFile:
	newFile.write(line)


fileRange = getFileRange(options.jobNumber, options.filesPerJob, options.totalNumberFiles)
newFile.write("\n\n")
newFile.write("process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )\n")
newFile.write("process.source.fileNames = process.source.fileNames[" + str(fileRange[0]) + ":" + str(fileRange[1]) + "]")
newFile.close()

print "cmsRun " + cfgFileName.replace('_cfg.py','_tmp_cfg.py') + " " + options.configParserCommands

subprocess.call("cmsRun " + cfgFileName.replace('_cfg.py','_tmp_cfg.py')+ " " + options.configParserCommands,shell=True)

rootFiles = glob.glob('*.root')
for rootFile in rootFiles:
	print 'mv ' + rootFile + " " + options.fileTag + "_" + rootFile
	subprocess.call('mv ' + rootFile + " " + options.fileTag + "_" + rootFile.replace('.root','') + '_' + str(options.jobNumber) + '.root', shell=True)

	
