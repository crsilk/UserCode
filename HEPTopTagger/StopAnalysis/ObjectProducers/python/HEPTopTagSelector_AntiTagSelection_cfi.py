import FWCore.ParameterSet.Config as cms


selectedHEPAntiTags = cms.EDProducer('HEPTopTagSelector',
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
    antiDijetCuts = cms.bool(True),
									
    labelName = cms.string("selectedHEPAntiTags")
)
