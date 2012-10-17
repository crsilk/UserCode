// -*- C++ -*-
//
// Package:    MT2Producers
// Class:      MT2Producers
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "recipeAUX/OxbridgeMT2/interface/ChengHanBisect_Mt2_332_Calculator.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
//includes needed for this file

using namespace edm;
using namespace reco;
using namespace std;

class MT2Producer : public edm::EDProducer {
   public:
      explicit MT2Producer(const edm::ParameterSet&);
      ~MT2Producer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      InputTag VisibleObject1Src_;
      InputTag VisibleObject2Src_;
      InputTag METSrc_;
      
      bool twoDifferentVisibleObjects_;

      string labelName_;
};


typedef reco::Candidate::LorentzVector LorentzVector;

MT2Producer::MT2Producer(const edm::ParameterSet& iConfig)
{
   VisibleObject1Src_ = iConfig.getParameter<InputTag>("VisibleObject1Src");
   VisibleObject2Src_ = iConfig.getParameter<InputTag>("VisibleObject2Src");
   METSrc_ = iConfig.getParameter<InputTag>("METSrc");
   twoDifferentVisibleObjects_= iConfig.getParameter<bool>("twoDifferentVisibleObjects");
   
   labelName_ = iConfig.getParameter<string>("labelName");
   
   




   produces<vector<double> >(labelName_);

}


MT2Producer::~MT2Producer()
{
 
   

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
MT2Producer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   
   Handle<View<LeafCandidate> > VisibleObjects1;
   iEvent.getByLabel(VisibleObject1Src_, VisibleObjects1);

   Handle<View<LeafCandidate> > VisibleObjects2;
   if(twoDifferentVisibleObjects_)
      iEvent.getByLabel(VisibleObject2Src_, VisibleObjects2);

   Handle<pat::METCollection> METs;
   iEvent.getByLabel(METSrc_, METs);
   pat::METCollection::const_iterator MET = METs->begin();

   auto_ptr<vector<double> > mt2(new vector<double>());
   Mt2::ChengHanBisect_Mt2_332_Calculator mt2Calculator;   


   if(VisibleObjects1->size() > 1 && !twoDifferentVisibleObjects_)
   {


   const double massOfSystemA =  (*VisibleObjects1)[0].mass(); 
   const double pxOfSystemA   =  (*VisibleObjects1)[0].px(); 
   const double pyOfSystemA   =  (*VisibleObjects1)[0].py(); 
   
   const double massOfSystemB =  (*VisibleObjects1)[1].mass(); 
   const double pxOfSystemB   =  (*VisibleObjects1)[1].px(); 
   const double pyOfSystemB   =  (*VisibleObjects1)[1].py(); 
   
   const double pxMiss        = MET->px(); 
   const double pyMiss        = MET->py(); 
   
   const double invis_mass    = 0; 

   Mt2::LorentzTransverseVector  vis_A(Mt2::TwoVector(pxOfSystemA, 
                                                      pyOfSystemA), massOfSystemA);
   Mt2::LorentzTransverseVector  vis_B(Mt2::TwoVector(pxOfSystemB, pyOfSystemB), massOfSystemB);
   Mt2::TwoVector                pT_Miss(pxMiss, pyMiss);
   
   mt2->push_back(mt2Calculator.mt2_332(vis_A, vis_B, pT_Miss, invis_mass));

   }
   else if(twoDifferentVisibleObjects_ && VisibleObjects1->size() > 0 && 
           VisibleObjects2->size() > 0)
   {
         const double massOfSystemA =  (*VisibleObjects1)[0].mass(); 
         const double pxOfSystemA   =  (*VisibleObjects1)[0].px(); 
         const double pyOfSystemA   =  (*VisibleObjects1)[0].py(); 
         
         const double massOfSystemB =  (*VisibleObjects2)[0].mass(); 
         const double pxOfSystemB   =  (*VisibleObjects2)[0].px(); 
         const double pyOfSystemB   =  (*VisibleObjects2)[0].py(); 
         
         const double pxMiss        = MET->px(); 
         const double pyMiss        = MET->py(); 
         
         const double invis_mass    = 0; 
         
         Mt2::LorentzTransverseVector  vis_A(Mt2::TwoVector(pxOfSystemA, 
                                                            pyOfSystemA), massOfSystemA);
         Mt2::LorentzTransverseVector  vis_B(Mt2::TwoVector(pxOfSystemB, pyOfSystemB), massOfSystemB);
         Mt2::TwoVector                pT_Miss(pxMiss, pyMiss);
         
   mt2->push_back(mt2Calculator.mt2_332(vis_A, vis_B, pT_Miss, invis_mass));
   }
   
   
   iEvent.put(mt2, labelName_);
   


}

// ------------ method called once each job just before starting event loop  ------------
void 
MT2Producer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MT2Producer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
MT2Producer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
MT2Producer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
MT2Producer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
MT2Producer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MT2Producer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MT2Producer);
