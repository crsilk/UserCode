import FWCore.ParameterSet.Config as cms

process = cms.Process("NEWNAME")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)


process.source = cms.Source (
    "PoolSource",
    #fileNames = cms.untracked.vstring('dcap://cmsdca3.fnal.gov:24143/pnfs/fnal.gov/usr/cms/WAX/resilient/lucieg/CMSDAS/DiPions/DiPions_38_1_xhW.root'),
    fileNames = cms.untracked.vstring('file:/uscms/home/cavanaug/work/CMSSW_3_8_7/src/RecoParticleFlow/Configuration/test/single-photon.root'),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
    )

process.ToyPFProducer = cms.EDProducer("ToyPF",
    PFCandidates = cms.InputTag("particleFlow"),
    verbose = cms.untracked.bool(True),
    printBlocks = cms.untracked.bool(False)
)

process.p = cms.Path(process.ToyPFProducer)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('ToyPF.root'),
                               outputCommands = cms.untracked.vstring('keep *')
                               )

process.outpath = cms.EndPath(process.out)
