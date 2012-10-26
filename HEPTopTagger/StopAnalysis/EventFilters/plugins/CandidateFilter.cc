

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"

using namespace edm;
using namespace reco;
using namespace std;

class CandidateFilter : public edm::EDFilter {
   public:
      explicit CandidateFilter(const edm::ParameterSet&);
      ~CandidateFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual bool beginRun(edm::Run&, edm::EventSetup const&);
      virtual bool endRun(edm::Run&, edm::EventSetup const&);
      virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      InputTag candidateSrc_;
      string cuts_;


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
CandidateFilter::CandidateFilter(const edm::ParameterSet& iConfig)
{
   candidateSrc_ = iConfig.getParameter<InputTag>("src");
   cuts_ = iConfig.getParameter<string>("cuts");

   //now do what ever initialization is needed

}


CandidateFilter::~CandidateFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
CandidateFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{



   Handle< View<Candidate> > candidates;
   iEvent.getByLabel( candidateSrc_, candidates );
   View<Candidate>::const_iterator candidate = candidates->begin();

   StringCutObjectSelector<Candidate> selector(cuts_);

   if(selector(*candidate))
      return true;

   return false;
}

// ------------ method called once each job just before starting event loop  ------------
void 
CandidateFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CandidateFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool 
CandidateFilter::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
bool 
CandidateFilter::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
CandidateFilter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
CandidateFilter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CandidateFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(CandidateFilter);
