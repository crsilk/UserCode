import FWCore.ParameterSet.Config as cms

deltaPhiFactory = cms.PSet(
       inputCollection = cms.InputTag("DeltaPhiAK5"),
       )
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgDeltaPhi = cms.EDFilter(
    "DeltaPhiPOProducer",
	inputCollection = cms.InputTag("DeltaPhiAK5"),
    cuts = cms.PSet()
)
