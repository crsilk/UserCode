#include "RecoParticleFlow/Configuration/test/ToyPF.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/PolarLorentzVector.h"


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

  //  produces<PFCandidateCollection>( "pfCand" ).setBranchAlias( "pfCand");
  produces<PFCandidateCollection>( );
  
   
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

  PFCandidateCollection pfCandidatesOut;
  PFCandidateCollection::const_iterator pfCandidateOut;

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
   
  vector<vector<vector<int> > > links;
  
  
  
  links = link(tracks, ecalClusters, hcalClusters);

  auto_ptr<PFCandidateCollection> pfCand(new PFCandidateCollection);
  *pfCand = makeParticles(tracks, ecalClusters, hcalClusters, links);
  
   
  iEvent.put( pfCand);
}



///////////////////////////
///////////////////////////
////Part 1 begin here//////
///////////////////////////
///////////////////////////

bool ToyPF::isLinked(const Track& ftrack, const PFCluster& fcluster, 
		     const char& fclusterType)
{
  bool linked = false;
  if(fclusterType == 'e') //Put linking condition for tracks to ecal here.
    {
    
    }
  if(fclusterType == 'h') //Put linking condition for tracks to hcal here.
    {
    
    }
  return linked;
}

bool ToyPF::isLinked(const PFCluster& fecal, const PFCluster& fhcal)
{
  bool linked = false;
  //Put linking condition for ecal to hcal here.
 
  
  return linked;
}

///////////////////////////
///////////////////////////
////Part 1 end here////////
///////////////////////////
///////////////////////////

//Links the tracks/cluster when all three vectors are non-zero
vector<vector<vector<int> > > ToyPF::link(const vector<Track>& ftracks,
					  const vector<PFCluster>& fecals,
					  const vector<PFCluster>& fhcals)
{
  //Build the three dimensional array to store link info.
  vector<int> temp1d(fhcals.size() + 1, -1);
  vector<vector<int> > temp2d(fecals.size() + 1, temp1d);
  vector<vector<vector<int> > > flinks(ftracks.size() + 1, temp2d);

  //Bool vectors that control the "deletion" of unwanted link info
  vector<bool> ftrackBool(ftracks.size() + 1, true);
  vector<bool> fecalsBool(fecals.size() + 1, true);
  vector<bool> fhcalsBool(fhcals.size() + 1, true);

  //Build bool vector that "saves" the wanted link info
  vector<vector<int> > tempBool(fecals.size()+1, 
				vector<int>(fhcals.size()+1));
  vector<vector<vector<int> > > fsave(ftracks.size()+1 , tempBool);
  //Cycle through all the possible combination of tracks and clusters. Then
  //test which are linked to each other. The 8 possible senarios of links are
  //then given a number (0-7) and this is the "type" of link.
  for(unsigned i = 0; i <= ftracks.size(); i++)
    {
      for(unsigned j = 0; j <= fecals.size(); j++)
	{
	  for(unsigned k = 0; k <= fhcals.size(); k++)
	    {
	      if(i != ftracks.size() && j != fecals.size() && 
		 k != fhcals.size()) 
		{
		  if(isLinked(ftracks[i], fecals[j], 'e') && 
		     isLinked(ftracks[i], fhcals[k], 'h') &&
		     isLinked(fecals[j], fhcals[k]))
		    {
		      flinks[i][j][k] = 0;
		    }
		  else if(!isLinked(ftracks[i], fecals[j], 'e') && 
		     isLinked(ftracks[i], fhcals[k], 'h') &&
		     isLinked(fecals[j], fhcals[k]))
		    {
		      flinks[i][j][k] = 1;
		    }
		  else if(isLinked(ftracks[i], fecals[j], 'e') && 
		     !isLinked(ftracks[i], fhcals[k], 'h') &&
		     isLinked(fecals[j], fhcals[k]))
		    {
		      flinks[i][j][k] = 2;
		    }
		  else if(isLinked(ftracks[i], fecals[j], 'e') && 
		     isLinked(ftracks[i], fhcals[k], 'h') &&
		     !isLinked(fecals[j], fhcals[k]))
		    {
		      flinks[i][j][k] = 3;
		    }
		  else
		    {
		      flinks[i][j][k] = -1;
		    }
		}
	      else if( i != ftracks.size() && j != fecals.size())
		{
		  if(isLinked(ftracks[i], fecals[j], 'e')) flinks[i][j][k] = 4;
		}
	      else if( i != ftracks.size() && k != fhcals.size())
		{
		  if(isLinked(ftracks[i], fhcals[k], 'h')) flinks[i][j][k] = 5;
		}
	      else if( j != fecals.size() && k != fhcals.size())
		{
		  if(isLinked(fecals[j], fhcals[k])) flinks[i][j][k]= 6;
		}
	      else if( i != ftracks.size())
		{
		  flinks[i][j][k] = 7;
		}
	      else if( j != fecals.size())
		{
		  flinks[i][j][k] = 8;
		}
	      else if( k != fhcals.size())
		{
		  flinks[i][j][k] = 9;
		}
	      else
		{
		  flinks[i][j][k] = -1;
		}
	      
	    }
	}
    }

  //Cycle through the link array and keep the link "type" with the most number
  //of links. Those are then "saved" and all other links that contain that 
  //track, ecal cluster or hcal cluster are "deleted", i.e. set to -1.
  for(int order = 0; order < 10; order++)
    {
      for(unsigned i = 0; i <= ftracks.size(); i++)
	{
	  for(unsigned j = 0; j <= fecals.size(); j++)
	    {
	      for(unsigned k = 0; k <= fhcals.size(); k++)
		{
		  
		  if(ftrackBool[i] && fecalsBool[j] && fhcalsBool[k])
		    {
		      if(flinks[i][j][k] == order)
			{
			 
			  fsave[i][j][k] = 1;
			  if(i != ftracks.size()) ftrackBool[i] = false;
			  if(j != fecals.size()) fecalsBool[j] = false;
			  if(k != fhcals.size()) fhcalsBool[k] = false;

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

///////////////////////////
///////////////////////////
////Part 2 begin here//////
///////////////////////////
///////////////////////////

PFCandidateCollection ToyPF::makeParticles(const vector<Track>& ftracks, 
					   const vector<PFCluster>& fecals,
					   const vector<PFCluster>& fhcals,
					   vector<vector<vector<int> > > flinks)
{
  PFCandidateCollection fpfCandidates;
  
  double fE;
  double fpx;
  double fpy;
  double fpz;
  double ft;
  double fx;
  double fy;
  double fz;
  


  Candidate::LorentzVector fp4;

    for( unsigned i = 0; i <= ftracks.size(); i++)
    {
      for( unsigned j = 0; j <= fecals.size(); j++)
	{
	  for( unsigned k =0; k <= fhcals.size(); k++)
	    {
	      if( flinks[i][j][k] != -1)
		{
		  if( flinks[i][j][k] == 0) //The trigger, ecal and hcals are
		    {                       //all linked to each other.

		      //PFCandidate pfTemp;
		      //fpfCandidates.push_back(pfTemp);  		  
		    }
		  if( flinks[i][j][k] == 1) //The hcals is linked to the trigger
		    {                       //and ecal but the trigger and ecal
		                            //aren't linked.
		      //PFCandidate pfTemp;
		      //fpfCandidates.push_back(pfTemp);  		  
		    }
		  if( flinks[i][j][k] == 2)//The ecal is linked to the trigger
		    {                       //and hcal but the trigger and hcal
		                            //aren't linked.
		      //PFCandidate pfTemp;
		      //fpfCandidates.push_back(pfTemp);  		 
		    }
		  if( flinks[i][j][k] == 3) //The trigger is linked to the ecal
		    {                       // and hcal but the hcal and ecal
		                            // aren't linked.
		      //PFCandidate pfTemp;
		      //fpfCandidates.push_back(pfTemp);  		 
		    }
		  if( flinks[i][j][k] == 4) //The trigger and ecal are linked
		    {

		      //PFCandidate pfTemp;
		      //fpfCandidates.push_back(pfTemp);  		
		    }
		  if( flinks[i][j][k] == 5) // The trigger and hcal are linked
		    {

		      //PFCandidate pfTemp;
		      //fpfCandidates.push_back(pfTemp);  
		    }
		  if( flinks[i][j][k] == 6) // The ecal and hcal are linked
		    {

		      //PFCandidate pfTemp;
		      //fpfCandidates.push_back(pfTemp);  
		    }
		  if( flinks[i][j][k] == 7) //A track by itself.
		    {

		      //PFCandidate pfTemp;
		      //fpfCandidates.push_back(pfTemp);  	
		    }
		  if( flinks[i][j][k] == 8) //An ecal by itself.
		    {

		      //PFCandidate pfTemp;
		      //fpfCandidates.push_back(pfTemp);		      
		    }
		  if( flinks[i][j][k] == 9) //An hcal by itself.
		    {

		      //PFCandidate pfTemp;
		      //fpfCandidates.push_back(pfTemp);  
		    }
		}
	    }
	}
    }

    return fpfCandidates;
}
				 
///////////////////////////
///////////////////////////
////Part 2 end here////////
///////////////////////////
///////////////////////////

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
