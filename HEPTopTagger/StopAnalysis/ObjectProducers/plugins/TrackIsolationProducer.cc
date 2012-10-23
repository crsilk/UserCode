// -*- C++ -*-
//
// Package:    IsolatedTrackProducers
// Class:      IsolatedTrackProducers
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "TMath.h"
#include "Math/VectorUtil.h"
#include "TLorentzVector.h"
#include "TTree.h"
//includes needed for this file

using namespace edm;
using namespace reco;
using namespace std;

class IsolatedTrackProducer : public edm::EDProducer {
   public:
      explicit IsolatedTrackProducer(const edm::ParameterSet&);
      ~IsolatedTrackProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      double dR_;
      double dzcut_;
      double minPt_;
      
      double isoCut_;

      edm::InputTag pfCandidatesTag_;
      edm::InputTag vertexInputTag_;
      
      int vtxSize;
      
      string labelName_;


};


typedef reco::Candidate::LorentzVector LorentzVector;

IsolatedTrackProducer::IsolatedTrackProducer(const edm::ParameterSet& iConfig)
{
  pfCandidatesTag_ = iConfig.getParameter<InputTag>	("pfCandidatesTag");
  vertexInputTag_  = iConfig.getParameter<InputTag>        ("vertexInputTag");
  
  dR_ = iConfig.getParameter<double>("dR_ConeSize");       // dR value used to define the isolation cone                (default 0.3 )
  dzcut_ = iConfig.getParameter<double> ("dz_CutValue");       // cut value for dz(trk,vtx) for track to include in iso sum (default 0.05)
  minPt_ = iConfig.getParameter<double>("minPt_PFCandidate"); // store PFCandidates with pt above this cut                 (default 0   )
  isoCut_ = iConfig.getParameter<double>("isoCut"); // isolation cut value
  labelName_ = iConfig.getParameter<string>("labelName"); 
  
  produces<std::vector<reco::PFCandidate> >(labelName_); 

  
}


IsolatedTrackProducer::~IsolatedTrackProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
IsolatedTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{


  //---------------------------------
  // get PFCandidate collection
  //---------------------------------
  
  edm::Handle<PFCandidateCollection> pfCandidatesHandle;
  iEvent.getByLabel(pfCandidatesTag_, pfCandidatesHandle);

  //---------------------------------
  // get Vertex Collection
  //---------------------------------

  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByLabel(vertexInputTag_, vertices);
  Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : Vertex::Point());

  vtxSize = vertices->size();
  
  //-------------------------------------------------------------------------------------------------
  // loop over PFCandidates and calculate the trackIsolation and dz w.r.t. 1st good PV for each one
  // for neutral PFCandidates, store trkiso = 999 and dzpv = 999
  //-------------------------------------------------------------------------------------------------

  std::auto_ptr<std::vector<reco::PFCandidate> > prod(new std::vector<reco::PFCandidate>());

  if( vertices->size() > 0) {

     for( PFCandidateCollection::const_iterator pf_it = pfCandidatesHandle->begin(); pf_it != pfCandidatesHandle->end(); pf_it++ ) {

        //-------------------------------------------------------------------------------------
        // only store PFCandidate values if pt > minPt
        //-------------------------------------------------------------------------------------

        if( pf_it->pt() < minPt_ ) continue;

        //-------------------------------------------------------------------------------------
        // store pt and charge of PFCandidate
        //-------------------------------------------------------------------------------------

        //-------------------------------------------------------------------------------------
        // if there's no good vertex in the event, we can't calculate anything so store 999999
        //-------------------------------------------------------------------------------------
    
        //-------------------------------------------------------
        // require PFCandidate is charged, otherwise store 999 
        //-------------------------------------------------------

        if( pf_it->charge() != 0 ){

           //----------------------------------------------------------------------------
           // now loop over other PFCandidates in the event to calculate trackIsolation
           //----------------------------------------------------------------------------

           float trkiso = 0.0;

           for( PFCandidateCollection::const_iterator pf_other = pfCandidatesHandle->begin(); pf_other != pfCandidatesHandle->end(); pf_other++ ) {

              // don't count the PFCandidate in its own isolation sum
              if( pf_it == pf_other       ) continue;

	      // require the PFCandidate to be charged
	      if( pf_other->charge() == 0 ) continue;

              // cut on dR between the PFCandidates
              float dR = deltaR(pf_it->eta(), pf_it->phi(), pf_other->eta(), pf_other->phi());
              if( dR > dR_ ) continue;

	      // cut on the PFCandidate dz
	      float dz_other = 100;

	      if ( pf_other->trackRef().isNonnull()) {
	         dz_other = pf_other->trackRef()->dz(vtxpos);
	      }

	      if( fabs(dz_other) > dzcut_ ) continue;

	      trkiso += pf_other->pt();
           }

           // calculate the dz of this candidate
           float dz_it = 100; //

           if ( pf_it->trackRef().isNonnull()) {
              dz_it = pf_it->trackRef()->dz(vtxpos);
           }

           if( trkiso/pf_it->pt() > isoCut_ ) continue;
           if( std::abs(dz_it) > dzcut_ ) continue;

           prod->push_back( (*pf_it) );

        }

     }
      
  }


  iEvent.put(prod, labelName_);

}

// ------------ method called once each job just before starting event loop  ------------
void 
IsolatedTrackProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
IsolatedTrackProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
IsolatedTrackProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
IsolatedTrackProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
IsolatedTrackProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
IsolatedTrackProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
IsolatedTrackProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(IsolatedTrackProducer);
