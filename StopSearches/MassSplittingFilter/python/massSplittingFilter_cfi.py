import FWCore.ParameterSet.Config as cms

massSplittingFilter = cms.EDFilter('MassSplittingFilter',
					  source = cms.InputTag("source"),
					  modelTag = cms.string("T2tt"),
					  minSplitting = cms.double(0),
					  maxSplitting = cms.double(175)
)
