#ifndef CMGTools_StopNTupleProduction_DeltaPhiPAT_h
#define CMGTools_StopNTupleProduction_DeltaPhiPAT_h

#include "DataFormats/PatCandidates/interface/PATObject.h"
#include "CMGTools/StopNTupleProduction/interface/DeltaPhi.h"
#include "DataFormats/Math/interface/LorentzVectorFwd.h"
namespace pat
{
  class DeltaPhi;
  typedef std::vector<DeltaPhi> DeltaPhiCollection;
  typedef edm::Ref<DeltaPhiCollection> DeltaPhiRef;
  typedef edm::RefVector<DeltaPhiCollection> DeltaPhiRefVector;
  typedef DeltaPhiRef DeltaPhiPtr;


  class DeltaPhi: public PATObject<reco::DeltaPhi>
  {
  public:
	DeltaPhi();
	DeltaPhi(const reco::DeltaPhi & deltaPhi);
	DeltaPhi(const edm::Ptr<reco::DeltaPhi> & deltaPhi);
	~DeltaPhi();
	math::XYZTLorentzVector deltaVector() const;
	double deltaPhi() const;
	double deltaPt() const;

  private:
	math::XYZTLorentzVector deltaVector_;
	double deltaPhi_;
	double deltaPt_;

  };

}



#endif /*CMGTools_StopNTupleProduction_DeltaPhiPAT_h*/
