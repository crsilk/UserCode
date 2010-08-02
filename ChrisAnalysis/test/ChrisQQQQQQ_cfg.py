import FWCore.ParameterSet.Config as cms



#########################################################
# IMPORTANT: 
# this file can be used with cmsRun (full framework), or
# with readEventsDemo (FWLite, see ../bin/ directory )

process = cms.Process("ANA")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10

#########################################################
# Configuration of the FWLiteTreeAnalyzer,
# USED ONLY IN FWLITE MODE

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


#process.load("PFAnalyses.ChrisAnalysis.Sources.source_chris_test_cff")
readFiles = cms.untracked.vstring()

process.source = cms.Source("PoolSource",
    fileNames = readFiles)
readFiles.extend([ 'file:$CMSSW_BASE/src/PhysicsTools/PatAlgos/test/SingleGammaPtQQQQQQ_PF2PAT.root'])





# BEGINNERS, DON'T HAVE A LOOK AT THIS PART YET. 
##In the metadata we pass the preselection efficiency and the
##sample name. The sample name will be added to the name of the
##output ROOT file with the histograms
##The output file location is defined by outputPath parameter
process.configurationMetadata = cms.untracked.PSet(
     version = cms.untracked.string('1.0'),
     name = cms.untracked.string('SingleGamma'),
     annotation = cms.untracked.string('None'),
     # If you don't know what it is, you don't care yet :)
     preselectionEff = cms.untracked.double(1.0),
     outputPath = cms.untracked.string('./'),
 )

process.Summary =  cms.PSet(
    selectionFlavours = cms.untracked.vstring("")
    )

########################################################
# Configuration of the ChrisAnalyzer used
# in the example analysis, USED ONLY IN FWLITE MODE
# There is no path, and no sequence in FWLite mode.
# Please have a look at
#   ../bin/readEventsDemo.cc
#   ../src/ChrisAnalyzer.cc
# for more information 

# master verbose flag
verbose = False

process.ChrisAnalyzer =  cms.PSet(
    verbose  = cms.bool( verbose ),
    photonMatchLabel = cms.InputTag("photonMatch"),    
    munLabel = cms.InputTag("selectedPatMuons"),
    jetLabel = cms.InputTag("selectedPatJets"),
    pfCandidateLabel = cms.InputTag("particleFlow"),
    genParticleLabel = cms.InputTag("genParticles"),
    pfParticleLabel = cms.InputTag("patPFParticles"),
    photonLabel =cms.InputTag("photons"),
    # met selection is done directly
    minMET = cms.double(20.0),

    eleSelector = cms.PSet(
       version = cms.int32(0),
       mvaCut = cms.double(-9999),
       maxMissingHitCut = cms.double(999),
       verbose = cms.untracked.bool( False )
       ),
    eleIsoSelector = cms.PSet(
       verbose = cms.untracked.bool( False ),

       isolationCuts = cms.VPSet(
        cms.PSet(
          name = cms.string("combinedFlorent"), 
          absoluteCut = cms.double(5),
          relativeCut = cms.double(0.04),
          chargedWeight = cms.double(1),
          photonWeight = cms.double(1),
          neutralWeight = cms.double(0.33)
          ),
        cms.PSet(
          name = cms.string("charged"), 
          absoluteCut = cms.double(5),
          relativeCut = cms.double(0.14),
          chargedWeight = cms.double(1),
          photonWeight = cms.double(0),
          neutralWeight = cms.double(0)
          )
       ),
       chargedIsoPar = cms.PSet(
        coneSize = cms.double(0.4), 
        threshold = cms.double(0.),
       ),
       neutralIsoPar = cms.PSet(
        coneSize = cms.double(0.4), 
        threshold = cms.double(0.5),
       ),
       photonsIsoPar = cms.PSet(
        coneSize = cms.double(0.4), 
        threshold = cms.double(0.5),
       ) 
   )

)


