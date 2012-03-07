import FWCore.ParameterSet.Config as cms

process = cms.Process("METTopAnalyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Analysis.StopAnalysis.sources.SMS_T2tt_Mstop_225to1200_mLSP_50to1025_7TeV_Pythia6Z_EDM_cfi")
process.load("Analysis.StopAnalysis.metTopAnalyzer_SMS_T2tt_Mstop_225to1200_mLSP_50to1025_7TeV_Pythia6Z_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.TFileService = cms.Service("TFileService",
	fileName = cms.string("SMS_T2tt_Mstop_225to1200_mLSP_50to1025_7TeV_Pythia6Z_METTop_1000_400.root")

)

process.metTopAnalyzer.modelParameters = cms.vdouble(1000.0, 400.0)
process.metTopAnalyzer.METCut = cms.double(200.0)
process.metTopAnalyzer.nTopCandCut = cms.int32 ( 2 ) 
process.metTopAnalyzer.alphaTCut = cms.double ( 0.55 ) 
process.p = cms.Path(process.metTopAnalyzer)
