#ifndef CMGTools_StopNTupleProduction_DeltaPhi_h
#define CMGTools_StopNTupleProduction_DeltaPhi_h

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "CMGTools/StopNTupleProduction/interface/DeltaPhiPAT.h"

#include <vector>

namespace cmg
{
  class DeltaPhi;

  class DeltaPhi : public PhysicsObjectWithPtr< pat::DeltaPhiPtr>
  {
  public:
	DeltaPhi() {}
	DeltaPhi(const value& m): 
	  PhysicsObjectWithPtr<value>::PhysicsObjectWithPtr(m)
	  {}
	  virtual ~DeltaPhi()
		{}

	  friend class DeltaPhiFactory;
  private:
  };
}
#endif /*CMGTools_StopNTupleProduction_DeltaPhi_h*/
