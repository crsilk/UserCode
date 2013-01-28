import FWCore.ParameterSet.Config as cms

requireHEPAntiTag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPAntiTags",""),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)

requireHEPAnti15Tag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPAnti15Tags",""),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)

requireHEPAnti125Tag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPAnti125Tags",""),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)

requireHEPAnti2Pt120Tag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPAnti2Pt120Tags",""),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)

requireHEPAnti2Pt150Tag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPAnti2Pt150Tags",""),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)
