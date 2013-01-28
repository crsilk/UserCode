// -*- C++ -*-
//
// Package:    TriangleCutFilter
// Class:      TriangleCutFilter
// 
/**\class TriangleCutFilter TriangleCutFilter.cc StopAnalysis/TriangleCutFilter/src/TriangleCutFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christopher Silkworth
//         Created:  Wed Oct  3 17:38:08 CDT 2012
// $Id: TriangleCutFilter.cc,v 1.2 2012/11/14 14:27:05 crsilk Exp $
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

class TriangleCutFilter : public edm::EDFilter {
   public:
      explicit TriangleCutFilter(const edm::ParameterSet&);
      ~TriangleCutFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual bool beginRun(edm::Run&, edm::EventSetup const&);
      virtual bool endRun(edm::Run&, edm::EventSetup const&);
      virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);


      InputTag src1_;
      InputTag src2_;
      double src1Coeff_;
      double src2Coeff_;
      double triangleMinCut_;
      double triangleMaxCut_;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TriangleCutFilter::TriangleCutFilter(const edm::ParameterSet& iConfig)
{
   src1_ = iConfig.getParameter<InputTag>("src1");
   src2_ = iConfig.getParameter<InputTag>("src2");

   src1Coeff_ = iConfig.getParameter<double>("src1Coeff");
   src2Coeff_ = iConfig.getParameter<double>("src2Coeff");   
   triangleMinCut_ = iConfig.getParameter<double>("triangleMinCut");
   triangleMaxCut_ = iConfig.getParameter<double>("triangleMaxCut");


}


TriangleCutFilter::~TriangleCutFilter()
{
      
}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
TriangleCutFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle<vector<double> > handle1;
   iEvent.getByLabel(src1_, handle1);

   Handle<vector<double> > handle2;
   iEvent.getByLabel(src2_, handle2);
   
   if(handle1->size() == 0 || handle2->size() == 0)
      return false;

   double value1 = *(handle1->begin());
   double value2 = *(handle2->begin());

   if( (src1Coeff_ * value1 + src2Coeff_ * value2 < triangleMinCut_) || 
       (src1Coeff_ * value1 + src2Coeff_ * value2 > triangleMaxCut_) )
      return false;

   
   return true;



}

// ------------ method called once each job just before starting event loop  ------------
void 
TriangleCutFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TriangleCutFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool 
TriangleCutFilter::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
bool 
TriangleCutFilter::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
TriangleCutFilter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
TriangleCutFilter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TriangleCutFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(TriangleCutFilter);
