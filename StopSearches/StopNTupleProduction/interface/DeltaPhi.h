#ifndef DELTAPHI_H_
#define DELTAPHI_H_

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/Math/interface/LorentzVectorFwd.h"
#include <vector>

namespace reco
{
  class DeltaPhi;

  typedef std::vector<DeltaPhi> DeltaPhiCollection;

  class DeltaPhi : public RecoCandidate
  {
  public:
	DeltaPhi();
	DeltaPhi(const math::XYZTLorentzVector deltaVector);
	DeltaPhi(const double deltaPhi, const double deltaPt);
	DeltaPhi(const reco::DeltaPhi & deltaPhi);
	DeltaPhi(const edm::Ptr<reco::DeltaPhi> & deltaPhi);
	math::XYZTLorentzVector deltaVector() const;
	double deltaPhi() const;
	double deltaPt() const;

  private:
	virtual bool overlap(const Candidate &) const; //required by RecoCandidate
	math::XYZTLorentzVector deltaVector_;
	double deltaPhi_;
	double deltaPt_;
  };
}
#endif /*DELTAPHI_H_*/
