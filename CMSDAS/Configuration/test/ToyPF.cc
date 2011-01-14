#include "RecoParticleFlow/Configuration/test/ToyPF.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"


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

  produces<PFCandidateCollection>( "pfCand" ).setBranchAlias( "pfCand");
  
   
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
  cout<<"Hi Mom                "<<endl;
   
  //////////////////////////////////////////////////////////////
  ///begin students' main coding////////////////////////////////
  vector<vector<vector<int> > > links;
  //PFCandidateCollection PFCandsOut;
  
  //fills the links array taking into account all the scenarios, i.e. if the 
  //tracks, ecal or hcal vectors are empty (Note: if two are empty there will 
  //be no links!).
  
  links = link(tracks, ecalClusters, hcalClusters);

  // auto_ptr<PFCandidateCollection> pfCand(new PFCandidateCollection);
  //*pfCand = makeParticles(tracks, ecalClusters, hcalClusters, links);

  //iEvent.put( pfCand );

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
  vector<int> temp1d(fcluster2.size() + 1, -1);
  vector<vector<int> > temp2d(fcluster1.size() + 1, temp1d);
  vector<vector<vector<int> > > flinks(ftracks.size() + 1, temp2d);

  //Bool vectors that control the "deletion" of unwanted link info
  vector<bool> ftrackBool(ftracks.size() + 1, true);
  vector<bool> fcluster1Bool(fcluster1.size() + 1, true);
  vector<bool> fcluster2Bool(fcluster2.size() + 1, true);

  //Build bool vector that "saves" the wanted link info
  vector<vector<int> > tempBool(fcluster1.size()+1, 
				vector<int>(fcluster2.size()+1));
  vector<vector<vector<int> > > fsave(ftracks.size()+1 , tempBool);
  //Cycle through all the possible combination of tracks and clusters. Then
  //test which are linked to each other. The 8 possible senarios of links are
  //then given a number (0-7) and this is the "type" of link.
  for(unsigned i = 0; i <= ftracks.size(); i++)
    {
      for(unsigned j = 0; j <= fcluster1.size(); j++)
	{
	  for(unsigned k = 0; k <= fcluster2.size(); k++)
	    {
	      if(i != ftracks.size() && j != fcluster1.size() && 
		 k != fcluster2.size()) 
		{
		  if(isLinked(ftracks[i], fcluster1[j]) && 
		     isLinked(ftracks[i], fcluster2[k]) &&
		     isLinked(fcluster1[j], fcluster2[k]))
		    {
		      flinks[i][j][k] = 0;
		    }
		  else if(!isLinked(ftracks[i], fcluster1[j]) && 
		     isLinked(ftracks[i], fcluster2[k]) &&
		     isLinked(fcluster1[j], fcluster2[k]))
		    {
		      flinks[i][j][k] = 1;
		    }
		  else if(isLinked(ftracks[i], fcluster1[j]) && 
		     !isLinked(ftracks[i], fcluster2[k]) &&
		     isLinked(fcluster1[j], fcluster2[k]))
		    {
		      flinks[i][j][k] = 2;
		    }
		  else if(isLinked(ftracks[i], fcluster1[j]) && 
		     isLinked(ftracks[i], fcluster2[k]) &&
		     !isLinked(fcluster1[j], fcluster2[k]))
		    {
		      flinks[i][j][k] = 3;
		    }
		  else
		    {
		      flinks[i][j][k] = -1;
		    }
		}
	      else if( i != ftracks.size() && j != fcluster1.size())
		{
		  if(isLinked(ftracks[i], fcluster1[j])) flinks[i][j][k] = 4;
		}
	      else if( i != ftracks.size() && k != fcluster2.size())
		{
		  if(isLinked(ftracks[i], fcluster2[k])) flinks[i][j][k] = 5;
		}
	      else if( j != fcluster1.size() && k != fcluster2.size())
		{
		  if(isLinked(fcluster1[j], fcluster2[k])) flinks[i][j][k]= 6;
		}
	      else if( i != ftracks.size())
		{
		  flinks[i][j][k] = 7;
		}
	      else if( j != fcluster1.size())
		{
		  flinks[i][j][k] = 8;
		}
	      else if( k != fcluster2.size())
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
	  for(unsigned j = 0; j <= fcluster1.size(); j++)
	    {
	      for(unsigned k = 0; k <= fcluster2.size(); k++)
		{
		  
		  if(ftrackBool[i] && fcluster1Bool[j] && fcluster2Bool[k])
		    {
		      if(flinks[i][j][k] == order)
			{
			 
			  fsave[i][j][k] = 1;
			  if(i != ftracks.size()) ftrackBool[i] = false;
			  if(j != fcluster1.size()) fcluster1Bool[j] = false;
			  if(k != fcluster2.size()) fcluster2Bool[k] = false;

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

PFCandidateCollection ToyPF::makeParticles(const vector<Track>& ftracks, 
					   const vector<PFCluster>& fcluster1,
					   const vector<PFCluster>& fcluster2,
					   vector<vector<vector<int> > > flinks)
{
  PFCandidateCollection fpfCandidates;
 
  double fE;
  double fpx;
  double fpy;
  double fpz;
  //  double ft;
  double fx;
  double fy;
  double fz;

  Candidate::LorentzVector fp4;

    for( unsigned i = 0; i <= ftracks.size(); i++)
    {
      for( unsigned j = 0; j <= fcluster1.size(); j++)
	{
	  for( unsigned k =0; k <= fcluster2.size(); k++)
	    {
	      if( flinks[i][j][k] != -1)
		{
		  if( flinks[i][j][k] == 0) 
		    {
		      cout<<"0";
		      fpx = ftracks[i].px();
		      fpy = ftracks[i].py();
		      fpz = ftracks[i].pz();
		      fE = sqrt(fpx*fpx + fpy*fpy + fpz*fpz);
		      fp4 = Candidate::LorentzVector(fpx, fpy, fpz, fE);

		      PFCandidate pfTemp;
		      pfTemp.setCharge(ftracks[i].charge());
		      pfTemp.setP4(fp4);
		      pfTemp.setPdgId(ftracks[i].charge()*211);
		      fpfCandidates.push_back(pfTemp);
		    }
		   if( flinks[i][j][k] == 1)
		    {
		      cout<<"1";
		      fpx = ftracks[i].px();
		      fpy = ftracks[i].py();
		      fpz = ftracks[i].pz();
		      fE = sqrt(fpx*fpx + fpy*fpy + fpz*fpz);
		      fp4 = Candidate::LorentzVector(fpx, fpy, fpz, fE);

		      PFCandidate pfTemp;
		      pfTemp.setCharge(ftracks[i].charge());
		      pfTemp.setP4(fp4);
		      pfTemp.setPdgId(ftracks[i].charge()*211);
		      fpfCandidates.push_back(pfTemp);
		    }
		  if( flinks[i][j][k] == 2)
		    {
		      cout<<"2";		      
		      fpx = ftracks[i].px();
		      fpy = ftracks[i].py();
		      fpz = ftracks[i].pz();
		      fE = sqrt(fpx*fpx + fpy*fpy + fpz*fpz);
		      fp4 = Candidate::LorentzVector(fpx, fpy, fpz, fE);

		      PFCandidate pfTemp;
		      pfTemp.setCharge(ftracks[i].charge());
		      pfTemp.setP4(fp4);
		      pfTemp.setPdgId(ftracks[i].charge()*211);
		      fpfCandidates.push_back(pfTemp);
		    }
		  if( flinks[i][j][k] == 3)
		    {
		      cout<<"3";
		      fpx = ftracks[i].px();
		      fpy = ftracks[i].py();
		      fpz = ftracks[i].pz();
		      fE = sqrt(fpx*fpx + fpy*fpy + fpz*fpz);
		      fp4 = Candidate::LorentzVector(fpx, fpy, fpz, fE);

		      PFCandidate pfTemp;
		      pfTemp.setCharge(ftracks[i].charge());
		      pfTemp.setP4(fp4);
		      pfTemp.setPdgId(ftracks[i].charge()*211);
		      fpfCandidates.push_back(pfTemp);
		    }
		  if( flinks[i][j][k] == 4)
		    {
		      cout<<"4";
		      fpx = ftracks[i].px();
		      fpy = ftracks[i].py();
		      fpz = ftracks[i].pz();
		      fE = sqrt(fpx*fpx + fpy*fpy + fpz*fpz);
		      fp4 = Candidate::LorentzVector(fpx, fpy, fpz, fE);

		      PFCandidate pfTemp;
		      pfTemp.setCharge(ftracks[i].charge());
		      pfTemp.setP4(fp4);
		      pfTemp.setPdgId(ftracks[i].charge()*211);
		      fpfCandidates.push_back(pfTemp);
		    }
		  if( flinks[i][j][k] == 5)
		    {
		      cout<<"5";
		      fpx = ftracks[i].px();
		      fpy = ftracks[i].py();
		      fpz = ftracks[i].pz();
		      fE = sqrt(fpx*fpx + fpy*fpy + fpz*fpz);
		      fp4 = Candidate::LorentzVector(fpx, fpy, fpz, fE);

		      PFCandidate pfTemp;
		      pfTemp.setCharge(ftracks[i].charge());
		      pfTemp.setP4(fp4);
		      pfTemp.setPdgId(ftracks[i].charge()*211);
		      fpfCandidates.push_back(pfTemp);	
		    }
		  if( flinks[i][j][k] == 6)
		    {
		      cout<<"6";
		      fx = fcluster1[j].x();
		      fy = fcluster1[j].y();
		      fz = fcluster1[j].z();
		      fE = fcluster1[j].energy() + fcluster2[k].energy();
		      fpx = fE*fx/sqrt(fx*fx +fy*fy +fz*fz); 
		      fpy = fE*fy/sqrt(fx*fx +fy*fy +fz*fz); 
		      fpz = fE*fz/sqrt(fx*fx +fy*fy +fz*fz); 
		      fp4 = Candidate::LorentzVector(fpx, fpy, fpz, fE);
		      if(fcluster1[j].energy() > fcluster2[k].energy())
			{
			  PFCandidate pfTemp;
			  pfTemp.setCharge(0);
			  pfTemp.setP4(fp4);
			  pfTemp.setPdgId(22);
			  fpfCandidates.push_back(pfTemp);
			}
		      else
			{
			  PFCandidate pfTemp;
			  pfTemp.setCharge(0);
			  pfTemp.setP4(fp4);
			  pfTemp.setPdgId(130);
			  fpfCandidates.push_back(pfTemp);
			}
		    }
		  if( flinks[i][j][k] == 7)
		    {
		      cout<<"7";
		      fpx = ftracks[i].px();
		      fpy = ftracks[i].py();
		      fpz = ftracks[i].pz();
		      fE = sqrt(fpx*fpx + fpy*fpy + fpz*fpz);
		      fp4 = Candidate::LorentzVector(fpx, fpy, fpz, fE);

		      PFCandidate pfTemp;
		      pfTemp.setCharge(ftracks[i].charge());
		      pfTemp.setP4(fp4);
		      pfTemp.setPdgId(ftracks[i].charge()*211);
		      fpfCandidates.push_back(pfTemp);
		    }
		  if( flinks[i][j][k] == 8)
		    {
		      cout<<"8";
		      fx = fcluster1[j].x();
		      fy = fcluster1[j].y();
		      fz = fcluster1[j].z();
		      fE = fcluster1[j].energy();
		      fpx = fE*fx/sqrt(fx*fx +fy*fy +fz*fz); 
		      fpy = fE*fy/sqrt(fx*fx +fy*fy +fz*fz); 
		      fpz = fE*fz/sqrt(fx*fx +fy*fy +fz*fz); 
		      fp4 = Candidate::LorentzVector(fpx, fpy, fpz, fE);

		      PFCandidate pfTemp;
		      pfTemp.setCharge(0);
		      pfTemp.setP4(fp4);
		      pfTemp.setPdgId(22);
		      fpfCandidates.push_back(pfTemp);		      
		    }
		  if( flinks[i][j][k] == 9)
		    {
		      cout<<"9";
		      fx = fcluster2[k].x();
		      fy = fcluster2[k].y();
		      fz = fcluster2[k].z();
		      fE = fcluster2[k].energy();
		      fpx = fE*fx/sqrt(fx*fx +fy*fy +fz*fz); 
		      fpy = fE*fy/sqrt(fx*fx +fy*fy +fz*fz);
		      fpz = fE*fz/sqrt(fx*fx +fy*fy +fz*fz); 
		      fp4 = Candidate::LorentzVector(fpx, fpy, fpz, fE);

		      PFCandidate pfTemp;
		      pfTemp.setCharge(0);
		      pfTemp.setP4(fp4);
		      pfTemp.setPdgId(130);
		      fpfCandidates.push_back(pfTemp);		      
		    }
		}
	    }
	}
    }

    return fpfCandidates;
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
