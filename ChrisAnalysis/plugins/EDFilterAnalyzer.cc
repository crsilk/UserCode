// system include files
#include <memory>
#include <algorithm>
#include <sstream>

// user include files
#include "PFAnalyses/ChrisAnalysis/plugins/EDFilterAnalyzer.h"
#include "PFAnalyses/ChrisAnalysis/interface/ChrisAnalyzer.h"
#include "PFAnalyses/CommonTools/interface/FWLiteTreeAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
//
// constructors and destructor
//
EDFilterAnalyzer::EDFilterAnalyzer(const edm::ParameterSet& iConfig){
   //now do what ever initialization is needed


 std::vector<FWLiteAnalyzer*> myAnalyzers;

  myAnalyzers.push_back(new ChrisAnalyzer("ChrisAnalyzer")); 
  
  std::string cfgFileName = iConfig.getUntrackedParameter<std::string>("cfgFileName");

  fwLiteTreeAnalyzer_ =  new FWLiteTreeAnalyzer("TreeAnalyzer",cfgFileName);
  fwLiteTreeAnalyzer_->init(myAnalyzers);

}


EDFilterAnalyzer::~EDFilterAnalyzer(){
 
  fwLiteTreeAnalyzer_->finalize();
  delete fwLiteTreeAnalyzer_;

}


//
// member functions
//

// ------------ method called to for each event  ------------
bool
EDFilterAnalyzer::filter(edm::Event& iEvent, edm::EventSetup const& iSetup)
{
   using namespace edm;
   using namespace reco;

   fwLiteTreeAnalyzer_->analyze(iEvent);
   const pat::strbitset & mySelections = fwLiteTreeAnalyzer_->getSelections();   

   bool decision = true;
   for(unsigned i=0;i<mySelections.strings().size();++i){
     if(mySelections.strings()[i].find("PF")!=std::string::npos) 
       decision&=mySelections.test(mySelections.strings()[i]);
   }

   return decision;
}


// ------------ method called once each run just before starting event loop  ------------
void EDFilterAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){ }

// ------------ method called once each job just before starting event loop  ------------
void EDFilterAnalyzer::beginJob(){ }

// ------------ method called once each job just after ending the event loop  ------------
void EDFilterAnalyzer::endJob() { }

//define this as a plug-in
DEFINE_FWK_MODULE(EDFilterAnalyzer);
