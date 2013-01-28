import FWCore.ParameterSet.Config as cms

dijetMassCutNoBJetVeto = cms.EDFilter(
    'DijetMassFilter',
    m12Src = cms.InputTag("dijetMassProducer", "m12"),
    m13Src = cms.InputTag("dijetMassProducer", "m13"),
    m23Src = cms.InputTag("dijetMassProducer", "m23"),
    m123Src = cms.InputTag("dijetMassProducer", "m123"),
    bJetsBehaveSrc = cms.InputTag("dijetMassProducer", "bJetsBehave"),
    bJetVeto = cms.bool(False),
    Rmin = cms.double(0.85 * 80.3/172.5),
    Rmax = cms.double(1.25 * 80.3/172.5),
    arctanmin = cms.double(0.2),
    arctanmax = cms.double(1.3),
    m23OverM123Cut = cms.double(0.35),
    topMass = cms.double(172.5)
)
dijetMassCutBJetVeto = cms.EDFilter(
    'DijetMassFilter',
    m12Src = cms.InputTag("dijetMassProducer", "m12"),
    m13Src = cms.InputTag("dijetMassProducer", "m13"),
    m23Src = cms.InputTag("dijetMassProducer", "m23"),
    m123Src = cms.InputTag("dijetMassProducer", "m123"),
    bJetsBehaveSrc = cms.InputTag("dijetMassProducer", "bJetsBehave"),
    bJetVeto = cms.bool(True),
    Rmin = cms.double(0.85 * 80.3/172.5),
    Rmax = cms.double(1.25 * 80.3/172.5),
    arctanmin = cms.double(0.2),
    arctanmax = cms.double(1.3),
    m23OverM123Cut = cms.double(0.35),
    topMass = cms.double(172.5)
)

