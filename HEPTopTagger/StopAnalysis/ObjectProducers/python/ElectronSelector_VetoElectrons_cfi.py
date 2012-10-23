import FWCore.ParameterSet.Config as cms

vetoElectrons = cms.EDProducer(
  "ElectronSelector",
  electronSource    = cms.InputTag('gsfElectrons'),
  conversionsSource = cms.InputTag("allConversions"), 
  vertexSource      = cms.InputTag('goodVertices'),
  beamSpotSource    = cms.InputTag("offlineBeamSpot"),
  rhoIsoSource      = cms.InputTag("kt6PFJetsForIsolation", "rho"),
  isoValInputTags   = cms.VInputTag(
	cms.InputTag('elPFIsoValueCharged03PFIdPFIso'),
	cms.InputTag('elPFIsoValueGamma03PFIdPFIso'),
	cms.InputTag('elPFIsoValueNeutral03PFIdPFIso')
	),
  minElePt       = cms.double(10),
  maxEleEta      = cms.double(2.5),
  DoElectronID             = cms.bool(True),
  DoElectronVtxAssociation = cms.bool(True),
  DoElectronIsolation      = cms.bool(True),
  labelName = cms.string("")
)

