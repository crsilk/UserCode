#!/usr/bin/env python

import os, string, sys,re

def main(argv):

    predictedFileName = argv[0]
    actualFileName = argv[1]
    outFileName = actualFileName[:-6] + '_Compare.twiki'


    
    actualFile = open(actualFileName,'r')
    outFile = open(outFileName, 'w')

    i = 0
    predBarLocations = []
    numberOfLines = 0
    
    for line in actualFile:
        barIter = re.finditer('\|', line)
        actBarLocations = []
        for j in barIter:
            actBarLocations.append(j.start())
##### For the first column /legend #####        
        if i == 0:

            line = re.sub(r'\*HLT Prescale\*', '*HLT Prescale(actual)* | *HLT Prescale(predicted)*', line)
            line = re.sub(r'\*HLT Rate \[Hz\]\*', '*HLT Rate(actual) [Hz]* | *HLT Rate(predicted) [Hz]*', line)
            line = re.sub(r'\*Total Rate \[Hz\]\*',  '*Total Rate(actual) [Hz]* | *Total Rate(predicted) [Hz]*', line)

##### For the total rate and throughput #####
        elif len(actBarLocations) == 4:
            if re.findall('\*Total', line) != []:
                line = re.sub('\*Rate \(AlCa not included\) \[Hz\]\*',
                              '*Rate (AlCa not included, actual)* [Hz] | *Rate (AlCa not included, predicted) [Hz]*| ', line)
                line = re.sub('\*Throughput \(AlCa included\) \[MB/s\]\*',
                               '*Throughput (AlCa included, actual) [MB/s]* | *Throughput (AlCa included, predicted) [MB/s]*', line)
            
            if re.findall('\| HLT ', line) != []:
                predictedFile = open(predictedFileName, 'r')
                for line2 in predictedFile:
                    if re.findall('\| HLT ', line2) != []:

                        barIter = re.finditer('\|', line2)
                        predBarLocations = []

                        for j in barIter:
                            predBarLocations.append(j.start())

                        hltRate = re.findall('[0-9\-+\.]+', line2[predBarLocations[1]:predBarLocations[2]])
                        throughputRate = re.findall('[0-9\-+\.]+', line2[predBarLocations[2]:predBarLocations[3]])

                        line = (line[:actBarLocations[2]] + '| ' + hltRate[0] +
                                line[actBarLocations[2]:actBarLocations[3]] +
                                '| ' + throughputRate[0] +
                                line[actBarLocations[3]:])
##### For the actual individual paths #####
        else:

            pathFound = False
            path = re.findall('\!HLT_[a-zA-Z0-9_]+', line)
            predictedFile = open(predictedFileName, 'r')

            for line2 in predictedFile:

                if re.findall(path[0], line2) != []:

                    pathFound = True
                    barIter = re.finditer('\|', line2)
                    predBarLocations = []

                    for j in barIter:
                        predBarLocations.append(j.start())
                    
                    predPrescale = re.findall('[.+\-0-9]+', line2[predBarLocations[3]:predBarLocations[4]])
                    predHLTRate = re.findall('[.+\-0-9]+', line2[predBarLocations[4]:predBarLocations[5]])
                    predTotalRate = re.findall('[.+\-0-9]+', line2[predBarLocations[5]:predBarLocations[6]])
            if pathFound == True :
                line = (line[:actBarLocations[4]] + '| ' + predPrescale[0] +
                        ' ' + line[actBarLocations[4]:actBarLocations[5]] +
                        '| ' + predHLTRate[0] + ' ' +
                        line[actBarLocations[5]:actBarLocations[6]] + '| ' +
                        predTotalRate[0] + ' ' + line[actBarLocations[6]:])
            else:
                line = (line[:actBarLocations[4]] + '| PATH NOT FOUND ' +
                        line[actBarLocations[4]:actBarLocations[5]]  +
                        ' | PATH NOT FOUND ' +
                        line[actBarLocations[5]:actBarLocations[6]] +
                        '| PATH NOT FOUND' + line[actBarLocations[6]:])
        outFile.write(line)
        i = i + 1
    
        
    outFile.close()


if __name__ == "__main__":
    main(sys.argv[1:])
