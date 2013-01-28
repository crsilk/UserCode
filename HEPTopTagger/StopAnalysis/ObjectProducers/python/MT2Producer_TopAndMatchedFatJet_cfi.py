import FWCore.ParameterSet.Config as cms

MT2TopAndMatchedFatJet = cms.EDProducer(
    'MT2Producer',
    VisibleObject1Src = cms.InputTag("selectedTopBJetPair", "Top"),
    VisibleObject2Src = cms.InputTag("selectedMatchedFatJet"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),
    light = cms.bool(False)						   

)

MT2Top15AndMatchedFatJet = cms.EDProducer(
    'MT2Producer',
    VisibleObject1Src = cms.InputTag("selectedTop15BJetPair", "Top"),
    VisibleObject2Src = cms.InputTag("selectedMatchedFatJet15"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),
    light = cms.bool(False)
    
)

MT2Top125AndMatchedFatJet = cms.EDProducer(
    'MT2Producer',
    VisibleObject1Src = cms.InputTag("selectedTop125BJetPair", "Top"),
    VisibleObject2Src = cms.InputTag("selectedMatchedFatJet125"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),
    light = cms.bool(False)
)

MT2Top2Pt120AndMatchedFatJet = cms.EDProducer(
    'MT2Producer',
    VisibleObject1Src = cms.InputTag("selectedTop2Pt120BJetPair", "Top"),
    VisibleObject2Src = cms.InputTag("selectedMatchedFatJet2Pt120"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),
    light = cms.bool(False)
)

MT2Top2Pt150AndMatchedFatJet = cms.EDProducer(
    'MT2Producer',
    VisibleObject1Src = cms.InputTag("selectedTop2Pt150BJetPair", "Top"),
    VisibleObject2Src = cms.InputTag("selectedMatchedFatJet2Pt150"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),
    light = cms.bool(False)
)
