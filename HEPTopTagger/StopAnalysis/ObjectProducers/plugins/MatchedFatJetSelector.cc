// -*- C++ -*-
//
// Package:    MatchedFatJetSelectors
// Class:      MatchedFatJetSelectors
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/JetReco/interface/PFJet.h"
//includes needed for this file

using namespace edm;
using namespace reco;
using namespace std;

class MatchedFatJetSelector : public edm::EDProducer {
   public:
      explicit MatchedFatJetSelector(const edm::ParameterSet&);
      ~MatchedFatJetSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      InputTag TopSrc_;
      InputTag BJetSrc_;
      InputTag FatJetSrc_;

      double fatJetR_;


};


typedef reco::Candidate::LorentzVector LorentzVector;

MatchedFatJetSelector::MatchedFatJetSelector(const edm::ParameterSet& iConfig)
{
      TopSrc_ = iConfig.getParameter<InputTag>("TopSrc");
      BJetSrc_ = iConfig.getParameter<InputTag>("BJetSrc");
      FatJetSrc_ = iConfig.getParameter<InputTag>("FatJetSrc");
      fatJetR_ = iConfig.getParameter<double>("fatJetR");

      produces<PFJetCollection>();
  
}


MatchedFatJetSelector::~MatchedFatJetSelector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
MatchedFatJetSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle<View<Candidate> > Tops;
   iEvent.getByLabel(TopSrc_, Tops);
   View<Candidate>::const_iterator Top = Tops->begin();
   
   Handle<View<Candidate> > BJets;
   iEvent.getByLabel(BJetSrc_,BJets);
   View<Candidate>::const_iterator BJet = BJets->begin();

   Handle<PFJetCollection > FatJets;
   iEvent.getByLabel(FatJetSrc_,FatJets);
   PFJetCollection::const_iterator FatJet;

   auto_ptr<PFJetCollection> matchedFatJet(new PFJetCollection());
   unsigned topFatJetIndex = 99999;
   unsigned bjetFatJetIndex = 99999;
   unsigned counter = 0;
   double currentDeltaR;
   double minDeltaR = 99999;

   if(Tops->size() != 0 && BJets->size() != 0)
   {

      for(FatJet = FatJets->begin(); FatJet != FatJets->end(); FatJet++)
      {
         currentDeltaR = deltaR(FatJet->eta(), FatJet->phi(), Top->eta(), Top->phi());
//         if( currentDeltaR < minDeltaR)
         if( currentDeltaR < fatJetR_)
         { 
//            minDeltaR = currentDeltaR;
            topFatJetIndex = counter;
            break;
         }
         counter++;
      }
      
      counter = 0;
      
      for(FatJet = FatJets->begin(); FatJet != FatJets->end(); FatJet++)
      {
         currentDeltaR = deltaR(FatJet->eta(), FatJet->phi(), BJet->eta(), BJet->phi());
         if(currentDeltaR < minDeltaR && counter != topFatJetIndex)
         {
            minDeltaR = currentDeltaR;
            bjetFatJetIndex = counter;
         }
         counter++;
      }
      
      if (minDeltaR != 99999)
         matchedFatJet->push_back((*FatJets)[bjetFatJetIndex]);
   }
   iEvent.put(matchedFatJet);

}

// ------------ method called once each job just before starting event loop  ------------
void 
MatchedFatJetSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MatchedFatJetSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
MatchedFatJetSelector::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
MatchedFatJetSelector::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
MatchedFatJetSelector::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
MatchedFatJetSelector::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MatchedFatJetSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MatchedFatJetSelector);
