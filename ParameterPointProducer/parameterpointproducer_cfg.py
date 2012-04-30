import FWCore.ParameterSet.Config as cms

process = cms.Process("ParameterPointProducer")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'dcap:///pnfs/cms/WAX/11/store/user/susygen/crsilk/SMS-T1tttt_Mgluino-350to1200_mLSP-0to850_8TeV-Pythia6Z/SMS-T1tttt_Mgluino-350to1200_mLSP-0to850_8TeV-Pythia6Z/696dafcd6650523e9b5c9c945a765dfa/T1tttt_500_100_0_1_1_5vt.root'
    )
)

process.parameterPointProducer = cms.EDProducer('ParameterPointProducer',
	source = cms.InputTag("source")
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('test.root'),
	SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('p'))
)

  
process.p = cms.Path(process.parameterPointProducer)

process.e = cms.EndPath(process.out)
