import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.HEPTopTagger_cfi import HEPTopTagJets
from RecoJets.JetProducers.HEPTopTagger_cfi import HEPTopTagInfos
from RecoJets.JetProducers.HEPTopTagger_cfi import HEPTopTag
from RecoJets.JetProducers.ca4PFJets_cfi import ca4PFJets


ca15PFJetsPFlow = ca4PFJets.clone(
	rParam = cms.double(1.5),
	src = cms.InputTag('particleFlow'),
	doAreaFastjet = cms.bool(True),
	doRhoFastjet = cms.bool(True),
	Rho_EtaMax = cms.double(6.0),
	Ghost_EtaMax = cms.double(7.0)
	)



