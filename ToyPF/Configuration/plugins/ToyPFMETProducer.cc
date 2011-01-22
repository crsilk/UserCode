#include "ToyPF/Configuration/plugins/ToyPFMETProducer.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace math;

ToyPFMET::ToyPFMET(const edm::ParameterSet& iConfig)  {
  
  inputTagPFCandidates_ 
    = iConfig.getParameter<InputTag>("PFCandidatesForMET");
 
  minPt_
    = iConfig.getUntrackedParameter<double>("minPt", 0.);

  produces<METCollection>();
  
}

ToyPFMET::~ToyPFMET() { }

void ToyPFMET::beginJob() { }

void ToyPFMET::produce(Event& iEvent, 
			  const EventSetup& iSetup) {
   
  // get PFCandidates
  Handle<PFCandidateCollection> pfCandidates;
  iEvent.getByLabel( inputTagPFCandidates_, pfCandidates);
  PFCandidateCollection::const_iterator pfCandidate;

  ////////////////////////
  ////Part 3 begins here//
  ////////////////////////

  //compute the pfMet 
  for( pfCandidate = pfCandidates->begin();
       pfCandidate != pfCandidates->end(); pfCandidate++) //loop over 
    {                                                     //pfCandidates
 
    }

  double Pt = 0.;// to be filled with sum output of the loop
  XYZTLorentzVector missingPt(0.,0.,0.,0.);//to be filled with sum output of
                                           //the loop : METx, METy, METz, MET 
  auto_ptr< METCollection > pfMetColl( new METCollection() ); 

  XYZPoint vertex; //dummy vertex
  MET ToyPfMet(Pt, missingPt, vertex);
  pfMetColl->push_back(ToyPfMet);
  iEvent.put(pfMetColl);  // add the collection to the output

  ////////////////////////
  /////Part 3 ends here///
  ////////////////////////
}


DEFINE_FWK_MODULE(ToyPFMET);
