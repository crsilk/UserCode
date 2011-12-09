import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.maxEvents = cms.untracked.PSet(
	    input = cms.untracked.int32(NUMBEROFEVENTS)
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cerr']
process.MessageLogger.statistics = []
process.MessageLogger.fwkJobReports = []
process.MessageLogger.cerr.FwkReport.reportEvery = 10000


process.source = cms.Source("EmptySource")

from Configuration.Generator.PythiaUEZ2Settings_cfi import *


process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    comEnergy = cms.double(7000.0),
    pythiaToLHE = cms.bool(True),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        processParameters = cms.vstring(

'MSEL=0         ! User defined processes',
ALLOWEDSUBPROCESSES

'MSTP(161)=67',
'MSTP(162)=68',
'MSTP(163)=69',
'IMSS(1) = 11             ! Spectrum from external SLHA file',
'IMSS(21) = 33            ! LUN number for SLHA File (must be 33) ',
'IMSS(22) = 33            ! Read-in SLHA decay table '),
        SLHAParameters = cms.vstring(
		'SLHAFILE = SLHADIRECTORY/RUNSLHA'),
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters','SLHAParameters')
    )
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    moduleSeeds = cms.PSet(
        generator = cms.untracked.uint32(RANDOMSEEDJOBNUMBER),
        g4SimHits = cms.untracked.uint32(123456788),
        VtxSmeared = cms.untracked.uint32(123456789)
    ),
)

process.pp = cms.Path(process.generator)

process.schedule = cms.Schedule(process.pp)
