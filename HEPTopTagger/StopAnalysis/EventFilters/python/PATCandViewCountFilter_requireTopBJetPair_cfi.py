import FWCore.ParameterSet.Config as cms

requireTopBJetPair = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedTopBJetPair","Top"),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)
requireTop15BJetPair = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedTop15BJetPair","Top"),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)
requireTop2BJetPair = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedTop2BJetPair","Top"),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)
requireTop125BJetPair = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedTop125BJetPair","Top"),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)
