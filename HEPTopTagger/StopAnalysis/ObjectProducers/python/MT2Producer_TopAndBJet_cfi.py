import FWCore.ParameterSet.Config as cms

MT2TopAndBJet = cms.EDProducer(
	'MT2Producer',
	VisibleObject1Src = cms.InputTag("selectedTopBJetPair", "Top"),
    VisibleObject2Src = cms.InputTag("selectedTopBJetPair", "BJet"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),
	light = cms.bool(False)						   

)

MT2Top15AndBJet = cms.EDProducer(
	'MT2Producer',
	VisibleObject1Src = cms.InputTag("selectedTop15BJetPair", "Top"),
    VisibleObject2Src = cms.InputTag("selectedTop15BJetPair", "BJet"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),
	light = cms.bool(False)
	
)

MT2Top125AndBJet = cms.EDProducer(
	'MT2Producer',
	VisibleObject1Src = cms.InputTag("selectedTop125BJetPair", "Top"),
    VisibleObject2Src = cms.InputTag("selectedTop125BJetPair", "BJet"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),
	light = cms.bool(False)
)

MT2Top2AndBJet = cms.EDProducer(
	'MT2Producer',
	VisibleObject1Src = cms.InputTag("selectedTop2BJetPair", "Top"),
    VisibleObject2Src = cms.InputTag("selectedTop2BJetPair", "BJet"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),
	light = cms.bool(False)
)
