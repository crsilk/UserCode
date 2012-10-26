import FWCore.ParameterSet.Config as cms
PFchsJetsPt30 = cms.EDProducer(
	'CandidateSelector',
	src = cms.InputTag('patJetsPF'),
	cuts = cms.string('pt > 30'),
	light = cms.bool(False)
)
