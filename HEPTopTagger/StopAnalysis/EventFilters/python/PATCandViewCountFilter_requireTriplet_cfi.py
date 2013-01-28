import FWCore.ParameterSet.Config as cms

requireTripletBJetVeto = cms.EDFilter(
	'PATCandViewCountFilter',
        src = cms.InputTag("tripletSelectorBJetVeto","selectedTriplet"),
        minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)

requireTripletNoBJetVeto = cms.EDFilter(
	'PATCandViewCountFilter',
        src = cms.InputTag("tripletSelectorNoBJetVeto","selectedTriplet"),
        minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	
)
