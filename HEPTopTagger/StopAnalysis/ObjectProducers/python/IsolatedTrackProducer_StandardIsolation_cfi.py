import FWCore.ParameterSet.Config as cms

isolatedTracks = cms.EDProducer(
	"IsolatedTrackProducer",
	pfCandidatesTag     = cms.InputTag("particleFlow"),
	vertexInputTag      = cms.InputTag("goodVertices"),
	dR_ConeSize         = cms.double(0.3),
	dz_CutValue         = cms.double(0.05),
	minPt_PFCandidate   = cms.double(15.0),
	isoCut              = cms.double(0.1),
	labelName = cms.string("isolatedTracks")
	)
