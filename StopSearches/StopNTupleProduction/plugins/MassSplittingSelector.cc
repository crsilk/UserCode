#include "CMGTools/StopNTupleProduction/plugins/MassSplittingSelector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

#include <memory>
#include <sstream>
#include <stdlib.h>

using namespace std;
using namespace edm;
using namespace cmg;

MassSplittingSelector::MassSplittingSelector(const edm::ParameterSet& iConfig)
{
   inputTagSource_  = iConfig.getParameter<InputTag>("source");
   modelTag_ = iConfig.getParameter<string>("modelTag");
   minSplitting_ = iConfig.getParameter<double>("minSplitting");
   maxSplitting_ = iConfig.getParameter<double>("maxSplitting");
}


MassSplittingSelector::~MassSplittingSelector()
{
}

bool MassSplittingSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   Handle<LHEEventProduct> product;
   iEvent.getByLabel(inputTagSource_, product);
   comments_const_iterator comment;

   string tempString;
   vector<string> parameters;
   double splitting;

   for(comment = product->comments_begin(); comment != product->comments_end();
       comment++)
   {
      if(comment->find(modelTag_) != string::npos)
      {
         tempString = comment->substr(comment->find(modelTag_),
                                      comment->size());
         tempString = tempString.substr(0, tempString.find(" "));
         parameters = split(tempString, "_");
         
         splitting = atof(parameters[1].c_str()) - 
            atof(parameters[2].c_str());
         if(minSplitting_ > splitting || maxSplitting_ < splitting)
         {
            cout<<"FAILED: "<<*comment<<endl;
            return false;
         }
      }
      cout<<"PASSED: "<<*comment<<endl;
      return true;
   }
   
   cout<<"FAILED: "<<*comment<<endl; 
   return false;
   
}
void MassSplittingSelector::beginJob()
{
}

void MassSplittingSelector::endJob() {
}

bool MassSplittingSelector::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

bool MassSplittingSelector::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

bool MassSplittingSelector::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

bool MassSplittingSelector::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

void MassSplittingSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
vector<string> MassSplittingSelector::split(string fstring, string splitter)
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
