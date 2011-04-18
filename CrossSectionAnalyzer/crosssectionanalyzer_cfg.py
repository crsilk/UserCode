import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:m0_500_m12_100_sgn_1_tanb_3_A0_0_GEN_SIM_HLT.root'
    )
)

process.demo = cms.EDAnalyzer('CrossSectionAnalyzer',
							  outputFile = cms.untracked.string('CrossSections.root')
)


process.p = cms.Path(process.demo)
