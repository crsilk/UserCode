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
ca2PFJetsPFlow = ca4PFJets.clone(
	rParam = cms.double(1.5),
	src = cms.InputTag('particleFlow'),
	doAreaFastjet = cms.bool(True),
	doRhoFastjet = cms.bool(True),
	Rho_EtaMax = cms.double(6.0),
	Ghost_EtaMax = cms.double(7.0)
	)


HEPTopTag15Jets = HEPTopTagJets.clone(
    )
HEPTopTag15Infos = HEPTopTagInfos.clone(
	src = cms.InputTag("HEPTopTag15Jets")
	)

HEPTopSelTag15Jets = HEPTopTagJets.clone(
    useSelTag = cms.bool(True)
    )
HEPTopSelTag15Infos = HEPTopTagInfos.clone(
	src = cms.InputTag("HEPTopSelTag15Jets")
	)

HEPTopTag2Jets = HEPTopTagJets.clone(
	rParam = cms.double(2.0)
    )
HEPTopTag2Infos = HEPTopTagInfos.clone(
	src = cms.InputTag("HEPTopTag2Jets")
	)


HEPTopSelTag2Jets = HEPTopTagJets.clone(
	rParam = cms.double(2.0),
    useSelTag = cms.bool(True)
    )
HEPTopSelTag2Infos = HEPTopTagInfos.clone(
	src = cms.InputTag("HEPTopSelTag2Jets")
	)

HEPTopSelTag2 = cms.Sequence(HEPTopSelTag2Jets + HEPTopSelTag2Infos)
HEPTopSelTag15= cms.Sequence(HEPTopSelTag15Jets + HEPTopSelTag15Infos)
	

