import FWCore.ParameterSet.Config as cms


MTTop = cms.EDProducer('MTProducer', 
    Object1Src = cms.InputTag("selectedTopBJetPair", "Top"),
    Object2Src = cms.InputTag("patMETsPF", ""),
    labelName = cms.string("MTTop")

)
