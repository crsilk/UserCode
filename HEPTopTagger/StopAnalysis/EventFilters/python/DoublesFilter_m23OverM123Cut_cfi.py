import FWCore.ParameterSet.Config as cms

m23OverM123CutBJetVeto0p35 = cms.EDFilter(
    'DoublesFilter',
    src = cms.InputTag("tripletSelectorBJetVeto", "selectedM23OverM123"),
    minCuts = cms.vdouble(0.35),
    maxCuts = cms.vdouble()
)

m23OverM123CutNoBJetVeto0p35 = cms.EDFilter(
    'DoublesFilter',
    src = cms.InputTag("tripletSelectorNoBJetVeto", "selectedM23OverM123"),
    minCuts = cms.vdouble(0.35),
    maxCuts = cms.vdouble()
)



