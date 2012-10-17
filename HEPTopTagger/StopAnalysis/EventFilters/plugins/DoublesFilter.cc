
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace std;
using namespace edm;

class DoublesFilter : public edm::EDFilter {
   public:
      explicit DoublesFilter(const edm::ParameterSet&);
      ~DoublesFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual bool beginRun(edm::Run&, edm::EventSetup const&);
      virtual bool endRun(edm::Run&, edm::EventSetup const&);
      virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

   
      InputTag src_;
      vector<double> minCuts_;
      vector<double> maxCuts_;


};


DoublesFilter::DoublesFilter(const edm::ParameterSet& iConfig)
{
   src_ = iConfig.getParameter<InputTag>("src");
   minCuts_ = iConfig.getParameter<vector<double> >("minCuts");
   maxCuts_ = iConfig.getParameter<vector<double> >("maxCuts");

}


DoublesFilter::~DoublesFilter()
{
 


}


bool
DoublesFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle<vector<double> > doubles;
   iEvent.getByLabel(src_, doubles);

   for(unsigned i = 0; i < minCuts_.size();i++)
   {
      if(doubles->size() == i) break;
      if((*doubles)[i] < minCuts_[i]) return false;
      
   }

   for(unsigned i = 0; i < maxCuts_.size();i++)
   {
      if(doubles->size() == i) break; 
      if((*doubles)[i] > maxCuts_[i]) return false;
      
   }

   return true;

}

// ------------ method called once each job just before starting event loop  ------------
void 
DoublesFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DoublesFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool 
DoublesFilter::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
bool 
DoublesFilter::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
DoublesFilter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
DoublesFilter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DoublesFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(DoublesFilter);
