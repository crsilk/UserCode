import FWCore.ParameterSet.Config as cms

process = cms.Process("testMETTopType11Analyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Analysis.StopAnalysis.sources.ZJetsToNuNu_200_HT_inf_7TeV_madgraph_EDM_cfi")
process.load("Analysis.StopAnalysis.metTopAnalyzer_ZJetsToNuNu_200_HT_inf_7TeV_madgraph_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.TFileService = cms.Service("TFileService",
	fileName = cms.string("ZJetsToNuNu_200_HT_inf_7TeV_madgraph_METTop_Type11.root")

)

process.metTopAnalyzer.nTopCandCut = cms.int32(2)
process.metTopAnalyzer.alphaTCut = cms.double(0.55)
process.p = cms.Path(process.metTopAnalyzer)
