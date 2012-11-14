import FWCore.ParameterSet.Config as cms
PFchsJetsPt70eta2p5 = cms.EDProducer(
	'CandidateSelector',
	src = cms.InputTag('patJetsPF'),
	cuts = cms.string('pt > 70 & abs(eta) < 2.4'),
	light = cms.bool(False)
)
