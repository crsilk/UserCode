import FWCore.ParameterSet.Config as cms


toyPFProducer = cms.EDProducer("ToyPF",
    PFCandidates = cms.InputTag("particleFlow"),
    verbose = cms.untracked.bool(False),
)



