import FWCore.ParameterSet.Config as cms

MT2TripletAndRSystemBJetVeto = cms.EDProducer(
    'MT2Producer',
    VisibleObject1Src = cms.InputTag("tripletSelectorBJetVeto", "selectedTriplet"),
    VisibleObject2Src = cms.InputTag("tripletSelectorBJetVeto", "selectedRSystem"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),

)

MT2TripletAndRSystemNoBJetVeto = cms.EDProducer(
    'MT2Producer',
    VisibleObject1Src = cms.InputTag("tripletSelectorNoBJetVeto", "selectedTriplet"),
    VisibleObject2Src = cms.InputTag("tripletSelectorNoBJetVeto", "selectedRSystem"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),

)
