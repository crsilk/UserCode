import FWCore.ParameterSet.Config as cms
PFchsJetsPt50eta2p5 = cms.EDProducer(
	'JetSelector',
	jetSrc = cms.InputTag('stopJetsPFchsPt30'),
	cuts = cms.string('pt > 50 & abs(eta) < 2.5'),
	light =cms.bool(False)
)
