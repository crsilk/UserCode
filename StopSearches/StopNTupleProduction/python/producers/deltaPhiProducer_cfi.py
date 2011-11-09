import FWCore.ParameterSet.Config as cms

deltaPhiProducer = cms.EDProducer('DeltaPhiProducer',
					  PFCandidates = cms.InputTag("particleFlow"),
					  neutralVsCharged = cms.bool(True)
)
