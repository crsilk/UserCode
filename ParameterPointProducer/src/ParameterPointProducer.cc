// -*- C++ -*-
//
// Package:    ParameterPointProducer
// Class:      ParameterPointProducer
// 
/**\class ParameterPointProducer ParameterPointProducer.cc UserCode/ParameterPointProducer/src/ParameterPointProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christopher Silkworth
//         Created:  Mon Apr 30 07:38:45 CDT 2012
// $Id$
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
class ParameterPointProducer : public edm::EDProducer {
   public:
      explicit ParameterPointProducer(const edm::ParameterSet&);
      ~ParameterPointProducer();

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

ParameterPointProducer::ParameterPointProducer(const edm::ParameterSet& iConfig)
{
   inputTagSource_  = iConfig.getParameter<InputTag>("source");

   produces<std::vector<double> > ("modelParameters");
   produces<double> ("pythiaCrossSection");
}


ParameterPointProducer::~ParameterPointProducer()
{
 
}

vector<string> ParameterPointProducer::split(string fstring, string splitter)
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


void
ParameterPointProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   Handle<LHEEventProduct> product;
   bool commentIsThere = iEvent.getByLabel(inputTagSource_, product);
   std::vector<std::string>::const_iterator comment;

   string tempString;
   vector<string> tempStrings;
   vector<string> parameters;
   auto_ptr<vector<double> > modelParameters(new vector<double>());
   auto_ptr<double> crossSection(new double(0.0));
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
         istringstream os(tempStrings[4]);
         os >> tempDouble;
         *crossSection = tempDouble;

         for(unsigned i = 1; i < parameters.size(); i++)
         {
            modelParameters->push_back(atof(parameters[i].c_str()));
         }
         
      }
   }   


   iEvent.put(modelParameters, "modelParameters");
   iEvent.put(crossSection, "pythiaCrossSection");

}

void 
ParameterPointProducer::beginJob()
{
}

void 
ParameterPointProducer::endJob() {
}

void 
ParameterPointProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

void 
ParameterPointProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

void 
ParameterPointProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

void 
ParameterPointProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

void
ParameterPointProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(ParameterPointProducer);
