// -*- C++ -*-
//
// Package:    CandidateSelectors
// Class:      CandidateSelectors
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
using namespace reco;
using namespace std;

class CandidateSelector : public edm::EDProducer {
   public:
      explicit CandidateSelector(const edm::ParameterSet&);
      ~CandidateSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      InputTag candidateSrc_;
      string cuts_;
      string labelName_;

};



CandidateSelector::CandidateSelector(const edm::ParameterSet& iConfig)
{
   candidateSrc_ = iConfig.getParameter<InputTag>("src");
   cuts_ = iConfig.getParameter<string>("cuts");
   
   if(iConfig.exists("labelName"))
      labelName_ = iConfig.getParameter<string>("labelName");
   else
   {
      labelName_ = "SelectedJets";
   }
      
   produces<CandidateCollection> (labelName_);
}


CandidateSelector::~CandidateSelector()
{
 
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
CandidateSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle< View<Candidate> > candidates;
   iEvent.getByLabel( candidateSrc_, candidates );
   View<Candidate>::const_iterator candidate;

   StringCutObjectSelector<Candidate> selector(cuts_);
   auto_ptr<CandidateCollection> selectedCandidates(new CandidateCollection());
   

   for(candidate = candidates->begin(); candidate != candidates->end(); candidate++)
   {
      if(selector(*candidate))
         selectedCandidates->push_back(*candidate);
   }

   iEvent.put(selectedCandidates, labelName_);
 }

// ------------ method called once each job just before starting event loop  ------------
void 
CandidateSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CandidateSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
CandidateSelector::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
CandidateSelector::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
CandidateSelector::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
CandidateSelector::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CandidateSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CandidateSelector);
