  // -*- C++ -*-
//
// Package:    HT
// Class:      HT
// 
/**\class HT HT.cc primo/HT/src/HT.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  giulio dujany
//         Created:  Thu Sep 11 09:30:00 CDT 2012
// $Id: HT.cc,v 1.3 2012/11/14 14:27:04 crsilk Exp $
//
//


// system include files
#include <memory>
#include <iostream>
#include <string>
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
#include "FWCore/Utilities/interface/RegexMatch.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


#include "TH1F.h"
#include "TH2F.h"

using namespace std;
using namespace edm;
using namespace reco;



class EventPrinter : public edm::EDAnalyzer {
public:
  explicit EventPrinter(const edm::ParameterSet&);
  ~EventPrinter();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  vector<string> getTriggerNames(vector<string> & pattern, TriggerNames & triggerNames);

  
  
};




EventPrinter::EventPrinter(const edm::ParameterSet& iConfig)
{
} 


EventPrinter::~EventPrinter()
{
 

}


//
// member functions
//

// ------------ method called for each event  ------------
void EventPrinter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   cout<<iEvent.eventAuxiliary().run()<< " "<<iEvent.eventAuxiliary().luminosityBlock()<< " "<<iEvent.eventAuxiliary().event()<<endl;
}


// ------------ method called once each job just before starting event loop  ------------
void 
EventPrinter::beginJob()
{
 
}

// ------------ method called once each job just after ending the event loop  ------------
void 
EventPrinter::endJob() 
{

}

// ------------ method called when starting to processes a run  ------------
void 
EventPrinter::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
EventPrinter::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
EventPrinter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
EventPrinter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EventPrinter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
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
vector<string> EventPrinter::getTriggerNames(vector<string> & triggerPatterns, TriggerNames & triggerNames)
{
   vector<string> matchedTriggerNames;
   for(unsigned i = 0; i < triggerPatterns.size(); i++) 
   {
      string pattern = triggerPatterns[i];
      if (edm::is_glob(pattern)) 
      {
         vector<vector<string>::const_iterator > matches = regexMatch(triggerNames.triggerNames(), pattern);
         if(matches.empty())
            throw cms::Exception("Configuration") << "requested pattern \"" << pattern <<  "\" does not match any HLT paths";
         else 
         {
            for(unsigned j = 0; j < matches.size(); j++)
            {
               vector<string>::const_iterator match = matches[j];
               matchedTriggerNames.push_back(*match);
            }
         }
      } 
      else 
         matchedTriggerNames.push_back(pattern);

   }
   return matchedTriggerNames;
}


//define this as a plug-in
DEFINE_FWK_MODULE(EventPrinter);
