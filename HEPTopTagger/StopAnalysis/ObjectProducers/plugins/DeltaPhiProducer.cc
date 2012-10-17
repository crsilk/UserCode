// -*- C++ -*-
//
// Package:    DeltaPhiObjectVsMETProducers
// Class:      DeltaPhiObjectVsMETProducers
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/PatCandidates/interface/MET.h"


//includes needed for this file

using namespace edm;
using namespace reco;
using namespace std;

class DeltaPhiObjectVsMETProducer : public edm::EDProducer {
   public:
      explicit DeltaPhiObjectVsMETProducer(const edm::ParameterSet&);
      ~DeltaPhiObjectVsMETProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);


      InputTag ObjectSrc_;
      InputTag METSrc_;

      unsigned nDeltaPhis_;
      string labelName_;

};




struct GreaterByPtCandPtr 
{
  bool operator()( LeafCandidate  t1, LeafCandidate  t2 ) const 
  {return t1.pt() > t2.pt();}
};


DeltaPhiObjectVsMETProducer::DeltaPhiObjectVsMETProducer(const edm::ParameterSet& iConfig)
{
   ObjectSrc_ = iConfig.getParameter<InputTag>("ObjectSrc");
   METSrc_ = iConfig.getParameter<InputTag>("METSrc");
   nDeltaPhis_ = iConfig.getParameter<unsigned>("nDeltaPhis");

   labelName_ = iConfig.getParameter<string>("labelName");

   produces<vector<double> > (labelName_);

}


DeltaPhiObjectVsMETProducer::~DeltaPhiObjectVsMETProducer()
{
 

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
DeltaPhiObjectVsMETProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle<View<LeafCandidate> > Objects;
   iEvent.getByLabel(ObjectSrc_, Objects); 
   vector<LeafCandidate> ObjectsVector;
   View<LeafCandidate>::const_iterator Object;

   
   Handle<View<MET> > METs;
   iEvent.getByLabel(METSrc_, METs); 
   MET met = (*METs)[0];

   Candidate::LorentzVector tempP4;
   auto_ptr<vector<double> > deltaPhis(new vector<double>());
   

   for(Object = Objects->begin(); Object != Objects->end(); Object++)
      ObjectsVector.push_back(*Object);
   
   sort(ObjectsVector.begin(), ObjectsVector.end(), GreaterByPtCandPtr());

   
   for(unsigned i = 0; i < ObjectsVector.size();i++)
   {   
      if(i >= nDeltaPhis_)
         break;

      tempP4 = ObjectsVector[i].p4() - met.p4();
      deltaPhis->push_back(abs(tempP4.phi()));
   }
   
   iEvent.put(deltaPhis, labelName_);
}

// ------------ method called once each job just before starting event loop  ------------
void 
DeltaPhiObjectVsMETProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DeltaPhiObjectVsMETProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
DeltaPhiObjectVsMETProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
DeltaPhiObjectVsMETProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
DeltaPhiObjectVsMETProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
DeltaPhiObjectVsMETProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DeltaPhiObjectVsMETProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DeltaPhiObjectVsMETProducer);
