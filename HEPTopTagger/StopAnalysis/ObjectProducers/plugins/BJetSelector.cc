// -*- C++ -*-
//
// Package:    BJetSelectors
// Class:      BJetSelectors
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"


//includes needed for this file

using namespace edm;
using namespace std;

class BJetSelector : public edm::EDProducer {
   public:
      explicit BJetSelector(const edm::ParameterSet&);
      ~BJetSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);


      InputTag BJetSrc_;

      double ptCut_;      
      double etaCut_;
      string discriminator_;
      double discriminatorCut_;

      string labelName_;

};


typedef reco::Candidate::LorentzVector LorentzVector;

BJetSelector::BJetSelector(const edm::ParameterSet& iConfig)
{

   BJetSrc_ = iConfig.getParameter<InputTag>("BJetSrc");

   ptCut_ = iConfig.getParameter<double>("ptCut");
   etaCut_ = iConfig.getParameter<double>("etaCut");
   discriminator_ = iConfig.getParameter<string>("discriminator");
   discriminatorCut_ = iConfig.getParameter<double>("discriminatorCut");

   labelName_ = iConfig.getParameter<string>("labelName");

   produces<vector<pat::Jet> > (labelName_);

}


BJetSelector::~BJetSelector()
{
 
   
   
 
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
BJetSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  Handle<pat::JetCollection> BJets;
  iEvent.getByLabel(BJetSrc_,BJets);
  pat::JetCollection::const_iterator BJet;

  auto_ptr<pat::JetCollection> SelectedBJets(new pat::JetCollection());

  for ( BJet = BJets->begin(); BJet != BJets->end(); BJet++)
  {
     if( BJet->pt() < ptCut_) continue;
     if( abs(BJet->eta()) > etaCut_) continue;
     if( BJet->bDiscriminator(discriminator_) < discriminatorCut_) continue;

     SelectedBJets->push_back(*BJet);
  }


  iEvent.put(SelectedBJets, labelName_);
  
}
// ------------ method called once each job just before starting event loop  ------------
void 
BJetSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
BJetSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
BJetSelector::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
BJetSelector::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
BJetSelector::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
BJetSelector::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
BJetSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(BJetSelector);
