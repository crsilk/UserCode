import FWCore.ParameterSet.Config as cms

MT2TopAndBJet = cms.EDProducer('MT2Producer',
	VisibleObject1Src = cms.InputTag("selectedTopBJetPair", "Top"),
    VisibleObject2Src = cms.InputTag("selectedTopBJetPair", "BJet"),
    METSrc = cms.InputTag("patMETsPF", ""),
    twoDifferentVisibleObjects = cms.bool(True),
    labelName = cms.string("MT2TopBJet")

)
