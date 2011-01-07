#include "RecoParticleFlow/Configuration/test/ToyPF.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"


using namespace std;
using namespace edm;
using namespace reco;

ToyPF::ToyPF(const edm::ParameterSet& iConfig) {
  


  inputTagPFCandidates_ 
    = iConfig.getParameter<InputTag>("PFCandidates");

  verbose_ = 
    iConfig.getUntrackedParameter<bool>("verbose",false);


  LogDebug("ToyPF")
    <<" input collection : "<<inputTagPFCandidates_ ;
   
}


ToyPF::~ToyPF() { }



void 
ToyPF::beginRun(const edm::Run& run, 
			      const edm::EventSetup & es) { }


void 
ToyPF::analyze(const Event& iEvent, 
			     const EventSetup& iSetup) {
  
  LogDebug("ToyPF")<<"START event: "<<iEvent.id().event()
			 <<" in run "<<iEvent.id().run()<<endl;
  
  
  Handle<PFCandidateCollection> pfCandidates;
  iEvent.getByLabel(inputTagPFCandidates_, pfCandidates);
  PFCandidateCollection::const_iterator pfCandidate;

  vector<Track> tracks;
  //vector<PFRecTrack> pfTracks;
  
  vector<PFCluster> ecalClusters;
  vector<PFCluster> hcalClusters;

  vector<Track> tracksTemp;
  //vector<PFRecTrack> pfTracksTemp;
  vector<PFCluster> ecalClustersTemp;
  vector<PFCluster> hcalClustersTemp;
  
  for( pfCandidate = pfCandidates->begin();
       pfCandidate != pfCandidates->end(); pfCandidate++)
    {
      tracksTemp = getTracks(pfCandidate);
      //      pfTracksTemp = getPFRecTracks(pfCandidate);
      ecalClustersTemp = getEcalClusters(pfCandidate);
      hcalClustersTemp = getHcalClusters(pfCandidate);
      
			     
      for(unsigned i = 0; i < tracksTemp.size(); i++)
	{
	  tracks.push_back(tracksTemp[i]);
	}
      /*  for(unsigned l = 0; l < tracksTemp.size(); l++)
	{
	  pfTracks.push_back(tracksTemp[l]);
	  }*/
      for(unsigned j = 0; j < ecalClustersTemp.size(); j++)
	{
	  ecalClusters.push_back(ecalClustersTemp[j]);
	}
      for(unsigned k = 0; k < hcalClustersTemp.size(); k++)
	{
	  hcalClusters.push_back(hcalClustersTemp[k]);
	}
    }

  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
  




  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////


}

vector<Track> ToyPF::getTracks(const PFCandidateCollection::const_iterator& fpfCandidate)
{
  
  OwnVector<PFBlockElement>  fBlockElements;
  OwnVector<PFBlockElement>::const_iterator fBlockElement;
  
  vector<Track> ftracks;

  for(unsigned i = 0; i < fpfCandidate->elementsInBlocks().size();i++)
	{
	  PFBlock fBlock = *fpfCandidate->elementsInBlocks()[i].first;
	  fBlockElements = fBlock.elements();
	  for(fBlockElement = fBlockElements.begin();
	      fBlockElement != fBlockElements.end(); fBlockElement++)
	    {
	      if(fBlockElement->type() == 1) 
		{
		  ftracks.push_back(*fBlockElement->trackRef());
		}
		
	    }
	}
  
  return ftracks;
}

//PFRecTracks are not necessarily in the datasets, so they are commented out

/*vector<PFRecTrack> ToyPF::getPFRecTracks(const PFCandidateCollection::const_iterator& fpfCandidate)
{

  OwnVector<PFBlockElement>  fBlockElements;
  OwnVector<PFBlockElement>::const_iterator fBlockElement;

  vector<PFRecTrack> fpfTracks;

  for(unsigned i = 0; i < fpfCandidate->elementsInBlocks().size();i++)
	{
	  PFBlock fBlock = *fpfCandidate->elementsInBlocks()[i].first;
	  fBlockElements = fBlock.elements();

	  for(fBlockElement = fBlockElements.begin();
	      fBlockElement != fBlockElements.end(); fBlockElement++)
	    {
	      if(fBlockElement->type() == 1) 
		{
		  fpfTracks.push_back(*fBlockElement->trackRefPF());
		}		
	    }
	}
  
  return fpfTracks;
  }*/

vector<PFCluster> ToyPF::getEcalClusters(const PFCandidateCollection::const_iterator& fpfCandidate)
{
  
  OwnVector<PFBlockElement>  fBlockElements;
  OwnVector<PFBlockElement>::const_iterator fBlockElement;

  vector<PFCluster> fecalClusters;

  for(unsigned i = 0; i < fpfCandidate->elementsInBlocks().size();i++)
	{
	  PFBlock fBlock = *fpfCandidate->elementsInBlocks()[i].first;
	  fBlockElements = fBlock.elements();
	  for(fBlockElement = fBlockElements.begin();
	      fBlockElement != fBlockElements.end(); fBlockElement++)
	    {
	      if(fBlockElement->type() == 4) 
		{
		  fecalClusters.push_back(*fBlockElement->clusterRef());
		}
		
	    }
	}
  
  return fecalClusters;
}

vector<PFCluster> ToyPF::getHcalClusters(const PFCandidateCollection::const_iterator& fpfCandidate)
{
  
  OwnVector<PFBlockElement>  fBlockElements;
  OwnVector<PFBlockElement>::const_iterator fBlockElement;

  vector<PFCluster> fhcalClusters;

  for(unsigned i = 0; i < fpfCandidate->elementsInBlocks().size();i++)
	{
	  PFBlock fBlock = *fpfCandidate->elementsInBlocks()[i].first;
	  fBlockElements = fBlock.elements();
	  for(fBlockElement = fBlockElements.begin();
	      fBlockElement != fBlockElements.end(); fBlockElement++)
	    {
	      if(fBlockElement->type() == 5) 
		{
		  fhcalClusters.push_back(*fBlockElement->clusterRef());
		}
		
	    }
	}
  
  return fhcalClusters;
}

DEFINE_FWK_MODULE(ToyPF);
