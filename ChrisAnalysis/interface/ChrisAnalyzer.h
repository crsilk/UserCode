#ifndef ChrisAnalyzer_H
#define ChrisAnalyzer_H

// -*- C++ -*-
//
//
// Original Author:  Artur Kalinowski
//         Created:  Wed Jul 22 14:16:44 CEST 2009
// $Id: ChrisAnalyzer.h,v 1.4 2010/04/21 15:28:23 cbern Exp $
//
//

#include "PFAnalyses/CommonTools/interface/FWLiteAnalyzer.h"
#include "PFAnalyses/CommonTools/interface/CandidateSelector.h"
#include "PFAnalyses/CommonTools/interface/PatJetSelector.h"
#include "PFAnalyses/CommonTools/interface/PatLeptonSelector.h"
#include "PFAnalyses/CommonTools/interface/PatElectronSelector.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PFParticle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/Common/interface/Handle.h"


#include "FWCore/Utilities/interface/InputTag.h"




class ChrisHistograms;
class ElectronHistograms;

class ChrisAnalyzer:public FWLiteAnalyzer{

 public:


  ChrisAnalyzer(const std::string & aName);

  virtual ~ChrisAnalyzer();
  virtual void initialize(const edm::ParameterSet&, 
			  TFileDirectory&,
			  pat::strbitset *aSelections);
  virtual bool analyze(const edm::EventBase& iEvent);
  virtual void addBranch(TTree *tree);
  virtual void addCutHistos(TList *aList);

  double transverseMass( const reco::Candidate::LorentzVector& lepton, 
			 const reco::Candidate::LorentzVector& met) const;

  bool isMatched(const reco::PFCandidateCollection::const_iterator&  pfcand,
		 const reco::GenParticleCollection::const_iterator& gencand, double& dr, double& dp);
  double getPhotonEfficiency();


 private:
  
  double pt_;    
  void registerCuts();
  void clear();

  ChrisHistograms *chrisHistos_;
  ElectronHistograms *eleHistos_;
  
  edm::InputTag pfCandidateLabel_; 
  edm::InputTag genParticleLabel_;
  edm::InputTag photonLabel_;

  bool verbose_;
  

   
};

#endif
