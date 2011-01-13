#include "RecoParticleFlow/Configuration/test/ToyPF.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/Math/interface/deltaR.h"

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
ToyPF::produce(Event& iEvent, 
	       const EventSetup& iSetup) {
  
  LogDebug("ToyPF")<<"START event: "<<iEvent.id().event()
			 <<" in run "<<iEvent.id().run()<<endl;
  
  
  Handle<PFCandidateCollection> pfCandidates;
  iEvent.getByLabel(inputTagPFCandidates_, pfCandidates);
  PFCandidateCollection::const_iterator pfCandidate;

  vector<Track> tracks;
  vector<PFCluster> ecalClusters;
  vector<PFCluster> hcalClusters;

  vector<Track> tracksTemp;
  vector<PFCluster> ecalClustersTemp;
  vector<PFCluster> hcalClustersTemp;
  
  //Cycle through all the PFCandidates in the event and make vectors of tracks,
  //ecal and hcal clusters.
  for( pfCandidate = pfCandidates->begin();
       pfCandidate != pfCandidates->end(); pfCandidate++)
    {    
      if(pfCandidate->pt() > 2) //apply an energy cut.
      {
	if(getTracks(pfCandidate).size()) //makes sure its not empty
	  {
	    tracksTemp = getTracks(pfCandidate);
	    for(unsigned i = 0; i < tracksTemp.size(); i++)
	      {
		tracks.push_back(tracksTemp[i]); 
	      }
	  }
	
	if(getEcalClusters(pfCandidate).size()) //makes sure its not empty
	  {
	    ecalClustersTemp = getEcalClusters(pfCandidate);
	    for
	      (unsigned j = 0; j < ecalClustersTemp.size(); j++)
	      {
		ecalClusters.push_back(ecalClustersTemp[j]); 
	      }
	  }
	
	if(getHcalClusters(pfCandidate).size()) //makes sure its not empty
	  {
	    hcalClustersTemp = getHcalClusters(pfCandidate);
	    
	    for(unsigned k = 0; k < hcalClustersTemp.size(); k++)
	      {
		hcalClusters.push_back(hcalClustersTemp[k]);
	      }
	  }
      }
    }
  cout<<"                 "<<endl;
  cout<<tracks.size()<<endl;
  cout<<ecalClusters.size()<<endl;
  cout<<hcalClusters.size()<<endl;
  cout<<"                "<<endl;
   
  //////////////////////////////////////////////////////////////
  ///begin students' main coding////////////////////////////////
  vector<vector<vector<int> > > links;
  
  //fills the links array taking into account all the scenarios, i.e. if the 
  //tracks, ecal or hcal vectors are empty (Note: if two are empty there will 
  //be no links!).
  if(tracks.size() != 0 && ecalClusters.size() != 0 && 
     hcalClusters.size() != 0)
    {

      links = link(tracks, ecalClusters, hcalClusters);
      
      for( unsigned i = 0; i < tracks.size(); i++)
	{
	  for (unsigned j = 0; j < ecalClusters.size(); j++)
	    {
	      for(unsigned k = 0; k < hcalClusters.size(); k++)
		{
		  if(links[i][j][k] != -1)
		    cout<<links[i][j][k]<<" ";
		  
		}
	      
	    }
	}
      
    }
  else if(tracks.size() != 0 && ecalClusters.size() != 0 && 
	  hcalClusters.size() == 0)
    {
      links = link(tracks, ecalClusters, 'e');
      
      for( unsigned i = 0; i < tracks.size(); i++)
	{
	  for (unsigned j = 0; j < ecalClusters.size(); j++)
	    {
	      if(links[i][j][0] != -1)
		  cout<<links[i][j][0]<<" ";
		 
	    }
	}
    }
 
  else if(tracks.size() != 0  && ecalClusters.size()  == 0 && 
	  hcalClusters.size() != 0 )
    {
      links = link(tracks, hcalClusters, 'h');
      
      for( unsigned i = 0; i < tracks.size(); i++)
	{
	  for (unsigned j = 0; j < hcalClusters.size(); j++)
	    {
	      if(links[i][0][j] != -1)
		cout<<links[i][0][j]<<" ";     
	    }
	}
    }
  else if(tracks.size() == 0 && ecalClusters.size() != 0 && 
	  hcalClusters.size()!= 0 )
    {
      links = link(ecalClusters, hcalClusters);
      for (unsigned j = 0; j < ecalClusters.size(); j++)
	{
	  for(unsigned k = 0; k < hcalClusters.size(); k++)
	    {
	      if(links[0][j][k] != -1)
		cout<<links[0][j][k]<<" ";
	      
	    }  
	}
    }
  else cout<<"no links"<<endl;



  //////////////////////////////////////////////////////////////
  //end students' main coding///////////////////////////////////


}


//--------------------------------------------------------------
//begin students' fucntions-------------------------------------

//Tests if a track and an ecal cluster are linked
bool ToyPF::isLinked(const Track& ftrack, const PFCluster& fcluster)
{
  bool linked = false;
  //Test if the track and cluster are within a deltaR of .7 of each other. If 
  //they are then they are said to be linked. The measurement for the track is
  //made at the entrace to the ecal(deltaR = sqrt( deltaEta^2+deltaPhi^2))
  if(deltaR(ftrack.outerEta(), ftrack.outerPhi(), 
	    fcluster.eta(), fcluster.phi())< .7) linked = true;
  
  return linked;
}

//Tests if two PFClusters are linked
bool ToyPF::isLinked(const PFCluster& fcluster1, const PFCluster& fcluster2)
{
  bool linked = false;
  
  //Test if the clusters are within a deltaR of .7 of each other. If they are
  //then they are said to be linked. (deltaR = sqrt( deltaEta^2+deltaPhi^2))
  if(deltaR(fcluster1.eta(), fcluster1.phi(), 
	    fcluster2.eta(), fcluster2.phi()) < .7) linked = true;
  
  return linked;
}


//Links the tracks/cluster when all three vectors are non-zero
vector<vector<vector<int> > > ToyPF::link(const vector<Track>& ftracks,
					  const vector<PFCluster>& fcluster1,
					  const vector<PFCluster>& fcluster2)
{
  //Build the three dimensional array to store link info.
  vector<int> temp1d(fcluster2.size(), -1);
  vector<vector<int> > temp2d(fcluster1.size(), temp1d);
  vector<vector<vector<int> > > flinks(ftracks.size(), temp2d);

  //Bool vectors that control the "deletion" of unwanted link info
  vector<bool> ftrackBool(ftracks.size(), true);
  vector<bool> fcluster1Bool(fcluster1.size(), true);
  vector<bool> fcluster2Bool(fcluster2.size(), true);

  //Build bool vector that "saves" the wanted link info
  vector<vector<int> > tempBool(fcluster1.size(), 
				vector<int>(fcluster2.size()));
  vector<vector<vector<int> > > fsave(ftracks.size(), tempBool);
  
  //Cycle through all the possible combination of tracks and clusters. Then
  //test which are linked to each other. The 8 possible senarios of links are
  //then given a number (0-7) and this is the "type" of link.
  for(unsigned i = 0; i < ftracks.size(); i++)
    {
      for(unsigned j = 0; j < fcluster1.size(); j++)
	{
	  for(unsigned k = 0; k < fcluster2.size(); k++)
	    {
	      if(isLinked(ftracks[i], fcluster1[j]) && 
		 isLinked(ftracks[i], fcluster2[k]) &&
		 isLinked(fcluster1[j], fcluster2[k]))
		{
		  flinks[i][j][k] = 0;
		}
	      if(!isLinked(ftracks[i], fcluster1[j]) && 
		 isLinked(ftracks[i], fcluster2[k]) &&
		 isLinked(fcluster1[j], fcluster2[k]))
		{
		 
		  flinks[i][j][k] = 1;
		}
	      if(isLinked(ftracks[i], fcluster1[j]) && 
		 !isLinked(ftracks[i], fcluster2[k]) &&
		 isLinked(fcluster1[j], fcluster2[k]))
		{
		 
		  flinks[i][j][k] = 2;
		}
	      if(isLinked(ftracks[i], fcluster1[j]) && 
		 isLinked(ftracks[i], fcluster2[k]) &&
		 !isLinked(fcluster1[j], fcluster2[k]))
		{
		 
		  flinks[i][j][k] = 3;
		}
	      if(!isLinked(ftracks[i], fcluster1[j]) && 
		 !isLinked(ftracks[i], fcluster2[k]) &&
		 isLinked(fcluster1[j], fcluster2[k]))
		{
		 
		  flinks[i][j][k] = 4;
		}
	      if(!isLinked(ftracks[i], fcluster1[j]) && 
		 isLinked(ftracks[i], fcluster2[k]) &&
		 !isLinked(fcluster1[j], fcluster2[k]))
		{
		  flinks[i][j][k] = 5;
		}
	      if(isLinked(ftracks[i], fcluster1[j]) && 
		 !isLinked(ftracks[i], fcluster2[k]) &&
		 !isLinked(fcluster1[j], fcluster2[k]))
		{
		  flinks[i][j][k] = 6;		  
		}
	      if(!isLinked(ftracks[i], fcluster1[j]) && 
		 !isLinked(ftracks[i], fcluster2[k]) &&
		 !isLinked(fcluster1[j], fcluster2[k]))
		{
		  flinks[i][j][k] = 7;
		}
	      
	    }
	}
    }

  //Cycle through the link array and keep the link "type" with the most number 
  //of links. Those are then "saved" and all other links that contain that 
  //track, ecal cluster or hcal cluster are "deleted", i.e. set to -1.
  for(int order = 0; order < 8; order++)
    {
      for(unsigned i = 0; i < ftracks.size(); i++)
	{
	  for(unsigned j = 0; j < fcluster1.size(); j++)
	    {
	      for(unsigned k = 0; k < fcluster2.size(); k++)
		{
		  if(ftrackBool[i] && fcluster1Bool[j] && fcluster2Bool[k])
		    {
		      if(flinks[i][j][k] == order)
			{
			  fsave[i][j][k] = 1;
			  ftrackBool[i] = false;
			  fcluster1Bool[j] = false;
			  fcluster2Bool[k] = false;
			}
		    }
		  else
		    {		      
		      if(!fsave[i][j][k]) flinks[i][j][k] = -1;
		    }
		}
	    }
	}
    }

  return flinks;
}

//Links the tracks/cluster when one of the clusters are zero. clusterType is 
//the non-zero collection of clusters. Either 'e' or ecal or 'h' for hcal.
vector<vector<vector<int> > > ToyPF::link(const vector<Track>& ftracks,
					  const vector<PFCluster>& fcluster1,
					  const char& clusterType)
{
  //Build the three dimensional array to store link info. However since one of
  //the dimensions is 1 its really a 2d array. flinks2 is for the case that its
  //an ecal cluster while flinks1 is if its an hcal cluster
  vector<vector<int> > temp2d1(1, vector<int>(fcluster1.size()));
  vector<vector<int> > temp2d2(fcluster1.size(), vector<int>(1));   
  vector<vector<vector<int> > > flinks1(ftracks.size(), temp2d1);
  vector<vector<vector<int> > > flinks2(ftracks.size(), temp2d2);
  vector<vector<vector<int> > > flinks;

  //Bool vectors that control the "deletion" of unwanted link info
  vector<bool> ftrackBool(ftracks.size(), true);
  vector<bool> fcluster1Bool(fcluster1.size(), true);

  //Build bool vector that "saves" the wanted link info  
  vector<vector<int> > fsave( ftracks.size(), vector<int>(fcluster1.size()));
  
  //Cycle through all the possible combination of tracks and clusters. Then
  //test which are linked to each other. There are only 2 possible scenarios
  //in this case (linked or unlinked), we use the same link "type" as above.
  for(unsigned i = 0; i < ftracks.size(); i++)
    {

      for(unsigned j = 0; j < fcluster1.size(); j++)
	{

	  if(clusterType == 'e')
	    {

	      if(isLinked(ftracks[i], fcluster1[j])) 
		{
		  flinks2[i][j][0] = 6;
		}
	      else 
		{
		  flinks2[i][j][0] = 7;
		}

	    }
	  else
	    {
	      if(isLinked(ftracks[i], fcluster1[j])) flinks1[i][0][j] = 5;
	      else flinks1[i][0][j] = 7;
	    }
	}
    }

 
  for(int order = 0; order < 8; order++)
    {
      for(unsigned i = 0; i < ftracks.size(); i++)
	{
	  for(unsigned j = 0; j < fcluster1.size(); j++)
	    {
	      if(clusterType == 'e')
		{
		  if(ftrackBool[i] && fcluster1Bool[j])  
		    { 
		      if(flinks2[i][j][0] == order)
			{
			  fsave[i][j] = 1;
			  ftrackBool[i] = false;
			  fcluster1Bool[j] = false;
			}
		    }
		  else
		    {
		      if(!fsave[i][j]) flinks2[i][j][0] = -1;
		    }
		}
	      else
		{
		  if(flinks1[i][0][j] == order)
		    {
		      fsave[i][j] = 1;
		      ftrackBool[i] = false;
		      fcluster1Bool[j] = false;
		    }
		  else
		    {
		      if(!fsave[i][j]) flinks1[i][0][j] = -1;
		    }
		  
		}
	    }
	}
    }

  if(clusterType == 'e') flinks = flinks2;
  else flinks = flinks1;

  return flinks;
}

//Links the two cluster collections when there are zero tracks.
vector<vector<vector<int> > > ToyPF::link(const vector<PFCluster>& fcluster1,
					  const vector<PFCluster>& fcluster2)
{
  //Build the three dimensional array to store link info.
  vector<vector<int> > temp2d(fcluster1.size(), vector<int>(fcluster2.size()));
  vector<vector<vector<int> > > flinks(1, temp2d);
  
  //Bool vectors that control the "deletion" of unwanted link info
  vector<bool> fcluster1Bool(fcluster1.size(), true);
  vector<bool> fcluster2Bool(fcluster2.size(), true);

 //Build bool vector that "saves" the wanted link info
  vector<vector<int> > fsave( fcluster1.size(), vector<int>(fcluster2.size()));

  //Cycle through all the possible combination of tracks and clusters. Then
  //test which are linked to each other. There are only 2 possible scenarios
  //in this case (linked or unlinked), we use the same link "type" as above.
  for(unsigned j = 0; j < fcluster1.size(); j++)
    {
      for(unsigned k = 0; k < fcluster2.size(); k++)
	{
	  if(isLinked(fcluster1[j], fcluster2[k])) flinks[0][j][k] = 4;
	  else flinks[0][j][k] = 7;
	}
    }

  //Cycle through the link array and keep the link "type" with the most number 
  //of links. Those are then "saved" and all other links that contain that 
  //track, ecal cluster or hcal cluster are "deleted", i.e. set to -1.
  for(int order = 0; order < 8; order++)
    {
      for(unsigned j = 0; j < fcluster1.size(); j++)
	{
	  for(unsigned k = 0; k < fcluster2.size(); k++)
	    {
	      if(fcluster1Bool[j] && fcluster2Bool[k])
		{
		  if(flinks[0][j][k] == order)
		    {
		      fsave[j][k] = 1;
		      fcluster1Bool[j] = false;
		      fcluster2Bool[k] = false;
		    }
		}
	      else
		{
		  if(!fsave[j][k]) flinks[0][j][k] = -1;
		}
	    }
	}
      
    }

  return flinks;
}


//--------------------------------------------------------------
//end students' functions---------------------------------------


//Returns a vector of tracks associated with that PFCandidate
vector<Track> ToyPF::getTracks(const PFCandidateCollection::const_iterator& fpfCandidate)
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

//Returns a vector of ecal clusters  associated with that PFCandidate
vector<PFCluster> ToyPF::getEcalClusters(const PFCandidateCollection::const_iterator& fpfCandidate)
{
  vector<PFCluster> fecalClusters;
  OwnVector<PFBlockElement>  fBlockElements;
  OwnVector<PFBlockElement>::const_iterator fBlockElement;
  
  //Get the PFBlock associated with the PFCandidate and then break the block up
  //into its individual elements.
  PFBlock fBlock = *fpfCandidate->elementsInBlocks()[0].first;
  fBlockElements = fBlock.elements();

  //Cycle through the elements and if an element is an ecal cluster, store it 
  //in a vector
  for(fBlockElement = fBlockElements.begin();
      fBlockElement != fBlockElements.end(); fBlockElement++)
    {
      if(fBlockElement->type() == 4) 
	{
	  fecalClusters.push_back(*fBlockElement->clusterRef());
	}
      
    }
  
  return fecalClusters;
}

//Returns a vector of hcal clusters associated with that PFCandidate
vector<PFCluster> ToyPF::getHcalClusters(const PFCandidateCollection::const_iterator& fpfCandidate)
{
  vector<PFCluster> fhcalClusters;
  OwnVector<PFBlockElement>  fBlockElements;
  OwnVector<PFBlockElement>::const_iterator fBlockElement;

  //Get the PFBlock associated with the PFCandidate and then break the block up
  //into its individual elements.
  PFBlock fBlock = *fpfCandidate->elementsInBlocks()[0].first;
  fBlockElements = fBlock.elements();

  //Cycle through the elements and if an element is an hcal cluster, store it 
  // in a vector
  for(fBlockElement = fBlockElements.begin();
      fBlockElement != fBlockElements.end(); fBlockElement++)
    {
      if(fBlockElement->type() == 5) 
	{
	  fhcalClusters.push_back(*fBlockElement->clusterRef());
	}
      
    }
	
  return fhcalClusters;
}


DEFINE_FWK_MODULE(ToyPF);
