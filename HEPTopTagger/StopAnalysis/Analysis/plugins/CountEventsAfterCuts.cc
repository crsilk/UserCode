  // -*- C++ -*-
//
// Package:    CountEventsAfterCuts
// Class:      CountEventsAfterCuts
// 
/**\class CountEventsAfterCuts CountEventsAfterCuts.cc primo/CountEventsAfterCuts/src/CountEventsAfterCuts.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  giulio dujany
//         Created:  Thu Sep 11 09:30:00 CDT 2012
// $Id$
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
using namespace std;
using namespace edm;



class CountEventsAfterCuts : public edm::EDAnalyzer {
public:
  explicit CountEventsAfterCuts(const edm::ParameterSet&);
  ~CountEventsAfterCuts();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  
      InputTag bitSetSrc_;
      InputTag genEventSrc_;
      vector<string> cutNames_;
      vector<int> cutDecisions_;

      vector<double> counts_;

};




CountEventsAfterCuts::CountEventsAfterCuts(const edm::ParameterSet& iConfig)
{
   bitSetSrc_ = iConfig.getParameter<InputTag>("bitSetSrc");
   genEventSrc_= iConfig.getParameter<InputTag>("genEventSrc");
   cutNames_ = iConfig.getParameter<vector<string> >("cutNames");
   cutDecisions_ = iConfig.getParameter<vector<int> >("cutDecisions");

   for(unsigned i = 0; i < cutNames_.size(); i++)
      counts_.push_back(0.0);
}


CountEventsAfterCuts::~CountEventsAfterCuts()
{
 

}


//
// member functions
//

// ------------ method called for each event  ------------
void CountEventsAfterCuts::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle<TriggerResults> bitSet;
   iEvent.getByLabel(bitSetSrc_, bitSet);
   
   Handle<GenEventInfoProduct>    genEvent;
   iEvent.getByLabel(genEventSrc_,  genEvent);

   TriggerNames  bitSetNames = iEvent.triggerNames(*bitSet);
   bool cutNameFound;
   double weight = genEvent->weight();

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
               return;
            }
         }
      }

      if(cutNameFound == false)
         throw cms::Exception("Configuration Error") <<cutNames_[i]<<" is not a valid cut name. Please check your configuration file";
      
      counts_[i] = counts_[i] + weight;


   }

}


// ------------ method called once each job just before starting event loop  ------------
void 
CountEventsAfterCuts::beginJob()
{
 
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CountEventsAfterCuts::endJob() 
{
   cout<<"Cut      Number that passed this cut "<<endl;
   for(unsigned i = 0; i < counts_.size(); i++)
      cout<<cutNames_[i]<<"  "<<counts_[i]<<endl;
}

// ------------ method called when starting to processes a run  ------------
void 
CountEventsAfterCuts::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
CountEventsAfterCuts::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
CountEventsAfterCuts::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
CountEventsAfterCuts::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CountEventsAfterCuts::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

 //Specify that only 'tracks' is allowed
 //To use, remove the default given above and uncomment below
 //ParameterSetDescription desc;
 //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
 //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CountEventsAfterCuts);
