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
// $Id$
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
      InputTag BJetSrc_;

      double deltaRFromFatjetCut_;
      double deltaRFromSubjetsCut_;

      string TopLabelName_;
      string BJetLabelName_;



};




TopBJetPairSelector::TopBJetPairSelector(const edm::ParameterSet& iConfig)
{
   TopSrc_ = iConfig.getParameter<InputTag>("TopSrc");
   BJetSrc_ = iConfig.getParameter<InputTag>("BJetSrc");
   deltaRFromFatjetCut_ = iConfig.getParameter<double>("deltaRFromFatjetCut");
   deltaRFromSubjetsCut_ = iConfig.getParameter<double>("deltaRFromSubjetsCut");

   TopLabelName_ = iConfig.getParameter<string>("TopLabelName");
   BJetLabelName_ = iConfig.getParameter<string>("BJetLabelName");

   produces<BasicJetCollection> (TopLabelName_);
   produces<pat::JetCollection> (BJetLabelName_);
   
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
   BasicJetCollection ::const_iterator Top;
   
   Handle<pat::JetCollection> BJets;
   iEvent.getByLabel(BJetSrc_,BJets);
   pat::JetCollection::const_iterator BJet;
   

   BasicJetCollection possibleTops;
   pat::JetCollection possibleBJets;

   auto_ptr<BasicJetCollection> producedTop(new BasicJetCollection());
   auto_ptr<pat::JetCollection> producedBJet(new pat::JetCollection());

   bool disjoint;
   Jet::Constituents subjets;
   double highTopPt= 0;
   double highBJetPt = 0;
   unsigned keepPairIndex = 0;

   for(Top = Tops->begin(); Top != Tops->end(); Top++)
   {
      for(BJet = BJets->begin(); BJet != BJets->end(); BJet++)
      {

         if(deltaR(Top->eta(), Top->phi(), BJet->eta(), BJet->phi() > deltaRFromFatjetCut_))
         {
            possibleTops.push_back(*Top);
            possibleBJets.push_back(*BJet);
         }
         
         disjoint = true;
         subjets = Top->getJetConstituents();
         for(unsigned i = 0; i < subjets.size();i++)
         {
            if(deltaR(BJet->eta(),BJet->phi(),subjets[0]->eta(), subjets[0]->phi()) < deltaRFromSubjetsCut_) 
               disjoint = false;
         }
         if(disjoint)
         {
            possibleTops.push_back(*Top);
            possibleBJets.push_back(*BJet);
         }
         
      }
   }


   if(possibleTops.size() > 0)
   {
      for(unsigned i = 0; i < possibleTops.size(); i++)
      {
         if(possibleTops[i].pt() >= highTopPt)
         {
            highTopPt = possibleTops[i].pt();
            if(possibleBJets[i].pt() > highBJetPt)
            {
               keepPairIndex = i;
            }
         }
         
      }
      
      producedTop->push_back(possibleTops[keepPairIndex]);
      producedBJet->push_back(possibleBJets[keepPairIndex]);
   }
   
   iEvent.put(producedTop, TopLabelName_);
   iEvent.put(producedBJet, BJetLabelName_);

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
