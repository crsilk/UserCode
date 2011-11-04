import FWCore.ParameterSet.Config as cms

massSplittingSelector = cms.EDFilter('MassSplittingSelector',
					  source = cms.InputTag("source"),
					  modelTag = cms.string(""),
					  minSplitting = cms.double(-999999999),
					  maxSplitting = cms.double(999999999)
)
