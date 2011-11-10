import FWCore.ParameterSet.Config as cms

modelSelector = cms.EDFilter('ModelSelector',
					  source = cms.InputTag("source"),
					  modelTag = cms.string("T2tt"),
					  selectOnRange = cms.bool(False),
					  selectOnSplitting = cms.bool(False),
					  parameterMins = cms.vdouble(-99999999,-9999999),
					  parameterMaxs = cms.vdouble(99999999,9999999),
					  minSplitting = cms.double(-99999999),
					  maxSplitting = cms.double(99999999)
)
