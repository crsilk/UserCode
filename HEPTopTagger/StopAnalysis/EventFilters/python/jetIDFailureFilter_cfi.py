import FWCore.ParameterSet.Config as cms

jetIDFailureFilter = cms.EDFilter(
    "JetIDFailureFilter",
    JetSource = cms.InputTag('stopJetsPFchsPt30'),
    MinJetPt      = cms.double(30.0),
    MaxJetEta     = cms.double(999.0),
    MaxNeutralHadFrac = cms.double(0.90),
    MaxNeutralEMFrac  = cms.double(0.95),
    debug         = cms.bool(False),
    taggingMode   = cms.bool(False),
)

