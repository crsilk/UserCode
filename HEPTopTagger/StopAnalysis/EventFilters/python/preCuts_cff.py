import FWCore.ParameterSet.Config as cms

METCut = cms.EDFilter(
	"PATCandViewCountFilter",
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(999999),
	cut = cms.string('pt > 175'),
    src = cms.InputTag("patMETsPF")
)

PFchsJetsPt30Cut = cms.EDFilter(
	"PATCandViewCountFilter",
    minNumber = cms.uint32(2),
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("PFchsJetsPt30", "PFchsJetsPt30")
)

PFchsJetsPt70eta2p5Cut = cms.EDFilter(
	"PATCandViewCountFilter",
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("PFchsJetsPt70eta2p5","PFchsJetsPt70eta2p5")
)

preCuts = cms.Sequence(
	METCut *
	PFchsJetsPt30Cut *
	PFchsJetsPt70eta2p5Cut
	)
