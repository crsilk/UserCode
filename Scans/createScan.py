#!/usr/local/bin/python
##########createScan.py##################
#Christopher Silkworth (crsilk@cern.ch) 08 Decemeber 2011
##########################################

import subprocess, re, glob, os, sys, ConfigParser


def ntabs(n):
    tab = ''
    for i in range (0, n):
        tab = tab +'    '
    return tab


def makeSLHA(templateName, outFileName, pdgIds, masses, branchingRatios, 
             decayProducts):
    
    oldFile = open(templateName, 'r')
    newFile = open(outFileName, 'w')
    inBlockMass = False
    inDecayTable = False
    decayLabel ='#           BR         NDA      ID1       ID2       ID3\n'

    if (len(pdgIds) != len(masses) or 
        len(pdgIds) != len(branchingRatios) or 
        len(pdgIds) != len(decayProducts)):
        sys.stderr.write('** SLHATemplate.slha: Number of masses or decay ' + 
                         'products does not match number of particles ** \n')
        sys.exit(0)

    for line in oldFile:
        if line.find('BLOCK MASS') > -1: inBlockMass = True
        if line.find('/') > -1: inBlockMass = False
        if line.find('DECAY TABLE') > -1: inDecayTable = True
        if line.find('/') > -1: inDecayTable = False

        if inBlockMass:
            for i in range(0, len(pdgIds)):
                if line.find(' ' + pdgIds[i] + ' ') > -1:
                    line = line.replace('1.00000000E+05', masses[i])
        if inDecayTable:
            for i in range(0, len(pdgIds)):
                
                if branchingRatios[i][0] == 'STABLE': continue

                if line.find('DECAY   '+ pdgIds[i] + ' ') > -1:
                    line = (line.replace('0.00000000E+00', '0.10000000E+00') + 
                            decayLabel)
                    for j in range(0, len(branchingRatios[i])):
                        
                        line = (line + '     ' +  branchingRatios[i][j] + 
                                '    ' + str(len(decayProducts[i][j])))
                        
                        for k in range(0, len(decayProducts[i][j])):
                            line = line + '    ' + decayProducts[i][j][k]
                        
                        line = line + '\n'
        newFile.write(line)
    
    oldFile.close()
    newFile.close()

    return outFileName
def stringToArray(string):
    array = string.split(" ")

    deletes = array.count('')
    for i in range(0,deletes):
        array.remove('')
    
    deletes =  array[-1].count('\t')
    for i in range(0, deletes):
            array[-1].remove('\t')
    return array
def stringToArrayN(string, delimiter):
	array = string.split(delimiter)
	deletes = array.count('')
    
	for i in range(0,deletes):
		array.remove('')
	return array
def stringToArrays(string):
    lines = string.split('\n')
    array = []
    for line in lines:
        array.append(line.split(' '))
        deletes = array[-1].count('')
        for i in range(0, deletes):
            array[-1].remove('')
        deletes =  array[-1].count('\t')
        for i in range(0, deletes):
            array[-1].remove('\t')
    return array
        
def makeSLHAToLHECfg(templateFileName, newFileName, numberOfEvents, 
subprocesses, otherPythiaCommands, slhaDirectory, energy, randomSeed):

    oldFile = open(templateFileName, 'r')
    newFile = open(newFileName, 'w')

    for line in oldFile:

        if line.find("NUMBEROFEVENTS") > -1:
            line = line.replace("NUMBEROFEVENTS", numberOfEvents)
        if line.find("ALLOWEDSUBPROCESSES") > -1:

            line = line.replace("ALLOWEDSUBPROCESSES", "")

            for subpro in subprocesses:
                line = line + "'MSUB(" + subpro + ") = 1',\n"
            for command in otherPythiaCommands:
                line = line +"'" + command + "',\n"
        if line.find("SLHADIRECTORY") > -1:
            line = line.replace("SLHADIRECTORY", slhaDirectory)
        
        if line.find("ENERGY") > -1:
            line = line.replace("ENERGY", energy)

        if line.find("RANDOMSEED") > -1:
            line = line.replace("RANDOMSEED", randomSeed)
        newFile.write(line)

    oldFile.close()
    newFile.close()
    return newFileName
def makeCreateSLHAs( SLHATemplateName, newFileName, outputDir, modelTag, scanNames, scanParameterMins, scanParameterMaxs, scanParameterSteps, massNames, massDefs, cuts):
    
    if (len(scanNames) != len(scanParameterMins) or 
        len(scanNames) != len(scanParameterMaxs) or 
        len(scanNames)  != len(scanParameterSteps)):
        sys.stderr.write("** createSLHAs.py: Mismatch in number of scan " + 
                         "parameter names, mins, maxes, step ** \n ")
        sys.exit(0)
    if (len(massNames) != len(massDefs)): 
        sys.stderr.write("** createSLHAs.py: Mismatch in number of masses "+ 
                         "and mass definitions **\n" )
        sys.exit(0)

    nScan = len(scanNames)
    newFile = open(newFileName, 'w')
    
    newFile.write('import os, sys, re\n\n')

    newFile.write('def frange(start,stop,step):\n' + ntabs(1) +
                  'step *= 2*((stop>start)^(step<0))-1\n' + ntabs(1) +
                  'return [start+i*step for i in' + ntabs(1) +
                  'range(0,1+int((stop-start)/step))]\n \n')  

    for i in range(0, nScan):
        newFile.write(ntabs(i) + 'for ' + scanNames[i] + ' in frange( ' + 
                      scanParameterMins[i] + ', ' + scanParameterMaxs[i] + 
                      ', ' + scanParameterSteps[i] + '):\n')
    newFile.write('\n')
    
    for cut in cuts:
        newFile.write(ntabs(nScan) + 'if ' + cut + ' : continue\n')
    newFile.write('\n')

    newFile.write(ntabs(nScan) + "newFileName = '" + modelTag + "'")

    for i in range(0, len(scanNames)):
        newFile.write(" + '_' + str(" + scanNames[i] +")")
    newFile.write(" + '.slha'\n \n")                      

    newFile.write(ntabs(nScan) + "SLHATemplate = open('" + SLHATemplateName +
                  "', 'r')\n")
    newFile.write(ntabs(nScan) + "newSLHAFile = open( " + "'" + outputDir + "'" + " + '/' + newFileName, 'w')\n \n")
    newFile.write(ntabs(nScan) + "for line in SLHATemplate:\n")

    for i in range(0, len(massNames)):
        newFile.write(ntabs(nScan + 1) + "if line.find('" + massNames[i] + "') > -1:\n")
        newFile.write(ntabs(nScan + 2) + "line = line.replace('" + massNames[i] +  "', str(" + massDefs[i] + ") )\n")
    newFile.write(ntabs(nScan + 1) + "newSLHAFile.write(line)\n \n")
    newFile.write(ntabs(nScan) +"newSLHAFile.close()\n")
    newFile.write(ntabs(nScan) +"SLHATemplate.close()\n")
    
    newFile.close()

    return newFileName

def makeCreateLHEs( oldFileName, newFileName, createScanDirectory, templateCfgName, filesPerJob, useDefaultHeader, runLheCheck, slhaScanDir, outputDir, sourceFileName, crabFile, insertXSection, energy):
    oldFile = open(oldFileName, 'r')
    newFile = open(newFileName, 'w')

    for line in oldFile:
        line = line.replace('FILESPERJOB', filesPerJob)
        line = line.replace('CREATESCANDIRECTORY', "'" + createScanDirectory + 
                            "'")
        line = line.replace('USEDEFAULTHEADER', useDefaultHeader)
        line = line.replace('RUNLHECHECK', runLheCheck)
        line = line.replace('TEMPLATECFG', "'" + templateCfgName +"'")
        line = line.replace('SLHASCANDIR', "'" + slhaScanDir + "'")
        line = line.replace('OUTPUTDIR', "'" + outputDir + "'")
        line = line.replace('SOURCEFILE', "'" + sourceFileName + "'")
        line = line.replace('CRABFILE', "'" + crabFile + "'")
        line = line.replace('INSERTXSECTION', insertXSection)
        line = line.replace('ENERGY', energy)
        newFile.write(line)
    oldFile.close()
    newFile.close()
    return newFileName
def makeCondorCreateLHEsSubmit(templateFileName, newFileName, modelTag,logFilesDir, createScanDir, outputDir, numberOfJobsToSubmit):
    oldFile = open(templateFileName, 'r')
    newFile = open(newFileName, 'w')

    for line in oldFile:
        line = line.replace('_LOGFILESDIR_', logFilesDir)
        line = line.replace('_MODELTAG_', modelTag)
        line = line.replace('_CREATESCANDIR_', createScanDir)
        line = line.replace('_NUMBEROFJOBS_', numberOfJobsToSubmit)
        newFile.write(line)
    oldFile.close()
    newFile.close()
    return newFileName
def makeCondorCreateLHEsSh(templateFileName, newFileName, scramArch):
    oldFile = open(templateFileName, 'r')
    newFile = open(newFileName, 'w')
    
    for line in oldFile:
        line = line.replace('SCRAMARCH', scramArch)
        newFile.write(line)

    oldFile.close()
    newFile.close()
    return newFileName
def makeLHEToEDMCfg(templateFileName, newFileName,cmsswDir, lheSourceFile, outputFileName):
    oldFile = open(templateFileName, 'r')
    newFile = open(newFileName, 'w')
    
    for line in oldFile:
        line = line.replace('OUTPUTFILE', "'" + outputFileName + "'")
        if line.find('LHESOURCEFILE') > -1:
            line = line.replace('LHESOURCEFILE', lheSourceFile)
            line = line.replace('.py','').replace(cmsswDir + '/src/', '').replace('python/', '').replace('/', '.')
        newFile.write(line)
    oldFile.close()
    newFile.close()
    return newFileName
def makeEDMToAODSIMCfg(templateFileName, newFileName, cmsswDir, edmSourceFile, outputFileName, cmEnergy):
    oldFile = open(templateFileName, 'r')
    newFile = open(newFileName, 'w')
    
    for line in oldFile:
        line = line.replace('OUTPUTFILE', "'" + outputFileName + "'")
        if line.find('EDMSOURCEFILE') > -1:
            line = line.replace('EDMSOURCEFILE', edmSourceFile)
            line = line.replace('.py','').replace(cmsswDir + '/src/', '').replace('python/', '').replace('/', '.')
        if line.find('CMENERGY') > -1:
            line = line.replace('CMENERGY', cmEnergy)
        newFile.write(line)
    oldFile.close()
    newFile.close()
    return newFileName
def makeCrabLHEToEDM(templateFileName, newFileName, scheduler, useServer,
totalNumberOfEvents, numberOfJobs, storagePath, userRemoteDir, whitelist, blacklist, uiWorkingDir):
    oldFile = open(templateFileName, 'r')
    newFile = open(newFileName, 'w')
    
    for line in oldFile:
        line = line.replace('SCHEDULER', scheduler)
        line = line.replace('USESERVER', useServer)
        line = line.replace('TOTALNUMBEROFEVENTS', totalNumberOfEvents)
        line = line.replace('NUMBEROFJOBS', numberOfJobs)
        line = line.replace('STORAGEPATH' , storagePath)
        line = line.replace('USERREMOTEDIR', userRemoteDir)
        line = line.replace('UIWORKINGDIR', uiWorkingDir)
        line = line.replace('WHITELIST', whitelist)
        line = line.replace('BLACKLIST', blacklist)
        newFile.write(line)
    oldFile.close()
    newFile.close()
    return newFileName
def makeCrabEDMToAODSIM(templateFileName, newFileName, scheduler, useServer,
totalNumberOfEvents, numberOfJobs, storagePath, userRemoteDir, whitelist, blacklist, uiWorkingDir):
    oldFile = open(templateFileName, 'r')
    newFile = open(newFileName, 'w')
    
    for line in oldFile:
        line = line.replace('SCHEDULER', scheduler)
        line = line.replace('USESERVER', useServer)
        line = line.replace('TOTALNUMBEROFEVENTS', totalNumberOfEvents)
        line = line.replace('NUMBEROFJOBS', numberOfJobs)
        line = line.replace('STORAGEPATH' , storagePath)
        line = line.replace('USERREMOTEDIR', userRemoteDir)
        line = line.replace('UIWORKINGDIR', uiWorkingDir)
        line = line.replace('WHITELIST', whitelist)
        line = line.replace('BLACKLIST', blacklist)
        newFile.write(line)
    oldFile.close()
    newFile.close()
    return newFileName

def makeLHEToAODSIMCfg(templateFileName, newFileName, cmsswDir, lheSourceFile, outputFileName, cmEnergy):
    oldFile = open(templateFileName, 'r')
    newFile = open(newFileName, 'w')
    
    for line in oldFile:
        line = line.replace('OUTPUTFILE', "'" + outputFileName + "'")
        if line.find('EDMSOURCEFILE') > -1:
            line = line.replace('LHESOURCEFILE', lheSourceFile)
            line = line.replace('.py','').replace(cmsswDir + '/src/', '').replace('python/', '').replace('/', '.')
        if line.find('CMENERGY') > -1:
            line = line.replace('CMENERGY', cmEnergy)
        newFile.write(line)
    oldFile.close()
    newFile.close()
    return newFileName

def makeCrabLHEToAODSIM(templateFileName, newFileName, scheduler, useServer, eventsPerJob, storagePath, userRemoteDir, whitelist, blacklist, newDatasetName, storageElement):
    oldFile = open(templateFileName, 'r')
    newFile = open(newFileName, 'w')
    
    for line in oldFile:
        line = line.replace('SCHEDULER', scheduler)
        line = line.replace('USESERVER', useServer)
        line = line.replace('EVENTSPERJOB', eventsPerJob)
        line = line.replace('STORAGEPATH' , storagePath)
        line = line.replace('USERREMOTEDIR', userRemoteDir)
        line = line.replace('WHITELIST', whitelist)
        line = line.replace('BLACKLIST', blacklist)
        line = line.replace('DATASETNAME', newDatasetName)
        line = line.replace('STORAGEELEMENT', storageElement)
        newFile.write(line)
    oldFile.close()
    newFile.close()
    return newFileName
    
if __name__ == '__main__':
    
    configFile = ''
    decay_of_pdgId = []
    branchingRatios = []
    masses = []
    mass_definition_of_pdgId = []
    cut_number = []
    nSlhaFiles = 1
    counter = 0
    

    cmssw_base= subprocess.Popen("echo $CMSSW_BASE",stdout=subprocess.PIPE,
                                  shell=True) 
    cmsDir = cmssw_base.stdout.read().replace('\n', '')
    if cmsDir == '':
        sys.stderr.write("** Please run cmsenv before running createScan **\n")
        sys.exit(0)
    scramArchCommand = subprocess.Popen("echo $SCRAM_ARCH", stdout=subprocess.PIPE,shell=True)
    scramArch = scramArchCommand.stdout.read().replace('/n', '')

    pwd = subprocess.Popen("pwd", stdout=subprocess.PIPE)
    createScanDir = pwd.stdout.read().replace('\n', '')


    try:
        configFile = sys.argv[1]
    except:
        sys.stderr.write("** Please provide input configuration file **\n")
        sys.exit(0)



############Begin Parse Configuration File###############
    parser = ConfigParser.ConfigParser()
    parser.read(configFile)

    #[General]
    if parser.has_option('General', 'model_tag'):
        model_tag = parser.get('General', 'model_tag')
    else:
        model_tag = 'NoModelTag'

    if parser.has_option('General', 'events_per_point'):
        events_per_point = parser.get('General', 'events_per_point')
    else:
        events_per_point = '1'


    if parser.has_option('General', 'energy'):
        energy = parser.get('General','energy')
    else:
        energy = '7000'

    if parser.has_option('General', 'do_step0'):
        do_step0 = parser.getboolean('General', 'do_step0')
    else:
        do_step0 = False
    if parser.has_option('General', 'points_per_file'):
        points_per_file = parser.get('General', 'points_per_file')
    else:
        points_per_file = '1'

    if parser.has_option('General', 'dataset_name'):
        dataset_name = parser.get('General', 'dataset_name')
    else:
        dataset_name = "NONEGIVEN"

    if float(points_per_file) < 1:
        events_per_point = str(int(float(points_per_file)*int(events_per_point)))

    #[SLHA]
    if parser.has_option('SLHA','involved_susy_pdgIds'):
        involved_susy_pdgIds = stringToArray(parser.get('SLHA','involved_susy_pdgIds'))
    else:
        sys.stderr.write('** No involved_susy_pdgIds given, please put them in you configuration file **\n')
        sys.exit(0)

    if parser.has_option('SLHA','scan_parameter_names'):
        scan_parameter_names = stringToArray(parser.get('SLHA','scan_parameter_names'))
    else:
        sys.stderr.write('** No scan_parameter_names given, please put them in you configuration file **\n')
        sys.exit(0)
    if parser.has_option('SLHA','scan_parameter_mins'):
        scan_parameter_mins = stringToArray(parser.get('SLHA','scan_parameter_mins'))
    else:
        sys.stderr.write('** No scan_parameter_mins given, please put them in you configuration file **\n')
        sys.exit(0)
    if parser.has_option('SLHA','scan_parameter_maxs'):
        scan_parameter_maxs= stringToArray(parser.get('SLHA','scan_parameter_maxs'))
    else:
        sys.stderr.write('** No scan_parameter_maxs given, please put them in you configuration file **\n')
        sys.exit(0)
    if parser.has_option('SLHA','scan_parameter_steps'):
        scan_parameter_steps = stringToArray(parser.get('SLHA','scan_parameter_steps'))
    else:
        sys.stderr.write('** No scan_parameter_steps given, please put them in you configuration file **\n')
        sys.exit(0)

    for i in range(0,len(involved_susy_pdgIds)):
        masses.append('M' + involved_susy_pdgIds[i])
        if parser.has_option('SLHA', 'decay_of_' + involved_susy_pdgIds[i]):
            tempString = parser.get('SLHA', 'decay_of_' + involved_susy_pdgIds[i])
            tempArray = stringToArrays(tempString)
            branchingRatios.append([])
            decay_of_pdgId.append([])
            for j in range(0, len(tempArray)):
                branchingRatios[-1].append(tempArray[j][0])
                decay_of_pdgId[-1].append(tempArray[j][1:])

        else:
            branchingRatios.append(['STABLE'])
            decay_of_pdgId.append([])
    
    for i in range(0, len(involved_susy_pdgIds)):
        if parser.has_option('SLHA', 'mass_definition_of_' + involved_susy_pdgIds[i]):
            mass_definition_of_pdgId.append(parser.get('SLHA', 'mass_definition_of_' +  involved_susy_pdgIds[i]) )
        else:
            sys.stderr.write('** Mass of involved particle: ' +  involved_susy_pdgIds[i] + ' not given. Fix cfg file **\n')
            sys.exit(0)
    while True:
        counter = counter + 1
        try:
            cut_number.append(parser.get('SLHA', 'cut_' + str(counter)))
        except:
            break
    if parser.has_option('SLHA', 'slha_output_directory'):
        slha_output_directory = parser.get('SLHA', 'slha_output_directory')
    else:
        slha_output_directory = createScanDir + '/' + configFile.replace('.cfg', '') + '/SLHA/files/'
 

    #[LHE]
        
    if parser.has_option('LHE', 'points_per_file'):
        points_per_file = parser.get('LHE', 'points_per_file')
    else:
        points_per_file = '1'

    if float(points_per_file) < 1:
        events_per_point = str(int(float(points_per_file)*int(events_per_point)))


    allowed_subprocesses = stringToArray(parser.get('LHE', 'allowed_subprocesses'))

    if parser.has_option('LHE', 'random_seed'):
        random_seed = parser.get('LHE', 'random_seed')
    else:
        random_seed = '1'

    if parser.has_option('LHE', 'lhe_output_directory'):
        lhe_output_directory = parser.get('LHE', 'lhe_output_directory')
    else:
        lhe_output_directory = createScanDir + '/' + configFile.replace('.cfg', '') + '/LHE/'

    if parser.has_option('LHE', 'lhe_input_directory'):
        lhe_input_directory = parser.get('LHE', 'lhe_input_directory')
    else:
        lhe_input_directory = slha_output_directory
    if parser.has_option('LHE', 'other_pythia_commands'):
        other_pythia_commands = stringToArrayN(parser.get('LHE', 'other_pythia_commands'),'\n')
    else:
        other_pythia_commands = []
    if parser.has_option('LHE', 'run_lhe_check'):
        run_lhe_check = parser.get('LHE', 'run_lhe_check')
    else:
        run_lhe_check = 'False'
    if parser.has_option('LHE', 'use_default_header'):
        use_default_header = parser.get('LHE', 'use_default_header')
    else:
        use_default_header = 'True'
    if parser.has_option('LHE', 'insert_cross_section'):
        insert_cross_section = parser.get('LHE', 'insert_cross_section')
    else:
        insert_cross_section = 'False'

    #[STEP0]
    
    if parser.has_option('STEP0', 'step0_scheduler'):
        step0_scheduler = parser.get('STEP0', 'step0_scheduler')
    else:
        step0_scheduler = 'condor'
    if parser.has_option('STEP0', 'step0_events_per_job'):
        step0_events_per_job = parser.get('STEP0', 'step0_events_per_job')
    else:
        step0_events_per_job = '1'
    if parser.has_option('STEP0', 'step0_storage_path'):
        step0_storage_path = parser.get('STEP0', 'step0_storage_path')
    else:
        step0_storage_path = '/srm/managerv2?SFN=11'
    if parser.has_option('STEP0', 'step0_user_remote_directory'):
        step0_user_remote_directory = parser.get('STEP0', 'step0_user_remote_directory')
    else:
        step0_user_remote_directory = '/store/user/susygen/'
    if parser.has_option('STEP0', 'step0_se_whitelist'):
        step0_se_whitelist = parser.get('STEP0', 'step0_se_whitelist')
    else:
        step0_se_whitelist = ''
    if parser.has_option('STEP0', 'step0_se_blacklist'):
        step0_se_blacklist = parser.get('STEP0', 'step0_se_blacklist')
    else:
        step0_se_blacklist = ''


    
    #[AODSIM]
    if parser.has_option('AODSIM', 'aodsim_input_directory'):
        aodsim_input_directory  = parser.get('AODSIM', 'aodsim_input_directory')
    else:
        aodsim_input_directory = lhe_output_directory

    if parser.has_option('AODSIM', 'aodsim_scheduler'):
        aodsim_scheduler = parser.get('AODSIM', 'aodsim_scheduler')
    else:
        aodsim_scheduler = 'condor'
    if parser.has_option('AODSIM', 'aodsim_events_per_job'):
        aodsim_events_per_job = parser.get('AODSIM', 'aodsim_events_per_job')
    else:
        aodsim_events_per_job = '1'
    if parser.has_option('AODSIM', 'aodsim_storage_path'):
        aodsim_storage_path = parser.get('AODSIM', 'aodsim_storage_path')
    else:
        aodsim_storage_path = '/srm/managerv2?SFN=11'
    if parser.has_option('AODSIM', 'aodsim_user_remote_directory'):
        aodsim_user_remote_directory = parser.get('AODSIM', 'aodsim_user_remote_directory')
    else:
        aodsim_user_remote_directory = '/store/user/susygen/'
    if parser.has_option('AODSIM', 'aodsim_se_whitelist'):
        aodsim_se_whitelist = parser.get('AODSIM', 'aodsim_se_whitelist')
    else:
        aodsim_se_whitelist = ''
    if parser.has_option('AODSIM', 'aodsim_se_blacklist'):
        aodsim_se_blacklist = parser.get('AODSIM', 'aodsim_se_blacklist')
    else:
        aodsim_se_blacklist = ''
    if parser.has_option('AODSIM', 'aodsim_storage_element'):
        aodsim_storage_element = parser.get('AODSIM', 'aodsim_storage_element')
    else:
        aodsim_storage_element = 'cmssrm.fnal.gov'

##########End Parse Configuration File###########

    LHEToAODSIM_cfgTemplateName = 'LHEToAODSIMTemplate_4_2_3_cfg.py'
    if cmsDir.split('/')[-1] == 'CMSSW_5_2_2':
        LHEToAODSIM_cfgTemplateName = 'LHEToAODSIMTemplate_5_2_2_cfg.py'
        if do_step0:
            sys.stderr.write('step0 only supported in 5_2_6, set "do_step0" to "False".')
            sys.exit(0)

    elif cmsDir.split('/')[-1] == 'CMSSW_5_2_6':
        LHEToAODSIM_cfgTemplateName = 'LHEToAODSIMTemplate_5_2_6_cfg.py'
        LHEToEDM_cfgTemplateName = 'LHEToEDMTemplate_5_2_6_cfg.py'
        EDMToAODSIM_cfgTemplateName = 'EDMToAODSIMTemplate_5_2_6_cfg.py'
    elif cmsDir.split('/')[-1] == 'CMSSW_4_2_3':
        if do_step0:
            sys.stderr.write('step0 only supported in 5_2_6, set "do_step0" to "False".')
            sys.exit(0)
    else:
        sys.stderr.write('Your cms environment is not set to either  CMSSW_4_2_3, CMSSW_5_2_2 or CMSSW_5_2_6. Using 4_2_3 cfg files...\n')

    crab_LHEToEDMTemplateName = 'crab_LHEToEDMTemplate.cfg'
    crab_EDMToAODSIMTemplateName = 'crab_EDMToAODSIMTemplate.cfg'
   
    slhaTemplateName = 'SLHATemplate.slha'
    createSLHAsName = 'createSLHAs.py'
    createLHEsTemplateName = 'createLHEsTemplate.py'
    SLHAToLHE_cfgTemplateName = 'SLHAToLHETemplate_cfg.py'
    condor_createLHEsShTemplateName = 'condor_createLHEsTemplate.sh'
    condor_createLHEsSubmitTemplateName = 'condor_createLHEsTemplate.submit'
    
    templateSourceFileName = 'source_cff.py'
    LHESourceFileName = configFile.replace('.cfg', '') + '_lhe_source_cff.py'
    EDMSourceFileName = configFile.replace('.cfg', '') + '_edm_source_cff.py'
    crab_LHEToAODSIMTemplateName = 'crab_LHEToAODSIMTemplate.cfg'
    
    pythonDir = createScanDir + '/python/'
    setupDir = createScanDir + '/setup/'
    scanHomeDir = createScanDir + '/'  + configFile.replace('.cfg', '') + '/'
    slhaDir = scanHomeDir + 'SLHA/'
    lheDir = scanHomeDir  + 'LHE/'
    step0Dir = scanHomeDir + 'STEP0/'
    aodsimDir = scanHomeDir +'AODSIM/'
    logFilesDir = lheDir + 'logfiles/'    
    craberFile = 'craber.py'

    numberOfJobs = 1

    if step0_scheduler.find('condor') > -1:
        step0_use_server = 0
    else:
        step0_use_server = 1

    if aodsim_scheduler.find('condor') > -1:
        aodsim_use_server = 0
    else:
        aodsim_use_server = 1

    subprocess.call("mkdir " +  scanHomeDir, shell=True)
    subprocess.call("mkdir " + slhaDir, shell=True)
    subprocess.call("mkdir " + slhaDir + '/files', shell=True)
    subprocess.call("mkdir " + lheDir, shell=True)
    subprocess.call("mkdir " + lheDir + '/logfiles', shell=True)
    subprocess.call("mkdir " + aodsimDir, shell=True)
    subprocess.call("cp " + setupDir + templateSourceFileName + " " + 
                    pythonDir + LHESourceFileName, shell=True)
    subprocess.call("cp " + setupDir + craberFile + ' ' + aodsimDir, shell=True)
    if do_step0:
        subprocess.call("mkdir " + step0Dir, shell=True)

###########Begin Making Files#########

    makeSLHA(setupDir + slhaTemplateName, slhaDir + slhaTemplateName.replace('Template', ''), involved_susy_pdgIds, masses, branchingRatios, decay_of_pdgId)

    makeCreateSLHAs( slhaTemplateName.replace('Template','') , slhaDir + createSLHAsName, slhaDir + 'files', model_tag, scan_parameter_names, scan_parameter_mins, scan_parameter_maxs, scan_parameter_steps, masses, mass_definition_of_pdgId, cut_number)
    
    makeSLHAToLHECfg(setupDir + SLHAToLHE_cfgTemplateName, lheDir + SLHAToLHE_cfgTemplateName.replace('Template', ''), events_per_point, allowed_subprocesses, other_pythia_commands, slha_output_directory.replace(cmsDir + '/src/', ''), energy, random_seed)

    makeCreateLHEs( setupDir + createLHEsTemplateName, lheDir + createLHEsTemplateName.replace('Template', ''), createScanDir, lheDir + SLHAToLHE_cfgTemplateName.replace('Template', ''), points_per_file, use_default_header, run_lhe_check, lhe_input_directory, lhe_output_directory, pythonDir + LHESourceFileName, aodsimDir + crab_LHEToAODSIMTemplateName.replace('Template', ''), insert_cross_section, energy)

    makeCondorCreateLHEsSubmit(setupDir + condor_createLHEsSubmitTemplateName, lheDir + condor_createLHEsSubmitTemplateName.replace('Template', ''), model_tag, logFilesDir, createScanDir, lhe_output_directory, str(numberOfJobs))
    
    makeCondorCreateLHEsSh(setupDir + condor_createLHEsShTemplateName, lheDir + condor_createLHEsShTemplateName.replace('Template', ''), scramArch)

    if do_step0:
        makeLHEToEDMCfg(setupDir + LHEToEDM_cfgTemplateName,  step0Dir + LHEToEDM_cfgTemplateName.replace('Template', '').replace('_5_2_2','').replace('_4_2_3', ''), cmsDir, pythonDir + LHESourceFileName, model_tag + "_STEP0.root")
        
        makeCrabLHEToEDM(setupDir + crab_LHEToEDMTemplateName, step0Dir + crab_LHEToEDMTemplateName.replace('Template', ''), step0_scheduler, str(step0_user_server), '0', events_per_job, step0_storage_path, step0_user_remote_directory, step0_se_whitelist, step0_se_blacklist, model_tag)

        makeEDMToAODSIMCfg(setupDir + EDMToAODSIM_cfgTemplateName,  aodsimDir + EDMToAODSIM_cfgTemplateName.replace('Template', '').replace('_5_2_2','').replace('_4_2_3', ''), cmsDir, pythonDir + EDMSourceFileName, model_tag + "_AODSIM.root", energy)
        
        makeCrabEDMToAODSIM(setupDir + crab_EDMToAODSIMTemplateName, aodsimDir + crab_EDMToAODSIMTemplateName.replace('Template', ''), aodsim_scheduler, str(aodsim_use_server), '0', events_per_job, aodsim_storage_path, aodsim_user_remote_directory,aodsim_se_whitelist, aodsim_se_blacklist, model_tag)
    else:
        makeLHEToAODSIMCfg(setupDir + LHEToAODSIM_cfgTemplateName,  aodsimDir + LHEToAODSIM_cfgTemplateName.replace('Template', '').replace('_5_2_2','').replace('_4_2_3', '').replace('_5_2_6', ''), cmsDir, pythonDir + LHESourceFileName, model_tag + "_AODSIM.root", energy)
        
        makeCrabLHEToAODSIM(setupDir + crab_LHEToAODSIMTemplateName, aodsimDir + crab_LHEToAODSIMTemplateName.replace('Template', ''), aodsim_scheduler, str(aodsim_use_server), aodsim_events_per_job, aodsim_storage_path, aodsim_user_remote_directory,aodsim_se_whitelist, aodsim_se_blacklist, dataset_name, aodsim_storage_element)


#############End Making Files###########
