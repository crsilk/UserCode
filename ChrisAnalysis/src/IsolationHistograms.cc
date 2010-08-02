#include <iostream>

#include "PFAnalyses/ChrisAnalysis/interface/IsolationHistograms.h"

IsolationHistograms::IsolationHistograms(TFileDirectory *myDir, const std::string & name){

  AnalysisHistograms::init(myDir,name);

}


IsolationHistograms::~IsolationHistograms(){ 

  std::cout<<"IsolationHistograms::~IsolationHistograms()"<<std::endl;

}



void IsolationHistograms::defineHistograms(){

  using namespace std;

  if(!histosInitialized_){

    add1DHistogram("hIso",
		   ";sum pT [GeV];Events"
		   ,100,0,10,file_);
    add1DHistogram("hIsoRel",
		   "; #sump_{T}/p_{T}^#mu [GeV];Events",
		   100,0,1,file_);
    
//    add1DHistogram("hNeutralIso",
// 		   "Neutral hadron isolation;sum pT [GeV];Events",
// 		   100,0,10,file_);
//     add1DHistogram("hNeutralIsoRel",
// 		   "Neutral hadron relative isolation; #sump_{T}/p_{T}^#mu [GeV];Events",
// 		   100,0,1,file_);

//     add1DHistogram("hPhotonIso",
// 		   "Photon isolation;sum pT [GeV];Events",
// 		   100,0,10,file_);
//     add1DHistogram("hPhotonIsoRel",
// 		   "Photon relative isolation; #sump_{T}/p_{T}^#mu [GeV];Events",
// 		   100,0,1,file_);

  
    histosInitialized_ = true;
  }
}



void IsolationHistograms::fill( double iso, double isoRel, double eventWeight){
  
  using namespace std;
  
  if(histosInitialized_){

    ///Here we define 1D histogram. The histograms are assigned to the TDirectory 
    /// pointed by the file argument 
    fill1DHistogram("hIso", iso, eventWeight);
    fill1DHistogram("hIsoRel", isoRel, eventWeight);
 
//     fill1DHistogram("hNeutralIso", nhIso, eventWeight);
//     fill1DHistogram("hNeutralIsoRel", nhIso/norm, eventWeight);
    
//     fill1DHistogram("hPhotonIso", phIso, eventWeight);
//     fill1DHistogram("hPhotonIsoRel", phIso/norm, eventWeight);
 
  }
}
