import FWCore.ParameterSet.Config as cms

MT2Cut = cms.EDFilter(
	'DoublesFilter',
    src = cms.InputTag("MT2TopAndMatchedFatJet"),
    minCuts = cms.vdouble(300.0),
    maxCuts = cms.vdouble()
)


MT2Cut125 = cms.EDFilter(
	'DoublesFilter',
    src = cms.InputTag("MT2Top125AndMatchedFatJet"),
    minCuts = cms.vdouble(300.0),
    maxCuts = cms.vdouble()
)


MT2Cut15 = cms.EDFilter(
	'DoublesFilter',
    src = cms.InputTag("MT2Top15AndMatchedFatJet"),
    minCuts = cms.vdouble(300.0),
    maxCuts = cms.vdouble()
)


MT2Cut2 = cms.EDFilter(
	'DoublesFilter',
    src = cms.InputTag("MT2Top2AndMatchedFatJet"),
    minCuts = cms.vdouble(300.0),
    maxCuts = cms.vdouble()
)
