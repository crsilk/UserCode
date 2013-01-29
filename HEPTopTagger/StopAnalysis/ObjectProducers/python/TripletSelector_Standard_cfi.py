import FWCore.ParameterSet.Config as cms

tripletSelectorBJetVeto = cms.EDProducer(
    'TripletSelector',
    jetSrc = cms.InputTag("stopJetsPFchsPt30"),
    bJetSrc = cms.InputTag("stopBJets"),
    indexSrc = cms.InputTag("type3topTagger", "finalCombfatJets"),
    bJetVeto = cms.bool(True),
    Rmin = cms.double(0.85 * 80.3/172.5),
    Rmax = cms.double(1.25 * 80.3/172.5),
    arctanmin = cms.double(0.2),
    arctanmax = cms.double(1.3),
    m23OverM123Cut = cms.double(0.35),
    topMass = cms.double(172.5)


)


tripletSelectorNoBJetVeto = cms.EDProducer(
    'TripletSelector',
    jetSrc = cms.InputTag("stopJetsPFchsPt30"),
    bJetSrc = cms.InputTag("stopBJets"),
    indexSrc = cms.InputTag("type3topTagger", "finalCombfatJets"),
    bJetVeto = cms.bool(False),
#    Rmin = cms.double(0.85 * 80.3/172.5),
#    Rmax = cms.double(1.25 * 80.3/172.5),
    Rmin = cms.double(0.85 * 80.385/173.5),
    Rmax = cms.double(1.25 * 80.385/173.5),
    arctanmin = cms.double(0.2),
    arctanmax = cms.double(1.3),
    m23OverM123Cut = cms.double(0.35),
    topMass = cms.double(173.5)


)
