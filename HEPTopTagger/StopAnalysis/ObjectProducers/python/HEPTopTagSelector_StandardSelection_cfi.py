import FWCore.ParameterSet.Config as cms


selectedHEPTopTags = cms.EDProducer(
	'HEPTopTagSelector',
    HEPTopTagSrc = cms.InputTag("HEPTopTagJets"),
    ptCut = cms.double(200),
    etaCut = cms.double(5.0),
    topMassMinCut = cms.double(	147.3),								   
    topMassMaxCut = cms.double(197.3),
	m13MinCut = cms.double(0.2),
	m13MaxCut = cms.double(1.3),
    rMinCut = cms.double(0.85*80.4/172.3),
    rMaxCut = cms.double(1.15*80.4/172.3),
    m23MinCut = cms.double(0.35),
    m23MaxCut = cms.double(1.00),
    antiDijetCuts = cms.bool(False),
	light = cms.bool(False)
									
)

selectedHEPTop15Tags = cms.EDProducer('HEPTopTagSelector',
    HEPTopTagSrc = cms.InputTag("HEPTopSelTag15Jets"),
    ptCut = cms.double(200),
    etaCut = cms.double(5.0),
    topMassMinCut = cms.double(	147.3),								   
    topMassMaxCut = cms.double(197.3),
	m13MinCut = cms.double(0.2),
	m13MaxCut = cms.double(1.3),
    rMinCut = cms.double(0.85*80.4/172.3),
    rMaxCut = cms.double(1.15*80.4/172.3),
    m23MinCut = cms.double(0.35),
    m23MaxCut = cms.double(1.00),
    antiDijetCuts = cms.bool(False),
	light = cms.bool(False)									
  )

selectedHEPTop2Pt120Tags = cms.EDProducer('HEPTopTagSelector',
    HEPTopTagSrc = cms.InputTag("HEPTopSelTag2Jets"),
    ptCut = cms.double(120),
    etaCut = cms.double(5.0),
    topMassMinCut = cms.double(	147.3),								   
    topMassMaxCut = cms.double(197.3),
	m13MinCut = cms.double(0.2),
	m13MaxCut = cms.double(1.3),
    rMinCut = cms.double(0.85*80.4/172.3),
    rMaxCut = cms.double(1.2*80.4/172.3),
    m23MinCut = cms.double(0.35),
    m23MaxCut = cms.double(1.00),
    antiDijetCuts = cms.bool(False),
	light = cms.bool(False)									
)

selectedHEPTop2Pt150Tags = cms.EDProducer('HEPTopTagSelector',
    HEPTopTagSrc = cms.InputTag("HEPTopSelTag2Jets"),
    ptCut = cms.double(150),
    etaCut = cms.double(5.0),
    topMassMinCut = cms.double(	147.3),								   
    topMassMaxCut = cms.double(197.3),
	m13MinCut = cms.double(0.2),
	m13MaxCut = cms.double(1.3),
    rMinCut = cms.double(0.85*80.4/172.3),
    rMaxCut = cms.double(1.2*80.4/172.3),
    m23MinCut = cms.double(0.35),
    m23MaxCut = cms.double(1.00),
    antiDijetCuts = cms.bool(False),
	light = cms.bool(False)									
)

selectedHEPTop125Tags = cms.EDProducer('HEPTopTagSelector',
    HEPTopTagSrc = cms.InputTag("HEPTopSelTag125Jets"),
    ptCut = cms.double(200),
    etaCut = cms.double(5.0),
    topMassMinCut = cms.double(	147.3),								   
    topMassMaxCut = cms.double(197.3),
	m13MinCut = cms.double(0.2),
	m13MaxCut = cms.double(1.3),
    rMinCut = cms.double(0.85*80.4/172.3),
    rMaxCut = cms.double(1.125*80.4/172.3),
    m23MinCut = cms.double(0.35),
    m23MaxCut = cms.double(1.00),
    antiDijetCuts = cms.bool(False),
	light = cms.bool(False)									
)
