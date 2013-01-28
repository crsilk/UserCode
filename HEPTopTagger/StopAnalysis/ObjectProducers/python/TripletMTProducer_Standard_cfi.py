import FWCore.ParameterSet.Config as cms

tripletMTProducerBJetVeto = cms.EDProducer(
    'TripletMTProducer',
    METSrc = cms.InputTag("patMETsPF", ""),
    jetSrc = cms.InputTag("patJetsAK5PFPt30"),
    bJetSrc = cms.InputTag("stopBJets"),
    tripletIndexSrc = cms.InputTag("tripletSelectorBJetVeto", "bestTripletIndex"),
    bJetIndexSrc = cms.InputTag("tripletSelectorBJetVeto", "bestBJetIndex"),
)

tripletMTProducerNoBJetVeto = cms.EDProducer(
    'TripletMTProducer',
    METSrc = cms.InputTag("patMETsPF", ""),
    jetSrc = cms.InputTag("patJetsAK5PFPt30"),
    bJetSrc = cms.InputTag("stopBJets"),
    tripletIndexSrc = cms.InputTag("tripletSelectorNoBJetVeto", "bestTripletIndex"),
    bJetIndexSrc = cms.InputTag("tripletSelectorNoBJetVeto", "bestBJetIndex"),


)
