import os, sys, subprocess, math, inspect

####So that pyroot doesn't take my command line#########
tmpargv = sys.argv[:]
sys.argv = []
from ROOT import gSystem, gROOT, gStyle, gDirectory, TROOT, TH1, TFile, TTree, TKey, TObject
sys.argv = tmpargv
from optparse import OptionParser
#########################################################
def lineno():
    """Returns the current line number in our program."""
    return inspect.currentframe().f_back.f_lineno

def MergeHistoFiles(inputFileNames, directory, weights = []):
    firstInputFile = TFile(inputFileNames[0])
    directoryPath = directory.GetPath().split(":/")[1]

    firstInputFile.cd(directoryPath)
    keys = gDirectory.GetListOfKeys()

    for key in keys:

        firstInputFile.cd(directoryPath)
        object = key.ReadObj()

        if object.IsA().InheritsFrom("TH1"):

		    histogram1 = object
		    histogram1.Scale(weights[0])
		    i = 1
		    for inputFileName in inputFileNames[1:]:
			    inputFile = TFile(inputFileName)
			    inputFile.cd(directoryPath)

			    histogram2 = gDirectory.Get(histogram1.GetName())
			    histogram2.Scale(weights[i])
			    histogram1.Add(histogram2)
			    i = i + 1
        elif object.IsA().InheritsFrom("TDirectory"):
            directory.cd()
            newDirectory = directory.mkdir(object.GetName(), object.GetTitle())

            MergeHistoFiles(inputFileNames, newDirectory, weights)
        else:
            print ("Cannot merge object of type\nname: " +
                   str(object.GetName()) + "\ntitle: " + str(object.GetTitle()))

        directory.cd()
        object.Write(key.GetName())

    directory.Write()
if __name__=="__main__":

    parser = OptionParser()

    parser.add_option("-w", "--weights", dest = "weights", default = "", help = 'Event weight of each input file (put in "" and separate by ",").', metavar = "FLOAT")

    parser.add_option("-o", "--outputFile", dest = "outputFileName", default = "mergedFile.root", help= "Name of the output file", metavar ="FILE")

    (options, args) = parser.parse_args()

    weights = []

    if options.weights == "" or len(options.weights.split(",")) != len(args):
	    for i in range(0, len(args)):
		    weights.append(1.0)
    else:
	    for weight in options.weights.split(","):
		    weights.append(float(weight))


#    if len(args) < 2:
#        sys.stderr.write("Must give at least two files to be merged\n")
#        sys.exit(0)

    inputFileNames = args
    outputFile = TFile(options.outputFileName, "RECREATE")

    MergeHistoFiles(inputFileNames, outputFile, weights)
