import FWCore.ParameterSet.Config as cms

demo = cms.EDProducer('DoubleValueProducer',
					  value1 = cms.untracked.double(0),
					  value2 = cms.untracked.double(0)
)
