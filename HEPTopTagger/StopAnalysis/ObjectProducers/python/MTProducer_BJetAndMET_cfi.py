import FWCore.ParameterSet.Config as cms


MTBJet = cms.EDProducer('MTProducer', 
    Object1Src = cms.InputTag("selectedTopBJetPair", "BJet"),
    Object2Src = cms.InputTag("patMETsPF", ""),
    labelName = cms.string("MTBJet")

)
