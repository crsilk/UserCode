// -*- C++ -*-
//
// Package:    DeltaPhiAnalyzer
// Class:      DeltaPhiAnalyzer
// 
/**\class DeltaPhiAnalyzer DeltaPhiAnalyzer.cc UserCode/DeltaPhiAnalyzer/src/DeltaPhiAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christopher Silkworth
//         Created:  Fri Oct 21 15:59:29 CDT 2011
// $Id$
//
//


// system include files
#include <memory>
#include <string>
#include <iostream>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "TFile.h"
#include "TH1.h"

using namespace std;
using namespace edm;
using namespace reco;


//
// class declaration
//

class DeltaPhiAnalyzer : public edm::EDAnalyzer {
   public:
      explicit DeltaPhiAnalyzer(const edm::ParameterSet&);
      ~DeltaPhiAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      edm::InputTag   inputTagPFCandidates_;
      string outputfile_;
      TFile * rootFile_;
      TH1F * deltaphiTracksAndMETHisto_;
      TH1F * deltaphiNeutralAndTracksHisto_;
      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
DeltaPhiAnalyzer::DeltaPhiAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

  inputTagPFCandidates_ 
    = iConfig.getParameter<InputTag>("PFCandidates");
  outputfile_ = iConfig.getUntrackedParameter<std::string>("rootOutputFile"); 

  rootFile_ = new TFile(outputfile_.c_str(), "RECREATE");
  deltaphiTracksAndMETHisto_ = new TH1F("deltaphiTracksAndMETHisto", "delta phi of Tracks and MET", 100, 0, 3.14);
  deltaphiNeutralAndTracksHisto_ = new TH1F("deltaphiNeutralAndTracksHisto", "delta phi of Neutral and Tracks", 100, 0, 3.14);
}


DeltaPhiAnalyzer::~DeltaPhiAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

   deltaphiTracksAndMETHisto_->Write();
   deltaphiNeutralAndTracksHisto_->Write();
   rootFile_->Close();


}


//
// member functions
//

// ------------ method called for each event  ------------
void
DeltaPhiAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  Handle<PFCandidateCollection> pfCandidates;
  iEvent.getByLabel(inputTagPFCandidates_, pfCandidates);
  PFCandidateCollection::const_iterator pfCandidate;

  math::XYZTLorentzVector METVector(0, 0, 0, 0);
  math::XYZTLorentzVector sumTracksVector(0, 0, 0, 0);
  math::XYZTLorentzVector sumNeutralVector(0, 0, 0, 0);

  double deltaphiTracksAndMET;
  double deltaphiNeutralAndTracks;

  for( pfCandidate = pfCandidates->begin(); pfCandidate != pfCandidates->end();
       pfCandidate++)
  {
     METVector = METVector + pfCandidate->p4();
     
     if(pfCandidate->particleId() == 1 || 
        pfCandidate->particleId() == 2 ||
        pfCandidate->particleId() == 3)
        sumTracksVector = sumTracksVector + pfCandidate->p4();
     if(pfCandidate->particleId() == 4 || 
        pfCandidate->particleId() == 5)
        sumNeutralVector = sumNeutralVector + pfCandidate->p4();


  }
  METVector = -METVector;
  sumTracksVector = -sumTracksVector;
  sumNeutralVector = -sumNeutralVector;

  deltaphiTracksAndMET = fabs(METVector.phi() - sumTracksVector.phi());
  
  if(deltaphiTracksAndMET > 3.14) 
     deltaphiTracksAndMET = 2*3.14 - deltaphiTracksAndMET;
  
  deltaphiTracksAndMETHisto_->Fill(deltaphiTracksAndMET);

  deltaphiNeutralAndTracks = fabs(sumTracksVector.phi() - sumNeutralVector.phi());
  
  if(deltaphiNeutralAndTracks > 3.14) 
     deltaphiNeutralAndTracks = 2*3.14 - deltaphiNeutralAndTracks;
  
  deltaphiNeutralAndTracksHisto_->Fill(deltaphiNeutralAndTracks);

  
}


// ------------ methodpar called once each job just before starting event loop  ------------
void 
DeltaPhiAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DeltaPhiAnalyzer::endJob() 
{

}

// ------------ method called when starting to processes a run  ------------
void 
DeltaPhiAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
DeltaPhiAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
DeltaPhiAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
DeltaPhiAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DeltaPhiAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DeltaPhiAnalyzer);
