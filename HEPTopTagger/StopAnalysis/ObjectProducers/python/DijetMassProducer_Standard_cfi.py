import FWCore.ParameterSet.Config as cms

dijetMassProducer = cms.EDProducer(
    'DijetMassProducer',
    jetSrc = cms.InputTag("patJetsAK5PFPt30"),
    bJetSrc = cms.InputTag("stopBJets"),
    indexSrc = cms.InputTag("type3topTagger", "finalCombfatJets"),
)
