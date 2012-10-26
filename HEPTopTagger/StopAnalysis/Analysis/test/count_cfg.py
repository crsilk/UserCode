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



options.parseArguments()


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

process.countEventsAfterCuts = cms.EDAnalyzer(
	"CountEventsAfterCuts",
	bitSetSrc = cms.InputTag("TriggerResults"),
	genEventSrc = cms.InputTag("generator"),
	cutNames = cms.vstring('METCut175_path', 'PFchs2JetsPt70eta2p5Cut_path','isolatedPFMuonVeto_path', 'isolatedElectronVeto_path'),
	cutDecisions = cms.vint32(1, 1, 1)
	)

process.p = cms.Path(process.countEventsAfterCuts)
