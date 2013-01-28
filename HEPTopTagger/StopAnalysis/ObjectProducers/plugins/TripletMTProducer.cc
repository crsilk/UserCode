// -*- C++ -*-
//
// Package:    TripletMTProducers
// Class:      TripletMTProducers
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
//includes needed for this file

using namespace edm;
using namespace reco;
using namespace std;

class TripletMTProducer : public edm::EDProducer {
   public:
   explicit TripletMTProducer(const edm::ParameterSet&);
   ~TripletMTProducer();

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
   InputTag bJetSrc_;
   InputTag METSrc_;
   InputTag tripletIndexSrc_;
   InputTag bJetIndexSrc_;
};


typedef reco::Candidate::LorentzVector LorentzVector;

TripletMTProducer::TripletMTProducer(const edm::ParameterSet& iConfig)
{
   jetSrc_ = iConfig.getParameter<InputTag>("jetSrc");
   bJetSrc_ = iConfig.getParameter<InputTag>("bJetSrc");
   METSrc_ = iConfig.getParameter<InputTag>("METSrc");
   tripletIndexSrc_ = iConfig.getParameter<InputTag>("tripletIndexSrc");
   bJetIndexSrc_ = iConfig.getParameter<InputTag>("bJetIndexSrc");
   
   produces<vector<double> >("mtB");
   produces<vector<double> >("mtTriplet");
}


TripletMTProducer::~TripletMTProducer()
{
 
   

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
TripletMTProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   Handle<View<Candidate> > METs;
   iEvent.getByLabel(METSrc_, METs);

   Handle<View<Jet> > jets;
   iEvent.getByLabel(jetSrc_, jets);

   Handle<View<Jet> > bJets;
   iEvent.getByLabel(bJetSrc_, bJets);

   Handle<vector<int> > tripletIndices;
   iEvent.getByLabel(tripletIndexSrc_, tripletIndices);

   Handle<vector<int> > bJetIndices;
   iEvent.getByLabel(bJetIndexSrc_, bJetIndices);

   LorentzVector METP4(0, 0, 0, 0);
   LorentzVector tripletP4(0, 0, 0, 0);
   LorentzVector bJetP4(0, 0, 0, 0);

   auto_ptr<vector<double > > mtB(new vector<double>());
   auto_ptr<vector<double > > mtTriplet(new vector<double>());

   METP4 = (*METs)[0].p4();
   

   if((*tripletIndices).size() > 0)
   {
      

      tripletP4 = ( (*jets)[(*tripletIndices)[0]].p4() + 
                    (*jets)[(*tripletIndices)[1]].p4() + 
                    (*jets)[(*tripletIndices)[2]].p4() );


      double Et_1=sqrt(METP4.mass()*METP4.mass() + METP4.pt()*METP4.pt());
      double Et_2=sqrt(tripletP4.mass()*tripletP4.mass() + 
                       tripletP4.pt()*tripletP4.pt());
      
      mtTriplet->push_back(sqrt( METP4.mass()*METP4.mass() + 
                                 tripletP4.mass()*tripletP4.mass() + 
                                 2*(Et_1*Et_2 - METP4.px()*tripletP4.px() - 
                                    METP4.py()*tripletP4.py() ) ));
      
   }

   if((*bJetIndices).size() > 0)
   {
      if( (*bJetIndices).size() > 1)
         throw cms::Exception("Index Size Error")<<"More than one 'bestIndex.'";
      
      bJetP4 = (*bJets)[(*bJetIndices)[0]].p4();


      double Et_1=sqrt(METP4.mass()*METP4.mass() + METP4.pt()*METP4.pt());
      double Et_2=sqrt(bJetP4.mass()*bJetP4.mass() + bJetP4.pt()*bJetP4.pt());
      
      mtB->push_back(sqrt( METP4.mass()*METP4.mass() + 
                           bJetP4.mass()*bJetP4.mass() + 
                           2*( Et_1*Et_2 - METP4.px()*bJetP4.px() - 
                               METP4.py()*bJetP4.py() ) ));
      
   }
   
   iEvent.put(mtB, "mtB");
   iEvent.put(mtTriplet, "mtTriplet");



}

// ------------ method called once each job just before starting event loop  ------------
void 
TripletMTProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TripletMTProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
TripletMTProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
TripletMTProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
TripletMTProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
TripletMTProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TripletMTProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TripletMTProducer);
