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
         


   add1DHistogram("inConePhotons", "; Number of PF Photons in the cone", 5,0 ,5, file_);
   add1DHistogram("inConeElectrons", "Number of Electrons in the cone",5, 0, 5, file_);
   add1DHistogram("inConePions", "Number of Pions in the cone",5,0 , 5, file_);
   add1DHistogram("inConeKL0s", "Number of KL0s in the cone",10,0 ,10, file_);
   add1DHistogram("inConePFParticles", "Number of PFParticles in the cone",20,0, 20, file_);
   add1DHistogram("inConeRecoPhotons", "; Number of Reco Photons in the cone", 5,0 ,5, file_); 

   add1DHistogram("inConeTotPhotonPt", "Total pt from photons in cone",200,0,60, file_);
   add1DHistogram("inConeTotElectronPt", "Total pt from electrons in cone",60, 0, 50, file_);
   add1DHistogram("inConeTotPionPt", "Total pt from pions in cone",200,0, 60, file_);
   add1DHistogram("inConeTotKL0Pt",    "Total pt from KL0s in cone",200, 0, 60, file_);
   add1DHistogram("inConeTotPFParticlePt",    "Total pt from PF Particles in cone",200, 0, 60, file_);
   add1DHistogram("inConeTotRecoPhotonPt",    "Total pt from reco photons in cone",200, 0, 60, file_);

   add1DHistogram("photonOverMeasured", "Photon distribution when total Pt>38",50,0,50, file_);
   add1DHistogram("electronOverMeasured", "Electron distribution when total Pt>38",50,0,50, file_);
   add1DHistogram("pionOverMeasured", "Pion distribution when total Pt>38",50,0,50, file_);
   add1DHistogram("KL0OverMeasured", "KL0 distribution when total Pt>38",50,0,50, file_);
   
   add1DHistogram("photonUnderMeasured", "Photon distribution when total Pt<32",50,0,40, file_);
   add1DHistogram("electronUnderMeasured", "Electron distribution when total Pt<32",50,0,40, file_);
   add1DHistogram("pionUnderMeasured", "Pion distribution when total Pt<32",50,0,40, file_);
   add1DHistogram("KL0UnderMeasured", "KL0 distribution when total Pt<32",50,0,40, file_);
   
   add1DHistogram("photonCorrectlyMeasured", "Photon distribution when total Pt>3 and Pt<38",50,0,38, file_);
   add1DHistogram("electronCorrectlyMeasured", "Electron distribution when total Pt>32 and Pt<38",50,0,38, file_);
   add1DHistogram("pionCorrectlyMeasured", "Pion distribution when total Pt>32 and Pt<38",50,0,38, file_);
   add1DHistogram("KL0CorrectlyMeasured", "KL0 distribution when total Pt>32 and Pt<38",50,0,38, file_);
   



   add2DHistogram("inConePhotonDeltaAngle", "Delta Eta and Delta Phi Distribution of photons in the cone",40, -0.1, .1, 80, -.5, .5, file_); 
   add2DHistogram("inConeElectronDeltaAngle", "Delta Eta and Delta Phi Distribution of electrons in the cone",40, -.05, .05, 40, -.15, .15, file_); 
   add2DHistogram("inConePionDeltaAngle", "Delta Eta and Delta Phi Distribution of pions in the cone",80, -.2, .2, 40, -1, 1, file_); 
   add2DHistogram("inConeKL0DeltaAngle", "Delta Eta and Delta Phi Distribution of KL0s in the cone",40, -1, 1, 40, -1, 1, file_); 
   add2DHistogram("inConePFParticleDeltaAngle", "Delta Eta and Delta Phi Distribution of pf particles in the cone",80, -1, 1, 20, -1, 1, file_); 

   

   histosInitialized_ = true;
 }
}
