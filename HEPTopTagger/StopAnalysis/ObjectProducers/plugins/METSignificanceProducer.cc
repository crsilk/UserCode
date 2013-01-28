// -*- C++ -*-
//
// Package:    METSignificanceProducers
// Class:      METSignificanceProducers
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
//includes needed for this file

using namespace edm;
using namespace reco;
using namespace std;

class METSignificanceProducer : public edm::EDProducer {
   public:
      explicit METSignificanceProducer(const edm::ParameterSet&);
      ~METSignificanceProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

   InputTag metSrc_;
   InputTag jetSrc_;



};


typedef reco::Candidate::LorentzVector LorentzVector;

METSignificanceProducer::METSignificanceProducer(const edm::ParameterSet& iConfig)
{
   metSrc_ = iConfig.getParameter<InputTag>("metSrc");
   jetSrc_ = iConfig.getParameter<InputTag>("jetSrc");
  
   produces<double> ("metPt");
   produces<double> ("sumPt");

}


METSignificanceProducer::~METSignificanceProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
METSignificanceProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle<View<MET> > mets;
   iEvent.getByLabel(metSrc_, mets);
   Handle<View<Jet> > jets;
   iEvent.getByLabel(jetSrc_, jets);

   auto_ptr<double> met(new double());
   auto_ptr<double> sumPt(new double());
   
   for(unsigned i = 0; i < jets->size(); i++)
   {
      *sumPt += (*jets)[i].pt();
   }
   
   *met = (*mets)[0].pt();

   iEvent.put(met, "metPt");
   iEvent.put(sumPt, "sumPt");



}

// ------------ method called once each job just before starting event loop  ------------
void 
METSignificanceProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
METSignificanceProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
METSignificanceProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
METSignificanceProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
METSignificanceProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
METSignificanceProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
METSignificanceProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(METSignificanceProducer);
