import FWCore.ParameterSet.Config as cms

process = cms.Process("testMETTopType11Analyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Analysis.StopAnalysis.sources.SMS_T2tt_Mstop_225to1200_mLSP_50to1025_7TeV_Pythia6Z_EDM_cfi")
process.load("Analysis.StopAnalysis.metTopAnalyzer_SMS_T2tt_Mstop_225to1200_mLSP_50to1025_7TeV_Pythia6Z_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.TFileService = cms.Service("TFileService",
	fileName = cms.string("SMS_T2tt_Mstop_225to1200_mLSP_50to1025_7TeV_Pythia6Z_METTop_Type11_1000_400.root")

)
process.metTopAnalyzer.modelParameters = cms.vdouble(800.0, 350.0)
process.metTopAnalyzer.nTopCandCut = cms.int32(2)
process.metTopAnalyzer.alphaTCut = cms.double(0.55)
process.p = cms.Path(process.metTopAnalyzer)
