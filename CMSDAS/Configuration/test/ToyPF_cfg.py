#cfg to Run ToyPF
import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)


process.source = cms.Source (
    "PoolSource",    
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_3_8_7/RelValTTbar_Tauola/GEN-SIM-RECO/START38_V13_PU_E7TeV_AVE_2_BX156-v1/0018/F0A1D986-4CFD-DF11-89B5-001A92971B62.root'),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
    )




process.pfCandidateAnalyzer = cms.EDAnalyzer("ToyPF",
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


process.p = cms.Path(process.pfCandidateAnalyzer)


