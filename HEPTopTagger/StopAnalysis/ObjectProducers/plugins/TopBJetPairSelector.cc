// -*- C++ -*-
//
// Package:    TopBJetFilter
// Class:      TopBJetFilter
// 
/**\class TopBJetPairSelector TopBJetPairSelector.cc StopAnalysis/TopBJetPairSelector/src/TopBJetPairSelector.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christopher Silkworth
//         Created:  Wed Oct  3 17:38:08 CDT 2012
// $Id: TopBJetPairSelector.cc,v 1.1 2012/10/17 22:48:56 crsilk Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace std;
using namespace reco;
using namespace edm;

class TopBJetPairSelector : public edm::EDProducer {
   public:
      explicit TopBJetPairSelector(const edm::ParameterSet&);
      ~TopBJetPairSelector();

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
      InputTag TopSubjetSrc_;
      InputTag BJetSrc_;

      double deltaRFromFatjetCut_;
      double deltaRFromSubjetsCut_;




};


typedef reco::Candidate::LorentzVector LorentzVector;

TopBJetPairSelector::TopBJetPairSelector(const edm::ParameterSet& iConfig)
{
   TopSrc_ = iConfig.getParameter<InputTag>("TopSrc");
   TopSubjetSrc_ = iConfig.getParameter<InputTag>("TopSubjetSrc");
   BJetSrc_ = iConfig.getParameter<InputTag>("BJetSrc");
   deltaRFromFatjetCut_ = iConfig.getParameter<double>("deltaRFromFatjetCut");
   deltaRFromSubjetsCut_ = iConfig.getParameter<double>("deltaRFromSubjetsCut");

   produces<BasicJetCollection >("Top");
   produces<PFJetCollection >("subjets");
   produces<pat::JetCollection >("BJet");

}

TopBJetPairSelector::~TopBJetPairSelector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
void
TopBJetPairSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{


   Handle<BasicJetCollection > Tops;
   iEvent.getByLabel(TopSrc_, Tops);
   BasicJetCollection::const_iterator Top;
   
   Handle<PFJetCollection > TopSubjets;
   iEvent.getByLabel(TopSubjetSrc_, TopSubjets);
   PFJetCollection::const_iterator TopSubjet0;
   PFJetCollection::const_iterator TopSubjet1;
   PFJetCollection::const_iterator TopSubjet2;

   Handle<pat::JetCollection > BJets;
   iEvent.getByLabel(BJetSrc_,BJets);
   pat::JetCollection::const_iterator BJet;

   vector<unsigned> possibleTops;
   vector<unsigned> possibleBJets;

   auto_ptr<BasicJetCollection> producedTop(new BasicJetCollection());
   auto_ptr<PFJetCollection> producedTopSubjets(new PFJetCollection());
   auto_ptr<pat::JetCollection> producedBJet(new pat::JetCollection());

   bool disjoint;
   Jet::Constituents subjets;
   double highTopPt= 0;
   double highBJetPt = 0;
   unsigned keepPairIndex = 0;
   unsigned TopIndex = 0;
   unsigned BJetIndex = 0;

   for(Top = Tops->begin(); Top != Tops->end(); Top++)
   {
      TopSubjet0 = TopSubjets->begin() + (3*TopIndex);
      TopSubjet1 = TopSubjets->begin() + (3*TopIndex + 1);
      TopSubjet2 = TopSubjets->begin() + (3*TopIndex + 2);

      BJetIndex = 0;
      for(BJet = BJets->begin(); BJet != BJets->end(); BJet++)
      {
         disjoint = true;

         if(deltaR(Top->eta(), Top->phi(), BJet->eta(), 
                   BJet->phi()) < deltaRFromFatjetCut_) disjoint = false;

         if(deltaR(BJet->eta(),BJet->phi(),
                   TopSubjet0->eta(), TopSubjet0->phi()) < deltaRFromSubjetsCut_) disjoint = false;

         if(deltaR(BJet->eta(),BJet->phi(),TopSubjet1->eta(), TopSubjet1->phi()) < deltaRFromSubjetsCut_) disjoint = false;

         if(deltaR(BJet->eta(),BJet->phi(),TopSubjet2->eta(), TopSubjet2->phi()) < deltaRFromSubjetsCut_) disjoint = false;

         if(disjoint)
         {
            possibleTops.push_back(TopIndex);
            possibleBJets.push_back(BJetIndex);
         }
         BJetIndex++;
      }
      TopIndex++;
   }


   if(possibleTops.size() > 0)
   {
      for(unsigned i = 0; i < possibleTops.size(); i++)
      {
       
         if((*Tops)[possibleTops[i]].pt() >= highTopPt)
         {
            highTopPt = (*Tops)[possibleTops[i]].pt();
            if((*BJets)[possibleBJets[i]].pt() > highBJetPt)
            {
               keepPairIndex = i;
            }
         }
         
      }
      
      producedTop->push_back((*Tops)[possibleTops[keepPairIndex]]);
      producedTopSubjets->push_back((*TopSubjets)[3*possibleTops[keepPairIndex]]);
      producedTopSubjets->push_back((*TopSubjets)[3*possibleTops[keepPairIndex] + 1]);
      producedTopSubjets->push_back((*TopSubjets)[3*possibleTops[keepPairIndex] + 2]);
      producedBJet->push_back((*BJets)[possibleBJets[keepPairIndex]]);
   }
   
      
   iEvent.put(producedTop, "Top");
   iEvent.put(producedTopSubjets, "subjets");
   iEvent.put(producedBJet, "BJet");

}

// ------------ method called once each job just before starting event loop  ------------
void 
TopBJetPairSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TopBJetPairSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
TopBJetPairSelector::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  
}

// ------------ method called when ending the processing of a run  ------------
void 
TopBJetPairSelector::endRun(edm::Run&, edm::EventSetup const&)
{
  
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
TopBJetPairSelector::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
TopBJetPairSelector::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TopBJetPairSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(TopBJetPairSelector);
