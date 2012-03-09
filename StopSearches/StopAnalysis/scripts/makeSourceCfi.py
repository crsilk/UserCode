import subprocess, glob
from optparse import OptionParser

if __name__ =="__main__":
    parser = OptionParser()

    parser.add_option("-o", "--outputFileName", dest = "outputFileName", default= "source_cfi.py", help = "Name of the outputted source file", metavar = "FILE")

    parser.add_option("-d", "--dcacheDirectory", dest = "dcap", action="store_true", default = False, help = "Files are located in dcache directory", metavar = "BOOL")

    parser.add_option("-s", "--storeDirectory", dest = "store", action="store_true", default = False, help = "Files are located in a store directory", metavar = "BOOL")

    
    (options, args) = parser.parse_args()
    
    fileNames = []
    for arg in args:
        fileNames.extend(glob.glob(arg + "*root"))

    outputFile = open(options.outputFileName, 'w')
    
header = ('import FWCore.ParameterSet.Config as cms\n'+
          '\n'+
          'maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )\n'+
          'readFiles = cms.untracked.vstring()\n'+
          'secFiles = cms.untracked.vstring() \n'+
          'source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)\n'+
          'readFiles.extend( [\n')
outputFile.write(header)

for fileName in fileNames:
    if options.dcap:
        fileName = 'dcap://' + fileName
    if options.store:
        fileName = '/store/' + fileName.split('/store/')[-1]
    outputFile.write('"' + fileName + '",\n')

outputFile.write('])')
