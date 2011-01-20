import FWCore.ParameterSet.Config as cms

toyPFMETProducer = cms.EDProducer("ToyPFMET",
                                  PFCandidatesForMET = cms.InputTag("toyPFProducer"),
                                  minPt = cms.untracked.double(2.)
                                  )
