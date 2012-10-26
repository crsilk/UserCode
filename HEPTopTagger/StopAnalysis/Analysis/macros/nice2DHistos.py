import os, sys, subprocess, math, inspect

####So that pyroot doesn't take my command line#########
tmpargv = sys.argv[:]
sys.argv = []
from ROOT import gSystem, gROOT, gStyle, gDirectory, TROOT, TH1, TFile, TTree, TKey, TObject, THStack, TLegend, TDirectory, TCanvas, TLatex
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
        
        
        if object.IsA().InheritsFrom("TH2") :
            histograms.append(object)
            histograms[-1].SetName(tag + histograms[-1].GetName())
            
if __name__== "__main__":

    parser = OptionParser()
    parser.add_option("-c", "--saveCanvases" , dest = "saveCanvases", action="store_true", default = False, help = "Save the canvases as .gif files.")
    parser.add_option("-r", "--saveRootOfHistograms" , dest = "saveRootOfHistograms", action="store_true", default = False, help = "Save a root file for each histogram.")
    parser.add_option("-p", "--pause", dest = "pause", action="store_true", default=False, help = "Pause before closing (and/or saving) canvas, in order to make any final adjustments.")
    parser.add_option("-t", "--tag", dest = "tag", default = "", help = "Tag to insert into the name of the .gif files that are made")
    parser.add_option("-o", "--outputDirectory", dest = "outputDirectory", default = "", help= "Directory of ouputted plots.", metavar ="FILE")
    parser.add_option("-f", "--filterHistograms", dest = "filters", default ="", help = 'Only go through histograms which have all strings listed in filters (seperarate multiplies by ",")', metavar = "STRING")
    parser.add_option("-a", "--appendTitle", dest = "appendTitle", default = "",help = 'Adds a string to the end of the title', metavar="STRING")
    parser.add_option("-x", "--axisTitles", dest = "axisTitles", default = ("", ""), help = "Titles of the x and y axis respectively", type ="string", nargs = 2, metavar = "STRINGS")
    (options, args) = parser.parse_args()



    inputFileName = args[0]
    inputFile = TFile(inputFileName) 
    directories = []
    histograms = []
    canvases = []
    filters = []

    if options.filters != "":
        for filter in options.filters.split(","):
            filters.append(filter)


    getAllDirectoryPaths(inputFile, directories)

    for directory in directories:
        getAllHistogramsFromDirectory(inputFile, directory, histograms, 
                                      options.tag)
    if len(directories) == 0:
        getAllHistogramsFromDirectory(inputFile, "", histograms, options.tag)

    for i in range(0, len(histograms)):

        canvases.append(TCanvas())
        canvases[-1].cd()
        canvases[-1].SetFillColor(0)
        skip = False

        for filter in filters:
            if histograms[i].GetName().find(filter) > -1:
                continue
            skip = True

        histograms[i].SetTitle(histograms[i].GetTitle() +	
                               options.appendTitle)
        histograms[i].GetXaxis().SetTitle(options.axisTitles[0])
        histograms[i].GetYaxis().SetTitle(options.axisTitles[1])
        if skip: continue
        histograms[i].Draw("COLZ")
#        histograms[i].Draw("TEXT")
        if options.pause:
            raw_input("Make any last minute adjustments to " + 
                      histograms[i].GetName() + 
                      " before saving the canvas. "  + "Then press Enter")
        if options.saveCanvases:
            canvases[-1].SaveAs(options.outputDirectory + histograms[i].GetName() + ".gif")
        if options.saveRootOfHistograms:
            rootFile = TFile(options.outputDirectory + histograms[i].GetName() + ".root", "RECREATE")
            rootFile.cd()
            histograms[i].Write()
            rootFile.Close()
            inputFile.cd()

    inputFile.Close()
