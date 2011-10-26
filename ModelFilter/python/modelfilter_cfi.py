import FWCore.ParameterSet.Config as cms

modelFilter = cms.EDFilter('ModelFilter',
					  source = cms.InputTag("source"),
					  modelTag = cms.string(""),
					  modelParameters = cms.vstring()
)
