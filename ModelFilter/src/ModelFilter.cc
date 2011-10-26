// -*- C++ -*-
//
// Package:    ModelFilter
// Class:      ModelFilter
// 
/**\class ModelFilter ModelFilter.cc UserCode/ModelFilter/src/ModelFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christopher Silkworth
//         Created:  Wed Oct 26 07:23:44 CDT 2011
// $Id$
//
//


// system include files
#include <memory>
#include <string>
#include <sstream>
#include <stdlib.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
//
// class declaration
//

using namespace std;
using namespace edm;

class ModelFilter : public edm::EDFilter {
   public:
      explicit ModelFilter(const edm::ParameterSet&);
      ~ModelFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      vector<string> split(string fstring, string splitter);
      typedef std::vector<std::string>::const_iterator comments_const_iterator;

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual bool beginRun(edm::Run&, edm::EventSetup const&);
      virtual bool endRun(edm::Run&, edm::EventSetup const&);
      virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      InputTag inputTagSource_;
      string modelTag_;
      vector<string> modelParameters_;
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
ModelFilter::ModelFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   inputTagSource_  = iConfig.getParameter<InputTag>("source");
   modelTag_ = iConfig.getParameter<string>("modelTag");
   modelParameters_ = iConfig.getParameter<vector<string> >("modelParameters");
}


ModelFilter::~ModelFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
ModelFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   Handle<LHEEventProduct> product;
   iEvent.getByLabel(inputTagSource_, product);
   comments_const_iterator comment;

   size_t cursor;
   string tempString;
   vector<string> parameters;

   for(comment = product->comments_begin(); comment != product->comments_end();
       comment++)
   {
      if(comment->find(modelTag_) != string::npos)
      {
         tempString = comment->substr(comment->find(modelTag_),
                                      comment->size());
         tempString = tempString.substr(0, tempString.find(" "));
         parameters = split(tempString, "_");

         if(parameters.size() - 1 != modelParameters_.size())
         {
            cout<<"Error: number of modeParameters does not match number of parameters in file"<<endl;
            return false;
         }
         else
         {
            for(unsigned i = 0; i < modelParameters_.size(); i++)
            {
               if(atof(modelParameters_[i].c_str()) != 
                  atof(parameters[i +1].c_str()))
                  return false;
            }
            return true;
         }

      }
   }
   
   return false;

}

// ------------ method called once each job just before starting event loop  ------------
void 
ModelFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ModelFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool 
ModelFilter::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
bool 
ModelFilter::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
ModelFilter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
ModelFilter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ModelFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
vector<string>
ModelFilter::split(string fstring, string splitter)
{
   vector<string> returnVector;
   size_t cursor;
   string beforeSplitter;
   string afterSplitter = fstring;
   if(fstring.find(splitter) == string::npos)
   {
      cout<<"No "<<splitter<<" found"<<endl;
      returnVector.push_back(fstring);      
      return returnVector;
   }
   else
   {
      while(afterSplitter.find(splitter) != string::npos)
      {
         cursor = afterSplitter.find(splitter);

         beforeSplitter = afterSplitter.substr(0, cursor);
         afterSplitter = afterSplitter.substr(cursor +1, afterSplitter.size());
         
         returnVector.push_back(beforeSplitter);
         
         if(afterSplitter.find(splitter) == string::npos)
            returnVector.push_back(afterSplitter);
      }
      return returnVector;
   }
}
//define this as a plug-in
DEFINE_FWK_MODULE(ModelFilter);
