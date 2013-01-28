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
options.register ('runOnSignal',
                  False,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.bool,
                  "Count over individual model points."
				  )

options.register ('CMSSW53x',
                  False,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.bool,
                  "Run over a sample made in 53x"
				  )



options.parseArguments()


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

files = glob.glob(options.fileList  + '*root')
readFiles = []
for file in files:
	if file.split('/')[1] == 'pnfs':
		file = 'dcap://' + file
	elif file.split('/')[1] == 'eos':
		file = 'root://cmseos:1094/' + file
	else:
		file = 'file:' + file
	readFiles.append(file)


if options.CMSSW53x:
	requireHEPAntiTag = 'requireHEPAnti15Tag_path'
	requireHEPTopTag = 'requireHEPTop15Tag_path'
else:
	requireHEPAntiTag = 'requireHEPAntiTag_path'
	requireHEPTopTag = 'requireHEPTopTag_path'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(readFiles),
)

process.HT_200 = cms.EDAnalyzer(
	"CountEventsAfterCuts",
	bitSetSrc = cms.InputTag("TriggerResults","","cutFlow"),
	triggerSrc = cms.InputTag("TriggerResults","","HLT"),
	genEventSrc = cms.InputTag("generator"),
	modelPointSrc = cms.InputTag("modelPoints", "modelParameters"),
	runOnSignal = cms.bool(options.runOnSignal),
	triggerNames = cms.vstring('HLT_HT200_v*'
	),
	triggerDecisions = cms.vint32(1),
	cutNames = cms.vstring(
	'PFchs4JetsPt30Cut_path',
	'isolatedMuonVeto_path',
	'isolatedElectronVeto_path',
	requireHEPAntiTag,
	requireHEPTopTag,
	'requireBJet_path',
	),
	cutDecisions = cms.vint32(1, 1, 1, 1, 1, 1)
	)
process.DoubleJet36Central = cms.EDAnalyzer(
	"CountEventsAfterCuts",
	bitSetSrc = cms.InputTag("TriggerResults","","cutFlow"),
	triggerSrc = cms.InputTag("TriggerResults","","HLT"),
	genEventSrc = cms.InputTag("generator"),
	modelPointSrc = cms.InputTag("modelPoints", "modelParameters"),
	runOnSignal = cms.bool(options.runOnSignal),
	triggerNames = cms.vstring('HLT_L1DoubleJet36Central_v*'
	),
	triggerDecisions = cms.vint32(1),
	cutNames = cms.vstring(
	'PFchs4JetsPt30Cut_path',
	'isolatedMuonVeto_path',
	'isolatedElectronVeto_path',
	requireHEPAntiTag,
	requireHEPTopTag,
	'requireBJet_path',
	),
	cutDecisions = cms.vint32(1, 1, 1, 1, 1, 1)
	)
process.PFJet40 = cms.EDAnalyzer(
	"CountEventsAfterCuts",
	bitSetSrc = cms.InputTag("TriggerResults","","cutFlow"),
	triggerSrc = cms.InputTag("TriggerResults","","HLT"),
	genEventSrc = cms.InputTag("generator"),
	modelPointSrc = cms.InputTag("modelPoints", "modelParameters"),
	runOnSignal = cms.bool(options.runOnSignal),
	triggerNames = cms.vstring('HLT_PFJet40_v*'
	),
	triggerDecisions = cms.vint32(1),
	cutNames = cms.vstring(
	'PFchs4JetsPt30Cut_path',
	'isolatedMuonVeto_path',
	'isolatedElectronVeto_path',
	requireHEPAntiTag,
	requireHEPTopTag,
	'requireBJet_path',
	),
	cutDecisions = cms.vint32(1, 1, 1, 1, 1, 1)
	)
process.PFJet80 = cms.EDAnalyzer(
	"CountEventsAfterCuts",
	bitSetSrc = cms.InputTag("TriggerResults","","cutFlow"),
	triggerSrc = cms.InputTag("TriggerResults","","HLT"),
	genEventSrc = cms.InputTag("generator"),
	modelPointSrc = cms.InputTag("modelPoints", "modelParameters"),
	runOnSignal = cms.bool(options.runOnSignal),
	triggerNames = cms.vstring('HLT_PFJet80_v*'
	),
	triggerDecisions = cms.vint32(1),
	cutNames = cms.vstring(
	'PFchs4JetsPt30Cut_path',
	'isolatedMuonVeto_path',
	'isolatedElectronVeto_path',
	requireHEPAntiTag,
	requireHEPTopTag,
	'requireBJet_path',
	),
	cutDecisions = cms.vint32(1, 1, 1, 1, 1, 1)
	)
process.QuadJet70 = cms.EDAnalyzer(
	"CountEventsAfterCuts",
	bitSetSrc = cms.InputTag("TriggerResults","","cutFlow"),
	triggerSrc = cms.InputTag("TriggerResults","","HLT"),
	genEventSrc = cms.InputTag("generator"),
	modelPointSrc = cms.InputTag("modelPoints", "modelParameters"),
	runOnSignal = cms.bool(options.runOnSignal),
	triggerNames = cms.vstring('HLT_QuadJet70_v*'
	),
	triggerDecisions = cms.vint32(1),
	cutNames = cms.vstring(
	'PFchs4JetsPt30Cut_path',
	'isolatedMuonVeto_path',
	'isolatedElectronVeto_path',
	requireHEPAntiTag,
	requireHEPTopTag,
	'requireBJet_path',
	),
	cutDecisions = cms.vint32(1, 1, 1, 1, 1, 1)
	)
process.QuadJet80 = cms.EDAnalyzer(
	"CountEventsAfterCuts",
	bitSetSrc = cms.InputTag("TriggerResults","","cutFlow"),
	triggerSrc = cms.InputTag("TriggerResults","","HLT"),
	genEventSrc = cms.InputTag("generator"),
	modelPointSrc = cms.InputTag("modelPoints", "modelParameters"),
	runOnSignal = cms.bool(options.runOnSignal),
	triggerNames = cms.vstring('HLT_QuadJet80_v*'
	),
	triggerDecisions = cms.vint32(1),
	cutNames = cms.vstring(
	'PFchs4JetsPt30Cut_path',
	'isolatedMuonVeto_path',
	'isolatedElectronVeto_path',
	requireHEPAntiTag,
	requireHEPTopTag,
	'requireBJet_path',
	),
	cutDecisions = cms.vint32(1, 1, 1, 1, 1, 1)
	)


process.p = cms.Path(process.HT_200 *
					 process.DoubleJet36Central *
					 process.PFJet40 *
					 process.PFJet80 *
					 process.QuadJet70 *
					 process.QuadJet80 
					 )
