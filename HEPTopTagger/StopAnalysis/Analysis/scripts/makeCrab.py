import subprocess, sys, os
from optparse import OptionParser

def createCrabCfg(cfgFile, sourceFile, njobs, nevents, otherOptions):
	fileName ='crab_' + sourceFile.split('.')[-1].replace('_cfi', '') + "_" + cfgFile.replace('_cfg','').replace('.py', '') + '.cfg'
	file = open(fileName, 'w')
	string = (
		"[CRAB]\n" +
		"jobtype = cmssw\n" +
		"scheduler = condor\n" +
		"use_server = 0\n" +
		"\n" +
		"[CMSSW]\n" +
		"datasetpath = none\n" +
		"pset = " + cfgFile + "\n" +
		"total_number_of_events = " + str(nevents) + "\n" +
		"number_of_jobs = " + njobs + "\n" +
		"get_edm_output = 1\n" +
		"pycfg_params = sourceFile=" + sourceFile + " " + otherOptions + "\n" +
		"\n" +
		"[USER]\n" +
		"return_data = 1\n" +
		"copy_data = 0\n" +
		"publish_data = 0\n" +
		"check_user_remote_dir = 0\n" +
		"ui_working_dir = ui_" + sourceFile.split('.')[-1].replace('_cfi', '') + "_" + cfgFile.replace('_cfg','').replace('.py', '')  +"\n"
		)
		
	file.write(string)
	file.close()
	return fileName
parser = OptionParser()
parser.add_option("-n", "--nJobs", dest = "nJobs", default = 1, help = 'Number of jobs to split up into' , metavar = "INT")
parser.add_option("-e", "--events", dest = "events", default = 1, help = 'Total number of events.' , metavar = "INT")
parser.add_option("-c", "--configParserCommands", dest = "configParserCommands", help='Any additional config parser commands to go along with the file', default = "", metavar= "STRING")
parser.add_option("-s", "--sourceFile", dest = "sourceFile", help='source for the cfg to run over', default = "", metavar= "STRING")
(options, args) = parser.parse_args()
cfgFileName = args[0]


crabCfg = createCrabCfg(cfgFileName, options.sourceFile, options.nJobs, options.events, options.configParserCommands)

subprocess.call('crab -create -cfg ' + crabCfg, shell=True)
