#include "DataFormats/Common/interface/Wrapper.h"

#include "CMGTools/StopNTupleProduction/interface/DeltaPhi.h"
#include "CMGTools/StopNTupleProduction/interface/DeltaPhiPAT.h"
#include "CMGTools/StopNTupleProduction/interface/DeltaPhiCMG.h"

#include <vector>

namespace {
  struct CMGTools_StopNTupleProduction {
	
	//Reco
	reco::DeltaPhi r_dp;
	std::vector<reco::DeltaPhi> r_dp_v;
	edm::Wrapper<reco::DeltaPhi> r_dp_w;
	edm::Wrapper<std::vector<reco::DeltaPhi> > r_dp_v_w;

	//PAT
	pat::DeltaPhi p_dp;
	std::vector<pat::DeltaPhi> p_dp_v;
	edm::Wrapper<pat::DeltaPhi> p_dp_w;
	edm::Wrapper<std::vector<pat::DeltaPhi> > p_dp_v_w;
	
	//CMG
	cmg::DeltaPhi c_dp;
	std::vector<cmg::DeltaPhi> c_dp_v;
	edm::Wrapper<cmg::DeltaPhi> c_dp_w;
	edm::Wrapper<std::vector<cmg::DeltaPhi> > c_dp_v_w;
	

  };
  

}
