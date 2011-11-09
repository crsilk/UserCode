#include "CMGTools/StopNTupleProduction/plugins/DeltaPhiPATProducer.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "CMGTools/StopNTupleProduction/interface/DeltaPhiPAT.h"
#include "CMGTools/StopNTupleProduction/interface/DeltaPhi.h"

using namespace reco;
using namespace std;
using namespace edm;

DeltaPhiPATProducer::DeltaPhiPATProducer(const edm::ParameterSet& iConfig)
{
  inputTagDeltaPhi_ = iConfig.getParameter<InputTag>("DeltaPhiCollection");
  
  produces<pat::DeltaPhi>();
}
DeltaPhiPATProducer::~DeltaPhiPATProducer()
{
}
void
DeltaPhiPATProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle<DeltaPhiCollection> deltaPhis;
   iEvent.getByLabel(inputTagDeltaPhi_, deltaPhis);
   DeltaPhiCollection::const_iterator deltaPhi;
   
  if (deltaPhis->size() != 1) throw cms::Exception("Corrupt Data") << "The input delta phi collection " << inputTagDeltaPhi_.encode() << " has size " << deltaPhis->size() << " instead of 1 as it should.\n";

  deltaPhi = deltaPhis->begin();

  auto_ptr<pat::DeltaPhi> pOut(new pat::DeltaPhi(*deltaPhi));
  iEvent.put(pOut);


 
}

void 
DeltaPhiPATProducer::beginJob()
{
}

void 
DeltaPhiPATProducer::endJob() {
}

void 
DeltaPhiPATProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

void 
DeltaPhiPATProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

void 
DeltaPhiPATProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

void 
DeltaPhiPATProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

void
DeltaPhiPATProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(DeltaPhiPATProducer);
