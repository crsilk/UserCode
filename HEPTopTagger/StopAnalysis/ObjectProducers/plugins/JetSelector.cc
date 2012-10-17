// -*- C++ -*-
//
// Package:    JetSelectors
// Class:      JetSelectors
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "DataFormats/PatCandidates/interface/Jet.h"


//includes needed for this file

using namespace edm;
using namespace pat;
using namespace std;

class JetSelector : public edm::EDProducer {
   public:
      explicit JetSelector(const edm::ParameterSet&);
      ~JetSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      InputTag jetSrc_;
      string cuts_;
      string labelName_;

};



JetSelector::JetSelector(const edm::ParameterSet& iConfig)
{
   jetSrc_ = iConfig.getParameter<InputTag>("jetSrc");
   cuts_ = iConfig.getParameter<string>("cuts");
   
   if(iConfig.exists("labelName"))
      labelName_ = iConfig.getParameter<string>("labelName");
   else
   {
      labelName_ = "SelectedJets";
   }
      
   produces<JetCollection> (labelName_);
}


JetSelector::~JetSelector()
{
 
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
JetSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle< View<Jet> > jets;
   iEvent.getByLabel( jetSrc_, jets );
   View<Jet>::const_iterator jet;

   StringCutObjectSelector<pat::Jet> selector(cuts_);
   auto_ptr<JetCollection> selectedJets(new JetCollection());
   

   for(jet = jets->begin(); jet != jets->end(); jet++)
   {
      if(selector(*jet))
         selectedJets->push_back(*jet);
   }

   iEvent.put(selectedJets, labelName_);
 }

// ------------ method called once each job just before starting event loop  ------------
void 
JetSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JetSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
JetSelector::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
JetSelector::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
JetSelector::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
JetSelector::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JetSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetSelector);
