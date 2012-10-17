import FWCore.ParameterSet.Config as cms

process = cms.Process("cutFlow")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'/store/user/lpcsusyhad/crsilk/TTJets_TuneZ2star_8TeV-madgraph-tauola/SUSYPAT_20_1_nXk.root'
)
)


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

####Load the modules for apply the cuts
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_requireTopBJetPair_cfi")
process.load("StopAnalysis.EventFilters.preCuts_cff")
process.load("StopAnalysis.EventFilters.DoublesFilter_DeltaPhiJetsAndMETCut_cfi")
process.load("SandBox.Stop.StopLeptons_cff")
process.load("SandBox.Stop.StopTauJets_cff")
process.load("SandBox.Stop.StopTrackIsolation_cff")


###Output Definition
process.output = cms.OutputModule(
	"PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *_*_*_cutFlow'

										   ),
    fileName = cms.untracked.string('cutFlow.root'),
    SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('cuts')
    )
								 
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
	process.PFchsJetsPt70eta2p5
	)

####Define the path that defines all of the cuts to be made
process.cuts = cms.Path(
	process.preCuts *
	process.requireTopBJetPair *
	process.deltaPhiJetsAndMETCut *
	process.stopPFMuonVeto *
	process.stopElectronVeto 
#	process.stopTauJetVeto 	*
#	process.stopIsoTrackVeto
	)

process.out = cms.EndPath(process.output)
process.schedule = cms.Schedule(process.produce, process.cuts, process.out)



