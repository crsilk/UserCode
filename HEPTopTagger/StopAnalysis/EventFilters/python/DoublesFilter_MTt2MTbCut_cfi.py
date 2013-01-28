import FWCore.ParameterSet.Config as cms

MT2BJetVetoCut100 = cms.EDFilter(
    'DoublesFilter',
    src = cms.InputTag("MTMT2BJetVeto", "MT2"),
    minCuts = cms.vdouble(100),
    maxCuts = cms.vdouble()
)

MT2NoBJetVetoCut100 = cms.EDFilter(
    'DoublesFilter',
    src = cms.InputTag("MTMT2NoBJetVeto", "MT2"),
    minCuts = cms.vdouble(100),
    maxCuts = cms.vdouble()
)


MT2BJetVetoCut200 = cms.EDFilter(
    'DoublesFilter',
    src = cms.InputTag("MTMT2BJetVeto", "MT2"),
    minCuts = cms.vdouble(200),
    maxCuts = cms.vdouble()
)


MT2NoBJetVetoCut200 = cms.EDFilter(
    'DoublesFilter',
    src = cms.InputTag("MTMT2NoBJetVeto", "MT2"),
    minCuts = cms.vdouble(200),
    maxCuts = cms.vdouble()
)


MT2BJetVetoCut300 = cms.EDFilter(
    'DoublesFilter',
    src = cms.InputTag("MTMT2BJetVeto", "MT2"),
    minCuts = cms.vdouble(300),
    maxCuts = cms.vdouble()
)

MT2NoBJetVetoCut300 = cms.EDFilter(
    'DoublesFilter',
    src = cms.InputTag("MTMT2NoBJetVeto", "MT2"),
    minCuts = cms.vdouble(300),
    maxCuts = cms.vdouble()
)

