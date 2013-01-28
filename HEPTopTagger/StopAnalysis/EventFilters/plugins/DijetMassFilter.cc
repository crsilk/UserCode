// -*- C++ -*-
//
// Package:    DijetMassFilter
// Class:      DijetMassFilter
// 
/**\class DijetMassFilter DijetMassFilter.cc StopAnalysis/DijetMassFilter/src/DijetMassFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christopher Silkworth
//         Created:  Wed Oct  3 17:38:08 CDT 2012
// $Id: DijetMassFilter.cc.bk,v 1.1 2012/10/17 22:56:38 crsilk Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace edm;
using namespace std;


class DijetMassFilter : public edm::EDFilter {
   public:
      explicit DijetMassFilter(const edm::ParameterSet&);
      ~DijetMassFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual bool beginRun(edm::Run&, edm::EventSetup const&);
      virtual bool endRun(edm::Run&, edm::EventSetup const&);
      virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

   InputTag m12Src_;
   InputTag m13Src_;
   InputTag m23Src_;
   InputTag m123Src_;
   InputTag bJetsBehaveSrc_;

   double Rmin_;
   double Rmax_;
   double arctanmin_;
   double arctanmax_;
   double m23OverM123Cut_;
   double topMass_;

   bool bJetVeto_;
};

DijetMassFilter::DijetMassFilter(const edm::ParameterSet& iConfig)
{
   m12Src_ = iConfig.getParameter<InputTag> ("m12Src");
   m13Src_ = iConfig.getParameter<InputTag> ("m13Src");
   m23Src_ = iConfig.getParameter<InputTag> ("m23Src");
   m123Src_ = iConfig.getParameter<InputTag> ("m123Src");
   bJetsBehaveSrc_ = iConfig.getParameter<InputTag> ("bJetsBehaveSrc");

   Rmin_ = iConfig.getParameter<double> ("Rmin");
   Rmax_ = iConfig.getParameter<double> ("Rmax");
   arctanmin_ = iConfig.getParameter<double> ("arctanmin");
   arctanmax_ = iConfig.getParameter<double> ("arctanmax");
   m23OverM123Cut_ = iConfig.getParameter<double>("m23OverM123Cut");
   topMass_ = iConfig.getParameter<double> ("topMass");
   bJetVeto_ = iConfig.getParameter<bool>("bJetVeto");
   produces <double> ("bestM23OverM123");
   produces <vector<double> > ("bestM123");
}


DijetMassFilter::~DijetMassFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
DijetMassFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   Handle<vector<double> > m12s;
   iEvent.getByLabel(m12Src_, m12s);

   Handle<vector<double> > m13s;
   iEvent.getByLabel(m13Src_, m13s);
   
   Handle<vector<double> > m23s;
   iEvent.getByLabel(m23Src_, m23s);

   Handle<vector<double> > m123s;
   iEvent.getByLabel(m123Src_, m123s);
   
   Handle<vector<bool> > bJetsBehave;
   iEvent.getByLabel(bJetsBehaveSrc_, bJetsBehave);
   
   vector<unsigned> passesDijetCuts;
   vector<unsigned> passesM23OverM123Cut;
   bool passDijet;
   double m12;
   double m13;
   double m23;
   double m123;
   double dTopMin = 99999999;
   double bestCombM123;
   auto_ptr<double> bestM23OverM123(new double(-1));
   auto_ptr<vector<double> > bestM123(new vector<double> ());

   for(unsigned i = 0; i < bJetsBehave->size(); i++)
   {
      if(bJetVeto_ && (*bJetsBehave)[i])
      {
         iEvent.put(bestM23OverM123, "bestM23OverM123");
         iEvent.put(bestM123, "bestM123");
         return false;
      } 
   }


   for(unsigned i = 0; i < m12s->size(); i++)
   {
      if(!bJetVeto_ && !(*bJetsBehave)[i]) continue;

      m12 = (*m12s)[i];
      m13 = (*m13s)[i];
      m23 = (*m23s)[i];
      m123 = (*m123s)[i];
      passDijet = false;
      
      if(arctanmin_ < atan(m13/m12) && atan(m13/m12) < arctanmax_ && 
         Rmin_ < m23/m123 && m23/m123 < Rmax_) 
         passDijet = true;

      if(Rmin_*Rmin_ * (1+m13*m13/(m12*m12))  < 1 - m23*m23/(m123*m123) &&
         Rmax_*Rmax_ * (1+m13*m13/(m12*m12))  > 1 - m23*m23/(m123*m123))
         passDijet = true;

      if(Rmin_*Rmin_ * (1+m12*m12/(m13*m13))  < 1 - m23*m23/(m123*m123) &&
         Rmax_*Rmax_ * (1+m12*m12/(m13*m13))  > 1 - m23*m23/(m123*m123)) 
         passDijet = true;

      if(passDijet)
      {         
         passesDijetCuts.push_back(i);
         if( m23/m123 > m23OverM123Cut_) 
            passesM23OverM123Cut.push_back(i);
      }
      
   }

   if(passesDijetCuts.size() == 0) 
   {
      iEvent.put(bestM23OverM123, "bestM23OverM123");
      iEvent.put(bestM123, "bestM123");
      return false;
   }

   for( unsigned i = 0; i < passesDijetCuts.size(); i++)
   {
      if( abs((*m123s)[passesDijetCuts[i]] - topMass_) < dTopMin)
      {
         dTopMin = abs((*m123s)[passesDijetCuts[i]] - topMass_) ;
         *bestM23OverM123 = (*m23s)[passesDijetCuts[i]]/((*m123s)[passesDijetCuts[i]]);
         bestCombM123 = (*m123s)[passesDijetCuts[i]];

      }
   }
   dTopMin = 9999999;
   for( unsigned i = 0; i < passesM23OverM123Cut.size(); i++)
   {
      if( abs((*m123s)[passesM23OverM123Cut[i]] - topMass_) < dTopMin)
      {
         dTopMin = abs((*m123s)[passesM23OverM123Cut[i]] - topMass_) ;
         *bestM23OverM123 = (*m23s)[passesM23OverM123Cut[i]]/((*m123s)[passesM23OverM123Cut[i]]);
         bestCombM123 = (*m123s)[passesM23OverM123Cut[i]];

      }
   }

   bestM123->push_back(bestCombM123);

   iEvent.put(bestM23OverM123, "bestM23OverM123");
   iEvent.put(bestM123, "bestM123");
   return true;
}

// ------------ method called once each job just before starting event loop  ------------
void 
DijetMassFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DijetMassFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool 
DijetMassFilter::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
bool 
DijetMassFilter::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
DijetMassFilter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
   return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
DijetMassFilter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DijetMassFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(DijetMassFilter);
