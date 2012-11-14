import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
process = cms.Process("cutFlow")

process.load("FWCore.MessageService.MessageLogger_cfi")


options = VarParsing ('python')

options.register ('sourceFile',
                  'StopAnalysis.Analysis.test_53x_cfi',
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
process.load("StopAnalysis.ObjectProducers.MT2Producer_TopAndMatchedFatJet_cfi")

process.load("StopAnalysis.ObjectProducers.DeltaPhiObjectVsMETProducer_patJetsAK5PF_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_PFchsJetsPt30_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_PFchsJetsPt70eta2p5_cfi")
process.load("StopAnalysis.ObjectProducers.IsolatedTrackProducer_StandardIsolation_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_Status3GenParticles_cfi")
process.load("StopAnalysis.ObjectProducers.ModelPointProducer_StandardConfiguration_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_MET_cfi")
process.load("StopAnalysis.ObjectProducers.MatchedFatJetSelector_BJet_cfi")
process.load("StopAnalysis.ObjectProducers.LostLeptonSelector_StandardSelection_cfi")
####Load the modules for apply the cuts
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_requireTopBJetPair_cfi")
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_requireHEPAntiTag_cfi")
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_requireHEPTopTag_cfi")
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_requireBJet_cfi")
process.load("StopAnalysis.EventFilters.preCuts_cff")
process.load("StopAnalysis.EventFilters.DoublesFilter_DeltaPhiJetsAndMETCut_cfi")
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_isolatedTrackVeto_cfi")
process.load("StopAnalysis.EventFilters.LeptonVetos_cff")
process.load("StopAnalysis.EventFilters.TriangleCutFilter_MTTopAndMTBJet_cfi")
process.load("StopAnalysis.EventFilters.DoublesFilter_MT2Cut_cfi")
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_lostLeptonVeto_cfi")
###Output Definition
process.output = cms.OutputModule(
	"PoolOutputModule",
	outputCommands = cms.untracked.vstring(
		'keep *_*_*_cutFlow',
		'keep *_patElectronsIDIso_*_*',
		'keep *_patMuonsIDIso_*_*',
		'keep *_selectedPatTausPF_*_*',
		'keep *_patMETsPF_*_*',
		'keep *_generator_*_*',
		'keep *_ca*PFJetsPFlow_*_*',
		'keep *_TriggerResults_*_*'
		'keep *_HEPTopSelTag*Jets_*_*',
		'keep *_addPileupInfo_*_*'
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
	process.selectedMatchedFatJet125 *
	process.selectedMatchedFatJet15 *
	process.selectedMatchedFatJet2 *
	process.MTTop125 *
	process.MTTop15 *
	process.MTTop2 *
	process.MT125BJet *
	process.MT15BJet *
	process.MT2BJet *
	process.MT2Top125AndMatchedFatJet *
	process.MT2Top15AndMatchedFatJet *
	process.MT2Top2AndMatchedFatJet *
	process.deltaPhiJetsAndMET *
	process.PFchsJetsPt30 *
	process.PFchsJetsPt70eta2p5 *
	process.isolatedTracks *
	process.status3GenParticles *
	process.MET175  *
	process.lostLeptons
	)

####Define the path that defines all of the cuts to be made
process.METCut150_path = cms.Path(process.METCut150 )
process.METCut125_path = cms.Path(process.METCut125 )
process.METCut175_path = cms.Path(process.METCut175 )
process.METCut200_path = cms.Path(process.METCut200 )
process.METCut225_path = cms.Path(process.METCut225 )
process.PFchs3JetsPt30Cut_path = cms.Path(process.PFchs3JetsPt30Cut )
process.PFchs4JetsPt30Cut_path = cms.Path(process.PFchs4JetsPt30Cut )
process.PFchs5JetsPt30Cut_path = cms.Path(process.PFchs5JetsPt30Cut )
process.PFchs6JetsPt30Cut_path = cms.Path(process.PFchs6JetsPt30Cut )
process.PFchs2JetsPt70eta2p5Cut_path = cms.Path(process.PFchs2JetsPt70eta2p5Cut )
process.requireHEPAnti125Tag_path = cms.Path(process.requireHEPAnti125Tag)
process.requireHEPAnti15Tag_path = cms.Path(process.requireHEPAnti15Tag)
process.requireHEPAnti2Tag_path = cms.Path(process.requireHEPAnti2Tag)
process.requireHEPTop125Tag_path = cms.Path(process.requireHEPTop125Tag)
process.requireHEPTop15Tag_path = cms.Path(process.requireHEPTop15Tag)
process.requireHEPTop2Tag_path = cms.Path(process.requireHEPTop2Tag)
process.requireBJet_path = cms.Path(process.requireBJet)
process.requireTop125BJetPair_path = cms.Path(process.requireTop125BJetPair)
process.requireTop15BJetPair_path = cms.Path(process.requireTop15BJetPair)
process.requireTop2BJetPair_path = cms.Path(process.requireTop2BJetPair)
process.deltaPhiJetsAndMETCut_path = cms.Path(process.deltaPhiJetsAndMETCut )
process.isolatedMuonVeto_path = cms.Path(process.isolatedMuonVeto )
process.isolatedElectronVeto_path = cms.Path(process.isolatedElectronVeto )
process.PFTauVeto_path = cms.Path(process.PFTauVeto)
process.isolatedTrackVeto_path = cms.Path(process.isolatedTrackVeto)
process.triangleCutMTTop125AndMTBJet_path = cms.Path(process.triangleCutMTTop125AndMTBJet)
process.triangleCutMTTop15AndMTBJet_path = cms.Path(process.triangleCutMTTop15AndMTBJet)
process.triangleCutMTTop2AndMTBJet_path = cms.Path(process.triangleCutMTTop2AndMTBJet)
process.MT2Cut125_path = cms.Path(process.MT2Cut125)
process.MT2Cut15_path = cms.Path(process.MT2Cut15)
process.MT2Cut2_path = cms.Path(process.MT2Cut2)
process.lostLeptonVeto_path = cms.Path(process.lostLeptonVeto)

process.out = cms.EndPath(process.output)
process.schedule = cms.Schedule(
	process.produce,
	process.METCut125_path,
	process.METCut150_path,
	process.METCut175_path,
	process.METCut200_path,
	process.METCut225_path,
	process.PFchs3JetsPt30Cut_path,
	process.PFchs4JetsPt30Cut_path,
	process.PFchs5JetsPt30Cut_path,
	process.PFchs6JetsPt30Cut_path,
	process.PFchs2JetsPt70eta2p5Cut_path,
	process.requireHEPAnti125Tag_path,
	process.requireHEPAnti15Tag_path,
	process.requireHEPAnti2Tag_path,
	process.requireHEPTop125Tag_path,
	process.requireHEPTop15Tag_path,
	process.requireHEPTop2Tag_path,
	process.requireBJet_path,
	process.requireTop125BJetPair_path,
	process.requireTop15BJetPair_path,
	process.requireTop2BJetPair_path, 
	process.deltaPhiJetsAndMETCut_path, 
	process.isolatedMuonVeto_path, 
	process.isolatedElectronVeto_path, 
	process.PFTauVeto_path, 
	process.isolatedTrackVeto_path,
	process.triangleCutMTTop125AndMTBJet_path,
	process.triangleCutMTTop15AndMTBJet_path,
	process.triangleCutMTTop2AndMTBJet_path,
	process.MT2Cut125_path,
	process.MT2Cut15_path,
	process.MT2Cut2_path,
	process.lostLeptonVeto_path,
	process.out
	)

if not options.saveSourceVariable == '':
	import pickle
	file = open(options.saveSourceVariable, 'w')
	pickle.dump( process.source.fileNames, file)
