import FWCore.ParameterSet.Config as cms
PFchsJetsPt30eta2p5 = cms.EDProducer(
	'CandidateSelector',
	src = cms.InputTag('stopJetsPFchsPt30'),
	cuts = cms.string('pt > 30 & abs(eta) < 2.5'),
	light = cms.bool(False)
)
