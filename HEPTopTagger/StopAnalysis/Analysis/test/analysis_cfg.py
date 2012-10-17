import FWCore.ParameterSet.Config as cms

process = cms.Process("StopNTuple")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100


process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
'/store/user/lpcsusyhad/crsilk/TTJets_TuneZ2star_8TeV-madgraph-tauola/SUSYPAT_20_1_nXk.root'
)
)

process.SelectedHEPTopTags = cms.EDProducer('HEPTopTagSelector',
    HEPTopTagSrc = cms.InputTag("HEPTopTagJets",""),
    ptCut = cms.double(200),
    etaCut = cms.double(5.0),
    topMassMinCut = cms.double(	147.3),								   
    topMassMaxCut = cms.double(197.3),
	m13MinCut = cms.double(0.2),
	m13MaxCut = cms.double(1.3),
    rMinCut = cms.double(.85*80.4/172.3),
    rMaxCut = cms.double(1.15*80.4/172.3),
    m23MinCut = cms.double(0.35),
    m23MaxCut = cms.double(1.00),
	antiDijetCuts = cms.bool(False),
    labelName = cms.string("SelectedHEPTopTags")									   
)

process.SelectedBJets = cms.EDProducer('BJetSelector',
    BJetSrc = cms.InputTag('patJetsAK5PF'),
    ptCut = cms.double(30),
    etaCut = cms.double(2.4),
    discriminator = cms.string(	"combinedSecondaryVertexBJetTags"),
    discriminatorCut = cms.double(0.679),
    labelName = cms.string("SelectedBJets")
)


process.mt = cms.EDProducer('MTProducer', 
    Object1Src = cms.InputTag("SelectedBJets", "SelectedBJets"),
    Object2Src = cms.InputTag("patMETsPF", ""),
    labelName = cms.string("MT")

)


process.finalTopAndBJet = cms.EDProducer('TopBJetPairSelector',
	TopSrc = cms.InputTag("SelectedHEPTopTags", "SelectedHEPTopTags", "StopNTuple"),
    BJetSrc = cms.InputTag("SelectedBJets", "SelectedBJets", "StopNTuple"),
    deltaRFromFatJet = cms.bool(False),
    deltaRCut = cms.double(0.8),
    TopLabelName = cms.string("Top"),
    BJetLabelName = cms.string("BJet")

)

process.mt2 = cms.EDProducer('MT2Producer',
	VisibleObject1Src = cms.InputTag("SelectedHEPTopTags", "SelectedHEPTopTags", "StopNTuple"),
    VisibleObject2Src = cms.InputTag("SelectedBJets", "SelectedBJets", "StopNTuple"),
    METSrc = cms.InputTag("patMETsPF", "", "PAT"),
    twoDifferentVisibleObjects = cms.bool(True),
    labelName = cms.string("MT2")

)


process.deltaPhiJetsAndMET = cms.EDProducer('DeltaPhiObjectVsMETProducer',
    ObjectSrc = cms.InputTag("patJetsAK5PF"),
    METSrc = cms.InputTag("patMETsPF"),
    nDeltaPhis = cms.uint32(3),
	labelName = cms.string("deltaPhiJetsAndMET")
)


process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *_*_Selected*_*', 'keep *_*_*MT*_*', 'keep *_finalTopAndBJet_*_*', 'keep *_*_deltaPhiJet*_*'),
    fileName = cms.untracked.string('test.root'),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('cuts')
    )
								 
)

process.requireTop = cms.EDFilter('CandViewCountFilter',
    src = cms.InputTag("finalTopAndBJet","Top"),
    minNumber = cms.uint32(1)


)

process.deltaPhiCuts = cms.EDFilter('DoublesFilter',
    src = cms.InputTag("deltaPhiJetsAndMET", "deltaPhiJetsAndMET"),
    minCuts = cms.vdouble(0.3, 0.3, 0.3),
    maxCuts = cms.vdouble()
)
process.p = cms.Path(process.SelectedHEPTopTags)
process.b = cms.Path(process.SelectedBJets)
process.m = cms.Path(process.mt)
process.m2 = cms.Path(process.mt2)
process.f = cms.Path(process.finalTopAndBJet)
process.out = cms.EndPath(process.output)
process.d = cms.Path(process.deltaPhiJetsAndMET)
#process.cuts = cms.Path(process.requireTop * process.deltaPhiCuts)
process.cuts1 = cms.Path(process.requireTop )
process.cuts = cms.Path( process.deltaPhiCuts)
process.schedule = cms.Schedule(process.p, process.b,process.m,process.d,process.f,process.m2,process.cuts1,process.cuts,process.out)



