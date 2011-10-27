import FWCore.ParameterSet.Config as cms

deltaPhiAnalyzer = cms.EDAnalyzer('DeltaPhiAnalyzer',
					  PFCandidates = cms.InputTag("particleFlow"),
					  PFJets = cms.InputTag("ak5PFJets"),
					  minNJets = cms.int32(3),
					  maxJetEta = cms.double(2.5),
					  minJetPt = cms.double(50),
					  minHT = cms.double(350),
					  minMET = cms.double(200)
)
