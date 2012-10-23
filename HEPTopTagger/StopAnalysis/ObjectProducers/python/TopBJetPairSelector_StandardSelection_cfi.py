import FWCore.ParameterSet.Config as cms

selectedTopBJetPair = cms.EDProducer('TopBJetPairSelector',
	TopSrc = cms.InputTag("selectedHEPTopTags", "selectedHEPTopTags"),
    BJetSrc = cms.InputTag("selectedBJets", "selectedBJets"),
    deltaRFromFatjetCut = cms.double(0),
    deltaRFromSubjetsCut = cms.double(0.8),
    TopLabelName = cms.string("Top"),
    BJetLabelName = cms.string("BJet")

)


selectedTop15BJetPair = cms.EDProducer('TopBJetPairSelector',
	TopSrc = cms.InputTag("selectedHEPTop15Tags", "selectedHEPTop15Tags"),
    BJetSrc = cms.InputTag("selectedBJets", "selectedBJets"),
    deltaRFromFatjetCut = cms.double(0),
    deltaRFromSubjetsCut = cms.double(0.8),
    TopLabelName = cms.string("Top"),
    BJetLabelName = cms.string("BJet")

)

selectedTop2BJetPair = cms.EDProducer('TopBJetPairSelector',
	TopSrc = cms.InputTag("selectedHEPTop2Tags", "selectedHEPTop2Tags"),
    BJetSrc = cms.InputTag("selectedBJets", "selectedBJets"),
    deltaRFromFatjetCut = cms.double(0),
    deltaRFromSubjetsCut = cms.double(0.8),
    TopLabelName = cms.string("Top"),
    BJetLabelName = cms.string("BJet")

)

selectedTop125BJetPair = cms.EDProducer('TopBJetPairSelector',
	TopSrc = cms.InputTag("selectedHEPTop125Tags", "selectedHEPTop125Tags"),
    BJetSrc = cms.InputTag("selectedBJets", "selectedBJets"),
    deltaRFromFatjetCut = cms.double(0),
    deltaRFromSubjetsCut = cms.double(0.8),
    TopLabelName = cms.string("Top"),
    BJetLabelName = cms.string("BJet")

)
