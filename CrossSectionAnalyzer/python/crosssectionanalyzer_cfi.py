import FWCore.ParameterSet.Config as cms

demo = cms.EDAnalyzer('CrossSectionAnalyzer',
					  outputFile = cms.untracked.string('CrossSectionAnalyzer.root')
)
