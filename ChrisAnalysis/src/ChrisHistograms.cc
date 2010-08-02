#include <iostream>

#include "PFAnalyses/ChrisAnalysis/interface/ChrisHistograms.h"

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
ChrisHistograms::ChrisHistograms(TFileDirectory *myDir, const std::string & name){

  AnalysisHistograms::init(myDir,name);

}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
ChrisHistograms::~ChrisHistograms(){ 

  std::cout<<"ChrisHistograms::~ChrisHistograms()"<<std::endl;

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void ChrisHistograms::defineHistograms(){

  using namespace std;

 if(!histosInitialized_){

   ///Here we define 1D histogram. The histograms are assigned to the TDirectory 
   /// pointed by the file argument 
   
   
   //   add1DHistogram("pDEta","; Photon Delta Eta; Evenes", 20, -2.5,2.5,file_);
   //   add1DHistogram("pDPhi","; Photon Delta Phi; Events", 20, -3.14,3.14,file_);
   
   add1DHistogram("inConePhotons", "; Number of Photons in the cone", 5,-.5,4.5, file_);
   add1DHistogram("inConeElectrons", "Number of Electrons in the cone",5,-.5,4.5, file_);
   add1DHistogram("inConePions", "Number of Pions in the cone",5,-.5,4.5, file_);
   add1DHistogram("inConeKL0s", "Number of KL0s in the cone",10,-.5,9.5, file_);
   add1DHistogram("inConePFParticles", "Number of PFParticles in the cone",20,-.5,19.5, file_);

      add1DHistogram("inConePhotonPt", " Photon pt distribution in the cone",40,-.5,39.5, file_);
   add1DHistogram("inConeElectronPt", "Electron pt distribution in the cone",40,-.5,39.5, file_);
   add1DHistogram("inConePionPt", "Pion pt distribution in the cone",40,-.5,39.5, file_);
   add1DHistogram("inConeKL0Pt",    "KL0 pt distribution in the cone",40,-.5,39.5, file_);
   add1DHistogram("inConePFParticlePt",    " PF Particle pt distribution in the cone",100,-.5,39.5, file_);

   add1DHistogram("inConeTotPhotonPt", "Total pt from photons in cone",200,-.5,39.5, file_);
   add1DHistogram("inConeTotElectronPt", "Total pt from electrons in cone",40,-.5,39.5, file_);
   add1DHistogram("inConeTotPionPt", "Total pt from pions in cone",200,-.5,39.5, file_);
   add1DHistogram("inConeTotKL0Pt",    "Total pt from KL0s in cone",200,-.5,39.5, file_);
   add1DHistogram("inConeTotPFParticlePt",    "Total pt from PF Particles in cone",200,-.5,49.5, file_);
   
   add2DHistogram("inConePhotonDeltaAngle", "Delta Eta and Delta Phi Distribution of photons in the cone",20, -0.5, .5, 20, -.5, .5, file_); 
   add2DHistogram("inConeElectronDeltaAngle", "Delta Eta and Delta Phi Distribution of electrons in the cone",20, -.5, .5, 20, -.5, .5, file_); 
   add2DHistogram("inConePionDeltaAngle", "Delta Eta and Delta Phi Distribution of pions in the cone",20, -1, 1, 20, -1, 1, file_); 
   add2DHistogram("inConeKL0DeltaAngle", "Delta Eta and Delta Phi Distribution of KL0s in the cone",20, -1, 1, 20, -1, 1, file_); 
   add2DHistogram("inConePFParticleDeltaAngle", "Delta Eta and Delta Phi Distribution of pf particles in the cone",20, -1, 1, 20, -1, 1, file_); 

   

   histosInitialized_ = true;
 }
}
