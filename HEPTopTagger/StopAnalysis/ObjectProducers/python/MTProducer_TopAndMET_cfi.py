import FWCore.ParameterSet.Config as cms


MTTop = cms.EDProducer(
    'MTProducer', 
    Object1Src = cms.InputTag("selectedTopBJetPair", "Top"),
    Object2Src = cms.InputTag("patMETsPF"),
    light = cms.bool(False)
    
    )

MTTop15 = cms.EDProducer(
    'MTProducer', 
    Object1Src = cms.InputTag("selectedTop15BJetPair", "Top"),
    Object2Src = cms.InputTag("patMETsPF"),
    light = cms.bool(False)
    
    )

MTTop125 = cms.EDProducer(
    'MTProducer', 
    Object1Src = cms.InputTag("selectedTop125BJetPair", "Top"),
    Object2Src = cms.InputTag("patMETsPF"),
    light = cms.bool(False)

    )

MTTop2Pt120 = cms.EDProducer(
    'MTProducer', 
    Object1Src = cms.InputTag("selectedTop2Pt120BJetPair", "Top"),
    Object2Src = cms.InputTag("patMETsPF"),
    light = cms.bool(False)
    
    )

MTTop2Pt150 = cms.EDProducer(
    'MTProducer', 
    Object1Src = cms.InputTag("selectedTop2Pt150BJetPair", "Top"),
    Object2Src = cms.InputTag("patMETsPF"),
    light = cms.bool(False)
    
    )
