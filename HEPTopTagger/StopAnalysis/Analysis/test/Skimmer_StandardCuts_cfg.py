import glob
import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
process = cms.Process("count")

process.load("FWCore.MessageService.MessageLogger_cfi")


options = VarParsing ('python')

options.register ('fileList',
                  '',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "files to cut and count over."
				  )


options.register ('CMSSW53x',
                  False,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.bool,
                  "Running over a 53x sample"
				  )



options.parseArguments()

process = cms.Process("StopNTuple")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


files = glob.glob(options.fileList  + '*root')
readFiles = []
for file in files:
	if file.split('/')[1] == 'pnfs':
		file = 'dcap://' + file
	else:
		file = 'file:' + file
	readFiles.append(file)


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(readFiles),
)

if options.CMSSW53x:
	HEPs = 'selectedHEPTop15Tags'
	requireHEPAnti = 'requireHEPAnti15Tag_path'
	requireTopBJetPair = 'requireTop15BJetPair_path'
	triangleCutMTTopAndMTBJet = 'triangleCutMTTop15AndMTBJet_path'
	MT2Cut = 'MT2Cut15_path'
else:
	HEPs = 'selectedHEPTopTags'
	requireHEPAnti = 'requireHEPAntiTag_path'
	requireTopBJetPair = 'requireTopBJetPair_path'
	triangleCutMTTopAndMTBJet = 'triangleCutMTTopAndMTBJet_path'
	MT2Cut = 'MT2Cut_path'
process.output = cms.OutputModule(
	"PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *_*_*_*'
								   ),
    fileName = cms.untracked.string(options.fileList.split('/')[-1] + '_Skimmer_StandardCuts.root'),
	SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('p'))
								 
)
process.skim = cms.EDFilter(
	"Skimmer",
	bitSetSrc = cms.InputTag("TriggerResults"),
	cutNames = cms.vstring(
	'METCut175_path',
	'PFchs2JetsPt70eta2p5Cut_path',
	'isolatedPFMuonVeto_path',
	'isolatedElectronVeto_path',
	'isolatedTrackVeto_path',
	'deltaPhiJetsAndMETCut_path',
	requireTopBJetPair,
	triangleCutMTTopAndMTBJet,
	MT2Cut
	),
	cutDecisions = cms.vint32(1, 1, 1, 1, 1, 1, 1, 1, 1)
)
process.p = cms.Path(
	process.skim 
	)
process.out = cms.EndPath(process.output)
process.schedule = cms.Schedule(process.p, process.out)
