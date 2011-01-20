#ifndef RecoParticleFlow_PFPatProducer_ToyPF_
#define RecoParticleFlow_PFPatProducer_ToyPF_

// system include files
#include <memory>
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"



/**\class ToyPF 
\brief produces IsolatedPFCandidates from PFCandidates

\author Richard Cavanaugh, Lucie Gauthier, Christopher Silkworth
\date   January 2011
*/



class ToyPF : public edm::EDProducer {
 public:

  explicit ToyPF(const edm::ParameterSet&);

  ~ToyPF();
  
  virtual void produce( edm::Event&, const edm::EventSetup&);

  virtual void beginRun(const edm::Run & r, const edm::EventSetup & c);

   std::vector<std::vector<std::vector<int> > > link(const std::vector<reco::Track>& ftracks, const std::vector<reco::PFCluster>& fcluster1, const std::vector<reco::PFCluster>& fcluster2);

  std::vector<std::vector<std::vector<int> > > link(const std::vector<reco::PFCluster>& fcluster1, const std::vector<reco::PFCluster>& fcluster2);
  
  std::vector<std::vector<std::vector<int> > > link(const std::vector<reco::Track>& ftracks, const std::vector<reco::PFCluster>& fcluster1, const char& clusterType);
  
  std::vector<std::vector<std::vector<int> > > link(const std::vector<reco::PFCluster>& fcluster1, const char& clusterType);
  
  std::vector<std::vector<std::vector<int> > > link(const std::vector<reco::Track>& ftracks);

  std::vector<reco::Track> getTracks(const reco::PFCandidateCollection::const_iterator& fpfCandidate);

  std::vector<reco::PFCluster> getEcalClusters(const reco::PFCandidateCollection::const_iterator& fpfCandidate);

  std::vector<reco::PFCluster> getHcalClusters(const reco::PFCandidateCollection::const_iterator& fpfCandidate);


  //////////////////////////////////////////////////////////
  ///////////////////////Part1//////////////////////////////
  //////////////////////////////////////////////////////////
 
  bool isLinked(const reco::Track& ftrack, const reco::PFCluster& fcluster, 
		const char& fclusterType);

  bool isLinked(const reco::PFCluster& ecal, const reco::PFCluster& hcal);

  //////////////////////////////////////////////////////////
  ///////////////////////Part2//////////////////////////////
  //////////////////////////////////////////////////////////

  reco::PFCandidateCollection makeParticles(const std::vector<reco::Track>& ftracks, const std::vector<reco::PFCluster>& fcluster1, const std::vector<reco::PFCluster>& fcluster2, std::vector<std::vector<std::vector<int> > > flinks);

  //////////////////////////////////////////////////////////

 private:
    
  /// PFCandidates from which we get the input tracks, ecal and hcal clusters (so as to have clean collections as input, full tracks, ecal and hcal clusters collections may suffer from noise 
  edm::InputTag   inputTagPFCandidates_;
  
  /// verbose -we won't really use it. Set to "True" it would make the "LogDebug" items in ToyPF.cc become talkative
  bool   verbose_;

};

#endif
