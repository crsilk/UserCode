import FWCore.ParameterSet.Config as cms

isolatedElectronVeto = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("patElectronsIDIso"),
    minNumber = cms.uint32(0),
	maxNumber = cms.uint32(0)
	
)
isolatedPFMuonVeto = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("patMuonsPFIDIso"),
    minNumber = cms.uint32(0),
	maxNumber = cms.uint32(0)
	
)
PFTauVeto = cms.EDFilter(
	'PATCandViewCountFilter',
    src = cms.InputTag("selectedPatTausPF"),
    minNumber = cms.uint32(0),
	maxNumber = cms.uint32(0)
	)
