import FWCore.ParameterSet.Config as cms

collectionSuffix = "ttbsmAna"

metTopAnalyzer = cms.EDAnalyzer('METTopAnalyzer',
	jetsP4Src = cms.InputTag( collectionSuffix, 'jetP4'),
    METP4Src = cms.InputTag( collectionSuffix, "metP4"),
	muonsP4Src = cms.InputTag( collectionSuffix, 'muonP4'),
	topCandP4Src = cms.InputTag( collectionSuffix, "topTagP4"),
	topCandMassSrc = cms.InputTag( collectionSuffix, "topTagTopMass"),
	topCandMinMassSrc = cms.InputTag( collectionSuffix, "topTagMinMass"),
	topCandNSubjetsSrc = cms.InputTag( collectionSuffix, "topTagNSubjets"),
	topCandPassTopTagSrc = cms.InputTag( collectionSuffix, "topTagPass"),
	eventWeightSrc = cms.InputTag(collectionSuffix, 'weight'),
	pdfWeightSrc = cms.InputTag( collectionSuffix, "pdfWeights"),
	modelParametersSrc = cms.InputTag( collectionSuffix, 'modelParameters'),
	jetPtCut = cms.double(25.0),
	muonPtCut = cms.double(10.0),
	useEDMEventWeight = cms.bool(False),
    #If useEDMEventWeight is set to True then this is overidden
	modelParameters = cms.vdouble(0.0, 0.0), 
	 eventWeight = cms.double(1.0),
	enactCuts = cms.bool(True),
	topCandPtDef = cms.double(450.0),

    nTopCandCut = cms.int32(1),
	pt_MinCuts = cms.vdouble(450.0, 450.0),
	pt_MaxCuts = cms.vdouble(99999999, 99999999),	
	eta_MinCuts = cms.vdouble(-5.0, -5.0),
	eta_MaxCuts = cms.vdouble(5.0, 5.0),	
	metTopDeltaPhi_MinCuts = cms.vdouble(2.1, 2.1),
	metTopDeltaPhi_MaxCuts = cms.vdouble(4.18, 4.18),	
	topMass_MinCuts = cms.vdouble(140, 140),
	topMass_MaxCuts = cms.vdouble(250, 250),	
	nSubjets_MinCuts = cms.vint32(1, 1),
	nSubjets_MaxCuts = cms.vint32(4, 4),	
	minMass_MinCuts = cms.vdouble(50.0, 50.0),
	minMass_MaxCuts = cms.vdouble(99999, 99999),
	METCut = cms.double(0.0),
	alphaTCut = cms.double(0.0)	
							  )
