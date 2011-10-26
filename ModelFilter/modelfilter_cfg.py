import FWCore.ParameterSet.Config as cms

process = cms.Process("ModelFilter")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("UserCode.ModelFilter.modelfilter_cfi")
process.load("FastSimulation.Configuration.EventContent_cff")

readFiles = cms.untracked.vstring()
#readFiles.extend()




process.source = cms.Source("PoolSource",
#    fileNames = readFiles
	 fileNames = cms.untracked.vstring("file:/uscmst1b_scratch/lpc1/3DayLifetime/crsilk/temp/06037E03-45CB-E011-A6F5-00261894384A.root")
)

#process.load("CMGTools.Common.sources.SMS_T2tt_Mstop_225to1200_mLSP_50to1025_7TeV_Pythia6Z.Summer11_PU_START42_V11_FastSim_v1.AODSIM.source_cff")


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.modelFilter.modelTag = cms.string("T2tt")
process.modelFilter.modelParameters = cms.vstring("700.0","450.0")

process.aod = cms.OutputModule("PoolOutputModule",
     process.AODSIMEventContent,
     fileName = cms.untracked.string('aod.root'),
	 SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('modelFilter_step'))
 )

process.aod_step = cms.EndPath(process.aod)
process.modelFilter_step = cms.Path(process.modelFilter)

process.schedule =cms.Schedule(process.modelFilter_step, process.aod_step)
