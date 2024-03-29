import FWCore.ParameterSet.Config as cms
PFchsJetsPt30 = cms.EDProducer(
	'CandidateSelector',
	src = cms.InputTag('stopJetsPFchsPt30'),
	cuts = cms.string('pt > 30'),
	light = cms.bool(False)
)
