import FWCore.ParameterSet.Config as cms

process = cms.Process("METTopAnalyzer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Analysis.StopAnalysis.sources.QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6_EDM_cfi")
process.load("Analysis.StopAnalysis.metTopAnalyzer_QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.TFileService = cms.Service("TFileService",
	fileName = cms.string("QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6_METTop.root")

)

process.metTopAnalyzer.METCut = cms.double(200.0)
process.metTopAnalyzer.nTopCandCut = cms.int32 ( 2 ) 
process.metTopAnalyzer.alphaTCut = cms.double ( 0.55 ) 
process.p = cms.Path(process.metTopAnalyzer)
