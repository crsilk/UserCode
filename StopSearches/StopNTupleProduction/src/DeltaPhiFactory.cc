#include "CMGTools/StopNTupleProduction/interface/DeltaPhiFactory.h"


using namespace cmg;

DeltaPhiFactory::event_ptr DeltaPhiFactory::create(const edm::Event& iEvent,
                           const edm::EventSetup&) const
{
  edm::Handle<pat::DeltaPhiCollection> cands;

  DeltaPhiFactory::event_ptr result(new DeltaPhiFactory::collection);
  iEvent.getByLabel(deltaPhiLabel_, cands);

  for(size_t i = 0, end = cands->size(); i != end; ++i) {

    pat::DeltaPhiPtr deltaPhiPtr(cands, i);
    DeltaPhi deltaPhi(deltaPhiPtr);

    result->push_back(deltaPhi);
  }
  return result;
}
