import FWCore.ParameterSet.Config as cms

process = cms.Process("testMETTopType11Analyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Analysis.StopAnalysis.sources.TTJets_TuneZ2_7TeV_madgraph_tauola_EDM_cfi")
process.load("Analysis.StopAnalysis.metTopAnalyzer_TTJets_TuneZ2_7TeV_madgraph_tauola_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.TFileService = cms.Service("TFileService",
	fileName = cms.string("TTJets_TuneZ2_7TeV_madgraph_tauola_METTop_Type11.root")

)

process.metTopAnalyzer.nTopCandCut = cms.int32(2)
process.metTopAnalyzer.alphaTCut = cms.double(0.55)
process.p = cms.Path(process.metTopAnalyzer)
