import FWCore.ParameterSet.Config as cms

requireTopBJetPair = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedTopBJetPair","Top"),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)
