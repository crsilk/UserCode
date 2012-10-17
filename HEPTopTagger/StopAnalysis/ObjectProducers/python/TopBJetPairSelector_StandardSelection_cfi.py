import FWCore.ParameterSet.Config as cms

selectedTopBJetPair = cms.EDProducer('TopBJetPairSelector',
	TopSrc = cms.InputTag("selectedHEPTopTags", "selectedHEPTopTags"),
    BJetSrc = cms.InputTag("selectedBJets", "selectedBJets"),
    deltaRFromFatjetCut = cms.double(0),
    deltaRFromSubjetsCut = cms.double(0.8),
    TopLabelName = cms.string("Top"),
    BJetLabelName = cms.string("BJet")

)
