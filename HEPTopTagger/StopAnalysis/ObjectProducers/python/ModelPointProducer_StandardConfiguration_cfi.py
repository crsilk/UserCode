import FWCore.ParameterSet.Config as cms

modelPoints = cms.EDProducer(
	'ModelPointProducer',
	source = cms.InputTag('source')
	)
