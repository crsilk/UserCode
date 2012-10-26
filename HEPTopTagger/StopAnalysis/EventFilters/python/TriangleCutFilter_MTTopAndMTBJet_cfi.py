import FWCore.ParameterSet.Config as cms


triangleCutMTTopAndMTBJet = cms.EDFilter(
	"TriangleCutFilter",
	src1 = cms.InputTag("MTTop"),
	src2 = cms.InputTag("MTBJet"),
	src1Coeff = cms.double(1),
	src2Coeff = cms.double(2),
	triangleMinCut = cms.double(1000),
	triangleMaxCut = cms.double(999999999)
	)

triangleCutMTTop125AndMTBJet = cms.EDFilter(
	"TriangleCutFilter",
	src1 = cms.InputTag("MTTop125"),
	src2 = cms.InputTag("MT125BJet"),
	src1Coeff = cms.double(1),
	src2Coeff = cms.double(2),
	triangleMinCut = cms.double(1000),
	triangleMaxCut = cms.double(999999999)
	)

triangleCutMTTop15AndMTBJet = cms.EDFilter(
	"TriangleCutFilter",
	src1 = cms.InputTag("MTTop15"),
	src2 = cms.InputTag("MT15BJet"),
	src1Coeff = cms.double(1),
	src2Coeff = cms.double(2),
	triangleMinCut = cms.double(1000),
	triangleMaxCut = cms.double(999999999)
	)

triangleCutMTTop2AndMTBJet = cms.EDFilter(
	"TriangleCutFilter",
	src1 = cms.InputTag("MTTop2"),
	src2 = cms.InputTag("MT2BJet"),
	src1Coeff = cms.double(1),
	src2Coeff = cms.double(2),
	triangleMinCut = cms.double(1000),
	triangleMaxCut = cms.double(999999999)
	)
