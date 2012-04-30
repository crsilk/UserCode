#include "UserCode/ModelFilter/interface/ModelFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

#include <memory>
#include <sstream>
#include <stdlib.h>

using namespace std;
using namespace edm;

ModelFilter::ModelFilter(const edm::ParameterSet& iConfig)
{
   inputTagSource_  = iConfig.getParameter<InputTag>("source");
   modelTag_ = iConfig.getParameter<string>("modelTag");
   selectOnRange_ = iConfig.getParameter<bool>("selectOnRange");
   selectOnSplitting_ = iConfig.getParameter<bool>("selectOnSplitting");
   parameterMins_ = iConfig.getParameter<vector<double> >("parameterMins");
   parameterMaxs_ = iConfig.getParameter<vector<double> >("parameterMaxs");
   minSplitting_ = iConfig.getParameter<double>("minSplitting");
   maxSplitting_ = iConfig.getParameter<double>("maxSplitting");
}


ModelFilter::~ModelFilter()
{
}

bool ModelFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   Handle<LHEEventProduct> product;
   bool commentIsThere = iEvent.getByLabel(inputTagSource_, product);
   comments_const_iterator comment;

   string tempString;
   vector<string> tempStrings;
   vector<string> parameters;
   double splitting;

   for(comment = product->comments_begin(); comment != product->comments_end();
       comment++)
   {
      if(commentIsThere)
      {
         tempString = comment->substr(0,comment->size());
         tempStrings = split(tempString," ");
         parameters = split(tempStrings[2], "_");

         splitting = atof(parameters[1].c_str()) - 
            atof(parameters[2].c_str());

         if((parameters.size() - 1 != parameterMins_.size() ||
            parameterMins_.size() != parameterMaxs_.size()) &&
            selectOnRange_)
         {
            cout<<"Error: number of modelParameters does not match number of parameters in file"<<endl;
            return false;
         }
         else
         {
            if((minSplitting_ > splitting || maxSplitting_ < splitting) &&
               selectOnSplitting_)
               return false;
            
            for(unsigned i = 0; i < parameterMins_.size(); i++)
            {
               if((parameterMins_[i] > atof(parameters[i +1 ].c_str()) ||
                   parameterMaxs_[i] < atof(parameters[i + 1].c_str())) &&
                  selectOnRange_)
                  return false;
            }
         }
         return true;

      }
   }
   return false;

}
void ModelFilter::beginJob()
{
}

void ModelFilter::endJob() {
}

bool ModelFilter::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

bool ModelFilter::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

bool ModelFilter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

bool ModelFilter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

void ModelFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
vector<string> ModelFilter::split(string fstring, string splitter)
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
DEFINE_FWK_MODULE(ModelFilter);
