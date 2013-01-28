import FWCore.ParameterSet.Config as cms


MTTripletAndMETBJetVeto = cms.EDProducer(
    'MTProducer', 
    Object1Src = cms.InputTag("tripletSelectorBJetVeto", "selectedTriplet"),
    Object2Src = cms.InputTag("patMETsPF"),
    )

MTTripletAndMETNoBJetVeto = cms.EDProducer(
    'MTProducer', 
    Object1Src = cms.InputTag("tripletSelectorNoBJetVeto", "selectedTriplet"),
    Object2Src = cms.InputTag("patMETsPF"),
    )
