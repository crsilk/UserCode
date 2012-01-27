import FWCore.ParameterSet.Config as cms

modelfilter = cms.EDFilter('ModelFilter',
    source = cms.InputTag("source"),
    modelTag = cms.string("T2tt"),
    selectOnRange = cms.bool(False),
    selectOnMassSplitting = cms.bool(False),
    MinSplitting = cms.double(0),
    maxSplitting = cms.double(7000),
    parameterMins = cms.vdouble(-99999999,-99999999),
    parameterMaxs = cms.vdouble(99999999, 99999999)
)
