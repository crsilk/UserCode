import FWCore.ParameterSet.Config as cms
PFchsJetsPt30 = cms.EDProducer(
	'JetSelector',
	jetSrc = cms.InputTag('patJetsPF'),
	cuts = cms.string('pt > 30'),
	labelName = cms.string('PFchsJetsPt30')
)
