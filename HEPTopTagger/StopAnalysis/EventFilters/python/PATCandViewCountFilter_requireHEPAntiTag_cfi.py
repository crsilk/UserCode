import FWCore.ParameterSet.Config as cms

requireHEPAntiTag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPAntiTags","selectedHEPAntiTags"),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)

requireHEPAnti15Tag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPAnti15Tags","selectedHEPAnti15Tags"),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)

requireHEPAnti125Tag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPAnti125Tags","selectedHEPAnti125Tags"),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)

requireHEPAnti2Tag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPAnti2Tags","selectedHEPAnti2Tags"),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)
