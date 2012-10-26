import FWCore.ParameterSet.Config as cms
MET175 = cms.EDProducer(
	'CandidateSelector',
	src = cms.InputTag('patMETsPF'),
	cuts = cms.string('pt > 175'),
	light = cms.bool(False)
)
