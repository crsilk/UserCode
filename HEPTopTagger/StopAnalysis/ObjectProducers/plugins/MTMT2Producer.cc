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
#include "DataFormats/Math/interface/deltaR.h"
//includes needed for this file

using namespace edm;
using namespace reco;
using namespace std;

class MTMT2Producer : public edm::EDProducer {
   public:
      explicit MTMT2Producer(const edm::ParameterSet&);
      ~MTMT2Producer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

   InputTag METSrc_;
   InputTag rSystemSrc_;
   InputTag bJetsInRSrc_;
   InputTag tripletSrc_;
   
   double mTop_;
   double mWMin_;
   double mWMax_;
   
};


typedef reco::Candidate::LorentzVector LorentzVector;

MTMT2Producer::MTMT2Producer(const edm::ParameterSet& iConfig)
{

   METSrc_ = iConfig.getParameter<InputTag>("METSrc");
   rSystemSrc_ = iConfig.getParameter<InputTag>("rSystemSrc");
   tripletSrc_ = iConfig.getParameter<InputTag>("tripletSrc");
   bJetsInRSrc_ = iConfig.getParameter<InputTag>("bJetsInRSrc");

   mTop_ = iConfig.getParameter<double>("mTop");
   mWMin_ = iConfig.getParameter<double>("mWMin");
   mWMax_ = iConfig.getParameter<double>("mWMax");

   produces<vector<double> >("MT2");
   produces<vector<double> >("MTt");
   produces<vector<double> >("MTb");
   produces<vector<double> >("MTt2MTb");


}


MTMT2Producer::~MTMT2Producer()
{
 
   

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
MTMT2Producer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   
   Handle<View<Candidate> > METs;
   iEvent.getByLabel(METSrc_, METs);

   Handle<View<Jet> > rSystem;
   iEvent.getByLabel(rSystemSrc_, rSystem);

   Handle<View<Jet> > triplet;
   iEvent.getByLabel(tripletSrc_, triplet);

   Handle<View<Jet> > bJetsInR;
   iEvent.getByLabel(bJetsInRSrc_, bJetsInR);
   
   unsigned bJetIndex;
   unsigned bJetIndexForDoublet;
   unsigned closestBJetToMETIndex;
   unsigned wJet1Index;
   unsigned wJet2Index;
   int otherJetIndex = -1;
   double dTopMin = 9999999;
   double dRMin = 9999999;
   double dPhiMin = 9999999;
   double dR;
   double dPhi;
   double Et_1;
   double Et_2;

   bool outsideWWindow = false;
   bool foundTriplet = false;

   LorentzVector bJetP4;
   LorentzVector wJet1P4;
   LorentzVector wJet2P4;
   LorentzVector otherTopP4;
   LorentzVector METP4 = (*METs)[0].p4();
   LorentzVector tripletP4;
   
   auto_ptr<vector<double> > MT2(new vector<double>());
   auto_ptr<vector<double> > MTt(new vector<double>());
   auto_ptr<vector<double> > MTb(new vector<double>());
   auto_ptr<vector<double> > MTt2MTb(new vector<double> ());
   Mt2::ChengHanBisect_Mt2_332_Calculator mt2Calculator;   

   
   if( (*triplet).size() != 0 && (*rSystem).size() != 0 && (*bJetsInR).size() != 0)
   {
      tripletP4 = (*triplet)[0].p4();
      for(unsigned i = 0; i < (*bJetsInR).size(); i++)
      {
         bJetP4 = (*bJetsInR)[i].p4();
         for(unsigned j = 0; j < (*rSystem).size(); j++)
         {
            wJet1P4 = (*rSystem)[j].p4();
            if((deltaR(wJet1P4.eta(), wJet1P4.phi(), 
                       bJetP4.eta(), bJetP4.phi()) < 0.01)) continue;
            
            for(unsigned k = j + 1; k < (*rSystem).size(); k++)
            {
               wJet2P4 = (*rSystem)[k].p4();
               
               if((deltaR(wJet2P4.eta(), wJet2P4.phi(), 
                          bJetP4.eta(), bJetP4.phi()) < 0.01) )  continue;
            
               if( fabs((bJetP4 + wJet1P4 + wJet2P4).mass() - mTop_) < dTopMin)
               {

                  dTopMin = fabs((bJetP4 + wJet1P4 + wJet2P4).mass() - mTop_);
                  foundTriplet = true;
                  bJetIndex = i;
                  wJet1Index = j;
                  wJet2Index = k;
               }
            }
         }
      }
      if(foundTriplet)
      {  
         bJetP4 = (*bJetsInR)[bJetIndex].p4();
         wJet1P4 = (*rSystem)[wJet1Index].p4();
         wJet2P4 = (*rSystem)[wJet2Index].p4();
         if ( (wJet1P4 + wJet2P4).mass() < mWMin_ || 
              (wJet1P4 + wJet2P4).mass() > mWMax_) outsideWWindow = true;
      }

      if(!foundTriplet)
      {
         dPhiMin = 999999;
         for(unsigned i = 0; i < (*bJetsInR).size(); i++)
         {
            dPhi = abs(deltaPhi((*bJetsInR)[i].phi(), (*METs)[0].phi()));
            if( dPhi < dPhiMin)
            {
               dPhiMin = dPhi;
               bJetP4 = (*bJetsInR)[i].p4();
               bJetIndexForDoublet = i;
            }
            
         }
      }     
      else
         bJetIndexForDoublet = bJetIndex;
      dRMin = 99999;
      for(unsigned i = 0; i < (*rSystem).size(); i++)
      {            
         dR = deltaR(bJetP4.eta(), bJetP4.phi(), 
                     (*rSystem)[i].eta(), (*rSystem)[i].phi());
         
         if(dR < 0.01) continue;
         if(dR > 2.0) continue;
         if( (bJetP4 + (*rSystem)[i].p4()).mass() > mTop_) continue;
         
         if(dR < dRMin)
            {            
               otherJetIndex = int(i);
               dRMin = dR;
            }
      }
      
      if(foundTriplet && !outsideWWindow)
      {
         otherTopP4 = bJetP4 + wJet1P4 + wJet2P4;
      }
      else if(outsideWWindow && otherJetIndex != -1)
      {
         otherTopP4 = (*rSystem)[otherJetIndex].p4() + bJetP4;
      }
      else if(otherJetIndex != -1)
      {
         otherTopP4 = (*rSystem)[otherJetIndex].p4() + bJetP4;
      }
      else
      {
         otherTopP4 = bJetP4;            
      }  


      const double massOfSystemA =  tripletP4.M(); 
      const double pxOfSystemA   =  tripletP4.Px(); 
      const double pyOfSystemA   =  tripletP4.Py(); 
      
      const double massOfSystemB =  otherTopP4.M(); 
      const double pxOfSystemB   =  otherTopP4.Px(); 
      const double pyOfSystemB   =  otherTopP4.Py(); 
      
      const double pxMiss        = METP4.Px(); 
      const double pyMiss        = METP4.Py(); 
      
      const double invis_mass    = METP4.M(); 
      
      Mt2::LorentzTransverseVector  vis_A(Mt2::TwoVector(pxOfSystemA, 
                                                         pyOfSystemA), massOfSystemA);
      Mt2::LorentzTransverseVector  vis_B(Mt2::TwoVector(pxOfSystemB, pyOfSystemB), massOfSystemB);
      Mt2::TwoVector                pT_Miss(pxMiss, pyMiss);
   
      MT2->push_back(mt2Calculator.mt2_332(vis_A, vis_B, pT_Miss, invis_mass));
   }
   if((*triplet).size() != 0)
   {
      tripletP4 = (*triplet)[0].p4();

      Et_1 = sqrt(tripletP4.mass()*tripletP4.mass() + 
                  tripletP4.pt()*tripletP4.pt());
      Et_2 = sqrt(METP4.mass()*METP4.mass() + METP4.pt()*METP4.pt());

      MTt->push_back(sqrt( tripletP4.mass() * tripletP4.mass() + 
                           METP4.mass()*METP4.mass() + 
                           2*(Et_1*Et_2 - tripletP4.px() * METP4.px() - 
                              tripletP4.py()*METP4.py())));

   }
   
   if((*bJetsInR).size() != 0)
   {
      dPhiMin = 999999;
      
      for(unsigned i = 0; i < (*bJetsInR).size(); i++)
      {
         dPhi = abs(deltaPhi( METP4.phi(), (*bJetsInR)[i].phi()));
         if( dPhi < dPhiMin)
         {
            bJetP4 = (*bJetsInR)[i].p4();
            closestBJetToMETIndex = i;
            dPhiMin = dPhi;
         }         

      } 
      otherTopP4 = bJetP4;

      if( otherJetIndex  != -1 && closestBJetToMETIndex == bJetIndexForDoublet)
      {
         otherTopP4 = (*rSystem)[otherJetIndex].p4() + bJetP4;            
      }

      

      Et_1 = sqrt(otherTopP4.mass()*otherTopP4.mass() + otherTopP4.pt() * otherTopP4.pt());
      Et_2 = sqrt(METP4.mass()*METP4.mass() + METP4.pt()*METP4.pt());

      MTb->push_back(sqrt( otherTopP4.mass() * otherTopP4.mass() +
                           METP4.mass() * METP4.mass() + 
                           2*(Et_1*Et_2 - otherTopP4.px() * METP4.px() - 
                              otherTopP4.py() * METP4.py())));

   }
   if((*bJetsInR).size() != 0 && (*triplet).size() != 0)
   {
      MTt2MTb->push_back((*MTt)[0] + 2 * (*MTb)[0]);
   }


   iEvent.put(MTt, "MTt");
   iEvent.put(MTb, "MTb");
   iEvent.put(MTt2MTb, "MTt2MTb");
   iEvent.put(MT2, "MT2");
   


}

// ------------ method called once each job just before starting event loop  ------------
void 
MTMT2Producer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MTMT2Producer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
MTMT2Producer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
MTMT2Producer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
MTMT2Producer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
MTMT2Producer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MTMT2Producer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MTMT2Producer);
