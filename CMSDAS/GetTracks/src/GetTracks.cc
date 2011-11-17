// -*- C++ -*-
//
// Package:    GetTracks
// Class:      GetTracks
// 
/**\class GetTracks GetTracks.cc UserCode/GetTracks/src/GetTracks.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christopher Silkworth
//         Created:  Thu Nov 17 09:11:34 CST 2011
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
//
// class declaration
//

   using namespace edm;
   using namespace reco;
   using namespace std;


class GetTracks : public edm::EDAnalyzer {
   public:
      explicit GetTracks(const edm::ParameterSet&);
      ~GetTracks();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      vector<Track> getTracks(const PFCandidatePtr& fpfCandidate);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      edm::InputTag PFJetInputTag_;
      

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
GetTracks::GetTracks(const edm::ParameterSet& iConfig)

{
   PFJetInputTag_ = iConfig.getParameter<InputTag>("PFJets");

}


GetTracks::~GetTracks()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
GetTracks::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;

   Handle<PFJetCollection> pfjets;
   iEvent.getByLabel(PFJetInputTag_, pfjets);
   PFJetCollection::const_iterator pfjet;

   vector<PFCandidatePtr> pfcands;


   //create a vector of a vector of tracks to store all the tracks associated
   //with each individual jet.
   vector<vector<Track> > pfjetTracks;
   vector<Track> tempTracks;

   //cycle through all the pfjets
   for(pfjet = pfjets->begin(); pfjet != pfjets->end(); pfjet++)
   {
      tempTracks.clear();
      pfcands = pfjet->getPFConstituents();
     
      //cycle through each pfcandidate associated with that jet
      for(unsigned i = 0; i < pfcands.size(); i++)
      {
         //if it's a charged particle and there are tracks
         if(abs(pfcands[i]->particleId()) < 4 &&
            getTracks(pfcands[i]).size() > 0) 
         {
            tempTracks.push_back(getTracks(pfcands[i])[0]);
         }

      }
      //Add all the tracks from the pfcandidates to the pfjetTracks.
      pfjetTracks.push_back(tempTracks); 
   }

   if(pfjetTracks.size() != 0)
   {
      if(pfjetTracks[0].size() > 1)
      {
         //Example of how to access the tracks: this spits out the first 
         //track of the zeroeth jet.
         cout<<pfjetTracks[0][1].pt()<<endl; 
      }
   }
}


// ------------ method called once each job just before starting event loop  ------------
void 
GetTracks::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GetTracks::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
GetTracks::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
GetTracks::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
GetTracks::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
GetTracks::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GetTracks::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//Returns a vector of tracks associated with that PFCandidate
vector<Track> GetTracks::getTracks(const PFCandidatePtr& fpfCandidate)
{
  vector<Track> ftracks;
  OwnVector<PFBlockElement>  fBlockElements;
  OwnVector<PFBlockElement>::const_iterator fBlockElement;
  
  
  PFBlock fBlock = *fpfCandidate->elementsInBlocks()[0].first;
  fBlockElements = fBlock.elements();
 
  //Cycle through the elements and if an element is a track, store it in a 
  //vector
  for(fBlockElement = fBlockElements.begin();
      fBlockElement != fBlockElements.end(); fBlockElement++)
    {
      if(fBlockElement->type() == 1) 
	{
	  ftracks.push_back(*fBlockElement->trackRef());
	}
      
    }

  return ftracks;

}

//define this as a plug-in
DEFINE_FWK_MODULE(GetTracks);
