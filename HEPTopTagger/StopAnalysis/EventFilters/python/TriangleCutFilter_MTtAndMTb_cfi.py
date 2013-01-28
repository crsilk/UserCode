import FWCore.ParameterSet.Config as cms


MTtBJetVetoAndMTb500 = cms.EDFilter(
	"TriangleCutFilter",
	src1 = cms.InputTag("MTMT2BJetVeto", "MTt"),
	src2 = cms.InputTag("MTMT2BJetVeto", "MTb"),
	src1Coeff = cms.double(1),
	src2Coeff = cms.double(2),
	triangleMinCut = cms.double(500),
	triangleMaxCut = cms.double(999999999)
	)

MTtNoBJetVetoAndMTb500 = cms.EDFilter(
	"TriangleCutFilter",
	src1 = cms.InputTag("MTMT2NoBJetVeto", "MTt"),
	src2 = cms.InputTag("MTMT2NoBJetVeto", "MTb"),
	src1Coeff = cms.double(1),
	src2Coeff = cms.double(2),
	triangleMinCut = cms.double(500),
	triangleMaxCut = cms.double(999999999)
	)

MTtBJetVetoAndMTb750 = cms.EDFilter(
	"TriangleCutFilter",
	src1 = cms.InputTag("MTMT2BJetVeto", "MTt"),
	src2 = cms.InputTag("MTMT2BJetVeto", "MTb"),
	src1Coeff = cms.double(1),
	src2Coeff = cms.double(2),
	triangleMinCut = cms.double(750),
	triangleMaxCut = cms.double(999999999)
	)

MTtNoBJetVetoAndMTb750 = cms.EDFilter(
	"TriangleCutFilter",
	src1 = cms.InputTag("MTMT2NoBJetVeto", "MTt"),
	src2 = cms.InputTag("MTMT2NoBJetVeto", "MTb"),
	src1Coeff = cms.double(1),
	src2Coeff = cms.double(2),
	triangleMinCut = cms.double(750),
	triangleMaxCut = cms.double(999999999)
	)

MTtBJetVetoAndMTb1000 = cms.EDFilter(
	"TriangleCutFilter",
	src1 = cms.InputTag("MTMT2BJetVeto", "MTt"),
	src2 = cms.InputTag("MTMT2BJetVeto", "MTb"),


	src1Coeff = cms.double(1),
	src2Coeff = cms.double(2),
	triangleMinCut = cms.double(1000),
	triangleMaxCut = cms.double(999999999)
	)

MTtNoBJetVetoAndMTb1000 = cms.EDFilter(
	"TriangleCutFilter",
	src1 = cms.InputTag("MTMT2NoBJetVeto", "MTt"),
	src2 = cms.InputTag("MTMT2NoBJetVeto", "MTb"),
	src1Coeff = cms.double(1),
	src2Coeff = cms.double(2),
	triangleMinCut = cms.double(1000),
	triangleMaxCut = cms.double(999999999)
	)
