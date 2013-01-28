import FWCore.ParameterSet.Config as cms

requireHEPTopTag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPTopTags",""),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)

requireHEPTop15Tag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPTop15Tags",""),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)

requireHEPTop125Tag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPTop125Tags",""),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)

requireHEPTop2Pt120Tag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPTop2Pt120Tags",""),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)


requireHEPTop2Pt150Tag = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedHEPTop2Pt150Tags",""),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)
