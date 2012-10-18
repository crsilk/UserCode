import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

process = cms.Process("antiTag")
process.load("FWCore.MessageService.MessageLogger_cfi")

options = VarParsing ('python')

options.register ('sourceFile',
                  'StopAnalysis.Analysis.test_cfi',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "Source file (ex StopAnalysis.Analysis.test_cfi) "
				  )

options.register('nEvents',
				 -1,
				  VarParsing.multiplicity.singleton,
				  VarParsing.varType.int,
				 "Number of events to run"
				 )
options.parseArguments()

####Load the source file
process.load(options.sourceFile)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.nEvents) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

####Load the modules of all the new collections
process.load("StopAnalysis.ObjectProducers.HEPTopTagSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.HEPTopTagSelector_AntiTagSelection_cfi")
process.load("StopAnalysis.ObjectProducers.BJetSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.TopBJetPairSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.MTProducer_TopAndMET_cfi")
process.load("StopAnalysis.ObjectProducers.MTProducer_BJetAndMET_cfi")
process.load("StopAnalysis.ObjectProducers.MT2Producer_TopAndBJet_cfi")
process.load("StopAnalysis.ObjectProducers.DeltaPhiObjectVsMETProducer_patJetsAK5PF_cfi")
process.load("StopAnalysis.ObjectProducers.PATJetSelector_PFchsJetsPt30_cfi")
process.load("StopAnalysis.ObjectProducers.PATJetSelector_PFchsJetsPt70eta2p5_cfi")

####Load the modules for apply the cuts
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_requireTopBJetPair_cfi")
process.load("StopAnalysis.EventFilters.preCuts_cff")
process.load("StopAnalysis.EventFilters.DoublesFilter_DeltaPhiJetsAndMETCut_cfi")
process.load("SandBox.Skims.RA2Leptons_cff")



###Other definitions
process.requireHEPAntiTag = cms.EDFilter(
	"PATCandViewCountFilter",
	src = cms.InputTag("selectedHEPAntiTags","selectedHEPAntiTags","antiTag"),
	minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	)

process.requireHEPTopTag = cms.EDFilter(
	"PATCandViewCountFilter",
	src = cms.InputTag("selectedHEPTopTags", "selectedHEPTopTags"),
	minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	)
process.requireBJet = cms.EDFilter(
	"PATCandViewCountFilter",
	src = cms.InputTag("selectedBJets", "selectedBJets"),
	minNumber = cms.uint32(1),
	maxNumber = cms.uint32(99999)
	)

process.PFchsJetsPt30Cut.src = cms.InputTag("PFchsJetsPt30","SelectedJets","antiTag")
process.PFchsJetsPt70eta2p5Cut.src = cms.InputTag("PFchsJetsPt70eta2p5","SelectedJets","antiTag")

###Output Definition
process.output1 = cms.OutputModule(
	"PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *_*_*_antiTag',
										   'keep *_generator_*_*'
										   ),
    fileName = cms.untracked.string('cuts1.root'),
    SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('cuts1')
    )
								 
)
process.output2 = cms.OutputModule(
	"PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *_*_*_antiTag',
										   'keep *_generator_*_*'
										   ),
    fileName = cms.untracked.string('cuts2.root'),
    SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('cuts2')
    )
								 
)
process.output3 = cms.OutputModule(
	"PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *_*_*_antiTag',
										   'keep *_generator_*_*'
										   ),
    fileName = cms.untracked.string('cuts3.root'),
    SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('cuts3')
    )
								 
)
process.output4 = cms.OutputModule(
	"PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *_*_*_antiTag',
										   'keep *_generator_*_*'

										   ),
    fileName = cms.untracked.string('cuts4.root'),
    SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('cuts4')
    )
								 
)


####Define the path that defines all the collections to be made
process.produce = cms.Path(
	process.selectedHEPTopTags *
	process.selectedHEPAntiTags *
	process.selectedBJets *
	process.selectedTopBJetPair *
	process.MTTop *
	process.MTBJet *
	process.MT2TopAndBJet *
	process.deltaPhiJetsAndMET *
	process.PFchsJetsPt30 *
	process.PFchsJetsPt70eta2p5
	)

####Define the path that defines all of the cuts to be made
process.cuts1 = cms.Path(
	process.preCuts *
	process.ra2PFMuonVeto *
	process.ra2ElectronVeto *
	process.requireHEPAntiTag *
	process.requireBJet 
	)
process.cuts2 = cms.Path(
	process.preCuts *
	process.ra2PFMuonVeto *
	process.ra2ElectronVeto *
	process.requireHEPAntiTag *
	~process.requireBJet 
	)
process.cuts3 = cms.Path(
	process.preCuts *
	process.ra2PFMuonVeto *
	process.ra2ElectronVeto *
	process.requireHEPAntiTag *
	process.requireBJet *
	process.requireHEPTopTag
	)
process.cuts4 = cms.Path(
	process.preCuts *
	process.ra2PFMuonVeto *
	process.ra2ElectronVeto *
	process.requireHEPAntiTag *
	~process.requireBJet *
	process.requireHEPTopTag
	)

process.out1 = cms.EndPath(process.output1)
process.out2 = cms.EndPath(process.output2)
process.out3 = cms.EndPath(process.output3)
process.out4 = cms.EndPath(process.output4)
process.schedule = cms.Schedule(process.produce, process.cuts1, process.out1,process.cuts2, process.out2,process.cuts3, process.out3,process.cuts4, process.out4)



