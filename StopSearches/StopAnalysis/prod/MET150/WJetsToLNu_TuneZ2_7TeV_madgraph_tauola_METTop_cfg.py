import FWCore.ParameterSet.Config as cms

process = cms.Process("METTopAnalyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Analysis.StopAnalysis.sources.WJetsToLNu_TuneZ2_7TeV_madgraph_tauola_EDM_cfi")
process.load("Analysis.StopAnalysis.metTopAnalyzer_WJetsToLNu_TuneZ2_7TeV_madgraph_tauola_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000000) )

process.TFileService = cms.Service("TFileService",
	fileName = cms.string("WJetsToLNu_TuneZ2_7TeV_madgraph-tauola_METTop.root")

)

process.metTopAnalyzer.METCut = cms.double(150.0)
process.p = cms.Path(process.metTopAnalyzer)
