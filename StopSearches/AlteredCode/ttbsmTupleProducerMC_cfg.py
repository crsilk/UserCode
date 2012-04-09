import FWCore.ParameterSet.Config as cms

process = cms.Process("TTBSM")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
       fileNames = cms.untracked.vstring(
"dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/crsilk/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_v9_Summer11-PU_S4_START42_V11-v1/2f45e7f986681f9c0bbcc7cc31425dac/ttbsm_42x_mc_100_1_dV9.root",
"dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/crsilk/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_v9_Summer11-PU_S4_START42_V11-v1/2f45e7f986681f9c0bbcc7cc31425dac/ttbsm_42x_mc_22_1_upM.root",
 #"dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/crsilk/ZJetsToNuNu_200_HT_inf_7TeV-madgraph/ttbsm_v9_Summer11-PU_S4_START42_V11-v1/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_9_1_67u.root" 
'file:/uscmst1b_scratch/lpc1/3DayLifetime/crsilk/temp/ttbsm_42x_mc_184_2_9wr.root'
                                    )
                                )



## Geometry and Detector Conditions (needed for a few patTuple production steps)
#process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'GR_R_42_V12::All'
#process.load("Configuration.StandardSequences.MagneticField_cff")


from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from Analysis.BoostedTopAnalysis.CATopTagParams_cfi import caTopTagParams
from Analysis.BoostedTopAnalysis.BoostedTopWTagParams_cfi import boostedTopWTagParams



process.ttbsmAna = cms.EDFilter('TTBSMProducer',
                                wTagSrc = cms.InputTag('goodPatJetsCA8PrunedPF'),
                                topTagSrc = cms.InputTag('goodPatJetsCATopTagPF'),
                                trigSrc = cms.InputTag('patTriggerEvent'),
                                rhoSrc = cms.InputTag('kt6PFJetsPFlow', 'rho'),
                                pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
#################<ADDED BY CHRIS>###########
                                #pfCandidateSrc = cms.InputTag('particleFlow'),
                                metSrc = cms.InputTag('patMETsPFlow'),
				jetSrc = cms.InputTag('goodPatJetsPFlow'),
				electronSrc = cms.InputTag('selectedPatElectronsPFlow'),
				muonSrc = cms.InputTag('selectedPatMuonsPFlow'),
                                subJetSrc = cms.InputTag('caTopTagPFlow'),
                                genSubJetSrc = cms.InputTag('caPrunedGen'),
                                genJetSrc = cms.InputTag('selectedPatJetsPFlow', 'genJets'),
                                eventWeightSrc = cms.InputTag('generator'),
                                lheProdSrc = cms.InputTag('source'),
#################<ADDED BY CHRIS>###########								
                                readTrig = cms.bool(False),
                                trigs = cms.vstring(
                                    ''
                                    ),
                                topTagParams = caTopTagParams.clone(
                                    tagName = cms.string('CATop')
                                    ),
                                wTagParams = boostedTopWTagParams.clone(
                                    yCut = cms.double(0.0)
                                    ),
                                jetScale = cms.double(0.0),   #
                                jetPtSmear = cms.double(0.1), # note these three are fractional
                                jetEtaSmear = cms.double(0.1),#
                                jecPayloads = cms.vstring([
                                    'Jec11_V3_L1FastJet_AK5PFchs.txt',
                                    'Jec11_V3_L2Relative_AK5PFchs.txt',
                                    'Jec11_V3_L3Absolute_AK5PFchs.txt',
                                    #'Jec11_V3_L2L3Residual_AK5PFchs.txt',
                                    'Jec11_V3_Uncertainty_AK5PFchs.txt'
                                    ]),
                                pdfSet = cms.string("")

)


process.ttbsmAnaScaleDown = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetScale = cms.double(-0.05)
    )
process.ttbsmAnaScaleUp = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetScale = cms.double(0.05)
    )
process.ttbsmAnaPtSmearDown = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetPtSmear = cms.double(0.0)
    )
process.ttbsmAnaPtSmearUp = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetPtSmear = cms.double(0.2)
    )
process.ttbsmAnaEtaSmearDown = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetEtaSmear = cms.double(0.0)
    )
process.ttbsmAnaEtaSmearUp = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetEtaSmear = cms.double(0.2)
    )


# Produce PDF weights (maximum is 3)
process.pdfWeights = cms.EDProducer("PdfWeightProducer",
      # Fix POWHEG if buggy (this PDF set will also appear on output, 
      # so only two more PDF sets can be added in PdfSetNames if not "")
      FixPOWHEG = cms.untracked.string(""),
      GenTag = cms.untracked.InputTag("prunedGenParticles"),
      PdfInfoTag = cms.untracked.InputTag("generator"),
      PdfSetNames = cms.untracked.vstring( "cteq66.LHgrid"
                                          ,"MRST2006nnlo.LHgrid"
                                           )
)

process.MessageLogger.cerr.FwkReport.reportEvery = 100

print 'Making the path'

process.p = cms.Path(
    #process.patTriggerDefaultSequence*
    #process.hltSelection*
    process.ttbsmAna
    *process.ttbsmAnaScaleDown
    *process.ttbsmAnaScaleUp
    *process.ttbsmAnaPtSmearDown
    *process.ttbsmAnaPtSmearUp
    *process.ttbsmAnaEtaSmearDown
    *process.ttbsmAnaEtaSmearUp
    #*process.pdfWeights
    )


process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string("ttbsm_ultraslim.root"),
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_ttbsmAna*_*_*'
                                                                      #, 'keep *_goodPatJetsCA8PrunedPF_*_*'
                                                                      #, 'keep *_goodPatJetsCATopTagPF_*_*'
                                                                      #, 'keep recoPFJets_*_*_*'
                                                                      ) 
                               )
process.outpath = cms.EndPath(process.out)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.out.dropMetaData = cms.untracked.string("DROPPED")
