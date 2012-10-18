import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
process = cms.Process("cutFlow")

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



process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.nEvents) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load(options.sourceFile)


####Load the modules of all the new collections
process.load("StopAnalysis.ObjectProducers.HEPTopTagSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.BJetSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.TopBJetPairSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.MTProducer_TopAndMET_cfi")
process.load("StopAnalysis.ObjectProducers.MTProducer_BJetAndMET_cfi")
process.load("StopAnalysis.ObjectProducers.MT2Producer_TopAndBJet_cfi")
process.load("StopAnalysis.ObjectProducers.DeltaPhiObjectVsMETProducer_patJetsAK5PF_cfi")
process.load("StopAnalysis.ObjectProducers.PATJetSelector_PFchsJetsPt30_cfi")
process.load("StopAnalysis.ObjectProducers.PATJetSelector_PFchsJetsPt70eta2p5_cfi")
process.load("StopAnalysis.ObjectProducers.IsolatedTrackProducer_StandardIsolation_cfi")


####Load the modules for apply the cuts
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_requireTopBJetPair_cfi")
process.load("StopAnalysis.EventFilters.preCuts_cff")
process.load("StopAnalysis.EventFilters.DoublesFilter_DeltaPhiJetsAndMETCut_cfi")
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_isolatedTrackVeto_cfi")
process.load("SandBox.Stop.StopLeptons_cff")
process.load("SandBox.Stop.StopTauJets_cff")



###Output Definition
process.output = cms.OutputModule(
	"PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *_*_*_cutFlow'

										   ),
    fileName = cms.untracked.string('cutFlow.root'),
								 
)


####Define the path that defines all the collections to be made
process.produce = cms.Path(
	process.selectedHEPTopTags *
	process.selectedBJets *
	process.selectedTopBJetPair *
	process.MTTop *
	process.MTBJet *
	process.MT2TopAndBJet *
	process.deltaPhiJetsAndMET *
	process.PFchsJetsPt30 *
	process.PFchsJetsPt70eta2p5 *
	process.isolatedTracks
	)

####Define the path that defines all of the cuts to be made
process.cuts = cms.Path(
	process.METCut *
	process.PFchsJetsPt30Cut *
	process.PFchsJetsPt70eta2p5Cut *
	process.requireTopBJetPair *
	process.deltaPhiJetsAndMETCut *
	process.stopPFMuonVeto *
	process.stopElectronVeto *
#	process.stopTauJetVeto 	*
	process.isolatedTrackVeto
	)

process.out = cms.EndPath(process.output)
process.schedule = cms.Schedule(process.produce, process.cuts, process.out)



