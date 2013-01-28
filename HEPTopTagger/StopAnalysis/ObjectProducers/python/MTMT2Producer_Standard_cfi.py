import FWCore.ParameterSet.Config as cms

MTMT2BJetVeto = cms.EDProducer(
    'MTMT2Producer',
    METSrc = cms.InputTag("patMETsPF", ""),
    rSystemSrc = cms.InputTag("tripletSelectorBJetVeto", 'selectedRSystem'),
    bJetsInRSrc = cms.InputTag("tripletSelectorBJetVeto", "selectedBJets"),
    tripletSrc = cms.InputTag("tripletSelectorBJetVeto", "selectedTriplet"),
    mTop = cms.double(172.5),
    mWMin = cms.double(50),
    mWMax = cms.double(120)
)

MTMT2NoBJetVeto = cms.EDProducer(
    'MTMT2Producer',
    METSrc = cms.InputTag("patMETsPF", ""),
    rSystemSrc = cms.InputTag("tripletSelectorNoBJetVeto", 'selectedRSystem'),
    bJetsInRSrc = cms.InputTag("tripletSelectorNoBJetVeto", "selectedBJets"),
    tripletSrc = cms.InputTag("tripletSelectorNoBJetVeto", "selectedTriplet"),
    mTop = cms.double(172.5),
    mWMin = cms.double(50),
    mWMax = cms.double(120)
)
