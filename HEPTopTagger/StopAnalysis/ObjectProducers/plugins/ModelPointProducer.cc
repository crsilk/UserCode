// -*- C++ -*-
//
// Package:    ModelPointProducer
// Class:      ModelPointProducer
// 
/**\class ModelPointProducer ModelPointProducer.cc UserCode/ModelPointProducer/src/ModelPointProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christopher Silkworth
//         Created:  Mon Apr 30 07:38:45 CDT 2012
// $Id: ModelPointProducer.cc,v 1.2 2012/08/29 15:33:52 joshmt Exp $
//
//


// system include files
#include <memory>
#include <sstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

using namespace std;
using namespace edm;
class ModelPointProducer : public edm::EDProducer {
   public:
      explicit ModelPointProducer(const edm::ParameterSet&);
      ~ModelPointProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      std::vector<std::string> split(std::string fstring, 
                                     std::string splitter); 

      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

   edm::InputTag inputTagSource_;

};

ModelPointProducer::ModelPointProducer(const edm::ParameterSet& iConfig)
{
   inputTagSource_  = iConfig.getParameter<InputTag>("source");

   produces<std::vector<double> > ("modelParameters");
}


ModelPointProducer::~ModelPointProducer()
{
 
}

vector<string> ModelPointProducer::split(string fstring, string splitter)
{
   vector<string> returnVector;
   size_t cursor;
   string beforeSplitter;
   string afterSplitter = fstring;
   if(fstring.find(splitter) == string::npos)
   {
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


void
ModelPointProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   Handle<LHEEventProduct> product;
   bool commentIsThere = iEvent.getByLabel(inputTagSource_, product);
   std::vector<std::string>::const_iterator comment;

   string tempString;
   vector<string> tempStrings;
   vector<string> parameters;
   auto_ptr<vector<double> > modelParameters(new vector<double>());
   double tempDouble;
   iEvent.getByLabel(inputTagSource_, product);
   for(comment = product->comments_begin(); 
       comment != product->comments_end(); comment++)
   {
      if(commentIsThere)
      {
         tempString = comment->substr(0, comment->size());
         tempStrings = split(tempString, " ");

         parameters = split(tempStrings[2], "_");
	 if (tempStrings.size() >3) { 
//prevent crash when string does not include cross-section
	   istringstream os(tempStrings[4]);
	   os >> tempDouble;
	 }
	 else tempDouble = 0;

         for(unsigned i = 1; i < parameters.size(); i++)
         {
            modelParameters->push_back(atof(parameters[i].c_str()));
         }
      }
   }   


   iEvent.put(modelParameters, "modelParameters");

}

void 
ModelPointProducer::beginJob()
{
}

void 
ModelPointProducer::endJob() {
}

void 
ModelPointProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

void 
ModelPointProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

void 
ModelPointProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

void 
ModelPointProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

void
ModelPointProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(ModelPointProducer);
