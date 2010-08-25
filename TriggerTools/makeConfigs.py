#!/usr/bin/env python

import os, string, sys,re
from optparse import OptionParser

########## MAIN ##########


def main(argv):
##### Options / define variables #####
    parser = OptionParser()

    parser.add_option("-m", dest = "in_menu",
                      help = "Name of menu",
                      metavar = "MENU")
    parser.add_option("-l", dest = "in_lumiscale", default = '1.0',
                      help = "Lumi scale factor (default is 1.0)",
                      metavar = "FLOAT")
    parser.add_option("-f", action = "store_true", dest = "in_fulltable",
                      default = False, help = "Run over all datasets")

    (options, args) = parser.parse_args()
    paths = []
    template = open('hltmenu_MENU_RUNNUMBER_DS_DATASET.cfg.template', 'r')
    L1Triggers = pickOutL1Triggers('hltmenu_extractedhltmenu.cfg')
    minBiasPath = ''

    if os.path.isfile(args[0]):
        pathFile = open(args[0], 'r')
        for line in pathFile:
            if re.findall('MinimumBias', line) != []:
                minBiasPath = line[:-1]
            if line[-1:] == '\n':
                line = line[:-1]
                paths.append(line)
    else:
        for a in args:
            paths.append(a)
##### For the individual data sets #####
    if options.in_fulltable == False:        
        for in_path in paths:
            runNumber = re.findall('r[0-9]{6}',in_path)
            versionTag = re.findall('20[0-9]{6}', in_path)

            dataSetName = re.findall('BTau|Commissioning|Cosmics|EGMonitor|EG|HcalHPDNoise|HcalNZS|JetMETTauMonitor|JetMET|MinimumBias|MuMonitor|MuOnia|Mu', in_path)
            dataSet = pickOutDataSet('hltmenu_extractedhltmenu.cfg',dataSetName[0])
            lastline = dataSet[len(dataSet) -1] 
            outfilename = 'hltmenu_' + options.in_menu + '_' + runNumber[0] + '_DS_' + dataSetName[0] + '.cfg'
            outfile = open(outfilename, 'w')

            changedline =''            

            for line in template :
                if line == '############# dataset DATASET ###############\n':
                    for line2 in dataSet:
                        if line2 != lastline:
                            outfile.write(line2)
                        else:
                            if lastline[-2:] == ',\n':
                                line2 = lastline[:-2]
                            outfile.write(line2)

                elif line == ' # For L1 prescale preloop to be used in HLT mode only\n':

                    outfile.write(line)
                    for line2 in L1Triggers:
                        outfile.write(line2)
                else :
                    line = re.sub(r'NTUPLEPATH', in_path, line)
                    line = re.sub(r'MENU', options.in_menu, line)
                    line = re.sub(r'TAGNUMBER', versionTag[0], line)
                    line = re.sub(r'RUNNUMBER', runNumber[0], line)
                    line = re.sub(r'DATASET', dataSetName[0], line)
                    line = re.sub(r'LUMIFACTOR', options.in_lumiscale, line)
                    outfile.write(line)
    
            outfile.close()

##### For the full table #####
    else:
        template = open('hltmenu_MENU_RUNNUMBER_DS_DATASET.cfg.template', 'r')


        
        runNumber = re.findall('r[0-9]{6}',paths[0])
        versionTag = re.findall('20[0-9]{6}', paths[0])
        outfilename = 'hltmenu_' + options.in_menu + '_' + runNumber[0] + '_DS_FullTable.cfg'
        outfile = open(outfilename, 'w')




        for line in template :
            if line == '############# dataset DATASET ###############\n':
                for in_path in paths:

                    dataSetName = re.findall('BTau|Commissioning|Cosmics|EGMonitor|EG|HcalHPDNoise|HcalNZS|JetMETTauMonitor|JetMET|MinimumBias|MuMonitor|MuOnia|Mu', in_path)
                    
                    dataSet = pickOutDataSet('hltmenu_extractedhltmenu.cfg',dataSetName[0])
                    lastline = dataSet[len(dataSet) -1] 


                    if in_path == paths[len(paths) -1]:
                        for line2 in dataSet:
                            if line2 != lastline:
                                outfile.write(line2)
                            else:
                                if lastline[-2:] == ',\n':
                                    line2 = lastline[:-2]
                                outfile.write(line2)
                    else:
                        for line2 in dataSet:
                            outfile.write(line2)
                        

            elif line == ' # For L1 prescale preloop to be used in HLT mode only\n':

                outfile.write(line)
                for line2 in L1Triggers:
                    outfile.write(line2)
            else :
                line = re.sub(r'NTUPLEPATH', minBiasPath, line)
                line = re.sub(r'MENU', options.in_menu, line)
                line = re.sub(r'TAGNUMBER', versionTag[0], line)
                line = re.sub(r'RUNNUMBER', runNumber[0], line)
                line = re.sub(r'DATASET', 'FullTable', line)
                line = re.sub(r'LUMIFACTOR', options.in_lumiscale, line)
                outfile.write(line)

        outfile.close()


########## Functions ##########

def pickOutDataSet(fileName, datasetname):
    linebyline = []
    dataset = []
    beginstring = '############# dataset ' + datasetname + ' ###############\n'
    endstring = '#'
    lastline = ''
    copy = False
       
    f = open(fileName, 'r')

    for line in f:
        linebyline.append(line)

    for line in linebyline :
        if line[:1] == endstring :
            copy = False
        if line == beginstring:
            copy = True
        if copy:
            dataset.append(line)
   
    return dataset

def pickOutL1Triggers(fileName):
    linebyline = []
    L1s = []
    beginstring = ' L1triggers = ( \n'
    copy = False

    f = open(fileName, 'r')

    for line in f:
        if line == beginstring:
            copy = True
        if copy:
            L1s.append(line)

    return L1s



if __name__ == "__main__":
    main(sys.argv[1:])
