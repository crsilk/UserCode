
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring(
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_10_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_11_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_12_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_13_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_14_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_15_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_16_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_17_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_18_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_19_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_1_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_20_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_2_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_3_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_4_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_5_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_6_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_7_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_8_1.root',
		'rfio:/castor/cern.ch/user/b/beaudett/grid/CMSSW356/AnalysisSamples7TeV/QCD_EMEnriched_Pt20to30/patLayer1_fromAOD_PF2PAT_full_9_1.root',
	)
)
