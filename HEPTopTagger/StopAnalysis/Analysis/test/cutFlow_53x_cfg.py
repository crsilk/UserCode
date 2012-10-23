import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
process = cms.Process("cutFlow")

process.load("FWCore.MessageService.MessageLogger_cfi")


options = VarParsing ('python')

options.register ('sourceFile',
                  'StopAnalysis.Analysis.test_cfi',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "Source file (e.g. StopAnalysis.Analysis.test_cfi) "
				  )

options.register('nEvents',
				 -1,
				 VarParsing.multiplicity.singleton,
				 VarParsing.varType.int,
				 "Number of events to run"
				 )
options.register('mc',
				 True,
				 VarParsing.multiplicity.singleton,
				 VarParsing.varType.bool,
				 "Run on MC"
				 )
options.register ('saveSourceVariable',
                  '',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "File name of the process.source variable to be saved in. If nothing given then it won't save."
				  )

options.parseArguments()



process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.nEvents) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load(options.sourceFile)
#process.load("StopAnalysis.Analysis.test_cfi")

####Load the modules of all the new collections
process.load("StopAnalysis.ObjectProducers.HEPTopTagSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.HEPTopTagSelector_AntiTagSelection_cfi")
process.load("StopAnalysis.ObjectProducers.BJetSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.TopBJetPairSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.MTProducer_TopAndMET_cfi")
process.load("StopAnalysis.ObjectProducers.MTProducer_BJetAndMET_cfi")
process.load("StopAnalysis.ObjectProducers.MT2Producer_TopAndBJet_cfi")
process.load("StopAnalysis.ObjectProducers.DeltaPhiObjectVsMETProducer_patJetsAK5PF_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_PFchsJetsPt30_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_PFchsJetsPt70eta2p5_cfi")
process.load("StopAnalysis.ObjectProducers.IsolatedTrackProducer_StandardIsolation_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_Status3GenParticles_cfi")
process.load("StopAnalysis.ObjectProducers.ModelPointProducer_StandardConfiguration_cfi")


####Load the modules for apply the cuts
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_requireTopBJetPair_cfi")
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_requireHEPAntiTag_cfi")
process.load("StopAnalysis.EventFilters.preCuts_cff")
process.load("StopAnalysis.EventFilters.DoublesFilter_DeltaPhiJetsAndMETCut_cfi")
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_isolatedTrackVeto_cfi")
process.load("StopAnalysis.EventFilters.LeptonVetos_cff")



###Output Definition
process.output = cms.OutputModule(
	"PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *_*_*_cutFlow',
										   'keep *_patElectronsIDIso_*_*',
										   'keep *_patMuonsPFIDIso_*_*',
										   'keep *_selectedPatTausPF_*_*',
										   'keep *_patMETsPF_*_*',
										   'keep *_generator_*_*',
										   'keep *_ca*PFJetsPFlow_*_*'

										   ),
    fileName = cms.untracked.string('cutFlow.root'),
								 
)

####Define the path that defines all the collections to be made
process.produce = cms.Path(
	process.modelPoints *
	process.selectedHEPTop125Tags *
	process.selectedHEPTop15Tags *
	process.selectedHEPTop2Tags *
	process.selectedHEPAnti125Tags *
	process.selectedHEPAnti15Tags *
	process.selectedHEPAnti2Tags *
	process.selectedBJets *
	process.selectedTop125BJetPair *
	process.selectedTop15BJetPair *
	process.selectedTop2BJetPair *
	process.MTTop125 *
	process.MTTop15 *
	process.MTTop2 *
	process.MT125BJet *
	process.MT15BJet *
	process.MT2BJet *
	process.MT2Top125AndBJet *
	process.MT2Top15AndBJet *
	process.MT2Top2AndBJet *
	process.deltaPhiJetsAndMET *
	process.PFchsJetsPt30 *
	process.PFchsJetsPt70eta2p5 *
	process.isolatedTracks *
	process.status3GenParticles 

	)

####Define the path that defines all of the cuts to be made
process.METCut_path = cms.Path(process.METCut )
process.PFchsJetsPt30Cut_path = cms.Path(process.PFchsJetsPt30Cut )
process.PFchsJetsPt70eta2p5Cut_path = cms.Path(process.PFchsJetsPt70eta2p5Cut )
process.requireHEPAnti125Tag_path = cms.Path(process.requireHEPAnti125Tag)
process.requireHEPAnti15Tag_path = cms.Path(process.requireHEPAnti15Tag)
process.requireHEPAnti2Tag_path = cms.Path(process.requireHEPAnti2Tag)
process.requireTop125BJetPair_path = cms.Path(process.requireTop125BJetPair)
process.requireTop15BJetPair_path = cms.Path(process.requireTop15BJetPair)
process.requireTop2BJetPair_path = cms.Path(process.requireTop2BJetPair)
process.deltaPhiJetsAndMETCut_path = cms.Path(process.deltaPhiJetsAndMETCut )
process.isolatedPFMuonVeto_path = cms.Path(process.isolatedPFMuonVeto )
process.isolatedElectronVeto_path = cms.Path(process.isolatedElectronVeto )
process.PFTauVeto_path = cms.Path(process.PFTauVeto)
process.isolatedTrackVeto_path = cms.Path(process.isolatedTrackVeto)

process.out = cms.EndPath(process.output)
process.schedule = cms.Schedule(
	process.produce,
	process.METCut_path,
	process.PFchsJetsPt30Cut_path,
	process.PFchsJetsPt70eta2p5Cut_path,
	process.requireHEPAnti125Tag_path,
	process.requireHEPAnti15Tag_path,
	process.requireHEPAnti2Tag_path,
	process.requireTop125BJetPair_path,
	process.requireTop15BJetPair_path,
	process.requireTop2BJetPair_path, 
	process.deltaPhiJetsAndMETCut_path, 
	process.isolatedPFMuonVeto_path, 
	process.isolatedElectronVeto_path, 
	process.PFTauVeto_path, 
	process.isolatedTrackVeto_path,
	process.out
	)

if not options.saveSourceVariable == '':
	import pickle
	file = open(options.saveSourceVariable, 'w')
	pickle.dump( process.source.fileNames, file)
