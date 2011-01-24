import FWCore.ParameterSet.Config as cms

#define the name of the process, change it if you want to####
process = cms.Process("NEWNAME")

#number of events to process#################################
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

######## input files #########################################
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring('file:/uscms/home/cavanaug/CMSDAS/dipion-gun-no-mat.root'),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
    )

###load the ToyPFProducer. Its parameters are set in ToyPF/Configuration/python/ToyPF_cff
process.load("ToyPF.Configuration.ToyPF_cff")

###load the ToyPFMETProducer. Not needed until part 3
##process.load("ToyPF.Configuration.ToyPFMET_cff")


####### define the path = the module(s) to be executed #######
process.toyPF = cms.Path(process.toyPFProducer)

##process.toyPF = cms.Path( process.toyPFProducer + process.toyPFMETProducer ) ##path for part 3


####### output commands ######################################
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('ToyPF.root'),
                               outputCommands = cms.untracked.vstring('keep recoPFCandidates_toyPFProducer__NEWNAME',
                                                                      'keep recoMETs_toyPFMETProducer__NEWNAME',
                                                                      'keep recoPFCandidates_particleFlow__PROD',
                                                                      'keep recoPFMETs_pfMet__PROD',
                                                                      'keep recoCaloMETs_met__PROD')
                               )


process.outpath = cms.EndPath(process.out)
