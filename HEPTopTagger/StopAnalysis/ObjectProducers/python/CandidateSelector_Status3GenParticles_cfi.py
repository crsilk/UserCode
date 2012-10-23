import FWCore.ParameterSet.Config as cms

status3GenParticles = cms.EDProducer(
	"CandidateSelector",
	src = cms.InputTag("genParticles"),
	cuts = cms.string  ('status == 3'),
   )
