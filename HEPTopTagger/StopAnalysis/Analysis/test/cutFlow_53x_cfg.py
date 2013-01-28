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
options.register('HEPTopTagOnFly',
		 False,
		 VarParsing.multiplicity.singleton,
		 VarParsing.varType.bool,
		 'Make HEPTopTags on the fly'
		 )
options.register ('saveSourceVariable',
                  '',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "File name of the process.source variable to be saved in. If nothing given then it won't save."
				  )

options.parseArguments()



process.maxEvents = cms.untracked.PSet( 
	input = cms.untracked.int32(options.nEvents) 
	)

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load(options.sourceFile)



####Load the modules of all the new collections
process.load("StopAnalysis.ObjectProducers.HEPTopTagSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.HEPTopTagSelector_AntiTagSelection_cfi")
process.load("StopAnalysis.ObjectProducers.BJetSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.TopBJetPairSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.MTProducer_TopAndMET_cfi")
process.load("StopAnalysis.ObjectProducers.MTProducer_BJetAndMET_cfi")
process.load("StopAnalysis.ObjectProducers.MT2Producer_TopAndMatchedFatJet_cfi")

process.load("StopAnalysis.ObjectProducers.DeltaPhiObjectVsMETProducer_PFchsJetsPt30_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_PFchsJetsPt30_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_PFchsJetsPt70eta2p5_cfi")
process.load("StopAnalysis.ObjectProducers.IsolatedTrackProducer_StandardIsolation_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_Status3GenParticles_cfi")
process.load("StopAnalysis.ObjectProducers.ModelPointProducer_StandardConfiguration_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_MET_cfi")
process.load("StopAnalysis.ObjectProducers.MatchedFatJetSelector_BJet_cfi")
process.load("StopAnalysis.ObjectProducers.LostLeptonSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.METSignificanceProducer_Standard_cfi")
process.load("StopAnalysis.ObjectProducers.HEPTopTagger_cfi")
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
		'keep *_TriggerResults_*_*',
		'keep *_HEPTopSelTag*Jets_*_*',
		'keep *_addPileupInfo_*_*'
		),
	fileName = cms.untracked.string('cutFlow.root'),
								 
)

if options.HEPTopTagOnFly:
	process.HEPTopSelTag15_path = cms.Path(
		process.HEPTopSelTag15 *
		process.ca15PFJetsPFlow
		)

if options.mc:
	process.genInfo_path = 	cms.Path(
		process.status3GenParticles *
		process.lostLeptons
		)


####Define the path that defines all the collections to be made
process.produce = cms.Path(
	process.modelPoints *
	process.PFchsJetsPt30 *
	process.PFchsJetsPt70eta2p5 *
	process.selectedHEPTop15Tags *
	process.selectedHEPAnti15Tags *
	process.selectedBJets *
	process.selectedTop15BJetPair *
	process.selectedMatchedFatJet15 *
	process.MTTop15 *
	process.MT15BJet *
	process.MT2Top15AndMatchedFatJet *
	process.deltaPhiJetsAndMET *
	process.metSignificance *
	process.isolatedTracks 
	)

####Define the path that defines all of the cuts to be made
process.METCut125_path = cms.Path(process.METCut125 )
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
process.requireHEPAnti15Tag_path = cms.Path(process.requireHEPAnti15Tag)
process.requireHEPTop15Tag_path = cms.Path(process.requireHEPTop15Tag)
process.requireBJet_path = cms.Path(process.requireBJet)
process.requireTop15BJetPair_path = cms.Path(process.requireTop15BJetPair)
process.deltaPhiJetsAndMETCut_path = cms.Path(process.deltaPhiJetsAndMETCut )
process.isolatedMuonVeto_path = cms.Path(process.isolatedMuonVeto )
process.isolatedElectronVeto_path = cms.Path(process.isolatedElectronVeto )
process.PFTauVeto_path = cms.Path(process.PFTauVeto)
process.isolatedTrackVeto_path = cms.Path(process.isolatedTrackVeto)
process.triangleCutMTTop15AndMTBJet_path = cms.Path(process.triangleCutMTTop15AndMTBJet)
process.MT2Cut15_path = cms.Path(process.MT2Cut15)
process.MT2Cut2Pt120_path = cms.Path(process.MT2Cut2Pt120)
process.MT2Cut2Pt150_path = cms.Path(process.MT2Cut2Pt150)
if options.mc:
	process.lostLeptonVeto_path = cms.Path(process.lostLeptonVeto)

process.out = cms.EndPath(process.output)

process.schedule = cms.Schedule()
if options.HEPTopTagOnFly:
	process.schedule.append(process.HEPTopSelTag15_path)

process.schedule.extend([
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
	process.requireHEPAnti15Tag_path,
	process.requireHEPTop15Tag_path,
	process.requireBJet_path,
	process.requireTop15BJetPair_path,
	process.deltaPhiJetsAndMETCut_path, 
	process.isolatedMuonVeto_path, 
	process.isolatedElectronVeto_path, 
	process.PFTauVeto_path, 
	process.isolatedTrackVeto_path,
	process.triangleCutMTTop15AndMTBJet_path,
	process.MT2Cut15_path,
	]
	)
if options.mc:
	process.schedule.extend([process.genInfo_path, process.lostLeptonVeto_path])
process.schedule.append(process.out)

if not options.saveSourceVariable == '':
	import pickle
	file = open(options.saveSourceVariable, 'w')
	pickle.dump( process.source.fileNames, file)
