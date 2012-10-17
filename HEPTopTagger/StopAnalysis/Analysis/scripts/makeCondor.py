import subprocess, sys, os
from optparse import OptionParser

def createCondorSh(CMSSW_BASE, SCRAM_ARCH, cfgFile, configParserCommands, fileTag, eventsPerJob,outputDirectory):
	string = (
		'#!/bin/bash\n' +
		'date\n' +
		'\n' +
		'source /uscmst1/prod/sw/cms/shrc prod\n' +
		'CMSSW=' + CMSSW_BASE  + '\n' +
		'SCRAM_ARCH=' + SCRAM_ARCH + '\n' +
		'\n' +
		'workerspace=$PWD\n' +
		'cd $CMSSW/src\n' +
		'eval `scramv1 runtime -sh`\n' +
		'cd $workerspace\n' +
		'\n' +
		'python cfgHandler.py ' + cfgFile + ' -e ' + eventsPerJob + ' -n $1 ' +'-c "' + configParserCommands + '" -t ' + fileTag + '\n' +
		'\n' +
		'\n' +
		'cp  *root  ' + outputDirectory + '\n' +
		'rm *.py\n' +
		'rm *.root\n' +
		'\n' +
		'date\n' +
		'pwd\n' +
		'#---------------------------------------------------------------------------------\n'
		)

	outputFile = open('condor_' + fileTag + '.sh', 'w')
	outputFile.write(string)
	outputFile.close()
	
def createCondorSubmit(fileTag, logFileDir, nJobs,cfgFile, cfgHandler):
	string = (
		'universe=vanilla\n' +
		'Executable=condor_' + fileTag + '.sh\n' +
		'output = ' + logFileDir  + fileTag + '_$(Process).out\n' +
		'error = ' + logFileDir + fileTag + '_$(Process).err\n' +
		'log = ' + logFileDir + fileTag + '_$(Process).log\n' +
		'Requirements = Memory >= 199 && OpSys == "LINUX" && (Arch != "DUMMY")\n' +
		'Should_Transfer_Files = YES\n' +
		'Transfer_Input_Files = ' + cfgFile + ', ' + cfgHandler + '\n' +
		'Arguments = $(Process)\n' +
		'WhenTOTransferOutput  = ON_EXIT\n' +
		'environment =  \n' +
		'Notification = error\n' +
		'Queue ' + nJobs + '\n' 
		)

	outputFile = open('condor_' + fileTag + '.submit', 'w')
	outputFile.write(string)
	outputFile.close()

parser = OptionParser()
parser.add_option("-n", "--nJobs", dest = "nJobs", default = 1, help = 'Number of jobs to split up into' , metavar = "INT")
parser.add_option("-e", "--eventsPerJob", dest = "eventsPerJob", default = 1, help = 'Events per job.' , metavar = "INT")
parser.add_option("-c", "--configParserCommands", dest = "configParserCommands", help='Any additional config parser commands to go along with the file', default = "", metavar= "STRING")
parser.add_option("-t", "--fileTag", dest = "fileTag", help='Tag that will be appended to the create files to distinguish them.', default = "", metavar="STRING")
parser.add_option("-o", "--outputDir", dest = "outputDir", help='Output directory for root files', default = "", metavar="STRING")

(options, args) = parser.parse_args()
cfgFileName = args[0]

if options.fileTag == "":
	options.fileTag = args[0].replace('_cfg.py' , '')

echoPWDCommand =  subprocess.Popen("echo $PWD", stdout=subprocess.PIPE,
								   shell=True)
PWD = echoPWDCommand.stdout.read().replace('\n', '')
if options.outputDir == "":
	options.outputDir = PWD + '/output'
echoCMSSWCommand =  subprocess.Popen("echo $CMSSW_BASE",
									 stdout=subprocess.PIPE, shell=True)
CMSSW_BASE = echoCMSSWCommand.stdout.read().replace('\n', '')

if CMSSW_BASE =='':
	sys.stderr.write("** Please run cmsenv before running makeCondor **\n")
	sys.exit(1)


echoSCRAMARCHCommand = subprocess.Popen("echo $SCRAM_ARCH", stdout=subprocess.PIPE,
									shell=True)
SCRAM_ARCH = echoSCRAMARCHCommand.stdout.read().replace('/n', '')



if not os.path.exists(PWD + "/logfiles"):
	subprocess.call("mkdir logfiles", shell=True)
if not os.path.exists(PWD + "/output"):
	subprocess.call("mkdir output", shell=True)

createCondorSh(CMSSW_BASE, SCRAM_ARCH, cfgFileName, options.configParserCommands, options.fileTag, str(options.eventsPerJob),options.outputDir)
createCondorSubmit(options.fileTag, PWD + "/logfiles/", options.nJobs,cfgFileName, CMSSW_BASE + '/src/StopAnalysis/Analysis/scripts/cfgHandler.py')
