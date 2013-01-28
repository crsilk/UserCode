
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

class BoolsFilter : public edm::EDFilter {
   public:
      explicit BoolsFilter(const edm::ParameterSet&);
      ~BoolsFilter();

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
   bool thereExistsA_;



};


BoolsFilter::BoolsFilter(const edm::ParameterSet& iConfig)
{
   src_ = iConfig.getParameter<InputTag>("src");
   thereExistsA_ = iConfig.getParameter<bool>("thereExistsA");

}


BoolsFilter::~BoolsFilter()
{
 


}


bool
BoolsFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle<View<bool> > bools;
   iEvent.getByLabel(src_, bools);


   for(unsigned i = 0; i < (*bools).size();i++)
   {
      if((*bools)[i] == thereExistsA_) return true;
   }

   return false;

}

// ------------ method called once each job just before starting event loop  ------------
void 
BoolsFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
BoolsFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool 
BoolsFilter::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
bool 
BoolsFilter::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
BoolsFilter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
BoolsFilter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
BoolsFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(BoolsFilter);
