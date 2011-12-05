#!/usr/local/bin/python
import subprocess, re, glob, os, sys, ConfigParser


def makeSLHA(templateName, outFileName, pgdIds, masses, branchingRatios, 
             decayProducts):
    
    oldFile = open(templateName, 'r')
    newFile = open(outFileName, 'w')
    inBlockMass = False
    inDecayTable = False
    decayLabel ='#           BR         NDA      ID1       ID2       ID3\n'

    if (len(pdgIds) != len(masses) or 
        len(pdgIds) != len(branchingRatios) or 
        len(pdgIds) != len(decayProducts)):
        sys.stderr.write('SLHA: Number of masses or decay products does not ' +
                         'match number of particles')
        return False
    for i in range(0, len(pdgIds)):
        if len(branchingRatios[i]) != len(decayProducts[i]):
            sys.stderr.write('SLHA: Number of branching ratios does not ' +  
                             'equal number of sets of decay products')
            return False
    
    for line in oldFile:
        if line.find('BLOCK MASS') > -1: inBlockMass = True
        if line.find('/') > -1: inBlockMass = False
        if line.find('DECAY TABLE') > -1: inDecayTable = True
        if line.find('/') > -1: inDecayTable = False

        if inBlockMass:
            for i in range(0, len(pdgIds)):
                if line.find(' ' + pdgIds[i] + ' ') > -1:
                    line = line.replace('1.00000000E+05', masses[i])
                    print 'New: ' + line
        if inDecayTable:
            for i in range(0, len(pdgIds)):
                
                if branchingRatios[i] == 'STABLE': continue

                if line.find(' ' + pdgIds[i] + ' ') > -1:
                    line = (line.replace('0.00000000E+00', '0.10000000E+00') + 
                            decayLabel)
                    print 'New: ' + line
                    for j in range(0, len(branchingRatios[i])):
                        
                        line = (line + '     ' +  branchingRatios[i][j] + 
                                '    ' + str(len(decayProducts[i][j])))
                        
                        for k in range(0, len(decayProducts[i][j])):
                            line = line + '    ' + decayProducts[i][j][k]
                        
                        line = line + '\n'
                        print 'New: ' + line
        newFile.write(line)
    
    oldFile.close()
    newFile.close()

if __name__ == '__main__':
    
    pdgIds = ['1000006', '1000021']
    masses = ['230', '490']
    branchingRatios = [ ['0.25000000E+00', '0.75000000+00'] , 
                       ['0.33333333E+00', '0.33333333E+00', '0.33333333E+00'] ]
    decayProducts = [ [  ['6', '1000022'], ['1000006', '11', '-11']  ] , 
                      [  ['1', '-1'], ['2', '-2'], ['3', '-3']  ] ]
    template = 'setup/SLHATemplate.slha'

    makeSLHA(template, 'temp.slha', pdgIds, masses, branchingRatios, decayProducts)
