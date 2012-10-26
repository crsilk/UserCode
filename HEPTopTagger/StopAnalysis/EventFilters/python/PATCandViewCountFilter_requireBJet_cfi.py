import FWCore.ParameterSet.Config as cms

requireBJet = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedBJets",""),
    minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)
