import FWCore.ParameterSet.Config as cms

process = cms.Process("ModelFilter")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("UserCode.ModelFilter.modelfilter_cfi")
process.load("FastSimulation.Configuration.EventContent_cff")

readFiles = cms.untracked.vstring()


process.source = cms.Source("PoolSource",
	 fileNames = cms.untracked.vstring("dcap:///pnfs/cms/WAX/11/store/mc/Summer11/SMS-T1tttt_Mgluino-450to1200_mLSP-50to800_7TeV-Pythia6Z/AODSIM/PU_START42_V11_FSIM-v2/0001/FE8EB85A-A1F9-E011-BC2D-0025902009B0.root")
)


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.modelfilter.selectOnRange = cms.bool(True)
process.modelfilter.parameterMins = cms.vdouble(0.0, 600, 175)
process.modelfilter.parameterMaxs = cms.vdouble(0.0, 600, 175)

process.aod = cms.OutputModule("PoolOutputModule",
     process.AODSIMEventContent,
     fileName = cms.untracked.string('T1tttt_test.root'),
	 SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('modelfilter_step'))
 )

process.aod_step = cms.EndPath(process.aod)
process.modelfilter_step = cms.Path(process.modelfilter)

process.schedule =cms.Schedule(process.modelfilter_step, process.aod_step)
