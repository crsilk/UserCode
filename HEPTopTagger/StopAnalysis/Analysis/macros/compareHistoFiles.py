import os, sys, subprocess, math, inspect

####So that pyroot doesn't take my command line#########
tmpargv = sys.argv[:]
sys.argv = []
from ROOT import gSystem, gROOT, gStyle, gDirectory, TROOT, TH1, TFile, TTree, TKey, TObject, THStack, TLegend, TDirectory, TCanvas
sys.argv = tmpargv
from optparse import OptionParser
#########################################################
gStyle.SetStatColor(0)
gStyle.SetTitleFillColor(0)
gStyle.SetFrameBorderMode(0)
gStyle.SetPalette(1)
gStyle.SetOptStat(0)
def lineno():
    """Returns the current line number in our program."""
    return inspect.currentframe().f_back.f_lineno
def shrink1DHisto(histo):
    maxI = 0
    minI = 0
    for i in range(histo.GetNbinsX(), 1, -1):
        if histo.GetBinContent(i) != 0:
            maxI = i + 2
            break
    for i in range(1, histo.GetNbinsX()):
        if histo.GetBinContent(i) != 0:
            minI = i - 1
            break
    return (minI, maxI)
def getAllDirectoryPaths(inputFile, directories, directoryName = ""):

    inputFile.cd(directoryName)
    keys = gDirectory.GetListOfKeys()
    
    for key in keys:        
        object = key.ReadObj()

        if object.IsA().InheritsFrom("TDirectory"):
            dir = object
            newPath = dir.GetPath().split(":/")[1]
            directories.append(newPath)
            getAllDirectoryPaths(inputFile, directories, 
                                 directoryName = newPath )

def getAllHistogramsFromDirectory(inputFile, directoryName, histograms, 
                                  tag =''):
    
    inputFile.cd(directoryName)
    keys = gDirectory.GetListOfKeys()

    for key in keys:
        object = key.ReadObj()

        if (object.IsA().InheritsFrom("TH1") and 
            not object.IsA().InheritsFrom("TH2")) :
            histograms.append(object)
            histograms[-1].SetName(tag + histograms[-1].GetName())

if __name__== "__main__":

    parser = OptionParser()
    parser.add_option("-l", "--legendNames", dest = "legendNames", default = "", help = 'Put in a list of names that you want to appear in the legend(put the list in ""). If not enough names are given the the file names will be used.' , metavar = "STRING")
    parser.add_option("-c", "--saveCanvases" , dest = "saveCanvases", action="store_true", default = False, help = "Save the canvases as .gif files.")
    parser.add_option("-p", "--pause", dest = "pause", action="store_true", default=False, help = "Pause before closing (and/or saving) canvas, in order to make any final adjustments.")
    parser.add_option("-t", "--tag", dest = "tag", default = "", help = "Tag to insert into the name of the .gif files that are made")
    parser.add_option("-o", "--outputDirectory", dest = "outputDirectory", default = "", help= "Directory of ouputted plots.", metavar ="FILE")
    parser.add_option("-w", "--weights", dest = "weights", default = "", help = 'List of event weights(put list in ""and separate by "," ), defaults are 1.0.', metavar = "FLOAT")
    parser.add_option("-f", "--filterHistograms", dest = "filters", default ="", help = 'Only go through histograms which have all strings listed in filters (seperarate multiplies by ",")', metavar = "STRING")
    parser.add_option("-a", "--appendTitle", dest = "appendTitle", default = "",help = 'Adds a string to the end of the title', metavar="STRING")
    (options, args) = parser.parse_args()
	
    inputFileNames = args
    inputFiles = []
    directories = []
    histograms = []
    canvases = []
    legendNames = []
    filters = []
    weights = []
    legend = TLegend(0.65, 0.75, 0.89, 0.89)
    legend.SetFillColor(0)

    if options.weights == "":
        for inputFileName in inputFileNames:
            weights.append(1.0)
    else:
        for weight in options.weights.split(","):
            weights.append(float(weight))

    if options.legendNames == "":
        for inputFileName in inputFileNames:
            legendNames.append(inputFileName.split("/")[-1].split("_")[0])
    else:
        legendNames = options.legendNames.split(",")
        if len(legendNames) != len(inputFileNames):
            sys.stderr.write("Number of legend names does not equal number of" +
                             "input files. Using default names.")
            legendNames = []
            for inputFileName in inputFileNames:
                legendNames.append(inputFileName.split("/")[-1].split("_")[0])

    if options.filters != "":
        for filter in options.filters.split(","):
            filters.append(filter)


    for i in range(0, len(inputFileNames)):
        inputFiles.append(TFile(inputFileNames[i]))
        directories.append([])
        histograms.append([])
        getAllDirectoryPaths(inputFiles[i], directories[i])

        for directory in directories[i]:
            getAllHistogramsFromDirectory(inputFiles[i], directory, 
                                          histograms[i], options.tag)

    for i in range(0, len(histograms[0])):

        canvases.append(TCanvas())
        canvases[-1].cd()
        canvases[-1].SetFillColor(0)

        for j in range(0, len(histograms)):
            skip = False

            for filter in filters:
                if histograms[j][i].GetName().find(filter) > -1:
                    continue
                skip = True

            histograms[j][i].Scale(weights[j])

            if j + 1 < 3:
                histograms[j][i].SetLineColor(j + 1)
                histograms[j][i].SetMarkerColor(j + 1)
            else:
                histograms[j][i].SetLineColor(j + 2)
                histograms[j][i].SetMarkerColor(j + 2)
            histograms[j][i].SetLineWidth(1)
            histograms[j][i].SetMarkerStyle(22)
            histograms[j][i].SetTitle(histograms[j][i].GetTitle() +
                                      options.appendTitle)
		
            
            if i == 0:
                legend.AddEntry(histograms[j][i], legendNames[j])
            if j == 0:
                histograms[j][i].Draw()
            else:
                histograms[j][i].Draw("SAME")
        
        legend.Draw()
        if skip: continue
        if options.pause:
            raw_input("Make any last minute adjustments to " + 
                      histograms[0][i].GetName() + 
                      " before saving the canvas. "  + "Then press Enter")
        if options.saveCanvases:
            canvases[-1].SaveAs(options.outputDirectory + histograms[0][i].GetName() + ".eps")
    for inputFile in inputFiles:
        inputFile.Close()
        
