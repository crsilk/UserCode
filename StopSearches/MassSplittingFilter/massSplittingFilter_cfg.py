import FWCore.ParameterSet.Config as cms

process = cms.Process("MassSplittingFilter")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("UserCode.MassSplittingFilter.massSplittingFilter_cfi")
process.load("FastSimulation.Configuration.EventContent_cff")

readFiles = cms.untracked.vstring()
#readFiles.extend()




process.source = cms.Source("PoolSource",
	 fileNames = cms.untracked.vstring("file:/uscmst1b_scratch/lpc1/3DayLifetime/crsilk/temp/00A9721F-44CB-E011-A65A-002618943869.root")
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.massSplittingFilter.modelTag = cms.string("T2tt")
process.massSplittingFilter.modelParameters = cms.vstring("700.0","450.0")

process.aod = cms.OutputModule("PoolOutputModule",
     process.AODSIMEventContent,
     fileName = cms.untracked.string('aod.root'),
	 SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('massSplittingFilter_step'))
 )

process.aod_step = cms.EndPath(process.aod)
process.massSplittingFilter_step = cms.Path(process.massSplittingFilter)

process.schedule =cms.Schedule(process.massSplittingFilter_step, process.aod_step)
