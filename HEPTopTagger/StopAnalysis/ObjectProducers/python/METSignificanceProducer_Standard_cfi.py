import FWCore.ParameterSet.Config as cms

metSignificance = cms.EDProducer(
    'METSignificanceProducer',
    jetSrc = cms.InputTag("patJetsPF"),
    metSrc = cms.InputTag("patMETsPF"),
)
