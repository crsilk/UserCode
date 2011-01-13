import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)


process.source = cms.Source (
    "PoolSource",
    #fileNames = cms.untracked.vstring('dcap://cmsdca3.fnal.gov:24143/pnfs/fnal.gov/usr/cms/WAX/resilient/lucieg/CMSDAS/DiPions/DiPions_38_1_xhW.root'),
    fileNames = cms.untracked.vstring('file:/uscms/home/cavanaug/work/CMSSW_3_8_7/src/RecoParticleFlow/Configuration/test/display_v2.root'),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
    )




process.pfCandidateProducer = cms.EDProducer("ToyPF",
    PFCandidates = cms.InputTag("particleFlow"),
    verbose = cms.untracked.bool(True),
    printBlocks = cms.untracked.bool(False)
)

process.load("FastSimulation.Configuration.EventContent_cff")
process.aod = cms.OutputModule("PoolOutputModule",
    process.AODSIMEventContent,
    fileName = cms.untracked.string('aod.root')
)

process.outpath = cms.EndPath(process.aod )


process.p = cms.Path(process.pfCandidateProducer)


