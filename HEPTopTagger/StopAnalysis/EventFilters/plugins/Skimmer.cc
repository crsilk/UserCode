#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/TriggerResults.h"
 #include "FWCore/Common/interface/TriggerNames.h"

using namespace std;
using namespace edm;

class Skimmer : public edm::EDFilter {
   public:
      explicit Skimmer(const edm::ParameterSet&);
      ~Skimmer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual bool beginRun(edm::Run&, edm::EventSetup const&);
      virtual bool endRun(edm::Run&, edm::EventSetup const&);
      virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);


      InputTag bitSetSrc_;
      vector<string> cutNames_;
      vector<int> cutDecisions_;
};


Skimmer::Skimmer(const edm::ParameterSet& iConfig)
{
   bitSetSrc_ = iConfig.getParameter<InputTag>("bitSetSrc");
   cutNames_ = iConfig.getParameter<vector<string> >("cutNames");
   cutDecisions_ = iConfig.getParameter<vector<int> >("cutDecisions");

}


Skimmer::~Skimmer()
{
 

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
Skimmer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   Handle<TriggerResults> bitSet;
   iEvent.getByLabel(bitSetSrc_, bitSet);
   
   TriggerNames  bitSetNames = iEvent.triggerNames(*bitSet);
   bool cutNameFound;

   if( cutNames_.size() != cutDecisions_.size())
      throw cms::Exception("Configuration Error") <<"Number of cut decisions does not match the number of cuts names.";
   
   

   for(unsigned i = 0; i < cutNames_.size(); i++)
   {      
      cutNameFound = false;

      for(unsigned j = 0; j < bitSetNames.size(); j++)
      {
         if(bitSetNames.triggerName(j) == cutNames_[i])
         {
            cutNameFound = true;
            if(bitSet->accept(j) != cutDecisions_[i])
            {
               return false;
            }
         }
      }

      if(cutNameFound == false)
         throw cms::Exception("Configuration Error") <<cutNames_[i]<<" is not a valid cut name. Please check your configuration file";

   }
return true;
}

// ------------ method called once each job just before starting event loop  ------------
void 
Skimmer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Skimmer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool 
Skimmer::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
bool 
Skimmer::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
Skimmer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
Skimmer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
Skimmer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(Skimmer);
