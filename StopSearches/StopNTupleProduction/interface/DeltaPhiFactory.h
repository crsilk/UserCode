#ifndef CMGTools_StopNTupleProduction_h
#define CMGTools_StopNTupleProduction_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/StopNTupleProduction/interface/DeltaPhiCMG.h"


namespace cmg 
{
  class DeltaPhiFactory : public Factory<cmg::DeltaPhi>
  {
  public:
	DeltaPhiFactory(const edm::ParameterSet& ps):
	  deltaPhiLabel_(ps.getParameter<edm::InputTag>("inputCollection"))
	  {}
	  virtual event_ptr create(const edm::Event&, const edm::EventSetup&) const;
  private:
	  const edm::InputTag deltaPhiLabel_;
  };
}


#endif /*CMGTools_StopNTupleProduction_h*/
