// -*- C++ -*-
//
// Package:    LostLeptonSelectors
// Class:      LostLeptonSelectors
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
//includes needed for this file

using namespace edm;
using namespace reco;
using namespace std;

class LostLeptonSelector : public edm::EDProducer {
   public:
      explicit LostLeptonSelector(const edm::ParameterSet&);
      ~LostLeptonSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);


      InputTag genSrc_;
      InputTag electronSrc_;
      InputTag muonSrc_;
      InputTag tauSrc_;
      InputTag isolatedTrackSrc_;


};


typedef reco::Candidate::LorentzVector LorentzVector;

LostLeptonSelector::LostLeptonSelector(const edm::ParameterSet& iConfig)
{

   genSrc_ = iConfig.getParameter<InputTag>("genSrc");
   electronSrc_ = iConfig.getParameter<InputTag>("electronSrc");
   muonSrc_ = iConfig.getParameter<InputTag>("muonSrc");
   if (iConfig.exists("tauSrc"))
      tauSrc_ = iConfig.getParameter<InputTag>("tauSrc");
   isolatedTrackSrc_ = iConfig.getParameter<InputTag>("isolatedTrackSrc");

   produces <vector<GenParticle> > ();

}


LostLeptonSelector::~LostLeptonSelector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
LostLeptonSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{


   Handle<View<LeafCandidate>> genParticles;
   iEvent.getByLabel(genSrc_, genParticles);
   View<LeafCandidate>::const_iterator genParticle;

   Handle<reco::GsfElectronCollection> electrons;
   iEvent.getByLabel(electronSrc_, electrons);
   
   Handle<pat::MuonCollection> muons;
   iEvent.getByLabel(muonSrc_, muons);
   
   Handle<pat::TauCollection> taus;
   bool gotTaus = iEvent.getByLabel(tauSrc_, taus);
   
   Handle<PFCandidateCollection> isolatedTracks;
   iEvent.getByLabel(isolatedTrackSrc_, isolatedTracks);

   bool recoLepton = false;

  std::auto_ptr<vector<GenParticle> > lostLeptons(new vector<GenParticle>());
   if(electrons->size() != 0 || muons->size() != 0 || isolatedTracks->size() != 0) recoLepton = true;
   if(gotTaus && taus->size() != 0) recoLepton = true;


   for(genParticle = genParticles->begin(); genParticle != genParticles->end(); genParticle++)
   {
      if(recoLepton) break;
      if(abs(genParticle->pdgId()) == 11 || abs(genParticle->pdgId()) == 13 || abs(genParticle->pdgId()) == 15)
         lostLeptons->push_back(*genParticle);
   }
   
   iEvent.put(lostLeptons);

}

// ------------ method called once each job just before starting event loop  ------------
void 
LostLeptonSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
LostLeptonSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
LostLeptonSelector::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
LostLeptonSelector::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
LostLeptonSelector::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
LostLeptonSelector::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
LostLeptonSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(LostLeptonSelector);
