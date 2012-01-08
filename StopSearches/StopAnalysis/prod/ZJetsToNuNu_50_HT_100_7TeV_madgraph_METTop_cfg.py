import FWCore.ParameterSet.Config as cms

process = cms.Process("testMETTopAnalyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Analysis.StopAnalysis.sources.ZJetsToNuNu_50_HT_100_7TeV_madgraph_EDM_cfi")
process.load("Analysis.StopAnalysis.metTopAnalyzer_ZJetsToNuNu_50_HT_100_7TeV_madgraph_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.TFileService = cms.Service("TFileService",
	fileName = cms.string("ZJetsToNuNu_50_HT_100_7TeV_madgraph_METTops.root")

)

process.p = cms.Path(process.metTopAnalyzer)
