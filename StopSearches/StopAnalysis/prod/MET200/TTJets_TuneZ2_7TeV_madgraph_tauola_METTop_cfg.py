import FWCore.ParameterSet.Config as cms

process = cms.Process("METTopAnalyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Analysis.StopAnalysis.sources.TTJets_TuneZ2_7TeV_madgraph_tauola_EDM_cfi")
process.load("Analysis.StopAnalysis.metTopAnalyzer_TTJets_TuneZ2_7TeV_madgraph_tauola_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.TFileService = cms.Service("TFileService",
	fileName = cms.string("TTJets_TuneZ2_7TeV_madgraph_tauola_METTop.root")

)

process.metTopAnalyzer.METCut = cms.double(200.0)
process.p = cms.Path(process.metTopAnalyzer)
