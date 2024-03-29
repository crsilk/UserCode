#include "PFAnalyses/CommonTools/interface/FWLiteTreeAnalyzer.h"
#include "PFAnalyses/ChrisAnalysis/interface/ChrisAnalyzer.h"
#include "PFAnalyses/ChrisAnalysis/interface/ChrisHistograms.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "FWCore/Utilities/interface/Algorithms.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"

#include <iostream>
#include <map>

using namespace std;
using namespace reco;  

ChrisAnalyzer:: ChrisAnalyzer(const std::string & aName):FWLiteAnalyzer(aName){}



ChrisAnalyzer::~ChrisAnalyzer(){

  std::cout<<"ChrisAnalyzer::~ChrisAnalyzer()"<<std::endl;

  delete chrisHistos_;
  

}


void ChrisAnalyzer::initialize(const edm::ParameterSet& ps, 
			      TFileDirectory& aDir,
			      pat::strbitset *aSelections) {
  pt_= 0;   
  nTruePhotons_ = 0;
  
  verbose_ = ps.getParameter<bool>("verbose");
  pfCandidateLabel_ = ps.getParameter<edm::InputTag>("pfCandidateLabel");
  genParticleLabel_ = ps.getParameter<edm::InputTag>("genParticleLabel");
  photonLabel_ = ps.getParameter<edm::InputTag>("photonLabel");
  chrisHistos_ = new ChrisHistograms(&aDir,"Photons");
  
  registerCuts();

}



void ChrisAnalyzer::registerCuts(){

  ///It is possible to have many versions of the selection flow,
  ///eg. compare PF and classic approach. Here we use only PF
  ///The cuts reported in the CutCounter histogram will
  ///appear in the order set below.
  //std::string labels[1] = {""};
     
}



bool ChrisAnalyzer::analyze(const edm::EventBase& iEvent){

  if(verbose_) std::cout<<iEvent.id()<<std::endl;  

  int k =0;					
  int i = 0;
  double inBoxPt;

  vector<int> inBoxPhotons(2);
  vector<int> inBoxElectrons(2);
  vector<int> inBoxPions(2);
  vector<int> inBoxKL0s(2);
  vector<int> inBoxPFParticles(2);
  vector<int> inBoxRecoPhotons(2);
  vector<double> inBoxTotPhotonPt(2);
  vector<double> inBoxTotElectronPt(2);
  vector<double> inBoxTotPionPt(2);
  vector<double> inBoxTotKL0Pt(2);
  vector<double> inBoxTotPFParticlePt(2);
  vector<double> inBoxTotRecoPhotonPt(2);
 
  edm::Handle<PFCandidateCollection> pfHandle;
  iEvent.getByLabel( pfCandidateLabel_, pfHandle );
  PFCandidateCollection::const_iterator pfParticle;
    
  edm::Handle<GenParticleCollection> genHandle;
  iEvent.getByLabel(genParticleLabel_, genHandle);
  GenParticleCollection::const_iterator genParticle;
    
  edm::Handle<PhotonCollection> photonHandle;
  iEvent.getByLabel(photonLabel_,photonHandle);
  PhotonCollection::const_iterator recoPhoton;
  
  pt_ = genHandle->begin()->pt();
  
  //Cuts to determine if it's matched or not.
  double includeAllPtCut = 50;
  double delRCut = .7;  
  double delPCut = pt_/3; 
  double delEtaCut = .1;
  double delPhiCut = .4;
  
  for(genParticle = genHandle->begin(); 
      genParticle != genHandle->end(); genParticle++)
    {
      inBoxPt = 0;
      //Sums up the total in cone/box pt.
      for(pfParticle = pfHandle->begin(); 
	  pfParticle != pfHandle->end(); pfParticle++)      
	{
	  if(isMatched(pfParticle, genParticle, delEtaCut, delPhiCut,
		       includeAllPtCut))
	    inBoxPt += pfParticle->pt();
	}

      for(pfParticle = pfHandle->begin(); 
	  pfParticle != pfHandle->end(); pfParticle++)
	{ 
	  //All PF particles
	  if(isMatched(pfParticle, genParticle, delEtaCut, delPhiCut,
		       includeAllPtCut))
	    {
	      inBoxPFParticles[k]++;
	      inBoxTotPFParticlePt[k] += pfParticle->pt();
	      chrisHistos_->fill2DHistogram("inBoxPFParticleDeltaAngle",
					    pfParticle->eta()-genParticle->eta(),
					    pfParticle->phi()-genParticle->phi());
	      
	    }
	  //Photons
	  if(pfParticle->pdgId() == 22)
	    {
	      if(isMatched(pfParticle, genParticle, delEtaCut, delPhiCut,
			   includeAllPtCut))
		{
		  inBoxPhotons[k]++;
		  inBoxTotPhotonPt[k] += pfParticle->pt();
		  if(inBoxPt > 38.0)
		    {
		      chrisHistos_->fill1DHistogram("photonOverMeasured",
						    pfParticle->pt());
		    }
	
		  if( inBoxPt < 38.0 && inBoxPt > 32.0)
		    {
		      chrisHistos_->fill1DHistogram("photonCorrectlyMeasured",
						    pfParticle->pt());
		    }
	
		  if(inBoxPt < 32.0)
		    {
		      chrisHistos_->fill1DHistogram("photonUnderMeasured",
						  pfParticle->pt());
		    }
		  
		  chrisHistos_->fill2DHistogram("inBoxPhotonDeltaAngle",
						pfParticle->eta()-genParticle->eta(),
						pfParticle->phi()-genParticle->phi());
		}       
	    }
	  //Electrons
	  if(abs(pfParticle->pdgId()) == 11)
	    {
	      if(isMatched(pfParticle, genParticle, delEtaCut,delPhiCut,
			   includeAllPtCut))
		{
		  inBoxElectrons[k]++;
		  inBoxTotElectronPt[k] += pfParticle->pt();
		  if(inBoxPt > 38.0)
		    {
		      chrisHistos_->fill1DHistogram("electronOverMeasured",
						    pfParticle->pt());
		    }
		  
		    if(inBoxPt > 32.0 && inBoxPt < 38.0)
		      {
			chrisHistos_->fill1DHistogram("electronCorrectlyMeasured",
						      pfParticle->pt());
		      }
		  
		    if(inBoxPt < 32.0)
		      {
			chrisHistos_->fill1DHistogram("electronUnderMeasured",
						      pfParticle->pt());
		      }
		    chrisHistos_->fill2DHistogram("inBoxElectronDeltaAngle",
						  pfParticle->eta()-genParticle->eta(),
						  pfParticle->phi()-genParticle->phi());
		}
	    }
	  //Pions
	  if(abs(pfParticle->pdgId()) == 211)
	    {
	      if(isMatched(pfParticle, genParticle, delEtaCut, delPhiCut,
			   includeAllPtCut))
		{
		  inBoxPions[k]++;
		  inBoxTotPionPt[k] += pfParticle->pt();
		  if(inBoxPt > 38.0)
		    {
		      chrisHistos_->fill1DHistogram("pionOverMeasured",
						    pfParticle->pt());
		    }

		  if(inBoxPt > 32.0 && inBoxPt < 38.0)
		    {
		      chrisHistos_->fill1DHistogram("pionCorrectlyMeasured",
						    pfParticle->pt());
		    }

		  if(inBoxPt < 32.0)
		    {
		      chrisHistos_->fill1DHistogram("pionUnderMeasured",
						    pfParticle->pt());
		    }
		  chrisHistos_->fill2DHistogram("inBoxPionDeltaAngle",
						pfParticle->eta()-genParticle->eta(),
						pfParticle->phi()-genParticle->phi());
		}
	    }
	  //KL0s
	  if(pfParticle->pdgId() == 130) 
	    {
	      if(isMatched(pfParticle, genParticle, delEtaCut, delPhiCut,
			   includeAllPtCut))
		{
		  inBoxKL0s[k]++;
		  inBoxTotKL0Pt[k] += pfParticle->pt();
		  if(inBoxPt > 38.0)
		    {
		      chrisHistos_->fill1DHistogram("KL0OverMeasured",
						    pfParticle->pt());
		    }
		  
		  if(inBoxPt > 32.0 && inBoxPt < 38.0)
		    {
		      chrisHistos_->fill1DHistogram("KL0CorrectlyMeasured",
						    pfParticle->pt());
		    }
		  
		  if(inBoxPt < 32.0)
		    {
		      chrisHistos_->fill1DHistogram("KL0UnderMeasured",
						    pfParticle->pt());
		    }
		  chrisHistos_->fill2DHistogram("inBoxKL0DeltaAngle",
					    pfParticle->eta()-genParticle->eta(),
					    pfParticle->phi()-genParticle->phi());
		}
	    }
	}
      //Go through the reco photons
      for(recoPhoton = photonHandle->begin(); 
	  recoPhoton != photonHandle->end(); recoPhoton++)
	{
	  if(isMatched(recoPhoton, genParticle, delEtaCut, delPhiCut,
		       includeAllPtCut))
	    {
	      inBoxRecoPhotons[k]++;
	      inBoxTotRecoPhotonPt[k] += recoPhoton->pt();
	    }
	}
      k++;
    }

  //fill the histograms
  for(genParticle = genHandle->begin(); 
      genParticle != genHandle->end(); genParticle++)
    {
      chrisHistos_->fill1DHistogram("inBoxPhotons", inBoxPhotons[i]);
      chrisHistos_->fill1DHistogram("inBoxElectrons", inBoxElectrons[i]);
      chrisHistos_->fill1DHistogram("inBoxPions", inBoxPions[i]);
      chrisHistos_->fill1DHistogram("inBoxKL0s", inBoxKL0s[i]);
      chrisHistos_->fill1DHistogram("inBoxPFParticles", inBoxPFParticles[i]);
      chrisHistos_->fill1DHistogram("inBoxRecoPhotons", inBoxRecoPhotons[i]);

      if(inBoxPhotons[i] != 0) 
	chrisHistos_->fill1DHistogram("inBoxTotPhotonPt", 
				      inBoxTotPhotonPt[i]);
      if(inBoxElectrons[i] != 0)
	chrisHistos_->fill1DHistogram("inBoxTotElectronPt", 
				      inBoxTotElectronPt[i]);
      if(inBoxPions[i] !=0)
	chrisHistos_->fill1DHistogram("inBoxTotPionPt", 
				      inBoxTotPionPt[i]);
      if(inBoxKL0s[i] !=0)
	chrisHistos_->fill1DHistogram("inBoxTotKL0Pt", 
				      inBoxTotKL0Pt[i]);

      if(inBoxPFParticles[i] !=0)
	chrisHistos_->fill1DHistogram("inBoxTotPFParticlePt", 
				      inBoxTotPFParticlePt[i]);
      if(inBoxRecoPhotons[i] != 0)
	chrisHistos_->fill1DHistogram("inBoxTotRecoPhotonPt", 
				      inBoxTotRecoPhotonPt[i]);

      i++;
    }


  //Counts the number of generated photons (since sometimes events fail).
  for(genParticle = genHandle->begin(); 
      genParticle != genHandle->end(); genParticle++)
    {
      nTruePhotons_++;
    }
  
  return true;
}



double ChrisAnalyzer::transverseMass( const reco::Candidate::LorentzVector& lepton, 
				      const reco::Candidate::LorentzVector& met) const {
  using namespace reco;


  Candidate::LorentzVector leptonT(lepton.Px(),lepton.Py(),0.,lepton.E()*sin(lepton.Theta()));
  Candidate::LorentzVector sumT=leptonT+met;
  return std::sqrt(sumT.M2());
}
 

//how to use automatic variables in the tree: 


void  ChrisAnalyzer::addBranch(TTree *tree){
  
  // add lorentz vectors or pat objects to the tree:
  // leading electron
  // MET 
}



void  ChrisAnalyzer::addCutHistos(TList *aList){

  

}



void ChrisAnalyzer::clear(){

  

}

bool ChrisAnalyzer::isMatched(const PFCandidateCollection::const_iterator&  pfcand, 
			      const GenParticleCollection::const_iterator& gencand, double& dr, double& dp)
{
  
  bool verbose =false;
  double gencandeta =gencand->eta();
  double gencandphi =gencand->phi();
  double pfcandeta =pfcand->eta();
  double pfcandphi =pfcand->phi();
  double pfcandpt = pfcand->pt();
  double gencandpt = gencand->pt();

  if(deltaR(pfcandeta ,pfcandphi ,gencandeta,gencandphi) < dr && abs(pfcandpt-gencandpt) < dp) verbose =true;
  
  return verbose;
  
}

bool ChrisAnalyzer::isMatched(const PhotonCollection::const_iterator& recopho,
			      const GenParticleCollection::const_iterator& gencand, 
			      double& dr, double& dp)
{
  bool verbose =false;  
  double gencandeta =gencand->eta();
  double gencandphi =gencand->phi();
  double recophoeta =recopho->eta();
  double recophophi =recopho->phi();
  double recophopt = recopho->pt();
  double gencandpt = gencand->pt();

  if(deltaR(recophoeta ,recophophi ,gencandeta,gencandphi) < dr &&
     abs(recophopt-gencandpt) < dp) verbose =true;
  
  return verbose;
  
}

bool ChrisAnalyzer::isMatched(const PFCandidateCollection::const_iterator& pfcand,
			      const GenParticleCollection::const_iterator& gencand, 
			      double& deta, double& dphi, double& dp)


{
  if(gencand->pdgId() != 22) return false;
  bool verbose =false;
  double gencandeta =gencand->eta();
  double gencandphi =gencand->phi();
  double gencandpt = gencand->pt();
  double pfcandeta =pfcand->eta();
  double pfcandphi =pfcand->phi();
  double pfcandpt = pfcand->pt();

  if(abs(gencandeta - pfcandeta) < deta && 
     abs(gencandphi - pfcandphi) < dphi &&
     abs(gencandpt - pfcandpt) < dp) verbose =true;
  return verbose;
}

bool ChrisAnalyzer::isMatched(const PhotonCollection::const_iterator& recopho,
			      const GenParticleCollection::const_iterator& gencand, 
			      double& deta, double& dphi, double& dp)


{
  if(gencand->pdgId() != 22) return false;
  bool verbose =false;
  double gencandeta =gencand->eta();
  double gencandphi =gencand->phi();
  double gencandpt = gencand->pt();
  double recophoeta =recopho->eta();
  double recophophi =recopho->phi();
  double recophopt = recopho->pt();

  if(abs(gencandeta - recophoeta) < deta && 
     abs(gencandphi - recophophi) < dphi &&
     abs(gencandpt - recophopt) < dp) verbose =true;
  return verbose;
}
