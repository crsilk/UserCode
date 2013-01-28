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
#include "DataFormats/Math/interface/deltaPhi.h"

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

   produces<vector<double> > ("all");
   produces<double> ("significance");
   produces<double> ("min");
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

   
   Handle<View<Candidate> > METs;
   iEvent.getByLabel(METSrc_, METs); 

   double met = (*METs)[0].pt();
   double phi;
   double minPhi = 99999;
   double sigMin = 99999;
   double sum;
   double resolution;
   auto_ptr<vector<double> > deltaPhis(new vector<double>());
   auto_ptr<double> minDeltaPhi(new double());
   auto_ptr<double> deltaPhiMinSignificance(new double());

/*   for(Object = Objects->begin(); Object != Objects->end(); Object++)
      ObjectsVector.push_back(*Object);
   
   sort(ObjectsVector.begin(), ObjectsVector.end(), GreaterByPtCandPtr());
*/
   
   for(unsigned i = 0; i < (*Objects).size();i++)
   {   
      sum = 0;
      
      phi = abs(deltaPhi((*METs)[0].phi(), (*Objects)[i].phi()));
      
      if(i < nDeltaPhis_)
      {
         deltaPhis->push_back(phi);
         if(phi < minPhi)      
            minPhi = phi;
      }
      
      for(unsigned j = 0; j < (*Objects).size(); j++)
      {
         if( i == j) continue;
         sum += (0.1*(*Objects)[j].pt()*sin( (*Objects)[i].phi() - 
                                                (*Objects)[j].phi() ) )*
            (0.1*(*Objects)[j].pt()*sin( (*Objects)[i].phi() - 
                                            (*Objects)[j].phi() ) );
      }

      resolution = atan(sqrt(sum)/met);

      if( phi/resolution < sigMin)
         sigMin = phi/resolution;
      
   }
   
   if(sigMin != 99999)
      *deltaPhiMinSignificance = sigMin;

   if(minPhi != 99999) 
      *minDeltaPhi = minPhi;

   iEvent.put(deltaPhis, "all");
   iEvent.put(minDeltaPhi, "min");
   iEvent.put(deltaPhiMinSignificance, "significance");
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
