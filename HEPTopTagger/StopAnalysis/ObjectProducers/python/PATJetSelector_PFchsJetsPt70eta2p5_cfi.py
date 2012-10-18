import FWCore.ParameterSet.Config as cms
PFchsJetsPt70eta2p5 = cms.EDProducer(
	'JetSelector',
	jetSrc = cms.InputTag('patJetsPF'),
	cuts = cms.string('pt > 70 & abs(eta) < 2.5'),
	labelName = cms.string('PFchsJetsPt70eta2p5')
)
