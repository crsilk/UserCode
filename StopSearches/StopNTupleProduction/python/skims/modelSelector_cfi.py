import FWCore.ParameterSet.Config as cms

modelSelector = cms.EDFilter('ModelSelector',
					  source = cms.InputTag("source"),
					  modelTag = cms.string(""),
					  parameterMins = cms.vdouble(-99999999),
					  parameterMaxs = cms.vdouble(99999999)
)
