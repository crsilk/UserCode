// -*- C++ -*-
//
// Package:    TripletSelectors
// Class:      TripletSelectors
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetCollection.h"

//includes needed for this file

using namespace edm;
using namespace reco;
using namespace std;

class TripletSelector : public edm::EDProducer {
public:
   explicit TripletSelector(const edm::ParameterSet&);
   ~TripletSelector();

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
   InputTag indexSrc_;
   
   double Rmin_;
   double Rmax_;
   double arctanmin_;
   double arctanmax_;
   double m23OverM123Cut_;
   double topMass_;

   bool bJetVeto_;

};


typedef reco::Candidate::LorentzVector LorentzVector;

TripletSelector::TripletSelector(const edm::ParameterSet& iConfig)
{
   jetSrc_ = iConfig.getParameter<InputTag>("jetSrc");
   bJetSrc_ = iConfig.getParameter<InputTag>("bJetSrc");
   indexSrc_ = iConfig.getParameter<InputTag>("indexSrc");
   bJetVeto_ = iConfig.getParameter<bool>("bJetVeto");

   Rmin_ = iConfig.getParameter<double> ("Rmin");
   Rmax_ = iConfig.getParameter<double> ("Rmax");
   arctanmin_ = iConfig.getParameter<double> ("arctanmin");
   arctanmax_ = iConfig.getParameter<double> ("arctanmax");
   m23OverM123Cut_ = iConfig.getParameter<double>("m23OverM123Cut");
   topMass_ = iConfig.getParameter<double> ("topMass");

   produces<vector<BasicJet> >("selectedTriplet");
   produces<vector<BasicJet> >("selectedRSystem");
   produces<vector<BasicJet> >("selectedBJets");
   produces<vector<int> >("selectedTripletIndex");
   produces<vector<int> >("selectedBJetsIndex");
   produces<vector<double> >("selectedM123");
   produces<vector<double> >("selectedM12");
   produces<vector<double> >("selectedM13");
   produces<vector<double> >("selectedM23");
   produces <double> ("selectedM23OverM123");
   
}


TripletSelector::~TripletSelector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void TripletSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   
   Handle<View<Jet> > jets;
   iEvent.getByLabel(jetSrc_, jets);
   
   Handle<View<Jet> > bJets;
   iEvent.getByLabel(bJetSrc_, bJets);

   Handle<vector<vector<int> > > indices;
   iEvent.getByLabel(indexSrc_, indices);
   
   LorentzVector p41;
   LorentzVector p42;
   LorentzVector p43;
   
   bool bJetPass;
   bool bJetInTrip;
   bool bJetOutTrip;
   bool passDijet;
   bool passM23OverM123;
   bool aBJetBehaves = false;
   bool atLeastOnePassed = false;
   bool skipFailedM23OverM123 =false;

   unsigned selectedIndex;
   
   double bJetEta;
   double bJetPhi;
   double m123;
   double m12;
   double m13;
   double m23;
   double dTopMin = 999999;

   vector<double> m123s;
   vector<double> m12s;
   vector<double> m13s;
   vector<double> m23s;
   vector<vector<int> > bJetInRIndices;

   vector<bool> bJetsBehave;
   vector<bool> passesDijetCuts;
   vector<bool> passesM23OverM123Cut;

   auto_ptr<vector<BasicJet> > selectedTriplet(new vector<BasicJet>());
   auto_ptr<vector<BasicJet> > selectedRSystem(new vector<BasicJet>());
   auto_ptr<vector<BasicJet> > selectedBJets(new vector<BasicJet>());
   auto_ptr<vector<int> > selectedTripletIndex(new vector<int>());
   auto_ptr<vector<int> > selectedBJetsIndex(new vector<int>());
   auto_ptr<vector<double> > selectedM123(new vector<double>());
   auto_ptr<vector<double> > selectedM12(new vector<double>());
   auto_ptr<vector<double> > selectedM13(new vector<double>());
   auto_ptr<vector<double> > selectedM23(new vector<double>());
   auto_ptr<double>  selectedM23OverM123(new double());


   for(unsigned i = 0; i < (*indices).size(); i++)
   {
      bJetPass = true;
      passDijet = false;
      passM23OverM123 = false;

      vector<int> tempBJetIndices;
      if((*indices)[i].size() != 3) 
         cout<<(*indices)[i].size()<<endl;

      p41 = (*jets)[(*indices)[i][0]].p4();
      p42 = (*jets)[(*indices)[i][1]].p4();
      p43 = (*jets)[(*indices)[i][2]].p4();
      
      m123 = (p41 + p42 + p43).mass();
      m12 = (p41 + p42).mass();
      m13 = (p41 + p43).mass();
      m23 = (p42 + p43).mass();

      //Check if the bjet(s) is in the right place

      if( (*bJets).size() == 0)
      {
         bJetPass = false;
      }
      else if( (*bJets).size() == 1)
      {         
         bJetEta = (*bJets)[0].eta();
         bJetPhi = (*bJets)[0].phi();
         if( (deltaR(p41.eta(), p41.phi(), bJetEta, bJetPhi) < 0.01)   || 
             (deltaR(p42.eta(), p42.phi(), bJetEta, bJetPhi) < 0.01)   || 
             (deltaR(p43.eta(), p43.phi(), bJetEta, bJetPhi) < 0.01) )
         {
            bJetPass = false;
         }

         else
         {
            tempBJetIndices.push_back(0);
         }
      }
      else if( (*bJets).size() >= 2)
      {
         bJetInTrip = false;
         bJetOutTrip = false;
         for (unsigned j = 0; j < (*bJets).size(); j++)
         {
            bJetEta = (*bJets)[j].eta();
            bJetPhi = (*bJets)[j].phi();
            if( (deltaR(p41.eta(), p41.phi(), bJetEta, bJetPhi) < 0.01)  || 
                (deltaR(p42.eta(), p42.phi(), bJetEta, bJetPhi) < 0.01)  || 
                (deltaR(p43.eta(), p43.phi(), bJetEta, bJetPhi) < 0.01) )
            {
               bJetInTrip = true;
            }
            else
            {
               tempBJetIndices.push_back(j);
               bJetOutTrip = true;
            }
         }

         if( !bJetInTrip || !bJetOutTrip)
            bJetPass = false;

      }

      
      //check if it passes the dijetmass cuts
      
      if(arctanmin_ < atan(m13/m12) && atan(m13/m12) < arctanmax_ && 
         Rmin_ < m23/m123 && m23/m123 < Rmax_) 
         passDijet = true;

      if(Rmin_*Rmin_ * (1+m13*m13/(m12*m12))  < 1 - m23*m23/(m123*m123) &&
         Rmax_*Rmax_ * (1+m13*m13/(m12*m12))  > 1 - m23*m23/(m123*m123))
         passDijet = true;

      if(Rmin_*Rmin_ * (1+m12*m12/(m13*m13))  < 1 - m23*m23/(m123*m123) &&
         Rmax_*Rmax_ * (1+m12*m12/(m13*m13))  > 1 - m23*m23/(m123*m123)) 
         passDijet = true;

      if(m23/m123 > m23OverM123Cut_) 
         passM23OverM123 = true;

      passesDijetCuts.push_back(passDijet);
      passesM23OverM123Cut.push_back(passM23OverM123);    
      bJetsBehave.push_back(bJetPass);
      bJetInRIndices.push_back(tempBJetIndices);

      m123s.push_back(m123);
      m12s.push_back(m12);
      m13s.push_back(m13);
      m23s.push_back(m23);
  
   }

   
   //Choose the selected triplet

   //With the bjet veto
   if(bJetVeto_)
   {
      for(unsigned i = 0; i < (*indices).size(); i++)
      {
         if(!passesDijetCuts[i]) continue;
         
         //If triplets that pass the dijets cuts have a bjet, throw them out
         if(!bJetsBehave[i])
         {
            aBJetBehaves = true;
            break;
         }

         //Preferentially pick ones that pass the m23/m123 cut
         if(passesM23OverM123Cut[i])
         {
            if(!skipFailedM23OverM123) 
               dTopMin = 999999;
            
            skipFailedM23OverM123 = true;

            if(abs(m123s[i] - topMass_) < dTopMin)
            {
               atLeastOnePassed = true;
               selectedIndex = i;
               dTopMin = abs(m123s[i] - topMass_);
            }
         }

         //Only consider these if you don't have a triplet passing m23/m123 
         if(!skipFailedM23OverM123)
         {
            if(abs(m123s[i] - topMass_) < dTopMin)
            {
               atLeastOnePassed = true;
               selectedIndex = i;
               dTopMin = abs(m123s[i] - topMass_);
            }
         }

         
      }

      if(aBJetBehaves)
      {
         iEvent.put(selectedTriplet, "selectedTriplet");
         iEvent.put(selectedRSystem, "selectedRSystem");
         iEvent.put(selectedBJets, "selectedBJets");
         iEvent.put(selectedTripletIndex, "selectedTripletIndex");
         iEvent.put(selectedBJetsIndex, "selectedBJetsIndex");
         iEvent.put(selectedM12, "selectedM12");
         iEvent.put(selectedM13, "selectedM13");
         iEvent.put(selectedM23, "selectedM23");
         iEvent.put(selectedM123, "selectedM123");
         iEvent.put(selectedM23OverM123, "selectedM23OverM123");
         return;
      }

      if(atLeastOnePassed)
      {

         Jet::Constituents tripletConsts;
         LorentzVector tripletP4(0, 0, 0, 0);
         Candidate::Point tripletVertex = (*jets)[(*indices)[selectedIndex][0]].vertex();
         for( int i = 0; i < int((*jets).size()); i++)
         {
            if( i == (*indices)[selectedIndex][0] || 
                i == (*indices)[selectedIndex][1] || 
                i == (*indices)[selectedIndex][2])
            {
               tripletP4 = tripletP4 + (*jets)[i].p4();
               for(unsigned j = 0; j < (*jets)[i].numberOfDaughters(); j++)
                  tripletConsts.push_back((*jets)[i].daughterPtr(j));
            }
            else
            {
               selectedRSystem->push_back(BasicJet((*jets)[i].p4(), (*jets)[i].vertex(), (*jets)[i].getJetConstituents()));
            }
         }
         
         selectedTriplet->push_back(BasicJet(tripletP4, tripletVertex, tripletConsts));

         selectedTripletIndex->push_back((*indices)[selectedIndex][0]);
         selectedTripletIndex->push_back((*indices)[selectedIndex][1]);
         selectedTripletIndex->push_back((*indices)[selectedIndex][2]);
         selectedM12->push_back(m12s[selectedIndex]);
         selectedM13->push_back(m13s[selectedIndex]);
         selectedM23->push_back(m23s[selectedIndex]);
         selectedM123->push_back(m123s[selectedIndex]);
         *selectedM23OverM123 = m23s[selectedIndex]/m123s[selectedIndex];
      }

   }
//Without the bJet veto
   else
   {
      for(unsigned i = 0; i < (*indices).size(); i++)
      {
         if(!passesDijetCuts[i] || !bJetsBehave[i]) continue;
         
         //Preferentially pick ones that pass the m23/m123 cut
         if(passesM23OverM123Cut[i])
         {
            if(!skipFailedM23OverM123) 
               dTopMin = 999999;
            
            skipFailedM23OverM123 = true;

            if(abs(m123s[i] - topMass_) < dTopMin)
            {
               atLeastOnePassed = true;
               selectedIndex = i;
               dTopMin = abs(m123s[i] - topMass_);
            }
         }

         //Only consider these if you don't have a triplet passing m23/m123 
         if(!skipFailedM23OverM123)
         {
            if(abs(m123s[i] - topMass_) < dTopMin)
            {
               atLeastOnePassed = true;
               selectedIndex = i;
               dTopMin = abs(m123s[i] - topMass_);
            }
         }
      }
      
      if(atLeastOnePassed)
      {

         Jet::Constituents tripletConsts;
         LorentzVector tripletP4(0, 0, 0, 0);
         Candidate::Point tripletVertex = (*jets)[(*indices)[selectedIndex][0]].vertex();
         for( int i = 0; i < int((*jets).size()); i++)
         {
            if( i == (*indices)[selectedIndex][0] || 
                i == (*indices)[selectedIndex][1] || 
                i == (*indices)[selectedIndex][2])
            {

               tripletP4 = tripletP4 + (*jets)[i].p4();
               for(unsigned j = 0; j < (*jets)[i].numberOfDaughters(); j++)
                  tripletConsts.push_back((*jets)[i].daughterPtr(j));

            }
            else
            {
               selectedRSystem->push_back(BasicJet((*jets)[i].p4(), (*jets)[i].vertex(), (*jets)[i].getJetConstituents()));
            }
         }
         
         selectedTriplet->push_back(BasicJet(tripletP4, tripletVertex, tripletConsts));
         for(unsigned i = 0; i < bJetInRIndices[selectedIndex].size(); i++)
         {
            selectedBJets->push_back(BasicJet((*bJets)[bJetInRIndices[selectedIndex][i]].p4(), (*bJets)[bJetInRIndices[selectedIndex][i]].vertex(), (*bJets)[bJetInRIndices[selectedIndex][i]].getJetConstituents()));
            selectedBJetsIndex->push_back(bJetInRIndices[selectedIndex][i]);
         }
         selectedTripletIndex->push_back((*indices)[selectedIndex][0]);
         selectedTripletIndex->push_back((*indices)[selectedIndex][1]);
         selectedTripletIndex->push_back((*indices)[selectedIndex][2]);
         selectedM12->push_back(m12s[selectedIndex]);
         selectedM13->push_back(m13s[selectedIndex]);
         selectedM23->push_back(m23s[selectedIndex]);
         selectedM123->push_back(m123s[selectedIndex]);
         *selectedM23OverM123 = m23s[selectedIndex]/m123s[selectedIndex];
         
      }
   }

   
   iEvent.put(selectedTriplet, "selectedTriplet");
   iEvent.put(selectedRSystem, "selectedRSystem");
   iEvent.put(selectedBJets, "selectedBJets");
   iEvent.put(selectedTripletIndex, "selectedTripletIndex");
   iEvent.put(selectedBJetsIndex, "selectedBJetsIndex");
   iEvent.put(selectedM12, "selectedM12");
   iEvent.put(selectedM13, "selectedM13");
   iEvent.put(selectedM23, "selectedM23");
   iEvent.put(selectedM123, "selectedM123");
   iEvent.put(selectedM23OverM123, "selectedM23OverM123");
   
   
}

// ------------ method called once each job just before starting event loop  ------------
void 
TripletSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TripletSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
TripletSelector::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
TripletSelector::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
TripletSelector::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
TripletSelector::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TripletSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TripletSelector);
