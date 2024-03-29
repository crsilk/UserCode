##########createLHEs.py##################
#Christopher Silkworth (crsilk@cern.ch) 08 Decemeber 2011
##########################################
import os,sys,glob, re, shutil, subprocess
from xml.dom import minidom

def SLHASort(a,b):
    aList = a.split('_')
    bList = b.split('_')

    aList[-1] = aList[-1].replace('.slha', '')
    bList[-1] = bList[-1].replace('.slha', '')

    for i in range(0, len(aList)):
        if isFloat(aList[i]) and isFloat(bList[i]):
            if float(aList[i]) == float(bList[i]):
                continue
            return cmp(float(aList[i]), float(bList[i]))
                
def isFloat(n):
    try:
        float(n)
        return True
    except ValueError:
        return False
def stringToArray(string):
    array = string.split(" ")

    deletes = array.count('')
    for i in range(0,deletes):
        array.remove('')
    
    deletes =  array[-1].count('\t')
    for i in range(0, deletes):
            array[-1].remove('\t')
    return array
def grabXSection(logFile):
    file = open(logFile, 'r')
    for line in file:
        if line.find('All included subprocesses') > -1:
            xsection = line.split("I")[-2]
            xsection = xsection.replace('D', 'E')
            break
    return xsection
def countEvents(lheFile):
    file = open(lheFile, 'r')
    count = 0
    for line in file:
        if line.find("<event") > -1: count = count + 1

    return count
            
def makeLHE(templateCfgName, slhaFile, jobNumber):
    oldFile = open(templateCfgName, 'r')
    newFile = open('SLHAToLHETemp_cfg.py', 'w')

    for line in oldFile:
        line = line.replace('RUNSLHA', slhaFile)
        line = line.replace('JOBNUMBER', jobNumber)
        newFile.write(line)
    
    oldFile.close()
    newFile.close()
    
    subprocess.call("echo  \n #" + slhaFile.replace('.slha', '') + 
                     "\n \n >> SLHAToLHETemp.log", shell=True)
    subprocess.call("cmsRun SLHAToLHETemp_cfg.py| tee SLHAToLHETemp.log", shell=True)
    subprocess.call("rm SLHAToLHETemp_cfg.py", shell=True)

def insertComment(fileName,  comment):
    subprocess.call('sed -i -e s,"</event>","# model ' + comment +'\\n</event>", ' + 
                    fileName, shell=True)

def mergeEvents(inFileList, outFileName):

    startWritingString = "<event"
    endWritingString = "</LesHouchesEvents>"
    isWriting = False
    outFile = open(outFileName, 'a')

    for fileName in inFileList:

        file = open(fileName, 'r')

        for line in file:
            if isWriting == True and len(line) > 0:
                if line.find(endWritingString) > -1:

                    isWriting = False
                    break

                outFile.write(line)
            else:
                if line.find(startWritingString) > -1:

                    isWriting = True
                    outFile.write(line)
        file.close()
    outFile.close()

def makeLHEFileName(beginFileName, endFileName):
    modelTag = beginFileName.split('_')[0]
    
    beginFileMiddle = beginFileName.replace(modelTag, '').replace('.slha', '')[1:]
    endFileMiddle = endFileName.replace(modelTag, '').replace('.slha', '')[1:]
    
    combinedFileName = (modelTag + '_' + beginFileMiddle + 'To' + endFileMiddle + 
    '.lhe' )

    return combinedFileName
def startOutputFile(fileName, defaultHeader, templateCfgName, slhaFile, jobNumber, halfOfBeamEnergy):
    if defaultHeader:
	header=str("<LesHouchesEvents>\n\
<!--\n\
File generated with PYTHIA 6.424\n\
-->\n\
<header>\n\
<MGVersion>\n\
# MG/ME version    : 4.0.0\n\
</MGVersion>\n\
<MGProcCard>\n\
# Begin PROCESS # This is TAG. Do not modify this line\n\
pp>MODELTAG    @0       # First Process\n\
# End PROCESS  # This is TAG. Do not modify this line\n\
</MGProcCard>\n\
<MGGenerationInfo>\n\
#  Number of Events        :      NUMEVT\n\
</MGGenerationInfo>\n\
<slha>\n\
DECAY   1000022   0.0E+00\n\
</slha>\n\
</header>\n\
<init>\n\
    2212    2212  " + halfOfBeamEnergy + "  " + halfOfBeamEnergy + "     0     0 10042 10042     3    84\n\
  1.000000E+00  1.000000E+00  1.000000E+00   201\n\
  1.000000E+00  1.000000E+00  1.000000E+00   202\n\
  1.000000E+00  1.000000E+00  1.000000E+00   204\n\
  1.000000E+00  1.000000E+00  1.000000E+00   205\n\
  1.000000E+00  1.000000E+00  1.000000E+00   207\n\
  1.000000E+00  1.000000E+00  1.000000E+00   208\n\
  1.000000E+00  1.000000E+00  1.000000E+00   209\n\
  1.000000E+00  1.000000E+00  1.000000E+00   210\n\
  1.000000E+00  1.000000E+00  1.000000E+00   211\n\
  1.000000E+00  1.000000E+00  1.000000E+00   212\n\
  1.000000E+00  1.000000E+00  1.000000E+00   213\n\
  1.000000E+00  1.000000E+00  1.000000E+00   214\n\
  1.000000E+00  1.000000E+00  1.000000E+00   216\n\
  1.000000E+00  1.000000E+00  1.000000E+00   217\n\
  1.000000E+00  1.000000E+00  1.000000E+00   218\n\
  1.000000E+00  1.000000E+00  1.000000E+00   219\n\
  1.000000E+00  1.000000E+00  1.000000E+00   220\n\
  1.000000E+00  1.000000E+00  1.000000E+00   221\n\
  1.000000E+00  1.000000E+00  1.000000E+00   222\n\
  1.000000E+00  1.000000E+00  1.000000E+00   223\n\
  1.000000E+00  1.000000E+00  1.000000E+00   224\n\
  1.000000E+00  1.000000E+00  1.000000E+00   225\n\
  1.000000E+00  1.000000E+00  1.000000E+00   226\n\
  1.000000E+00  1.000000E+00  1.000000E+00   227\n\
  1.000000E+00  1.000000E+00  1.000000E+00   228\n\
  1.000000E+00  1.000000E+00  1.000000E+00   229\n\
  1.000000E+00  1.000000E+00  1.000000E+00   230\n\
  1.000000E+00  1.000000E+00  1.000000E+00   231\n\
  1.000000E+00  1.000000E+00  1.000000E+00   232\n\
  1.000000E+00  1.000000E+00  1.000000E+00   233\n\
  1.000000E+00  1.000000E+00  1.000000E+00   234\n\
  1.000000E+00  1.000000E+00  1.000000E+00   235\n\
  1.000000E+00  1.000000E+00  1.000000E+00   236\n\
  1.000000E+00  1.000000E+00  1.000000E+00   237\n\
  1.000000E+00  1.000000E+00  1.000000E+00   238\n\
  1.000000E+00  1.000000E+00  1.000000E+00   239\n\
  1.000000E+00  1.000000E+00  1.000000E+00   240\n\
  1.000000E+00  1.000000E+00  1.000000E+00   241\n\
  1.000000E+00  1.000000E+00  1.000000E+00   242\n\
  1.000000E+00  1.000000E+00  1.000000E+00   243\n\
  1.000000E+00  1.000000E+00  1.000000E+00   244\n\
  1.000000E+00  1.000000E+00  1.000000E+00   246\n\
  1.000000E+00  1.000000E+00  1.000000E+00   247\n\
  1.000000E+00  1.000000E+00  1.000000E+00   248\n\
  1.000000E+00  1.000000E+00  1.000000E+00   249\n\
  1.000000E+00  1.000000E+00  1.000000E+00   250\n\
  1.000000E+00  1.000000E+00  1.000000E+00   251\n\
  1.000000E+00  1.000000E+00  1.000000E+00   252\n\
  1.000000E+00  1.000000E+00  1.000000E+00   253\n\
  1.000000E+00  1.000000E+00  1.000000E+00   254\n\
  1.000000E+00  1.000000E+00  1.000000E+00   256\n\
  1.000000E+00  1.000000E+00  1.000000E+00   258\n\
  1.000000E+00  1.000000E+00  1.000000E+00   259\n\
  1.000000E+00  1.000000E+00  1.000000E+00   261\n\
  1.000000E+00  1.000000E+00  1.000000E+00   262\n\
  1.000000E+00  1.000000E+00  1.000000E+00   263\n\
  1.000000E+00  1.000000E+00  1.000000E+00   264\n\
  1.000000E+00  1.000000E+00  1.000000E+00   265\n\
  1.000000E+00  1.000000E+00  1.000000E+00   271\n\
  1.000000E+00  1.000000E+00  1.000000E+00   272\n\
  1.000000E+00  1.000000E+00  1.000000E+00   273\n\
  1.000000E+00  1.000000E+00  1.000000E+00   274\n\
  1.000000E+00  1.000000E+00  1.000000E+00   275\n\
  1.000000E+00  1.000000E+00  1.000000E+00   276\n\
  1.000000E+00  1.000000E+00  1.000000E+00   277\n\
  1.000000E+00  1.000000E+00  1.000000E+00   278\n\
  1.000000E+00  1.000000E+00  1.000000E+00   279\n\
  1.000000E+00  1.000000E+00  1.000000E+00   280\n\
  1.000000E+00  1.000000E+00  1.000000E+00   281\n\
  1.000000E+00  1.000000E+00  1.000000E+00   282\n\
  1.000000E+00  1.000000E+00  1.000000E+00   283\n\
  1.000000E+00  1.000000E+00  1.000000E+00   284\n\
  1.000000E+00  1.000000E+00  1.000000E+00   285\n\
  1.000000E+00  1.000000E+00  1.000000E+00   286\n\
  1.000000E+00  1.000000E+00  1.000000E+00   287\n\
  1.000000E+00  1.000000E+00  1.000000E+00   288\n\
  1.000000E+00  1.000000E+00  1.000000E+00   289\n\
  1.000000E+00  1.000000E+00  1.000000E+00   290\n\
  1.000000E+00  1.000000E+00  1.000000E+00   291\n\
  1.000000E+00  1.000000E+00  1.000000E+00   292\n\
  1.000000E+00  1.000000E+00  1.000000E+00   293\n\
  1.000000E+00  1.000000E+00  1.000000E+00   294\n\
  1.000000E+00  1.000000E+00  1.000000E+00   295\n\
  1.000000E+00  1.000000E+00  1.000000E+00   296\n\
</init>\n")
    else:
        makeLHE(templateCfgName, slhaFile, jobNumber)
        subprocess.call('mv fort.69' + fileName)
    header = header.replace("MODELTAG", fileName.split('_')[0])
    file = open(fileName, 'w')
    file.writelines(header)
    file.close()


def endOutputFile(fileName):
#    file = open(fileName, 'r')
#    numberOfEvents = 0

#    for line in file:
#        if line.find("<event") > -1:
#            numberOfEvents = numberOfEvents + 1

#    file.close()
    file = open(fileName , 'a')

    file.writelines("</LesHouchesEvents>")
    file.close()

#    os.system('sed -i 1,20s/NUMEVT/' + str(numberOfEvents) + '/ ' + fileName)
def checkStablesInLhe(lheFileName):
    file = open(lheFileName, 'r')

    for line in file:
        if line.find('1000022') == 1:
            if line[11:13] != ' 1':
                sys.stderr.write( "Unstable Chi_10. Skipping " + lheFileName)
                return False
        elif line.find('0000') == 2:
            if line[11:13] == ' 1':
                sys.stderr.write("Stable SUSY particle that isn't Chi_10. " + 
                                 "Skipping " + lheFileName)
                return False
    return True
def createCrabCfg(crabTemplateName, lheFileName, firstLumiNumber, totalEvents):
    newFileName = crabTemplateName.replace(crabTemplateName.split('/')[-1], '') + 'crab_' + lheFileName.split('/')[-1].replace('.lhe', '.cfg')

    oldFile = open(crabTemplateName, 'r')
    newFile = open(newFileName, 'w')

    for line in oldFile:
        line = line.replace('FULLLHEFILENAME',lheFileName)
        line = line.replace('FIRSTLUMINUMBER' , firstLumiNumber)
        line = line.replace('TOTALNUMBEROFEVENTS', totalEvents)
        line = line.replace('UIWORKINGDIR' , 'ui_' + lheFileName.split('/')[-1].replace('.lhe', ''))
        line = line.replace('LHEFILENAME' , lheFileName.split('/')[-1])
        newFile.write(line)

    oldFile.close()
    newFile.close()
def filterLHEs(lheFileName, target, filterPdgIds, filterRequirement):

    passFilter = False
    eventLines = []
    passedEvents = 0
    nParticles = 0
    modelTagList = ['']
    event = [0]

    oldFile = open(lheFileName, 'r')
    newFile = open(lheFileName + '.filtered', 'w')

    while True:
        line = oldFile.readline()
        if line.find("</init>") > -1:
            newFile.write(line)
            break
        if line.find("<event>") > -1:
            print "*** WARNING: No header for file ***"
            break
        newFile.write(line)
    
    while line and passedEvents < target:
        line = oldFile.readline()
        if line.find('<event>') > -1:
            eventLines = [line]
            nParticles = 0
            passFilter = False

            while line and not (line.find('</event>') > -1):
                line = oldFile.readline()
                eventLines.append(line)
            
            if not (eventLines[-1].find('</event>') > -1):
                print "*** WARNING: <event> block not closed ***"

            for eventLine in eventLines[2:-1]:
                pdgId = int(stringToArray(eventLine)[0])
                for filterPdgId in filterPdgIds:
                    if pdgId == int(filterPdgId):
                        nParticles = nParticles + 1
                        break
            if filterRequirement[0] == "<":
                if nParticles < int(filterRequirement[1]):
                    passFilter = True
            elif filterRequirement[0] == "==":
                if nParticles == int(filterRequirement[1]):
                    passFilter = True
            elif filterRequirement[0] == ">":
                if nParticles > int(filterRequirement[1]):
                    passFilter = True
            else:
                sys.stderr.write("*** Invalid filter_requirement value. Must be '<','==' or '>' . you have it set to " + str(filterRequirement) + " ***\n")
                sys.exit(0)
            if passFilter:
                passedEvents = passedEvents + 1
                for eventLine in eventLines:
                    newFile.write(eventLine)

    
    newFile.write("</LesHouchesEvents>\n")
    newFile.close()
    oldFile.close()
    
    subprocess.call('mv ' + lheFileName + '.filtered ' + lheFileName, 
                    shell=True)

if __name__ == '__main__':
    if len(sys.argv) != 2:
         sys.stderr.write("Usage <SLHA file bunch to run over>")

    jobNumber = sys.argv[1]
    filesPerJob = FILESPERJOB
    createScanDirectory = CREATESCANDIRECTORY
    useDefaultHeader = USEDEFAULTHEADER
    runLHECheck = RUNLHECHECK
    templateCfgName = TEMPLATECFG
    slhaScanDir = SLHASCANDIR
    outputDir = OUTPUTDIR
    sourceFileName = SOURCEFILE
    crabFileName = CRABFILE
    insertXSection = INSERTXSECTION
    energy = ENERGY
    jobsPerDir = JOBSPERDIR
    filterlhes = FILTERLHES
    target = TARGET
    filterPdgIds = stringToArray(FILTERPDGIDS)
    filterRequirement = stringToArray(FILTERREQUIREMENT)
    events = 0
    slhaBunch = []
    outputFileName =''
    allSlhas = glob.glob(slhaScanDir + '/*slha')

    halfOfBeamEnergy = '%(#)E' % {"#":energy/2.0}
	
    for i in range(0,len(allSlhas)):
        allSlhas[i] = allSlhas[i].split('/')[-1]

    allSlhas.sort(SLHASort)

    if filesPerJob < 1:
        slhaBunch = allSlhas[int(filesPerJob * int(jobNumber)):
                                 int(filesPerJob * int(jobNumber)) + 1]
    else:
        if int(filesPerJob)*(int(jobNumber) + 1)  > len(allSlhas): 
            slhaBunch = allSlhas[int(filesPerJob) * int(jobNumber): 
                                 len(allSlhas)]
        else:
            slhaBunch = allSlhas[ int(filesPerJob) * int(jobNumber): 
                                  int(filesPerJob) * (int(jobNumber) + 1)]

    if slhaBunch[0] == slhaBunch[-1]:
        if filesPerJob < 1:
            whichOne = int(jobNumber) % int(1/filesPerJob)
            outputFileName = slhaBunch[0].replace('.slha', 
                                                  '_' + str(whichOne) + '.lhe')
        else:
            outputFileName = slhaBunch[0].replace('.slha', '.lhe')

    else:
        outputFileName = makeLHEFileName(slhaBunch[0],slhaBunch[-1]) 
    
    if useDefaultHeader:
        slhaToStartOn = 0
    else:
        slhaToStartOn = 1
    
    startOutputFile(outputFileName, useDefaultHeader, templateCfgName, 
                    slhaBunch[0], jobNumber, halfOfBeamEnergy)

    for i in range(slhaToStartOn, len(slhaBunch)):
        
        makeLHE(templateCfgName, slhaBunch[i], jobNumber)
        if glob.glob('fort.69') == []:
            sys.stderr.write("No LHE file was created using slha file: " + 
                             slhaBunch[i] + "\nPossible causes:\n1) You" + 
                             " need to cmsenv.\n2) Something went wrong with" + 
                             " the cmsRun on that point.\n3) You do not have" +
                             "the correctly modified version of Pythia\n")
            continue
        if runLHECheck and not checkStablesInLhe:
            continue
        
        xsection = grabXSection('SLHAToLHETemp.log')
        if filterlhes:
            filterLHEs('fort.69', target, filterPdgIds, filterRequirement)
        events = countEvents('fort.69')
        if events != target and filterlhes:
            print "*** WARNING: target number of events not met for " + slhaBunch[i].replace('.slha', '')
        if insertXSection:
            insertComment('fort.69', slhaBunch[i].replace('.slha', '') + 
                          xsection)
        else:
            insertComment('fort.69', slhaBunch[i].replace('.slha', ''))
        mergeEvents(['fort.69'], outputFileName)
        
        subprocess.call('rm SLHAToLHETemp.log', shell=True)

	endOutputFile(outputFileName)
    subprocess.call('mv ' + outputFileName +' ' +outputDir, shell =True)
    subprocess.call('rm fort.69', shell=True)
    createCrabCfg(crabFileName, outputDir  + outputFileName, str( jobsPerDir*int(jobNumber)), str(events))
