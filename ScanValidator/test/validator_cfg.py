import FWCore.ParameterSet.Config as cms

process = cms.Process("ScanValidator")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

readFiles = cms.untracked.vstring()
readFiles.extend([


	])

process.source = cms.Source("PoolSource",
    fileNames = readFiles,
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

process.ScanValidator = cms.EDAnalyzer('ScanValidator',
    source = cms.InputTag("source"),
    dileptonFilter = cms.bool(False),
    getcomment = cms.bool(False),
    modelTag = cms.string(""),
    nevents = cms.int32(10000),
    leptonCut = cms.double(10),
    tauCut = cms.double(40),									   
    jetCut = cms.double(40),
    genjets = cms.InputTag('ak5GenJets'),
    pfjets = cms.InputTag("ak5PFJets"),
    pfcandidates = cms.InputTag("particleFlow"),
    pftaus = cms.InputTag("hpsPFTauProducer"),
    genparticles = cms.InputTag("genParticles")

)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string("LM4_Fastsim.root")
								   )

process.p = cms.Path(process.ScanValidator)
