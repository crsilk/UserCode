import FWCore.ParameterSet.Config as cms

deltaPhiJetsAndMETCut = cms.EDFilter('DoublesFilter',
    src = cms.InputTag("deltaPhiJetsAndMET"),
    minCuts = cms.vdouble(0.5, 0.5, 0.3),
    maxCuts = cms.vdouble()
)
