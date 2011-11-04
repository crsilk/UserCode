#ifndef DELTAPHI_H_
#define DELTAPHI_H_

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/Math/interface/LorentzVectorFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include <vector>

namespace reco
{
  class DeltaPhi : public RecoCandidate
  {
  public:
	DeltaPhi();
	DeltaPhi(const math::XYZTLorentzVector deltaVector);
	DeltaPhi(const double deltaPhi, const double deltaPt);
	math::XYZTLorentzVector deltaVector();
	double deltaPhi();
	double deltaPt();

  private:
	virtual bool overlap(const Candidate &) const; //required by RecoCandidate
	math::XYZTLorentzVector deltaVector_;
	double deltaPhi_;
	double deltaPt_;
  };
}
#endif /*DELTAPHI_H_*/
