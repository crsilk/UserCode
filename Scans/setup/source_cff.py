import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"LHESource",

	fileNames = cms.untracked.vstring(),
	)
source.fileNames.extend([

])
