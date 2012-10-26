import FWCore.ParameterSet.Config as cms

METCut125 = cms.EDFilter(
	"CandidateFilter",
	cuts = cms.string("pt > 125"),
    src = cms.InputTag("patMETsPF")
)
METCut150 = cms.EDFilter(
	"CandidateFilter",
	cuts = cms.string("pt > 150"),
    src = cms.InputTag("patMETsPF")
)
METCut175 = cms.EDFilter(
	"CandidateFilter",
	cuts = cms.string("pt > 175"),
    src = cms.InputTag("patMETsPF")
)
METCut200 = cms.EDFilter(
	"CandidateFilter",
	cuts = cms.string("pt > 200"),
    src = cms.InputTag("patMETsPF")
)
METCut225 = cms.EDFilter(
	"CandidateFilter",
	cuts = cms.string("pt > 225"),
    src = cms.InputTag("patMETsPF")
)

PFchs3JetsPt30Cut = cms.EDFilter(
	"PATCandViewCountFilter",
    minNumber = cms.uint32(3),
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("PFchsJetsPt30",)
)
PFchs4JetsPt30Cut = cms.EDFilter(
	"PATCandViewCountFilter",
    minNumber = cms.uint32(4),
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("PFchsJetsPt30",)
)
PFchs5JetsPt30Cut = cms.EDFilter(
	"PATCandViewCountFilter",
    minNumber = cms.uint32(5),
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("PFchsJetsPt30",)
)
PFchs6JetsPt30Cut = cms.EDFilter(
	"PATCandViewCountFilter",
    minNumber = cms.uint32(6),
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("PFchsJetsPt30",)
)

PFchs2JetsPt70eta2p5Cut = cms.EDFilter(
	"PATCandViewCountFilter",
    minNumber = cms.uint32(2),
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("PFchsJetsPt70eta2p5")
)

preCuts = cms.Sequence(
	METCut175 *
	PFchs4JetsPt30Cut *
	PFchs2JetsPt70eta2p5Cut
	)
