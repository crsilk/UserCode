import FWCore.ParameterSet.Config as cms

preSelectedHEPTop15Tags = cms.EDProducer(
	'HEPTopTagSelector',
    HEPTopTagSrc = cms.InputTag("HEPTopSelTag15Jets"),
    ptCut = cms.double(100),
    etaCut = cms.double(5.0),
    topMassMinCut = cms.double(	0),								   
    topMassMaxCut = cms.double(9999999),
	m13MinCut = cms.double(0.0),
	m13MaxCut = cms.double(9999999),
    rMinCut = cms.double(0),
    rMaxCut = cms.double(999999999),
    m23MinCut = cms.double(0),
    m23MaxCut = cms.double(9999999999),
    antiDijetCuts = cms.bool(False),
	light = cms.bool(False)
									
)
