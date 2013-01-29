import FWCore.ParameterSet.Config as cms
import glob
process = cms.Process("number")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
    )
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')


options.register('dataset',
                 False,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "qcd flat/binned dataset"
                 )

options.register('nEvents',
                 -1,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "Number of events to run"
                 )

options.register ('fileList',
                  '',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "files to cut and count over."
                  )
options.register ('appendTitle',
                  '',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "Add string to beginning of file name."
                  )
options.register ('saveSourceVariable',
                  '',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "File name of the process.source variable to be saved in. If nothing given then it won't save."
		  )

options.parseArguments()

#-- Message Logger ------------------------------------------------------------
process.MessageLogger.cerr.FwkReport.reportEvery = 100000

 
#-- Input Source --------------------------------------------------------------

files = glob.glob(options.fileList  + '*root')
readFiles = []
for file in files:
    if len(file.split('/')) < 2:
        file = "file:" + file
    elif file.split('/')[1] == 'pnfs':
        file = 'dcap://' + file
    elif file.split('/')[1] == 'eos':
        file = 'root://cmseos:1094/' + file
    else:
        file = 'file:' + file
    readFiles.append(file)





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
process.load("StopAnalysis.ObjectProducers.CandidateSelector_PFchsJetsPt50eta2p5_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_PFchsJetsPt70eta2p5_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_PFchsJetsPt30eta2p5_cfi")
process.load("StopAnalysis.ObjectProducers.IsolatedTrackProducer_StandardIsolation_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_Status3GenParticles_cfi")
process.load("StopAnalysis.ObjectProducers.ModelPointProducer_StandardConfiguration_cfi")
process.load("StopAnalysis.ObjectProducers.CandidateSelector_MET_cfi")
process.load("StopAnalysis.ObjectProducers.MatchedFatJetSelector_BJet_cfi")
process.load("StopAnalysis.ObjectProducers.LostLeptonSelector_StandardSelection_cfi")
process.load("StopAnalysis.ObjectProducers.HEPTopTagger_cfi")
process.load("StopAnalysis.ObjectProducers.METSignificanceProducer_Standard_cfi")
process.load("StopAnalysis.ObjectProducers.MTMT2Producer_Standard_cfi")
process.load("StopAnalysis.ObjectProducers.TripletSelector_Standard_cfi")
process.load("StopAnalysis.ObjectProducers.StopType3TopTagger_cff")

process.load("SandBox.Stop.StopObjects_cff")

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
process.load("StopAnalysis.EventFilters.PATCandViewCountFilter_requireTriplet_cfi")
process.load("StopAnalysis.EventFilters.DoublesFilter_m123Cut_cfi")
process.load('StopAnalysis.EventFilters.DoublesFilter_MT2Cut_cfi')
process.load('StopAnalysis.EventFilters.DoublesFilter_m23OverM123Cut_cfi')
process.load('StopAnalysis.EventFilters.TriangleCutFilter_MTtAndMTb_cfi')
process.load('StopAnalysis.EventFilters.RA2Cuts_cff')
process.load('StopAnalysis.EventFilters.jetIDFailureFilter_cfi')
process.load("SandBox.Stop.StopSelection_cff")

process.load("SandBox.Stop.sakLooseMuonSelector_cfi")
process.load("SandBox.Stop.sakLooseElectronSelector_cfi")
process.load("StopAnalysis.Analysis.EventPrinter_cfi")

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
"file:temp/susypat.root"
#            '/store/user/lpcsusyhad/53X_ntuples/QCD_HT_1000ToInf_MGPythia_v1_lpc1/seema/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia_v1_NOCUTS_12Oct2012V3/c31a0db70b73f0b9a355af58227b92dd/susypat_1748_2_l8t.root',
#       '/store/user/lpcsusyhad/53X_ntuples/QCD_HT_1000ToInf_MGPythia_v1_lpc1/seema/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia_v1_NOCUTS_12Oct2012V3/c31a0db70b73f0b9a355af58227b92dd/susypat_1745_2_4bf.root',
#       '/store/user/lpcsusyhad/53X_ntuples/QCD_HT_1000ToInf_MGPythia_v1_lpc1/seema/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia_v1_NOCUTS_12Oct2012V3/c31a0db70b73f0b9a355af58227b92dd/susypat_160_1_kZc.root',
#       '/store/user/lpcsusyhad/53X_ntuples/QCD_HT_1000ToInf_MGPythia_v1_lpc1/seema/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia_v1_NOCUTS_12Oct2012V3/c31a0db70b73f0b9a355af58227b92dd/susypat_1746_2_g5j.root',
#       '/store/user/lpcsusyhad/53X_ntuples/QCD_HT_1000ToInf_MGPythia_v1_lpc1/seema/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia_v1_NOCUTS_12Oct2012V3/c31a0db70b73f0b9a355af58227b92dd/susypat_1607_1_b2v.root',
#       '/store/user/lpcsusyhad/53X_ntuples/QCD_HT_1000ToInf_MGPythia_v1_lpc1/seema/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia_v1_NOCUTS_12Oct2012V3/c31a0db70b73f0b9a355af58227b92dd/susypat_1000_1_yvT.root',
#       '/store/user/lpcsusyhad/53X_ntuples/QCD_HT_1000ToInf_MGPythia_v1_lpc1/seema/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia_v1_NOCUTS_12Oct2012V3/c31a0db70b73f0b9a355af58227b92dd/susypat_1001_1_ntS.root',
#       '/store/user/lpcsusyhad/53X_ntuples/QCD_HT_1000ToInf_MGPythia_v1_lpc1/seema/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia_v1_NOCUTS_12Oct2012V3/c31a0db70b73f0b9a355af58227b92dd/susypat_1002_1_zyR.root',
#       '/store/user/lpcsusyhad/53X_ntuples/QCD_HT_1000ToInf_MGPythia_v1_lpc1/seema/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia_v1_NOCUTS_12Oct2012V3/c31a0db70b73f0b9a355af58227b92dd/susypat_1003_1_Igu.root',
#       '/store/user/lpcsusyhad/53X_ntuples/QCD_HT_1000ToInf_MGPythia_v1_lpc1/seema/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia_v1_NOCUTS_12Oct2012V3/c31a0db70b73f0b9a355af58227b92dd/susypat_1004_1_8Ks.root'

#"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/53X_ntuples/kasmi/TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2_NOCUTS_12Oct2012V3_corrTag/b9d339f81100b66394e7e5c0a998fe80/susypat_27_1_c56.root",
#"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/53X_ntuples/kasmi/TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2_NOCUTS_12Oct2012V3_corrTag/b9d339f81100b66394e7e5c0a998fe80/susypat_155_1_K7R.root",
#"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/53X_ntuples/kasmi/TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2_NOCUTS_12Oct2012V3_corrTag/b9d339f81100b66394e7e5c0a998fe80/susypat_236_1_hq9.root",
#"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/53X_ntuples/kasmi/TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2_NOCUTS_12Oct2012V3_corrTag/b9d339f81100b66394e7e5c0a998fe80/susypat_396_1_9dX.root",
#"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/53X_ntuples/kasmi/TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2_NOCUTS_12Oct2012V3_corrTag/b9d339f81100b66394e7e5c0a998fe80/susypat_1012_1_Mcp.root",
#"dcap:///pnfs/cms/WAX/11/store/user/lpcsusyhad/53X_ntuples/kasmi/TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2_NOCUTS_12Oct2012V3_corrTag/b9d339f81100b66394e7e5c0a998fe80/susypat_910_1_aEA.root",   

            )
    )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.nEvents))




if options.appendTitle != '':
    if options.appendTitle == '_':
        outputFileName = options.appendTitle + "number_hongxuan.root"
    else:
        outputFileName = options.appendTitle + "_number_hongxuan.root"
else:
    outputFileName = "number_hongxuan.root"



process.output = cms.OutputModule(
	"PoolOutputModule",
	outputCommands = cms.untracked.vstring(
		'keep *_*_*_number',
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
	fileName = cms.untracked.string(outputFileName),
	SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('*')
        )								 
)

process.out = cms.EndPath(process.output)


process.type3topTagger.taggingMode = cms.untracked.bool(False)
process.produce = cms.Path(
    process.stopObjectsOnPatTuples *
    process.PFchsJetsPt30 *
    process.PFchsJetsPt30eta2p5 *
    process.PFchsJetsPt50eta2p5 *
    process.PFchsJetsPt70eta2p5 *
    #	process.isolatedTracks *
    process.RA2_HBHENoiseFilterRA2 *
    process.RA2_beamHaloFilter *
    process.RA2_trackingFailureFilter *
    process.RA2_inconsistentMuons *
    process.RA2_greedyMuons *
    process.RA2_EcalTPFilter *
    process.RA2_EcalBEFilter *
    process.HcalLaserEventFilter *
    process.EEBadScFilter *
    #        RA2_eeNoiseFilter *
    process.jetIDFailureFilter *
    process.sakLooseMuonSelector * 
    process.sakLooseElectronSelector * 
    process.deltaPhiJetsAndMET * 
#        process.isolatedTrackVeto *
    process.PFchs2JetsPt70eta2p5Cut *
    process.PFchs4JetsPt50eta2p5Cut *
    process.PFchs5JetsPt30eta2p5Cut *
    process.deltaPhiJetsAndMETCut *
    process.stopCountBJets *  
    process.type3topTagger *
    process.METCut175 *
    process.eventPrinter 

        )
        


        



#process.p = cms.Path( process.metSignificance * process.Number)
if not options.saveSourceVariable == '':
    import pickle
    file = open(options.saveSourceVariable, 'w')
    pickle.dump( process.source.fileNames, file)

