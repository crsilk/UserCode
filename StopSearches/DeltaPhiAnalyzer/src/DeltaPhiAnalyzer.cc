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
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

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

      InputTag inputTagPFCandidates_;
      InputTag inputTagPFJets_;
      int minNJets_;
      double maxJetEta_;
      double minJetPt_;
      double minHT_;
      double minMET_;
      TH1F * deltaPhiCHAndMETHisto_;
      TH1F * deltaPhiHisto_;
      TH2F * deltaPhiVsMETHisto_;
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
  inputTagPFJets_ 
    = iConfig.getParameter<InputTag>("PFJets");
  minNJets_ = iConfig.getParameter<int>("minNJets");
  maxJetEta_ = iConfig.getParameter<double>("maxJetEta");
  minJetPt_ = iConfig.getParameter<double>("minJetPt");
  minHT_ = iConfig.getParameter<double>("minHT");
  minMET_ = iConfig.getParameter<double>("minMET");

  Service<TFileService> rootFile;
  deltaPhiCHAndMETHisto_ = rootFile->make<TH1F>("deltaPhiCHAndMETHisto", "delta phi of charged hadrons and MET", 1000, 0, 3.15);
  deltaPhiHisto_ = rootFile->make<TH1F>("deltaPhiHisto", "delta phi of neutral hadrons and charged hadrons", 1000, 0, 3.15);
  deltaPhiVsMETHisto_ = rootFile->make<TH2F>("deltaPhiNAndTVsMET_", "deta phi vs MET", 100, 0, 3.15, 1000, 0, 2000);
  
}


DeltaPhiAnalyzer::~DeltaPhiAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.

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

  Handle<PFJetCollection> pfJets;
  iEvent.getByLabel(inputTagPFJets_, pfJets);
  PFJetCollection::const_iterator pfJet;


  math::XYZTLorentzVector METVector(0, 0, 0, 0);
  math::XYZTLorentzVector sumCHVector(0, 0, 0, 0);
  math::XYZTLorentzVector sumNHVector(0, 0, 0, 0);

  double deltaPhiCHAndMET;
  double deltaPhi;
  double sumHT = 0;
  double MET= 0;
  int nJetsPassCut = 0;
  
  for( pfCandidate = pfCandidates->begin(); pfCandidate != pfCandidates->end();
       pfCandidate++)
  {     
     METVector = METVector + pfCandidate->p4();
     sumHT = sumHT + pfCandidate->pt();
     
     if(pfCandidate->particleId() == 1 || 
        pfCandidate->particleId() == 2 ||
        pfCandidate->particleId() == 3)
        sumCHVector = sumCHVector + pfCandidate->p4();
     if(pfCandidate->particleId() == 4 || 
        pfCandidate->particleId() == 5)
        sumNHVector = sumNHVector + pfCandidate->p4();
  }

  MET = METVector.pt();

  /////////////////////Selection Cuts///////////////////////////
  if(sumHT < minHT_) return;
  if(MET < minMET_)return;  
  
  for(pfJet = pfJets->begin(); pfJet != pfJets->end(); pfJet++)
  {
     if( fabs(pfJet->eta()) > maxJetEta_ || pfJet->pt() < minJetPt_)
        continue;
     nJetsPassCut++;
  }
  
  if(nJetsPassCut < minNJets_) return;
  /////////////////////////////////////////////////////////////

  METVector = -METVector;
  sumCHVector = -sumCHVector;
  sumNHVector = -sumNHVector;

  deltaPhiCHAndMET = fabs(METVector.phi() - sumCHVector.phi());
  deltaPhi = fabs(sumCHVector.phi() -  sumNHVector.phi());
  
  if(deltaPhiCHAndMET > 3.14) deltaPhiCHAndMET = 2*3.14 - deltaPhiCHAndMET;
  if(deltaPhi > 3.14) deltaPhi = 2*3.14 - deltaPhi;
  
  deltaPhiCHAndMETHisto_->Fill(deltaPhiCHAndMET);  
  deltaPhiHisto_->Fill(deltaPhi);
  deltaPhiVsMETHisto_->Fill(deltaPhi, MET);
  
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
