import FWCore.ParameterSet.Config as cms


selectedMatchedFatJet = cms.EDProducer(
	"MatchedFatJetSelector",
	TopSrc = cms.InputTag("selectedTopBJetPair", "Top"),
	BJetSrc = cms.InputTag("selectedTopBJetPair", "BJet"),
	FatJetSrc = cms.InputTag("ca15PFJetsPFlow"),	
	fatJetR = cms.double(1.5)
	)


selectedMatchedFatJet125 = cms.EDProducer(
	"MatchedFatJetSelector",
	TopSrc = cms.InputTag("selectedTop125BJetPair", "Top"),
	BJetSrc = cms.InputTag("selectedTop125BJetPair", "BJet"),
	FatJetSrc = cms.InputTag("ca125PFJetsPFlow"),	
	fatJetR = cms.double(1.25)
	)


selectedMatchedFatJet15 = cms.EDProducer(
	"MatchedFatJetSelector",
	TopSrc = cms.InputTag("selectedTop15BJetPair", "Top"),
	BJetSrc = cms.InputTag("selectedTop15BJetPair", "BJet"),
	FatJetSrc = cms.InputTag("ca15PFJetsPFlow"),	
	fatJetR = cms.double(1.5)
	)


selectedMatchedFatJet2 = cms.EDProducer(
	"MatchedFatJetSelector",
	TopSrc = cms.InputTag("selectedTop2BJetPair", "Top"),
	BJetSrc = cms.InputTag("selectedTop2BJetPair", "BJet"),
	FatJetSrc = cms.InputTag("ca2PFJetsPFlow"),	
	fatJetR = cms.double(2.0)
	)
