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
         


     add1DHistogram("inBoxPhotons", "; Number of PF Photons in the box", 5,0 ,5, file_);
   add1DHistogram("inBoxElectrons", "Number of Electrons in the box",5, 0, 5, file_);
   add1DHistogram("inBoxPions", "Number of Pions in the box",5,0 , 5, file_);
   add1DHistogram("inBoxKL0s", "Number of KL0s in the box",10,0 ,10, file_);
   add1DHistogram("inBoxPFParticles", "Number of PFParticles in the box",20,0, 20, file_);
   add1DHistogram("inBoxRecoPhotons", "; Number of Reco Photons in the box", 5,0 ,5, file_); 

   add1DHistogram("inBoxTotPhotonPt", "Total pt from photons in box",200,0,60, file_);
   add1DHistogram("inBoxTotElectronPt", "Total pt from electrons in box",60, 0, 50, file_);
   add1DHistogram("inBoxTotPionPt", "Total pt from pions in box",200,0, 60, file_);
   add1DHistogram("inBoxTotKL0Pt",    "Total pt from KL0s in box",200, 0, 60, file_);
   add1DHistogram("inBoxTotPFParticlePt",    "Total pt from PF Particles in box",200, 0, 60, file_);
   add1DHistogram("inBoxTotRecoPhotonPt",    "Total pt from reco photons in box",200, 0, 60, file_);

   add1DHistogram("photonOverMeasured", "Photon distribution when total Pt>38",50,0,50, file_);
   add1DHistogram("electronOverMeasured", "Electron distribution when total Pt>38",50,0,50, file_);
   add1DHistogram("pionOverMeasured", "Pion distribution when total Pt>38",50,0,50, file_);
   add1DHistogram("KL0OverMeasured", "KL0 distribution when total Pt>38",50,0,50, file_);
   
   add1DHistogram("photonUnderMeasured", "Photon distribution when total Pt<32",50,0,32, file_);
   add1DHistogram("electronUnderMeasured", "Electron distribution when total Pt<32",50,0,32, file_);
   add1DHistogram("pionUnderMeasured", "Pion distribution when total Pt<32",50,0,32, file_);
   add1DHistogram("KL0UnderMeasured", "KL0 distribution when total Pt<32",50,0,32, file_);
   
   add1DHistogram("photonCorrectlyMeasured", "Photon distribution when total Pt>3 and Pt<38",50,0,38, file_);
   add1DHistogram("electronCorrectlyMeasured", "Electron distribution when total Pt>32 and Pt<38",50,0,38, file_);
   add1DHistogram("pionCorrectlyMeasured", "Pion distribution when total Pt>32 and Pt<38",50,0,38, file_);
   add1DHistogram("KL0CorrectlyMeasured", "KL0 distribution when total Pt>32 and Pt<38",50,0,38, file_);
   



   add2DHistogram("inBoxPhotonDeltaAngle", "Delta Eta and Delta Phi Distribution of photons in the box",40, -0.1, .1, 80, -.5, .5, file_); 
   add2DHistogram("inBoxElectronDeltaAngle", "Delta Eta and Delta Phi Distribution of electrons in the box",40, -.05, .05, 40, -.15, .15, file_); 
   add2DHistogram("inBoxPionDeltaAngle", "Delta Eta and Delta Phi Distribution of pions in the box",80, -.2, .2, 40, -1, 1, file_); 
   add2DHistogram("inBoxKL0DeltaAngle", "Delta Eta and Delta Phi Distribution of KL0s in the box",40, -1, 1, 40, -1, 1, file_); 
   add2DHistogram("inBoxPFParticleDeltaAngle", "Delta Eta and Delta Phi Distribution of pf particles in the box",80, -1, 1, 20, -1, 1, file_); 

   

   histosInitialized_ = true;
 }
}
