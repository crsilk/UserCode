import FWCore.ParameterSet.Config as cms

deltaPhiAnalyzer = cms.EDAnalyzer('DeltaPhiAnalyzer',
					  PFCandidates = cms.InputTag("particleFlow"),
					  rootOutputFile = cms.untracked.string("deltaphianalyzer.root")
)
