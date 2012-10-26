import FWCore.ParameterSet.Config as cms

deltaPhiJetsAndMET = cms.EDProducer(
	'DeltaPhiObjectVsMETProducer',
    ObjectSrc = cms.InputTag("patJetsPF"),
    METSrc = cms.InputTag("patMETsPF"),
    nDeltaPhis = cms.uint32(3)
)
