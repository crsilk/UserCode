#include <iostream>

#include "PFAnalyses/CommonTools/interface/ElectronHistograms.h"

ElectronHistograms::ElectronHistograms(TFileDirectory *myDir, const std::string & name){

  AnalysisHistograms::init(myDir,name);

}


ElectronHistograms::~ElectronHistograms(){ 

  std::cout<<"ElectronHistograms::~ElectronHistograms()"<<std::endl;

}



void ElectronHistograms::defineHistograms(){

  using namespace std;

  if(!histosInitialized_){

    ///Here we define 1D histogram. The histograms are assigned to the TDirectory 
    /// pointed by the file argument 
    add1DHistogram("hPT","; p_{T} [GeV];Events",100,0,100,file_);
    add2DHistogram("hPT1vsPT2","; p_{T} PF [GeV];p{T} EG [GeV]",100,0,100,100,0,100,file_);
    add1DHistogram("hPT1oPT2","; p_{T}PF/p{T}EG; Events",100,0,2.,file_);
    add1DHistogram("hGSFPT","; p_{T}GSF; Events",100,0,100.,file_);
    add1DHistogram("hETA","; #eta ;Events",120,-3,3,file_);
    add1DHistogram("hMVA","; MVA ;Events",50,-1.,1.,file_);
    // needs the GSF track extra
    //    add1DHistogram("hRFirstHit","; Radius first hit [r];Events",50,0.,50.,file_);
    // needs the GsfElectronCore
    //    add2DHistogram("hSeed","Tracker driven; Ecal driven ;",2,-0.5,1.5,2,-0.5,1.5,file_);
    add1DHistogram("hGSFd0","; d0 ;Events",100,-.2,.2,file_);
    add1DHistogram("hGSFd0error","; d0error ;Events",100,0.,0.05,file_);
    add1DHistogram("hGSFd0sig","; d0sig ;Events",100,-20.,20.,file_);
    add1DHistogram("hGSFNhits","; Nhits ;Events",30,-0.5,29.5,file_);
    add1DHistogram("hGSFfbrem","; fbrem ;Events",100,0.,1.,file_);
    add1DHistogram("hGSFIPvtx",";IP(x-y); Events",100,-.2,.2,file_);
    add1DHistogram("hGSFDzvtx",";GSF-vtx dz; Events",100,-10.,10.,file_);
    add1DHistogram("hGSFNmissedhits",";Number of missing hits; Events",10,-.5,9.5,file_);
    histosInitialized_ = true;
  }
}



void ElectronHistograms::fill( const pat::Electron& ele, double eventWeight ){
  
  if(histosInitialized_){

    ///Here we define 1D histogram. The histograms are assigned to the TDirectory 
    /// pointed by the file argument 
    fill1DHistogram("hPT", ele.pt(), eventWeight);
    fill2DHistogram("hPT1vsPT2", ele.pfCandidateRef()->pt(), ele.pt(),eventWeight);
    fill1DHistogram("hPT1oPT2", ele.pfCandidateRef()->pt()/ele.pt(),eventWeight);
    fill1DHistogram("hETA", ele.eta(), eventWeight);
    fill1DHistogram("hMVA", ele.pfCandidateRef()->mva_e_pi(), eventWeight);
    // needs the GsfElectronCore 
    //    fill2DHistogram("hSeed",ele.ecalDrivenSeed(),ele.trackerDrivenSeed(),eventWeight);
    // needs the GsfTrackExtra
    //    fill1DHistogram("hRFirstHit", ele.gsfTrack()->innerPosition().rho(), eventWeight);
    fill1DHistogram("hGSFPT",ele.gsfTrack()->momentumMode().rho(),eventWeight);
    fill1DHistogram("hGSFd0",ele.gsfTrack()->d0(),eventWeight);
    fill1DHistogram("hGSFd0error",ele.gsfTrack()->d0Error(),eventWeight);
    if(ele.gsfTrack()->d0Error()!=0.)
      fill1DHistogram("hGSFd0sig",ele.gsfTrack()->d0()/ele.gsfTrack()->d0Error(),eventWeight);
    fill1DHistogram("hGSFNhits",ele.gsfTrack()->found(),eventWeight);
    fill1DHistogram("hGSFfbrem",ele.fbrem(),eventWeight);
    fill1DHistogram("hGSFNmissedhits",
		    ele.gsfTrack()->trackerExpectedHitsInner().numberOfHits(),eventWeight);
  }
}

void ElectronHistograms::fillWithVertex(const pat::Electron& ele, const math::XYZPoint & vtx,double eventWeight) {
  
  if(histosInitialized_){
    fill1DHistogram("hGSFIPvtx",ele.gsfTrack()->dxy(vtx),eventWeight);
    fill1DHistogram("hGSFDzvtx",ele.gsfTrack()->dz(vtx),eventWeight);
  }
}
