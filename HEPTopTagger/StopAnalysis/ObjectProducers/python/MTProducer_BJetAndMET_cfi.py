import FWCore.ParameterSet.Config as cms


MTBJet = cms.EDProducer('MTProducer', 
    Object1Src = cms.InputTag("selectedTopBJetPair", "BJet"),
    Object2Src = cms.InputTag("patMETsPF", ""),
    labelName = cms.string("MTBJet")

)

MT125BJet = cms.EDProducer('MTProducer', 
    Object1Src = cms.InputTag("selectedTop125BJetPair", "BJet"),
    Object2Src = cms.InputTag("patMETsPF", ""),
    labelName = cms.string("MTBJet")

)

MT15BJet = cms.EDProducer('MTProducer', 
    Object1Src = cms.InputTag("selectedTop15BJetPair", "BJet"),
    Object2Src = cms.InputTag("patMETsPF", ""),
    labelName = cms.string("MTBJet")

)

MT2BJet = cms.EDProducer('MTProducer', 
    Object1Src = cms.InputTag("selectedTop2BJetPair", "BJet"),
    Object2Src = cms.InputTag("patMETsPF", ""),
    labelName = cms.string("MTBJet")

)
