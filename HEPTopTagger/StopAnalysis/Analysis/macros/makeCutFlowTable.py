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


if __name__=="__main__":

    parser = OptionParser()

    parser.add_option("-w", "--weights", dest = "weights", default = "", help = 'Event weight of each input file (put in "" and separate by ",").', metavar = "FLOAT")

    parser.add_option("-l", "--legendNames", dest = "legendNames", default = "", help = 'Put in a list of names that you want to appear in the legend(put the list in "" and separate by ","). If not enough names are given the the file names will be used.' , metavar = "STRING")
    (options, args) = parser.parse_args()

    inputFileNames = args
    weights = []
    cutFlowTable = []
    printLines = []
    legendNames = []

    if options.weights == "" or len(options.weights.split(",")) != len(args):
	    for i in range(0, len(args)):
		    weights.append(1.0)
    else:
	    for weight in options.weights.split(","):
		    weights.append(float(weight))


    if len(args) < 2:
        sys.stderr.write("Must give at least two files to be merged\n")
        sys.exit(0)


    if options.legendNames == "":
        for inputFileName in inputFileNames:
            legendNames.append(inputFileName.split("/")[-1].split("_")[0])
    else:
        legendNames = options.legendNames.split(",")
        if len(legendNames) != len(inputFileNames):
            sys.stderr.write("Number of legend names does not equal number of" +
                             "input files. Using default names.\n")
            legendNames = []
            for inputFileName in inputFileNames:
                legendNames.append(inputFileName.split("/")[-1].split("_")[0])


    
    printLines.append("Dataset\tNo Cuts\t")
    for i in range(0,len(args)):
        file = TFile(args[i])
        histogram = file.Get("metTopAnalyzer/cutFlow")
        cutFlowTable.append([])
        for j in range(1, histogram.GetNbinsX() + 1):
            if i == 0 and j > 1:
                printLines[0] = printLines[0] + "Cut " + str(j-1) + "\t "
            if j == 1:
                printLines.append(legendNames[i] + "\t ")
                
            cutFlowTable[-1].append(weights[i]*histogram.GetBinContent(j))
            printLines[-1] = printLines[-1] + str(cutFlowTable[-1][-1]) + "\t "

    
    for line in printLines:
        print line
        
