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
    return array
    
def stringToArrays(string):
    lines = string.split('\n')
    array = []
    for line in lines:
        array.append(line.split(' '))
        deletes = array[-1].count('')
        
        for i in range(0, deletes):
            array[-1].remove('')
    return array
        
def makeSLHAToLHECfg(templateFileName, newFileName, numberOfEvents, 
subprocesses, otherPythiaCommands, slhaDirectory, randomSeed):

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
                line = line + "'" + command + "',\n"
        if line.find("SLHADIRECTORY") > -1:
            line = line.replace("SLHADIRECTORY", slhaDirectory)
        
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

def makeCreateLHEs( oldFileName, newFileName, createScanDirectory, templateCfgName, filesPerJob, useDefaultHeader, runLheCheck, slhaScanDir, outputDir, sourceFileName, crabFile):
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
def makeLHEToAODSIMCfg(templateFileName, newFileName, cmsswDir, lheSourceFile, outputFileName):
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

def makeCrabLHEToAODSIM(templateFileName, newFileName, scheduler, useServer,
totalNumberOfEvents, numberOfJobs, storagePath, userRemoteDir, uiWorkingDir):
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
        newFile.write(line)
    oldFile.close()
    newFile.close()
    return newFileName
    
if __name__ == '__main__':

    decayProducts =[]
    branchingRatios = []
    masses = []
    massDefs = []
    cuts = []
    nSlhaFiles = 1
    counter = 0
    

    cmssw_base= subprocess.Popen("echo $CMSSW_BASE",stdout=subprocess.PIPE,
                                  shell=True) 
    cmsDir = cmssw_base.stdout.read().replace('\n', '')
    if cmsDir == '':
        sys.stderr.write("** Please run cmsenv before running createScan **\n")
        sys.exit(0)
    pwd = subprocess.Popen("pwd", stdout=subprocess.PIPE)
    createScanDir = pwd.stdout.read().replace('\n', '')

    configFile = sys.argv[1]
    
    parser = ConfigParser.ConfigParser()
    parser.read(configFile)

    #[General]
    modelTag = parser.get("General", "model_tag") 
    eventsPerPoint = parser.get("General", "events_per_point")
    pointsPerFile = parser.get("General", "points_per_file")

    #[SLHA]
    involvedParticles = stringToArray(parser.get("SLHA", 
                                                 "involved_susy_pdgIds"))
    scanParameterNames = stringToArray(parser.get("SLHA", 
                                                  "scan_parameter_names"))
    scanParameterMins = stringToArray(parser.get("SLHA", 
                                                  "scan_parameter_mins"))
    scanParameterMaxs = stringToArray(parser.get("SLHA", 
                                                  "scan_parameter_maxs"))
    scanParameterSteps = stringToArray(parser.get("SLHA", 
                                                  "scan_parameter_steps"))

    for i in range(0,len(involvedParticles)):
        masses.append('M' + involvedParticles[i])
        try:
            tempString = parser.get("SLHA", "decay_of_" + involvedParticles[i])
            tempArray = stringToArrays(tempString)
            branchingRatios.append([])
            decayProducts.append([])
            for j in range(0, len(tempArray)):
                branchingRatios[-1].append(tempArray[j][0])
                decayProducts[-1].append(tempArray[j][1:])

        except:
            branchingRatios.append(['STABLE'])
            decayProducts.append([])
    
    for i in range(0, len(involvedParticles)):
        try:
            massDefs.append(parser.get("SLHA", "mass_definition" + "_of_" + 
                                                     involvedParticles[i]) )
        except:
            sys.stderr.write("Mass of one of the one of involved particles " +
                             "not given. Fix cfg file")
            sys.exit(0)
    while True:
        counter = counter + 1
        try:
            cuts.append(parser.get("SLHA", "cut_" + str(counter)))
        except:
            break
    try:
        slhaOutputDir = parser.get("SLHA", "output_directory")
    except:
        slhaOutputDir = createScanDir + '/' + modelTag + '/SLHA/files/'


    #[LHE]
    allowedSubprocesses = stringToArray(parser.get("LHE", "allowed_subprocesses"))
    randomSeed = parser.get("LHE", "random_seed")
    lheOutputDir = parser.get("LHE", "output_directory")

    try:
        lheInputDir = parser.get("LHE", "input_directory")
    except:
        lheInputDir = createScanDir + '/' + modelTag + '/SLHA/files/'
    try:
        otherPythias = stringToArray(parser.get("LHE", "other_pythia_commands"))
    except:
        otherPythias = []
    try:
        runLheCheck = parser.get("LHE", "run_lhe_check")
    except:
        runLheCheck = "False"
    try:
        useDefaultHeader = parser.get("LHE", "use_default_header")
    except:
        useDefaultHeader = "True"


    #[AODSIM]
    scheduler = parser.get("AODSIM", "crab_scheduler")
    nCrabJobs = parser.get("AODSIM", "crab_number_of_jobs")
    storagePath = parser.get("AODSIM", "crab_storage_path")
    userRemoteDir = parser.get("AODSIM", "crab_user_remote_directory")
    

    slhaTemplateName = 'SLHATemplate.slha'
    createSLHAsName = 'createSLHAs.py'
    SLHAToLHE_cfgTemplateName = 'SLHAToLHETemplate_cfg.py'
    createLHEsTemplateName = 'createLHEsTemplate.py'
    condor_createLHEsName = 'condor_createLHEs.sh'
    condor_createLHEsSubmitTemplateName = 'condor_createLHEsTemplate.submit'
    templateSourceFileName = 'source_cff.py'
    sourceFileName = modelTag + '_source_cff.py'
    LHEToAODSIM_cfgTemplateName = 'LHEToAODSIMTemplate_cfg.py'
    crab_LHEToAODSIMTemplateName = 'crab_LHEToAODSIMTemplate.cfg'
    
    pythonDir = createScanDir + "/python/"
    setupDir = createScanDir + "/setup/"
    modelTagDir = createScanDir + "/"  + modelTag + "/"
    slhaDir = modelTagDir + "SLHA/"
    lheDir = modelTagDir  + "LHE/"
    aodsimDir = modelTagDir +"AODSIM/"
    logFilesDir = lheDir + 'logfiles/'
    
    for i in range(0, len(scanParameterNames)):
        nSlhasFiles = nSlhaFiles * (float(scanParameterMaxs[i]) - float(scanParameterMins[i]) )/float(scanParameterSteps[i])
    numberOfJobs = int(nSlhasFiles/float(pointsPerFile)) + 1

    if scheduler.find('condor') > -1:
        useServer = 0
    else:
        useServer = 1

    subprocess.call("mkdir " + createScanDir + "/" + modelTag, shell=True)
    subprocess.call("mkdir " + createScanDir + "/" + modelTag + "/" + "SLHA/",
                    shell=True)
    subprocess.call("mkdir " + createScanDir + "/" + modelTag + "/" +
                    "SLHA/files", shell=True)
    subprocess.call("mkdir " + createScanDir + "/" + modelTag + "/" + "LHE",
                    shell=True)
    subprocess.call("mkdir " + createScanDir + "/" + modelTag + "/" + 
                    "LHE/logfiles", shell=True)
    subprocess.call("mkdir " + createScanDir + "/" + modelTag + "/" + "AODSIM",
                    shell=True)
    subprocess.call("cp " + setupDir + templateSourceFileName + " " + 
                    pythonDir + sourceFileName, shell=True)
    subprocess.call("cp " + setupDir + condor_createLHEsName + " " + lheDir + 
                    condor_createLHEsName, shell=True)
    



    

#######################

    makeSLHA(setupDir + slhaTemplateName, slhaDir + slhaTemplateName.replace('Template', ''), involvedParticles, masses, branchingRatios, decayProducts)

    makeCreateSLHAs( slhaTemplateName.replace('Template','') , slhaDir + createSLHAsName, slhaDir + 'files', modelTag, scanParameterNames, scanParameterMins, scanParameterMaxs, scanParameterSteps, masses, massDefs, cuts)
    
    makeSLHAToLHECfg(setupDir + SLHAToLHE_cfgTemplateName, lheDir + SLHAToLHE_cfgTemplateName.replace('Template', ''), eventsPerPoint, allowedSubprocesses, otherPythias, slhaOutputDir.replace(cmsDir + '/src/', ''), randomSeed)

    makeCreateLHEs( setupDir + createLHEsTemplateName, lheDir + createLHEsTemplateName.replace('Template', ''), createScanDir, lheDir + SLHAToLHE_cfgTemplateName.replace('Template', ''), pointsPerFile, useDefaultHeader, runLheCheck, lheInputDir, lheOutputDir, pythonDir + sourceFileName, aodsimDir + crab_LHEToAODSIMTemplateName.replace('Template', ''))

    makeCondorCreateLHEsSubmit(setupDir + condor_createLHEsSubmitTemplateName, lheDir + condor_createLHEsSubmitTemplateName.replace('Template', ''), modelTag, logFilesDir, createScanDir, lheOutputDir, str(numberOfJobs))

    makeLHEToAODSIMCfg(setupDir + LHEToAODSIM_cfgTemplateName,  aodsimDir + LHEToAODSIM_cfgTemplateName.replace('Template', ''), cmsDir, pythonDir + sourceFileName, modelTag + "_AODSIM.root")

    makeCrabLHEToAODSIM(setupDir + crab_LHEToAODSIMTemplateName, aodsimDir + crab_LHEToAODSIMTemplateName.replace('Template', ''), scheduler, str(useServer), '0', nCrabJobs, storagePath, userRemoteDir + "/" + modelTag, modelTag)
