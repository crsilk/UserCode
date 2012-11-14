import FWCore.ParameterSet.Config as cms

lostLeptonVeto = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("lostLeptons"),
    minNumber = cms.uint32(0),
	maxNumber = cms.uint32(0)
	
)
