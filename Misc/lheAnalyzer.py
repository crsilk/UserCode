import os, re, subprocess, sys, math

#So that pyroot doesn't take my command line
tmpargv = sys.argv[:]
sys.argv = []
from ROOT import gSystem, gROOT, gStyle, TH1F, TH2F, TCanvas, TFile, TLatex
sys.argv = tmpargv
from optparse import OptionParser
#### ROOT stuff #####
gStyle.SetOptStat(111)
gStyle.SetStatColor(0);
gStyle.SetTitleFillColor(0);
gStyle.SetFrameBorderMode(0);
gStyle.SetPalette(1);
######################

def splitLine(line):
    lineArray = []
    for i in line.split(' '):
        if i != '' and i != '\n' and i != '\t':
            lineArray.append(i.replace('\n', ''))
    return lineArray
def ptSort(particle1, particle2):
    return cmp(particle1.pt(), particle2.pt())
def average(array):
    value = 0
    for element in array:
        value = value + element

    return value/len(array)
def pdgIdToString(pdgId, type):
    pdgIdList = [1, 2, 3, 4, 5, 6, 11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 
                 1000001, 1000002, 1000003, 1000004, 1000005, 1000006, 1000011,
                 1000012, 1000013, 1000014, 1000015, 1000016, 2000001, 2000002,
                 2000003, 2000004, 2000005, 2000006, 2000011, 2000013, 2000015,
                 1000021, 1000022, 1000023, 1000024, 1000025]
    latexList = ['d', 'u', 's', 'c', 'b', 't', 'e', '#nu_{e}', '#mu', 
                 '#nu_{#mu}', '#tau', '#nu_{#tau}', 'g', '#gamma', 'Z_{0}', 
                 'W', '#tilde{d}_{L}', '#tilde{u}_{L}', '#tilde{s}_{L}', 
                 '#tilde{c}_{L}', '#tilde{b}_{L}', '#tilde{t}_{1}', 
                 '#tilde{e}_{L}', '#tilde{#nu}_{eL}',  '#tilde{#mu}_{L}', 
                 '#tilde{#nu}_{#mu L}', '#tilde{#tau}_{L}', 
                 '#tilde{nu}_{#tau L}','#tilde{d}_{R}', '#tilde{u}_{R}', 
                 '#tilde{s}_{R}', '#tilde{c}_{R}', '#tilde{b}_{R}', 
                 '#tilde{t}_{2}','#tilde{e}_{R}', '#tilde{#mu}_{R}', 
                 '#tilde{#tau}_{R}', '#tilde{g}', '#tilde{#chi}^{0}_{1}', 
                 '#tilde{#chi}^{0}_{2}', '#tilde{#chi}^{#pm}_{1}', 
                 '#tilde{#chi}^{0}_{3}']
    tagList = ['down', 'up', 'strange', 'charm', 'bottom', 'top', 'electron',
                'electronNeutrino', 'muon', 'muonNeutrino', 'tau', 
                'tauNeutrino', 'gluon', 'photon', 'Z', 'W', 'sdownL', 'supL', 
                'sstrangeL', 'scharmL', 'sbottomL', 'stopL', 'selectronL', 
                'electronSneutrino', 'smuonL', 'muonSneutrino', 'stauL', 
                'stauNeutrino','sdownR', 'supR', 'sstrangeR', 'scharmR', 
                'sbottomR', 'stopR', 'selectronR', 'smuonR', 'stauR', 'gluino',
                'neutralino1', 'neutralino2', 'chargino1', 'neutralino3']
	
    string = str(pdgId)
    for i in range(0, len(pdgIdList)):
        if pdgIdList[i] == pdgId:
            if type == "latex":
                string = latexList[i]
                break
            if type == "tag":
                string = tagList[i]
                break
    return string
def shrink1DHisto(histo):
    maxI = 0
    for i in range(histo.GetNbinsX(), 1, -1):
        if histo.GetBinContent(i) != 0:
            maxI = i + 1
            break
    if maxI > 1:
        histo.GetXaxis().SetRange(0, maxI+1)
def shrink2DHisto(histo):
    maxI = 0
    maxJ = 0
    for i in range(histo.GetNbinsX(), 1, -1):
        if maxI != 0:
            break
        for j in range(histo.GetNbinsY(), 1, -1):
            if histo.GetBinContent(i, j) != 0:
                maxI = i + 1
                break

    for j in range(histo.GetNbinsY(), 1, -1):
        for i in range(histo.GetNbinsX(), 1, -1):
            if histo.GetBinContent(i, j) != 0:
                maxJ = j + 1
    if maxI > 1:
        histo.GetXaxis().SetRange(0, maxI + 1)
    if maxJ > 1:
        histo.GetYaxis.SetRange(0, maxJ + 1)

class Particle:
    def __init__(self, lheLineArray):
        self.pdgId_ = int(lheLineArray[0])
        self.status_ = int(lheLineArray[1])
        self.px_ = float(lheLineArray[6])
        self.py_ = float(lheLineArray[7])
        self.pz_ = float(lheLineArray[8])
        self.energy_ = float(lheLineArray[9])
        self.mass_ = float(lheLineArray[10])
    def pdgId(self):
        return self.pdgId_
    def status(self):
        return self.status_
    def px(self):
        return self.px_
    def py(self):
        return self.py_
    def pz(self):
        return self.pz_
    def energy(self):
        return self.energy_
    def mass(self):
        return self.mass_
    def p(self):
        return math.sqrt( self.px()*self.px() + self.py()*self.py() +
                                         self.pz()*self.pz() )
    def pt(self):
        return math.sqrt( self.px()*self.px() + self.py()*self.py() )
    def eta(self):
        if abs(self.p() - self.pz()) < 0.00000001: return 6
        if abs(self.p() + self.pz()) < 0.00000001: return -6

        return 0.5 * math.log( ( self.p() + self.pz() )/( self.p() - self.pz() ) )
    def phi(self):
        if self.pt() < 0.000001: return 0

        if self.py() > 0:
            return math.acos( self.px()/self.pt() )
        else:
            return -math.acos(self.px()/self.pt())
class Event:
    def __init__(self, particles, modelParameters, otherParameters):
        self.particles_ = particles
        self.particles_.sort(ptSort)
        self.modelParameters_ = modelParameters
        self.otherParameters_ = otherParameters

    def modelParameter(self, param):
        return self.modelParameters_[param]
    def modelParameters(self):
        return self.modelParameters_
    def otherParameter(self, param):
        return self.otherParameters_[param]
    def sumPt(self):
        Pt = 0
        for particle in self.particles_:
            if particle.status() != 1: continue
            Pt = Pt + particle.pt()
        return Pt
    def met(self):
        px = 0
        py = 0

        for particle in self.particles_:
            if particle.status() != 1: continue
            if particle.pdgId() > 1000000: continue
            px = px + particle.px()
            py = py + particle.py()
        return math.sqrt(px*px + py*py)
    def grabParticles(self, pdgIdList, statusCheck = True):
        particles = []
        for particle in self.particles_:
            if pdgIdList.count(abs(particle.pdgId())) > 0:
                if statusCheck  and particle.status() != 1: continue
                particles.append(particle)

        return particles
    def nParticles(self, pdgIdList):
        number = 0
        for particle in self.particles_:
            if particle.status() != 1: continue
            if pdgIdList.count(abs(particle.pdgId())) > 0:
                number = number + 1
        return number
    def rankedPt(self, pdgIdList, rank):
        rankCounter = 0
        pt = -1
        for particle in self.particles_:
            if particle.status() != 1: continue
            if pdgIdList.count(abs(particle.pdgId())) > 0:
                rankCounter = rankCounter + 1
                if rank == rankCounter:
                    pt = particle.pt()
                    break
        return p
class ParticleHistos:

    def __init__(self, pdgId, tag = "", latex = ""):
        self.histograms = []
        self.pdgId = pdgId
        self.tag = ""
        self.latex = ""

        if tag == "": 
            self.tag = pdgIdToString(pdgId, "tag")
        else: 
            self.tag = tag
        if latex == "": 
            self.latex = pdgIdToString(pdgId, "latex")
        else: 
            self.latex = latex
#########These graphs were rebinned to 5 GeV##########
        self.histograms.append(TH1F(self.tag +"Number", "Number of " + 
                                    self.latex, 20, 0 , 20))
        self.histograms[-1].GetXaxis().SetTitle("Number")
        self.histograms.append(TH1F(self.tag + "Pt", self.latex + " p_{t}", 
                                    400, 0, 2000))
        self.histograms[-1].GetXaxis().SetTitle("p_{t} (GeV)")
        self.histograms.append(TH1F(self.tag + "1Pt", "1st " + self.latex + 
                                    " p_{t}", 400, 0, 2000))
        self.histograms[-1].GetXaxis().SetTitle("p_{t} (GeV)")
        self.histograms.append(TH1F(self.tag + "2Pt", "2nd " + self.latex + 
                                    " p_{t}", 400, 0, 2000))
        self.histograms[-1].GetXaxis().SetTitle("p_{t} (GeV)")
        self.histograms.append(TH1F(self.tag + "Eta", self.latex + " #eta", 
                                    100, -5, 5))
        self.histograms[-1].GetXaxis().SetTitle("#eta")
        self.histograms.append(TH1F(self.tag + "Phi", self.latex + " #phi", 
                                    100,-math.pi, math.pi))
        self.histograms[-1].GetXaxis().SetTitle("#phi")
        self.histograms.append(TH1F(self.tag +"Mass", self.latex + " mass", 
                                    400, 0,  2000))
        self.histograms[-1].GetXaxis().SetTitle("Mass (GeV)")
        
        
    def Fill(self,particleList):
        self.histograms[0].Fill(len(particleList))
        if len(particleList) > 0:
            self.histograms[2].Fill(particleList[0].pt())
        if len(particleList) > 1:
            self.histograms[3].Fill(particleList[1].pt())
            
        for particle in particleList:
            self.histograms[1].Fill(particle.pt())
            self.histograms[4].Fill(particle.eta())
            self.histograms[5].Fill(particle.phi())
            self.histograms[6].Fill(particle.mass())
    
    def ShrinkHistos(self):
        shrink1DHisto(self.histograms[0])
        shrink1DHisto(self.histograms[1])
        shrink1DHisto(self.histograms[2])
        shrink1DHisto(self.histograms[3])
        shrink1DHisto(self.histograms[6])
    def Write(self):
        for histogram in self.histograms:
            histogram.Write()
        
    def SaveAsGif(self, tag):
        canvas = TCanvas()
        canvas.SetFillColor(0)
        canvas.cd()
        
        for histogram in self.histograms:
            histogram.Draw()
            canvas.SaveAs(tag+ "_" + histogram.GetName() + ".gif")
class ModelPoint:
    def __init__(self,parameters, particlePdgIds = [], 
                 otherParameterNames = []):
        self.parameters_ = parameters
        self.nEvents_ = 0
        self.particlePdgIds_ = particlePdgIds
        self.particleNames_ = []
        self.particleNumbers_ = []
        self.particleMasses_ = []
        for pdgId in particlePdgIds:
            self.particleNames_.append(pdgIdToString(pdgId, "tag"))
            self.particleNumbers_.append(0)
            self.particleMasses_.append(0)
        self.otherParameterNames_ = otherParameterNames
        self.otherParameterValues_ = []
        for otherParameter in self.otherParameterNames_:
            otherParameterValues_.append(0)
        

    def addEvent(self, event):
        self.nEvents_ = self.nEvents_ + 1
        for particle in event.particles_:
            for i in range(0, len(self.particlePdgIds_)):
                if abs(self.particlePdgIds_[i]) == abs(particle.pdgId()):
                    self.particleMasses_[i] = ((self.particleMasses_[i] * 
                                                self.particleNumbers_[i] + 
                                                particle.mass()) /
                                               (self.particleNumbers_[i] + 1))
                    
                    self.particleNumbers_[i] = self.particleNumbers_[i] + 1

    def setOtherParameters(self, otherParameters):
        for i in range(0, len(otherParameters)):
            self.otherParameterValues_[i] = otherParameters[i]
    def getParameters(self):
        return self.parameters_
    def particleNames(self):
        return self.particleNames_
    def particlePdgIds(self):
        return self.particlePdgIds_
    def particleMasses(self):
        return self.particleMasses_
    def otherParameterNames(self):
        return self.otherParticleNames_
    def otherParameterValues(self):
        return self.otherParameterValues_
    def nEvents(self):
        return self.nEvents_
                                                              
            
        
if __name__=="__main__":
    parser = OptionParser()

    parser.add_option('-n', "--nEvents", dest = "nEvents", default = -1, type = "int", help = 'Number of events to run over.', metavar ="INT")

    parser.add_option("-s", "--singlePoint", dest = "singlePoint", action = "store_true", default=False, help = "Create plots for single point (default creates a different set of plots for the entire scan).")

    parser.add_option("-p", "--parameterPoint", dest ="parameterPoint", help = "parameter point to run over (only works in -s mode).", type = "float", nargs = 2, metavar="POINT")

    parser.add_option("-w", "--whichParameters", dest="whichParameters", help= "which parameters are you to scan over in your model of the form modelTag_parameter1_parameter2_...._parameterN (default is -2 -1, i.e. parameters N-1, N).", default=(-2, -1), type = "int", nargs = 2, metavar ="PARAMETERS")

    parser.add_option("-t", "--parameterTags", dest="parameterTags", help="Name of the parameters you are scanning over (just to put in the histograms).", default = ("parameter1", "parmeter2"), type="string", nargs = 2, metavar="PARAMETERNAMES")

    parser.add_option("-c", "--saveCanvases", dest="saveCanvases", action = "store_true", default = False, help = "Save the histogram Canvases")
    (options, args) = parser.parse_args()
    if len(args) == 0:
        sys.stderr.write('No input lhe file given\n')
        sys.exit(0)
    
    
    outputFileName = ''
    eventCounter = 0
    inEvent = False
    particles = []
    modelParameters =[]
    otherParameters = []
    particleHistograms = []
    allPdgIds = [1, 2, 3, 4, 5, 6, 11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 
                 1000001, 1000002, 1000003, 1000004, 1000005, 1000006, 1000011,
                 1000012, 1000013, 1000014, 1000015, 1000016, 2000001, 2000002,
                 2000003, 2000004, 2000005, 2000006, 2000011, 2000013, 2000015,
                 1000021, 1000022, 1000023, 1000024, 1000025]
    susyPdgIds = [1000001, 1000002, 1000003, 1000004, 1000005, 1000006, 1000011,
                  1000012, 1000013, 1000014, 1000015, 1000016, 2000001, 2000002,
                  2000003, 2000004, 2000005, 2000006, 2000011, 2000013, 2000015,
                  1000021, 1000022, 1000023, 1000025, 1000024 ]
    jetPdgIds = [1, 2, 3, 4, 5, 21]
    leptonPdgIds = [11, 13, 15]
    lightSquarkPdgIds = [1000001, 1000002, 1000003, 1000004, 2000001, 2000002, 
                         2000003, 2000004]
    sleptonPdgIds = [1000011, 1000013, 1000015, 2000011, 2000013, 1000015]
    sneutrinoPdgIds = [1000012, 1000014, 1000016]
    existingModelPoints =[]
    modelPoints = []

    tag = args[0].split('/')[-1].replace('.lhe', '').split('_')[0] 

    if options.singlePoint:
        outputFileName = (tag + '_' + str(options.parameterPoint[0]) + '_' + 
                          str(options.parameterPoint[1]) + '.root')
    else:
        outputFileName = (tag + '_fullScan.root')
    outputRootFile = TFile(outputFileName, 'RECREATE')

    if options.singlePoint:
        outputRootFile.cd()
        outputRootFile.mkdir("generalHistograms")
        outputRootFile.mkdir("jetHistograms")
        outputRootFile.mkdir("leptonHistograms")
        outputRootFile.mkdir("lightSquarkHistograms")
        outputRootFile.mkdir("sleptonHistograms")
        outputRootFile.mkdir("sneutrinoHistograms")

        for pdgId in allPdgIds:
            outputRootFile.mkdir(pdgIdToString(pdgId, "tag") + "Histograms")
            
    
    metHistogram = TH1F("MET", "MET", 400, 0, 2000)
    metHistogram.GetXaxis().SetTitle("MET (GeV)")
    sumPtHistogram = TH1F("SumPt", "#Sigma P_{T}", 400, 0, 4000)
    sumPtHistogram.GetXaxis().SetTitle("#Sigma P_{T} (GeV)")

    jetHistograms = ParticleHistos(-1, tag = "jet", 
                                    latex = " 'jet'(unhadronized partons)")
    leptonHistograms = ParticleHistos(-1, tag = "lepton", 
                                       latex = "lepton")
    lightSquarkHistograms = ParticleHistos(-1, tag = "lightSquark", 
                                            latex = "#tilde{q}")
    sleptonHistograms = ParticleHistos(-1, tag = "slepton", latex = "#tilde{l}")
    sneutrinoHistograms = ParticleHistos(-1, tag ="sneutrinos", 
                                          latex = "#tilde{#nu}")
    for pdgId in allPdgIds:
        particleHistograms.append(ParticleHistos(pdgId))

        
    for inputFileName in args:
        lheFile = open(inputFileName, 'r')

        if options.nEvents != -1 and eventCounter > int(options.nEvents):
            break

        for line in lheFile:
            lineArray = []
            lineArray = splitLine(line)

            if lineArray[0].find("<event>") > -1:
                if options.nEvents != -1 and eventCounter >= int(options.nEvents):
                    break
                eventCounter = eventCounter + 1
                inEvent = True
                inFirstLine = True
                continue
            if lineArray[0].find("</event>") > -1:
                event = Event(particles, modelParameters, otherParameters)
                #################Begin Analyzing Event####################
                if options.singlePoint:
                    if (event.modelParameter(options.whichParameters[0]) ==
						options.parameterPoint[0] and
                        event.modelParameter(options.whichParameters[1]) ==
						options.parameterPoint[1]):

                        metHistogram.Fill(event.met())
                        sumPtHistogram.Fill(event.sumPt())
                        jetHistograms.Fill(event.grabParticles(jetPdgIds))
                        leptonHistograms.Fill(event.grabParticles(leptonPdgIds))
                        lightSquarkHistograms.Fill(event.grabParticles(lightSquarkPdgIds, statusCheck = False))
                        sleptonHistograms.Fill(event.grabParticles(sleptonPdgIds, statusCheck = False))
                        sneutrinoHistograms.Fill(event.grabParticles(sneutrinoPdgIds, statusCheck = False))

                        for i in range(0, len(allPdgIds)):
                            check = True

                            if allPdgIds[i] > 1000000: 
                                check = False

                            particleHistograms[i].Fill(event.grabParticles([allPdgIds[i]], statusCheck = check))
                            
                                                    
                else:
                    eventModelPoint = event.modelParameters()

                    if existingModelPoints.count(eventModelPoint) > 0:

                        index = existingModelPoints.index(eventModelPoint)
                        modelPoints[index].addEvent(event)

                    else:
                        existingModelPoints.append(eventModelPoint)
                        modelPoints.append(ModelPoint(eventModelPoint, particlePdgIds = susyPdgIds))
                        modelPoints[-1].addEvent(event)
                                           
                ####################End Analyzing Event ######################
                inEvent = False
                particles = []
                modelParameters = []
                otherParameters = []
                continue
            
            if not inEvent: continue
            if inFirstLine: 
                inFirstLine = False
                continue

            if lineArray[0] == '#':
                modelArray = lineArray[2].split('_')[1:]
                modelParameters = []
                for string in modelArray:
                    modelParameters.append(float(string))
                    

                otherParamters = lineArray[3:]
                continue
            
            if len(lineArray) < 9:
                sys.stderr.write("Something wrong with this line, too short: " + 
                           line + "\n")
                sys.exit(0)
            
            particles.append(Particle(lineArray))


    
    if options.singlePoint:
        outputRootFile.cd("generalHistograms")
        metHistogram.Write()
        sumPtHistogram.Write()
        outputRootFile.cd("jetHistograms")
        jetHistograms.ShrinkHistos()
        jetHistograms.Write()
        outputRootFile.cd("leptonHistograms")
        leptonHistograms.ShrinkHistos()
        leptonHistograms.Write()
        outputRootFile.cd("lightSquarkHistograms")
        lightSquarkHistograms.ShrinkHistos()
        lightSquarkHistograms.Write()
        outputRootFile.cd("sleptonHistograms")
        sleptonHistograms.ShrinkHistos()
        sleptonHistograms.Write()
        outputRootFile.cd("sneutrinoHistograms")
        sneutrinoHistograms.ShrinkHistos()
        sneutrinoHistograms.Write()
        
        for i in range(0, len(allPdgIds)):
            outputRootFile.cd(pdgIdToString(allPdgIds[i], "tag") + "Histograms")
            particleHistograms[i].ShrinkHistos()
            particleHistograms[i].Write()
    else:
        massHistograms = []
        
        minX = 100000000000
        maxX =-100000000000
        stepX = 10000000000
        nBinsX = 0
        nBinsY = 0
        minY = 100000000000
        maxY = -100000000000
        stepY = 100000000000
        
        arrayIndex1 = options.whichParameters[0]
        arrayIndex2 = options.whichParameters[1]
        
        for i  in range(0, len(existingModelPoints)):
            iparameter1 = existingModelPoints[i][arrayIndex1]
            iparameter2 = existingModelPoints[i][arrayIndex2]
            
            if minX > iparameter1:
                minX = iparameter1
            if maxX < iparameter1:
                maxX = iparameter1
            if minY > iparameter2:
                minY = iparameter2
            if maxY < iparameter2:
                maxY = iparameter2
            
            for j in range(0, len(existingModelPoints)):

                jparameter1 = existingModelPoints[j][arrayIndex1]
                jparameter2 = existingModelPoints[j][arrayIndex2]

                if (abs(iparameter1 - jparameter1) < stepX and
                    abs(iparameter1 - jparameter1) != 0 ) :
                    stepX = abs(iparameter1 - jparameter1)
                    
                if (abs(iparameter2 - jparameter2) < stepY and
                    abs(iparameter2 - jparameter2) != 0 ) :
                    stepY = abs(iparameter2 - jparameter2)
        
        if stepX == 10000000000 or stepY == 10000000000:
            sys.stderr.write("Not a scan, need at least four model points " + 
                             "with  (x1, y1),...,  (xn,yn) such that xi != xj"+
                             "and yi != yj for all i and j\n")
            sys.exit(0)

        print minX
        print maxX
        print stepX
        print " " 
        print minY
        print maxY
        print stepY
        maxX = maxX + 2* stepX
        maxY = maxY + 2* stepY
        if minX -  stepX >= 0:
            minX = minX  - stepX
        if minY -  stepY >= 0:
            minY = minY - stepY
        nBinsX = int((maxX - minX)/stepX)
        nBinsY = int((maxY - minY)/stepY)
        
        nEventsHistogram = TH2F("NEvents", "Number of Events", nBinsX, minX, maxX, nBinsY, minY, maxY)
        for modelPoint in modelPoints:
            nEventsHistogram.Fill(modelPoint.getParameters()[arrayIndex1],
                                  modelPoint.getParameters()[arrayIndex2],
                                  modelPoint.nEvents())
        nEventsHistogram.Write()

        for i in range(0, len(modelPoints[0].particleNames())):

            particleName = modelPoints[0].particleNames()[i]
            particlePdgId = modelPoints[0].particlePdgIds()[i]
            massHistograms.append(TH2F(particleName + "MassScan",
                                       pdgIdToString(particlePdgId, "latex") + 
                                       " mass", nBinsX, minX, maxX, nBinsY, 
                                       minY, maxY))
            massHistograms[-1].GetXaxis().SetTitle(options.parameterTags[0])
            massHistograms[-1].GetYaxis().SetTitle(options.parameterTags[1])
            for modelPoint in modelPoints:
                
                massHistograms[-1].Fill(modelPoint.getParameters()[arrayIndex1],
                                        modelPoint.getParameters()[arrayIndex2],
                                        modelPoint.particleMasses()[i])
        for massHistogram in massHistograms:

            massHistogram.Write()

        if options.saveCanvases:
            canvas = TCanvas()
            canvas.SetFillColor(0)
            canvas.cd()
            nEventsHistogram.Draw("colz")
            canvas.SaveAs(tag + "_" +nEventsHistogram.GetName() + ".gif")
            for massHistogram in massHistograms:
                massHistogram.Draw("colz")
                canvas.SaveAs(tag + "_" +massHistogram.GetName() + ".gif")
#        if options.saveCanvases:
#            neutralino1Histograms.ShrinkHistos()
#            neutralino1Histograms.SaveAsGif(tag)
