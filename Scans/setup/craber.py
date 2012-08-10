import os, sys, re, glob, subprocess, time, string
from optparse import OptionParser
def isFloat(n):
    try:
        float(n)
        return True
    except ValueError:
        return False

def fileSort(a,b):
    aArray1 = a.split('_')
    bArray1 = b.split('_')
    aArray2 = []
    bArray2 = []

    for i in range(0, len(aArray1)):
        if aArray1[i].find('To') > -1:
            aArray2.extend(aArray1[i].split('To'))
        else:
            aArray2.append(aArray1[i])
    for i in range(0, len(bArray1)):
        if bArray1[i].find('To') > -1:
            bArray2.extend(bArray1[i].split('To'))
        else:
            bArray2.append(bArray1[i])

    for i in range(0, len(aArray2)):
        if len(bArray2) <= i:
            return cmp(a, b)
        if isFloat(aArray2[i]) and isFloat(bArray2[i]):
            if aArray2[i] == bArray2[i]:
                continue
            else:
                return cmp(float(aArray2[i]), float(bArray2[i]))
        else:
            if aArray2[i] == bArray2[i]:
                continue
            else:
                return cmp(aArray2[i], bArray2[i])
    return cmp(a, b)

def getValuesFromCfg(crabCfg):
    crabFile = open(crabCfg, 'r')

    for line in crabFile:
        if line.find('total_number_of_events') > -1:
            total_number_of_events = line.split('=')[-1].replace(' ', '').replace('\n', '').replace('\t', '')
        if line.find('events_per_job') > -1:
            events_per_job = line.split('=')[-1].replace(' ', '').replace('\n', '').replace('\t', '')
        if line.find('additional_input_files') > -1:
            additional_input_files = line.split('=')[-1].replace(' ', '').replace('\n', '').replace('\t', '')

    lheFile = additional_input_files
    if int(int(total_number_of_events)/int(events_per_job)) == float(total_number_of_events)/float(events_per_job):
        numberOfJobs = int(total_number_of_events)/int(events_per_job)
    else:
        numberOfJobs = int(total_number_of_events)/int(events_per_job) +1
    eventsPerJob = int(events_per_job)
    return (lheFile, numberOfJobs, eventsPerJob)
def deleteSections(string, delimiterOfSection, arrayOfSectionsToDelete):
    splitter = string.split(delimiterOfSection)
    returnString = ''

    for i in arrayOfSectionsToDelete:
        splitter[i] = "DELETEME"

    for i in range(0, len(splitter)):
        if splitter[i] != "DELETEME":
            if i != len(splitter):
                returnString = returnString + splitter[i] + delimiterOfSection
            else:
                returnString = returnString + splitter[i]
    return returnString

if __name__ == '__main__':
    parser = OptionParser()
    
    parser.add_option("-r", "--crabRange", dest ="crabRange", help = "Range of crab_modelTag...cfg files to run over (default is all). If a list of crab_modelTag...cfg is given as an argument, they will override this option.", type = "int", nargs = 2, default = (0, -1), metavar="INTS")
    parser.add_option('-f', "--locationOfRootFiles", dest="locationOfRootFiles", help= "The outputted root file path used in '-k' mode.", default = '-1', metavar ="STRING")
    parser.add_option('-m', "--modelTag", dest="modelTag", help= "The model tag, that goes along with this scan (for example T2bqq). It will look for crab files of the form crab_modelTag_...cfg so you must enter this.", default = 'NOMODELTAG', metavar ="STRING")
    parser.add_option('-c', "--create", dest="create", help ="Create crab directories from all crab_modelTag...cfg files in jobRange (default is false)", action = "store_true", default =False, metavar="BOOL")
    parser.add_option('-s', "--submit", dest="submit", help ="Submit crab directories from all crab_modelTag...cfg files in jobRange (default is false)", action = "store_true", default =False, metavar="BOOL")
    parser.add_option('-t', "--status", dest="status", help ="Get status of crab directories from all crab_modelTag...cfg files in jobRange (default is false)", action = "store_true", default =False, metavar="BOOL")
    parser.add_option('-g', "--getoutput", dest="getoutput", help ="Get out put of crab directories from all crab_modelTag...cfg files in jobRange (default is false)", action = "store_true", default =False, metavar="BOOL")
    parser.add_option('-p', "--publish", dest="publish", help ="Publish crab directories from all crab_modelTag...cfg files in jobRange (default is false)", action = "store_true", default =False, metavar="BOOL")
    parser.add_option('-k', '--checkRootFiles', dest="checkRootFiles", help = "For each crab_modelTag...cfg in range, check which of the root files have been sucessfully finished and are in the root file path (-f). Also, checks for duplicate rootFiles. ", action="store_true", default=False, metavar="BOOL")
    parser.add_option('-l', '--listCrabNumbers', dest="listCrabNumbers", help = "Show list of all crab_modelTag...cfg and their corresponding 'crab number'.", action="store_true", default=False, metavar="BOOL")
    (options, args) = parser.parse_args()
    
    if options.modelTag == "NOMODELTAG":
        sys.stderr.write('** No model tag given (-m). Must give model tag **\n')
        sys.exit(0)
    if options.locationOfRootFiles == '-1' and options.checkRootFiles:
        sys.stderr.write('** Must give root file location (-l) in -k mode **\n')
        sys.exit(0)
    crabCfgs = glob.glob('crab_' + options.modelTag + '*.cfg')
    crabCfgs.sort(fileSort)

    selectedCrabCfgs = []
    tempDictionary = []
    dictionary = []
    count = -1
    if len(args) != 0:
        selectedCrabCfgs = args
    else:
        selectedCrabCfgs = crabCfgs[options.crabRange[0]:options.crabRange[1]]

  
    for crabCfg in crabCfgs:
        count = count + 1
        for selectedCrabCfg in selectedCrabCfgs:
            if selectedCrabCfg == crabCfg:
                tempDictionary.append({"crabNumber":count, "crabCfg":crabCfg})
                selectedCrabCfgs.remove(selectedCrabCfg)
                break
  
    for temp in tempDictionary:
        if options.checkRootFiles:
            allRootFiles = []
            allFjrFiles = []
            (lheFile, numberOfJobs, eventsPerJob) = getValuesFromCfg(temp['crabCfg'])
            for i in range(1, numberOfJobs + 1):
                allRootFiles.append(options.locationOfRootFiles + temp['crabCfg'].replace('crab_', '').replace('.cfg', '') +'_' + str(i) + "_")
                allFjrFiles.append("ui_" + temp['crabCfg'].replace('crab', '').replace('.cfg', '') + '/res/crab_fjr_' + str(i) + '.xml')
            dictionary.append({"crabNumber": temp['crabNumber'],
                               "crabCfg":temp['crabCfg'],
                               "uiDirectory": temp['crabCfg'].replace('crab_', 'ui_').replace('.cfg', ''),
                               "lheFile":lheFile, 
                               "numberOfJobs":numberOfJobs, 
                               "eventsPerJob":eventsPerJob, 
                               "totalEvents":(numberOfJobs*eventsPerJob),
                               "allRootFiles": allRootFiles,
                               "haveRootFiles": [],
                               "missingRootFiles":[],
                               "duplicateRootFiles":[],
                               "allFjrFiles":allFjrFiles,
                               "haveFjrFiles":[],
                               "missingFjrFiles": []})
                                    
        else:
            (lheFile, numberOfJobs, eventsPerJob) = getValuesFromCfg(temp['crabCfg'])
            dictionary.append({"crabNumber": temp['crabNumber'],
                               "crabCfg":temp['crabCfg'], 
                               "uiDirectory": temp['crabCfg'].replace('crab_', 'ui_').replace('.cfg', ''),
                               "lheFile":lheFile, 
                               "numberOfJobs":numberOfJobs, 
                               "eventsPerJob":eventsPerJob, 
                               "totalEvents":(numberOfJobs*eventsPerJob)})
                                  
    

    if options.listCrabNumbers:
        col_width = max(len(word) for word in crabCfgs) + 2
        for i in range(0, len(crabCfgs)):
            print "".join(crabCfgs[i].ljust(col_width)) + str(i)


    if options.checkRootFiles:
        for i in range(0, len(dictionary)):
            haveRootFiles = glob.glob(options.locationOfRootFiles + dictionary[i]["crabCfg"].replace('crab_', '').replace('.cfg', '') + '_*')
            duplicateRootFiles = []
            missingRootFiles = []
            haveRootFiles.sort(fileSort)
            
            for j in range(0, len(haveRootFiles) - 1):
                if deleteSections(haveRootFiles[j], '_', [-2, -1]) == deleteSections(haveRootFiles[j+1], '_', [-2, -1]):
                    duplicateRootFiles.append(haveRootFiles[j])
                    if duplicateRootFiles[-1] != haveRootFiles[j+1]:
                        duplicateRootFiles.append(haveRootFiles[j+1])
            
            for j in range(0, len(dictionary[i]["allRootFiles"])):
                hasIt = False
                for k in range(0, len(haveRootFiles)):

                    if dictionary[i]["allRootFiles"][j] == deleteSections(haveRootFiles[k], '_', [-2, -1]):
                        hasIt = True
                        break
                if not hasIt:
                    missingRootFiles.append(dictionary[i]["allRootFiles"][j])


            dictionary[i]["duplicateRootFiles"] = duplicateRootFiles
            dictionary[i]["missingRootFiles"] = missingRootFiles

            print "\n***" + dictionary[i]["crabCfg"].replace('crab_','').replace('.cfg', '') + "***"
            print "\nDuplicate root files:"
            for rootFile in dictionary[i]["duplicateRootFiles"]:
                print rootFile
            print "\nMissing root files:"
            for rootFile in dictionary[i]["missingRootFiles"]:
                print rootFile + "...root"

                
    if options.create or options.submit or options.status or options.getoutput or options.publish:
        for dict in dictionary:
            if options.create:
                print "crab -create -cfg " + dict["crabCfg"]
                subprocess.call("crab -create -cfg " + dict["crabCfg"], shell=True)
            if options.submit:
                print "crab -submit -c " + dict['uiDirectory']
                subprocess.call("crab -submit -c " + dict['uiDirectory'], shell=True)
            if options.status:
                print "crab -status -c " + dict['uiDirectory']
                subprocess.call("crab -status -c " + dict['uiDirectory'], shell=True)
            if options.getoutput:
                print "crab -getoutput -c " + dict['uiDirectory']
                subprocess.call("crab -getoutput -c " + dict['uiDirectory'], shell=True)
            if options.publish:
                print "crab -publish -c " + dict['uiDirectory']
                subprocess.call("crab -publish -c " + dict['uiDirectory'], shell=True)
