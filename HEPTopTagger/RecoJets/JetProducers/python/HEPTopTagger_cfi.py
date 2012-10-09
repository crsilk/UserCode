import FWCore.ParameterSet.Config as cms
from RecoJets.JetProducers.CATopJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from RecoJets.JetProducers.PFJetParameters_cfi import *

HEPTopTagJets = cms.EDProducer(
	"CATopJetProducer",
   PFJetParameters.clone( src = cms.InputTag('particleFlow'),
                          doAreaFastjet = cms.bool(True),
                          doRhoFastjet = cms.bool(False),
                          jetPtMin = cms.double(100.0)
                          ),
   AnomalousCellParameters,
   CATopJetParameters.clone(
       verbose = cms.bool(True),
       tagAlgo = cms.int32(2) ),
   jetAlgorithm = cms.string("CambridgeAachen"),
   rParam = cms.double(1.5),
   writeCompound = cms.bool(True),
   useSelTag = cms.bool(False)
   )

HEPTopTagInfos = cms.EDProducer(
	"CATopJetTagger",
	src = cms.InputTag("HEPTopTagJets"),
	TopMass = cms.double(171),
	TopMassMin = cms.double(0.),
	TopMassMax = cms.double(250.),
	WMass = cms.double(80.4),
	WMassMin = cms.double(0.0),
	WMassMax = cms.double(200.0),
	MinMassMin = cms.double(0.0),
	MinMassMax = cms.double(200.0),
	verbose = cms.bool(False)
	)

HEPTopTag = cms.Sequence(HEPTopTagJets + HEPTopTagInfos)
