import FWCore.ParameterSet.Config as cms

selectedBJets = cms.EDProducer('BJetSelector',
    BJetSrc = cms.InputTag('patJetsPF'),
    ptCut = cms.double(30),
    etaCut = cms.double(2.4),
    discriminator = cms.string(	"combinedSecondaryVertexBJetTags"),
    discriminatorCut = cms.double(0.679),
    labelName = cms.string("selectedBJets")
)
