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
// $Id: CountEventsAfterCuts.cc,v 1.1 2012/10/26 20:58:20 crsilk Exp $
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
      InputTag modelPointSrc_;
      vector<string> cutNames_;
      vector<int> cutDecisions_;
      double totalCount_;
      vector<double> counts_;
      unsigned countsSize_;

      bool runOnSignal_;
      vector<vector<double> > allModelPoints_;
      vector<double> signalTotalCount_;
      vector<vector<double> > signalCounts_;
      int eventNumber_;
      
};




CountEventsAfterCuts::CountEventsAfterCuts(const edm::ParameterSet& iConfig)
{
   bitSetSrc_ = iConfig.getParameter<InputTag>("bitSetSrc");
   genEventSrc_= iConfig.getParameter<InputTag>("genEventSrc");
   modelPointSrc_ = iConfig.getParameter<InputTag>("modelPointSrc");
   cutNames_ = iConfig.getParameter<vector<string> >("cutNames");
   cutDecisions_ = iConfig.getParameter<vector<int> >("cutDecisions");

   if(iConfig.exists("runOnSignal"))
      runOnSignal_ = iConfig.getParameter<bool>("runOnSignal");

   else
   {
      runOnSignal_ = false;
   }
      

   eventNumber_ = 0;
   totalCount_ = 0;
   countsSize_ = 0;
   for(unsigned i = 0; i < cutNames_.size(); i++)
   {
      countsSize_++;
      counts_.push_back(0.0);
   }
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

   Handle<vector<double> > modelPoints;
   unsigned eventIndex = 0;

   TriggerNames  bitSetNames = iEvent.triggerNames(*bitSet);
   bool cutNameFound;
   double weight = genEvent->weight();

   eventNumber_++;

   if( cutNames_.size() != cutDecisions_.size())
   {
      cout<<"Number of cutNames ="<<cutNames_.size()<<"\nNumber of cutDecisions ="<<cutDecisions_.size()<<endl;
      throw cms::Exception("Configuration Error") <<"Number of cut decisions does not match the number of cuts names.";
   }

   if (runOnSignal_)
   {
      bool alreadyThere = false;
      iEvent.getByLabel(modelPointSrc_, modelPoints);

      for(unsigned i = allModelPoints_.size() ; i-- > 0;)
      {
         if (allModelPoints_.size() == 0) 
            break;
         
         if(allModelPoints_[i] == *modelPoints)
         {
            eventIndex = i;
            alreadyThere =true;
            signalTotalCount_[i] = signalTotalCount_[i] + weight;
            break;
         }
      }
      if(!alreadyThere)
      {
         signalTotalCount_.push_back(1);
         signalCounts_.push_back(vector<double>(countsSize_, 0));
         allModelPoints_.push_back(*modelPoints);
         eventIndex = allModelPoints_.size() -1;
         
      }
      
   }

   
   totalCount_ = totalCount_ + weight;
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
      if(runOnSignal_)
      {
         signalCounts_[eventIndex][i] = signalCounts_[eventIndex][i] + weight;
      }

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
   cout<<"Total Count: "<<totalCount_<<endl;
   cout<<"Cut      Number that passed this cut "<<endl;
   for(unsigned i = 0; i < counts_.size(); i++)
      cout<<cutNames_[i]<<"\t";
   cout<<endl;
   for(unsigned i = 0; i < counts_.size(); i++)
      cout<<counts_[i]<<"\t";
   cout<<endl;

   if(runOnSignal_)
   {
      cout<<"ModelPoint\tTotalCount\t";
      for(unsigned i = 0; i < counts_.size(); i++)
         cout<<cutNames_[i]<<"\t";
      cout<<endl;
      for(unsigned i= 0 ; i < allModelPoints_.size(); i++)
      {
         for(unsigned j = 0; j < allModelPoints_[i].size(); j++)
         {
            cout<<allModelPoints_[i][j];
            if( j != allModelPoints_[i].size()-1) cout<<"_";
         }
         
         cout<<"\t"<<signalTotalCount_[i]<<"\t";
         for(unsigned j = 0; j < signalCounts_[i].size(); j++)
         {
            cout<<signalCounts_[i][j]<<"\t";
         }
         cout<<endl;
      }
   }

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
