// -*- C++ -*-
//
// Package:    PFMuonSelectors
// Class:      PFMuonSelectors
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"


//includes needed for this file

using namespace edm;
using namespace reco;
using namespace std;

class PFMuonSelector : public edm::EDProducer {
   public:
      explicit PFMuonSelector(const edm::ParameterSet&);
      ~PFMuonSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);


      InputTag muonSrc_;
      InputTag vtxSrc_;
      bool debug_;
      bool doMuonVeto_, doMuonID_, doMuonVtx_, doMuonIso_;
      double minMuPt_, maxMuEta_, maxMuD0_, maxMuDz_, maxMuRelIso_, minMuNumHit_;
      string labelName_;


};


typedef Candidate::LorentzVector LorentzVector;

PFMuonSelector::PFMuonSelector(const ParameterSet& iConfig)
{

   muonSrc_     = iConfig.getParameter<InputTag>("muonSource");
   vtxSrc_      = iConfig.getParameter<InputTag>("vertexSource");
   minMuPt_     = iConfig.getParameter<double>("minMuPt");
   maxMuEta_    = iConfig.getParameter<double>("maxMuEta");
   maxMuD0_     = iConfig.getParameter<double>("maxMuD0");
   maxMuDz_     = iConfig.getParameter<double>("maxMuDz");
   maxMuRelIso_ = iConfig.getParameter<double>("maxMuRelIso");
   minMuNumHit_ = iConfig.getParameter<double>("minMuNumHit");
   doMuonVeto_  = iConfig.getParameter<bool>("doMuonVeto");
   doMuonID_    = iConfig.getParameter<bool>("doMuonID");
   doMuonVtx_   = iConfig.getParameter<bool>("doMuonVtxAssociation");
   doMuonIso_   = iConfig.getParameter<bool>("doMuonIsolation");
   labelName_       = iConfig.getParameter<string>("labelName");

   produces<vector<pat::Muon> >(labelName_);
}


PFMuonSelector::~PFMuonSelector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
PFMuonSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  Handle<vector<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_, muons);
  Handle< vector<Vertex> > vertices;
  iEvent.getByLabel(vtxSrc_, vertices);
 
 Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : Vertex::Point());

  auto_ptr<vector<pat::Muon> > selectedMuons(new vector<pat::Muon>());

  if (vertices->size() > 0) 
  {
     for (vector<pat::Muon>::const_iterator m = muons->begin(); m != muons->end(); ++m) {
        
        if (m->pt() < minMuPt_) continue;
        if (abs(m->eta()) >= maxMuEta_) continue;
        
        if (m->muonID("AllGlobalMuons") == 0) continue;
        if (doMuonID_) 
        {
           if(!m->isPFMuon() ) continue; 
           if( m->globalTrack()->normalizedChi2() >= 10. ) continue;
           if( m->globalTrack()->hitPattern().numberOfValidMuonHits() <=0 ) continue;
           if( m->numberOfMatchedStations() <=1 ) continue;
           if( m->innerTrack()->hitPattern().numberOfValidPixelHits() == 0) continue;
           if( m->innerTrack()->hitPattern().trackerLayersWithMeasurement() <=5 ) continue;
        }

        if (doMuonVtx_) 
        {
           if (abs(m->innerTrack()->dxy(vtxpos)) >= maxMuD0_ && 
               vertices->size() > 0) continue;
           if (abs(m->innerTrack()->dz(vtxpos))  >= maxMuDz_ && 
               vertices->size() > 0) continue;
        }

        if (doMuonIso_) {
           float muRelIso = -1;

           muRelIso = (m->pfIsolationR04().sumChargedHadronPt + max(0., m->pfIsolationR04().sumNeutralHadronEt + m->pfIsolationR04().sumPhotonEt - 0.5*m->pfIsolationR04().sumPUPt) )/ m->pt();
           if (muRelIso >= maxMuRelIso_) continue;
        }

        if (vertices->size() > 0) selectedMuons->push_back(pat::Muon(*m));
     }
  }
  
  iEvent.put(selectedMuons, labelName_);


}

// ------------ method called once each job just before starting event loop  ------------
void 
PFMuonSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PFMuonSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
PFMuonSelector::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
PFMuonSelector::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
PFMuonSelector::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
PFMuonSelector::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PFMuonSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PFMuonSelector);
