#include "CMGTools/StopNTupleProduction/plugins/DeltaPhiProducer.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "CMGTools/StopNTupleProduction/interface/DeltaPhi.h"

using namespace reco;
using namespace std;
using namespace edm;
using namespace math;

DeltaPhiProducer::DeltaPhiProducer(const edm::ParameterSet& iConfig)
{
  inputTagPFCandidates_ = iConfig.getParameter<InputTag>("PFCandidates");
  neutralVsCharged_ = iConfig.getParameter<bool>("neutralVsCharged");
}
DeltaPhiProducer::~DeltaPhiProducer()
{
}
void
DeltaPhiProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle<PFCandidateCollection> pfCandidates;
   iEvent.getByLabel(inputTagPFCandidates_, pfCandidates);
   PFCandidateCollection::const_iterator pfCandidate;
    
   math::XYZTLorentzVector sumCHVector(0, 0, 0, 0);
   math::XYZTLorentzVector sumNVector(0, 0, 0, 0);
   math::XYZTLorentzVector deltaVector;

   if(neutralVsCharged_)
   {
      for(pfCandidate = pfCandidates->begin(); 
          pfCandidate != pfCandidates->end();pfCandidate++)
      {     
         if(pfCandidate->particleId() == 1 || pfCandidate->particleId() == 2 ||
            pfCandidate->particleId() == 3)
            sumCHVector = sumCHVector + pfCandidate->p4();
         else
            sumNVector = sumNVector + pfCandidate->p4();
      }
      deltaVector = sumCHVector - sumNVector;
   }
   

   auto_ptr<DeltaPhi> pOut(new DeltaPhi(deltaVector));
   iEvent.put(pOut);


 
}

void 
DeltaPhiProducer::beginJob()
{
}

void 
DeltaPhiProducer::endJob() {
}

void 
DeltaPhiProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

void 
DeltaPhiProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

void 
DeltaPhiProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

void 
DeltaPhiProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

void
DeltaPhiProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(DeltaPhiProducer);
