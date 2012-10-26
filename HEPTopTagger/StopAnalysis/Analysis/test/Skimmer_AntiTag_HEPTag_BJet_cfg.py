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
else:
	requireHEPAnti = 'requireHEPAntiTag_path'
	HEPs = 'selectedHEPTopTags'
process.output = cms.OutputModule(
	"PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *_*_*_*'
								   ),
    fileName = cms.untracked.string(options.fileList.split('/')[-1] + '_Skimmer_AntiTag_HEPTopTag_BJet.root'),
	SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('p'))
								 
)
process.skim = cms.EDFilter(
	"Skimmer",
	bitSetSrc = cms.InputTag("TriggerResults"),
	cutNames = cms.vstring('METCut175_path',
						   'PFchs2JetsPt70eta2p5Cut_path',
						   'isolatedPFMuonVeto_path',
						   'isolatedElectronVeto_path',
						   requireHEPAnti
						   ),
	cutDecisions = cms.vint32(1, 1, 1, 1, 1)
)
process.requireBJet =cms.EDFilter(
	"PATCandViewCountFilter",
	src = cms.InputTag("selectedBJets"),
	minNumber = cms.uint32(1),
	maxNumber = cms.uint32(999999)
	)
process.requireHEPTopTag =cms.EDFilter(
	"PATCandViewCountFilter",
	src = cms.InputTag(HEPs),
	minNumber = cms.uint32(1),
	maxNumber = cms.uint32(999999)
	)
process.p = cms.Path(
	process.skim *
	process.requireBJet *
	process.requireHEPTopTag
	)
process.out = cms.EndPath(process.output)
process.schedule = cms.Schedule(process.p, process.out)
