import FWCore.ParameterSet.Config as cms

BJetWithType3 = cms.EDFilter(
    'BoolsFilter',
    src = cms.InputTag("dijetMassFilter", "bJetsBehave"),
    thereExistsA = cms.bool(True)
)

