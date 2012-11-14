import FWCore.ParameterSet.Config as cms

selectedTopBJetPair = cms.EDProducer(
	'TopBJetPairSelector',
	TopSrc = cms.InputTag("selectedHEPTopTags", ""),
	TopSubjetSrc = cms.InputTag("selectedHEPTopTags", "subjets"),
    BJetSrc = cms.InputTag("selectedBJets", ""),
	FatJetSrc = cms.InputTag("ca15PFJetsPFlow"),
	fatJetR = cms.double(1.5),
    deltaRFromFatjetCut = cms.double(2.0),
    deltaRFromSubjetsCut = cms.double(0.8),
	light = cms.bool(False)
)


selectedTop15BJetPair = cms.EDProducer(
	'TopBJetPairSelector',
	TopSrc = cms.InputTag("selectedHEPTop15Tags", ""),
	TopSubjetSrc = cms.InputTag("selectedHEPTop15Tags", "subjets"),
    BJetSrc = cms.InputTag("selectedBJets", ""),
   	FatJetSrc = cms.InputTag("ca15PFJetsPFlow"),
	fatJetR = cms.double(1.5),
	deltaRFromFatjetCut = cms.double(2.0),
    deltaRFromSubjetsCut = cms.double(0.8),
 	light = cms.bool(False)
)

selectedTop2BJetPair = cms.EDProducer(
	'TopBJetPairSelector',
	TopSrc = cms.InputTag("selectedHEPTop2Tags", ""),
	TopSubjetSrc = cms.InputTag("selectedHEPTop2Tags", "subjets"),
    BJetSrc = cms.InputTag("selectedBJets", ""),
  	FatJetSrc = cms.InputTag("ca2PFJetsPFlow"),
	fatJetR = cms.double(2.0),
  deltaRFromFatjetCut = cms.double(2.5),
    deltaRFromSubjetsCut = cms.double(0.8),
	light = cms.bool(False)
)

selectedTop125BJetPair = cms.EDProducer(
	'TopBJetPairSelector',
	TopSrc = cms.InputTag("selectedHEPTop125Tags", ""),
	TopSubjetSrc = cms.InputTag("selectedHEPTop125Tags", "subjets"),
    BJetSrc = cms.InputTag("selectedBJets", ""),
   	FatJetSrc = cms.InputTag("ca125PFJetsPFlow"),
	fatJetR = cms.double(1.25),
	deltaRFromFatjetCut = cms.double(1.75),
    deltaRFromSubjetsCut = cms.double(0.8),
   	light = cms.bool(False)
)
