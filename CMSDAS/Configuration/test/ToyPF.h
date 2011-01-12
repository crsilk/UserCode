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

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"

/**\class ToyPF 
\brief produces IsolatedPFCandidates from PFCandidates

\author Colin Bernet
\date   february 2008
*/




class ToyPF : public edm::EDProducer {
 public:

  explicit ToyPF(const edm::ParameterSet&);

  ~ToyPF();
  
  virtual void produce( edm::Event&, const edm::EventSetup&);

  virtual void beginRun(const edm::Run & r, const edm::EventSetup & c);

  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////

  bool isLinked(const reco::Track& ftrack, const reco::PFCluster& fcluster);

  bool isLinked(const reco::PFCluster& ecal, const reco::PFCluster& hcal);

  std::vector<std::vector<std::vector<int> > > link(const std::vector<reco::Track>& ftracks, const std::vector<reco::PFCluster>& fcluster1, const std::vector<reco::PFCluster>& fcluster2);
  std::vector<std::vector<std::vector<int> > > link(const std::vector<reco::PFCluster>& fcluster1, const std::vector<reco::PFCluster>& fcluster2);
  std::vector<std::vector<std::vector<int> > > link(const std::vector<reco::Track>& ftracks, const std::vector<reco::PFCluster>& fcluster1, const char& clusterType);

  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////

  std::vector<reco::Track> getTracks(const reco::PFCandidateCollection::const_iterator& fpfCandidate);

  std::vector<reco::PFCluster> getEcalClusters(const reco::PFCandidateCollection::const_iterator& fpfCandidate);

  std::vector<reco::PFCluster> getHcalClusters(const reco::PFCandidateCollection::const_iterator& fpfCandidate);



 private:
  
  
  /// PFCandidates in which we'll look for pile up particles 
  edm::InputTag   inputTagPFCandidates_;
  
  /// verbose ?
  bool   verbose_;

  /// print the blocks associated to a given candidate ?
  bool   printBlocks_;

  /// rank the candidates by Pt
  bool rankByPt_;

  static bool greaterPt( const reco::PFCandidate& a, const reco::PFCandidate& b ) {
    return (a.pt()>b.pt());
  }



};

#endif
