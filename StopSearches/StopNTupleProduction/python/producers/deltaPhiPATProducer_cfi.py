import FWCore.ParameterSet.Config as cms

deltaPhiProducer = cms.EDFilter('DeltaPhiPATProducer',
					  DeltaPhiCollection = cms.InputTag("DeltaPhi")
)
