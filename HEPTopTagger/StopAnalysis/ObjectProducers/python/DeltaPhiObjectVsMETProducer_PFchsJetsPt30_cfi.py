import FWCore.ParameterSet.Config as cms

deltaPhiJetsAndMET = cms.EDProducer(
    'DeltaPhiObjectVsMETProducer',
    ObjectSrc = cms.InputTag("stopJetsPFchsPt30"),
    METSrc = cms.InputTag("patMETsPF"),
    nDeltaPhis = cms.uint32(3)
)
