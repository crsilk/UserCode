import FWCore.ParameterSet.Config as cms

m123CutNoBJetVeto = cms.EDFilter(
    'DoublesFilter',
    src = cms.InputTag("tripletSelectorNoBJetVeto", "selectedM123"),
    minCuts = cms.vdouble(80),
    maxCuts = cms.vdouble(270)
)
m123CutBJetVeto = cms.EDFilter(
    'DoublesFilter',
    src = cms.InputTag("tripletSelectorBJetVeto", "selectedM123"),
    minCuts = cms.vdouble(80),
    maxCuts = cms.vdouble(270)
)
