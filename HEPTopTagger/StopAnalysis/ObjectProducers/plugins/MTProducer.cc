// -*- C++ -*-
//
// Package:    MTProducers
// Class:      MTProducers
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
//includes needed for this file

using namespace edm;
using namespace reco;
using namespace std;

class MTProducer : public edm::EDProducer {
   public:
      explicit MTProducer(const edm::ParameterSet&);
      ~MTProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      InputTag Object1Src_;
      InputTag Object2Src_;

};


typedef reco::Candidate::LorentzVector LorentzVector;

MTProducer::MTProducer(const edm::ParameterSet& iConfig)
{
   Object1Src_ = iConfig.getParameter<InputTag>("Object1Src");
   Object2Src_ = iConfig.getParameter<InputTag>("Object2Src");
   
   
   produces<vector<double> >();

}


MTProducer::~MTProducer()
{
 
   

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
MTProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   
   Handle<View<Candidate> > Objects1;
   iEvent.getByLabel(Object1Src_, Objects1);


   Handle<View<Candidate> > Objects2;
   iEvent.getByLabel(Object2Src_, Objects2);

   auto_ptr<vector<double > > mt(new vector<double>());

   if( Objects1->size() > 0 && Objects2->size() > 0)
   {


   double Et_1=sqrt((*Objects1)[0].mass()*(*Objects1)[0].mass() + (*Objects1)[0].pt()*(*Objects1)[0].pt());
   double Et_2=sqrt((*Objects2)[0].mass()*(*Objects2)[0].mass() + (*Objects2)[0].pt()*(*Objects2)[0].pt());

   mt->push_back(sqrt( (*Objects1)[0].mass()*(*Objects1)[0].mass() + (*Objects2)[0].mass()*(*Objects2)[0].mass() + 2*( Et_1*Et_2 - (*Objects1)[0].px()*(*Objects2)[0].px() - (*Objects1)[0].py()*(*Objects2)[0].py() ) ));
   }
   
   iEvent.put(mt);



}

// ------------ method called once each job just before starting event loop  ------------
void 
MTProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MTProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
MTProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
MTProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
MTProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
MTProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MTProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MTProducer);
