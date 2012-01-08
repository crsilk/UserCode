import FWCore.ParameterSet.Config as cms

collectionSuffix = "ttbsmAna"

metTopAnalyzer = cms.EDAnalyzer('METTopAnalyzer',
    METP4Src = cms.InputTag( collectionSuffix, "metP4"),
	topCandP4Src = cms.InputTag( collectionSuffix, "topTagP4"),
	topCandMassSrc = cms.InputTag( collectionSuffix, "topTagTopMass"),
	topCandMinMassSrc = cms.InputTag( collectionSuffix, "topTagMinMass"),
	topCandNSubjetsSrc = cms.InputTag( collectionSuffix, "topTagNSubjets"),
	topCandPassTopTagSrc = cms.InputTag( collectionSuffix, "topTagPass"),
	eventWeightSrc = cms.InputTag(collectionSuffix, 'weight'),
	pdfWeightSrc = cms.InputTag( collectionSuffix, "pdfWeights"),
	useEDMEventWeight = cms.bool(False),
    #If useEDMEventWeight is set to True then this is overidden
	eventWeight = cms.double(0.000005042),
	topCandPtDef = cms.double(450.0),

    nTopCandCut = cms.int32(1),
	pt_MinCuts = cms.vdouble(450.0),
	pt_MaxCuts = cms.vdouble(99999999),	
	eta_MinCuts = cms.vdouble(-5.0),
	eta_MaxCuts = cms.vdouble(5.0),	
	deltaPhi_MinCuts = cms.vdouble(2.1),
	deltaPhi_MaxCuts = cms.vdouble(4.18),	
	topMass_MinCuts = cms.vdouble(140),
	topMass_MaxCuts = cms.vdouble(250),	
	nSubjets_MinCuts = cms.vint32(1),
	nSubjets_MaxCuts = cms.vint32(4),	
	minMass_MinCuts = cms.vdouble(50.0),
	minMass_MaxCuts = cms.vdouble(99999)	
							  )
