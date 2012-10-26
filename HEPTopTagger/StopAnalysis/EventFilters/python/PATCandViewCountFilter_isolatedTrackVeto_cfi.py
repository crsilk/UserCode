import FWCore.ParameterSet.Config as cms

isolatedTrackVeto = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("isolatedTracks"),
    minNumber = cms.uint32(0),
	maxNumber = cms.uint32(0)
	
)
