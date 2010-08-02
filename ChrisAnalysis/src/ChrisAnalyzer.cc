#include "PFAnalyses/CommonTools/interface/FWLiteTreeAnalyzer.h"
#include "PFAnalyses/ChrisAnalysis/interface/ChrisAnalyzer.h"
#include "PFAnalyses/ChrisAnalysis/interface/ChrisHistograms.h"
#include "PFAnalyses/ChrisAnalysis/interface/ElectronHistograms.h"
#include "PFAnalyses/ChrisAnalysis/interface/IsolationHistograms.h"

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
  nTruePhotons_ = 0.0;
  vector<double> tmp(32);
  for(int i = 0; i < 32; i++)
    {
      nInCutKL0s_.push_back(0);
      nOutCutKL0s_.push_back(0);
      nInCutEles_.push_back(0);
      nOutCutEles_.push_back(0);
      nInCutPions_.push_back(0);
      nOutCutPions_.push_back(0);
      inCutPt_.push_back(tmp);
      nInCutPhotons_.push_back(tmp);
      nOutCutPhotons_.push_back(tmp);
      totPure_.push_back(tmp);
    }
  
   

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
  
  //vector<double> delR;
  //vector<double> delP;
  
  //double nMatchedToGenPho1;
  //double nMatchedToGenPho2;
  int k =0;					
  int i = 0;
  vector<int> inConePhotons(2);
  vector<int> inConeElectrons(2);
  vector<int> inConePions(2);
  vector<int> inConeKL0s(2);
  vector<int> inConePFParticles(2);
  vector<double> inConeTotPhotonPt(2);
  vector<double> inConeTotElectronPt(2);
  vector<double> inConeTotPionPt(2);
  vector<double> inConeTotKL0Pt(2);
  vector<double> inConeTotPFParticlePt(2);
  
  //for(int i2 = 0; i2<16; i2++)
  // {
  //   delR.push_back(.05*(i2+1));
  //   delP.push_back(.75*(i2+1));
  //  }
 
  edm::Handle<PFCandidateCollection> pfHandle;
  iEvent.getByLabel( pfCandidateLabel_, pfHandle );
  PFCandidateCollection::const_iterator pfParticle;
    
  edm::Handle<GenParticleCollection> genHandle;
  iEvent.getByLabel(genParticleLabel_, genHandle);
  GenParticleCollection::const_iterator genParticle;
    
  edm::Handle<PhotonCollection> photonHandle;
  iEvent.getByLabel(photonLabel_,photonHandle);
  
  pt_ = genHandle->begin()->pt();
  
  //Cuts to determine if it's matched or not.
  double includeAllPtCut = 50;
  double delRCut = .7;  
  double delPCut = pt_/3; 

  
  for(genParticle = genHandle->begin(); genParticle != genHandle->end(); genParticle++)
    {
      for(pfParticle = pfHandle->begin(); pfParticle != pfHandle->end(); pfParticle++)
	{ 
	  //All PF particles
	  if(isMatched(pfParticle, genParticle, delRCut, includeAllPtCut))
	    {
	      inConePFParticles[k] ++;
	      inConeTotPFParticlePt[k] += pfParticle->pt();
	      chrisHistos_->fill1DHistogram("inConePFParticlePt", 
					    pfParticle->pt());
	      chrisHistos_->fill2DHistogram("inConePFParticleDeltaAngle",
					    pfParticle->eta()-genParticle->eta(),
					    pfParticle->phi()-genParticle->phi());
	      
	    }
	  //Photons
	  if(pfParticle->pdgId() == 22)
	    {
	      if(isMatched(pfParticle, genParticle, delRCut, delPCut))
		{
		  inConePhotons[k]++;
		  inConeTotPhotonPt[k] += pfParticle->pt();
		  chrisHistos_->fill1DHistogram("inConePhotonPt", 
					    pfParticle->pt());
		  chrisHistos_->fill2DHistogram("inConePhotonDeltaAngle",
						pfParticle->eta()-genParticle->eta(),
						pfParticle->phi()-genParticle->phi());
		}       
	    }
	  //Electrons
	  if(pfParticle->pdgId() == abs(11))
	    {
	      if(isMatched(pfParticle, genParticle, delRCut, includeAllPtCut))
		{
		  inConeElectrons[k]++;
		  inConeTotElectronPt[k] += pfParticle->pt();
		  chrisHistos_->fill1DHistogram("inConeElectronPt", 
					    pfParticle->pt());
		  chrisHistos_->fill2DHistogram("inConeElectronDeltaAngle",
						pfParticle->eta()-genParticle->eta(),
						pfParticle->phi()-genParticle->phi());
		}
	    }
	  //Pions
	  if(pfParticle->pdgId() == abs(211))
	    {
	      if(isMatched(pfParticle, genParticle, delRCut, includeAllPtCut))
		{
		  inConePions[k]++;
		  inConeTotPionPt[k] += pfParticle->pt();
		  chrisHistos_->fill1DHistogram("inConePionPt", 
						pfParticle->pt());
		  chrisHistos_->fill2DHistogram("inConePionDeltaAngle",
						pfParticle->eta()-genParticle->eta(),
						pfParticle->phi()-genParticle->phi());
		}
	    }
	  //KL0s
	  if(pfParticle->pdgId() == 130) 
	    {
	      if(isMatched(pfParticle, genParticle, delRCut, includeAllPtCut))
		{
		  inConeKL0s[k]++;
		  inConeTotKL0Pt[k] += pfParticle->pt();
		  chrisHistos_->fill1DHistogram("inConeKL0Pt", 
						pfParticle->pt());
		  chrisHistos_->fill2DHistogram("inConeKL0DeltaAngle",
					    pfParticle->eta()-genParticle->eta(),
					    pfParticle->phi()-genParticle->phi());
		}
	    }
	}
      k++;
    }

  //fill the histograms
  for(genParticle = genHandle->begin(); genParticle != genHandle->end(); genParticle++)
    {
      chrisHistos_->fill1DHistogram("inConePhotons", inConePhotons[i]);
      chrisHistos_->fill1DHistogram("inConeElectrons", inConeElectrons[i]);
      chrisHistos_->fill1DHistogram("inConePions", inConePions[i]);
      chrisHistos_->fill1DHistogram("inConeKL0s", inConeKL0s[i]);
      chrisHistos_->fill1DHistogram("inConePFParticles", inConePFParticles[i]);


      chrisHistos_->fill1DHistogram("inConeTotPhotonPt", inConeTotPhotonPt[i]);
      chrisHistos_->fill1DHistogram("inConeTotElectronPt", inConeTotElectronPt[i]);
      chrisHistos_->fill1DHistogram("inConeTotPionPt", inConeTotPionPt[i]);
      chrisHistos_->fill1DHistogram("inConeTotKL0Pt", inConeTotKL0Pt[i]);
      chrisHistos_->fill1DHistogram("inConeTotPFParticlePt", inConeTotPFParticlePt[i]);
      i++;
    }





  
  //Code for getting all the plots for efficiency, 'purity', delta R, delta
  //eta, delta phi and delta pt for True photon pt = 35.


  /*for(int i = 0; i < 16; i++)
    {
      for(int j = 0; j < 16; j++)
	{
	   nMatchedToGenPho1 = 0;
	   nMatchedToGenPho2 = 0;
	   for(pfParticle = pfHandle->begin(); pfParticle != pfHandle->end(); pfParticle++)
	    {
	      
	      DeltaR1 = deltaR(pfParticle->eta(),pfParticle->phi(),
			       genHandle->begin()->eta(),genHandle->begin()->phi());
	      
	      DeltaR2 = deltaR(pfParticle->eta(),pfParticle->phi(),
			       (genHandle->begin()+1)->eta(),(genHandle->begin()+1)->phi());
	      
	      if(DeltaR1 < DeltaR2) genParticle = genHandle->begin();
	      else genParticle = (genHandle->begin()+1);
	      
	      if(pfParticle->pdgId() == 22)
		{
		  if( i == 0 && j==0) 
		    {
		      chrisHistos_->fill1DHistogram("PhotonDeltaR", deltaR(pfParticle->eta(),pfParticle->phi(), genParticle->eta(), genParticle->phi()));
		      chrisHistos_->fill1DHistogram("PhotonDeltaPt", abs(pfParticle->pt() - genParticle->pt()));
		      chrisHistos_->fill1DHistogram("PhotonDeltaEta", abs(pfParticle->eta() - genParticle->eta()));
		      chrisHistos_->fill1DHistogram("PhotonDeltaPhi", abs(pfParticle->phi() - genParticle->phi()));
		    }
		  if(isMatched(pfParticle,genParticle, delR[i],delP[j]))
		    {
		      nInCutPhotons_[i][j]++;
		      if(genParticle == genHandle->begin()) nMatchedToGenPho1++;
		      else nMatchedToGenPho2++;
		    }
		}
	    }
	  if(nMatchedToGenPho1 != 0 && nMatchedToGenPho2 != 0) totPure_[i][j] += (1/nMatchedToGenPho1 + 1/nMatchedToGenPho2);
	}
    }
  
  */
  


  //counts the total number of true photons. I do this because its not quite 
  //always 2000 since some of generated events can fail
 
  for(genParticle = genHandle->begin(); genParticle != genHandle->end(); genParticle++)
    {
      nTruePhotons_++;
    }
      
  return true;
}


//FLORIAN 
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


  


