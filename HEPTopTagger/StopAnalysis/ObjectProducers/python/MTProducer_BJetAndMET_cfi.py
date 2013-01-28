import FWCore.ParameterSet.Config as cms


MTBJetAndMETBJetVeto = cms.EDProducer(
    'MTProducer', 
    Object1Src = cms.InputTag("tripletSelectorBJetVeto", "selectedBJet"),
    Object2Src = cms.InputTag("patMETsPF"),
)

MTBJetAndMETNoBJetVeto = cms.EDProducer(
    'MTProducer', 
    Object1Src = cms.InputTag("tripletSelectorNoBJetVeto", "selectedBJet"),
    Object2Src = cms.InputTag("patMETsPF"),
)
