#ifndef ToyPF_Configuration_ToyPFMET_
#define ToyPF_Configuration_ToyPFMET_

// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"

/**\class ToyPFMET 
\brief Computes the MET from a collection of PFCandidates. 

*/

class ToyPFMET : public edm::EDProducer {
 public:

  explicit ToyPFMET(const edm::ParameterSet&);

  ~ToyPFMET();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

  virtual void beginJob();

 private:
 
  /// Input PFCandidates
  edm::InputTag       inputTagPFCandidates_;
  double              minPt_;  //in case you want to apply a pt cut when computing the pfMet

};

#endif
