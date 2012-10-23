import FWCore.ParameterSet.Config as cms



vetoMuons = cms.EDProducer(
  "PFMuonSelector",
  muonSource   = cms.InputTag('patMuonsPF'),
  vertexSource = cms.InputTag('goodVertices'),
  minMuPt      = cms.double(10),
  maxMuEta     = cms.double(2.4),
  maxMuD0      = cms.double(0.2),
  maxMuDz      = cms.double(0.5),
  maxMuRelIso  = cms.double(0.20),
  minMuNumHit  = cms.double(11),
  doMuonVeto           = cms.bool(False),
  doMuonID             = cms.bool(True),
  doMuonVtxAssociation = cms.bool(True),
  doMuonIsolation      = cms.bool(True),
  labelName = cms.string("")
)
