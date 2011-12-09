import FWCore.ParameterSet.Config as cms

from Configuration.Generator.PythiaUESettings_cfi import *

process = cms.Process('HLT')


process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('FastSimulation.Configuration.EventContent_cff')
process.load('FastSimulation.PileUpProducer.PileUpSimulator_FlatDist10_2011EarlyData_50ns_cff')
process.load('FastSimulation.Configuration.Geometries_START_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('FastSimulation.Configuration.FamosSequences_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedParameters_cfi')
process.load('FastSimulation.Configuration.HLT_GRun_cff')
process.load('FastSimulation.Configuration.Validation_cff')
process.load('DQMOffline.Configuration.DQMOfflineMC_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
)


process.load('LHESOURCEFILE')


process.generator = cms.EDFilter("Pythia6HadronizerFilter",
   pythiaHepMCVerbosity = cms.untracked.bool(False),
   maxEventsToPrint = cms.untracked.int32(1),
   pythiaPylistVerbosity = cms.untracked.int32(0),
   comEnergy = cms.double(7000.0),
   PythiaParameters = cms.PSet(
       pythiaUESettingsBlock,
       processParameters = cms.vstring('MSEL=0         ! User defined processes',
                       'PMAS(5,1)=4.4   ! b quark mass',
                       'PMAS(6,1)=172.5 ! t quark mass',
                       'MSTJ(1)=1       ! Fragmentation/hadronization on or off',
                       'PARP(67)=100.0',
                       'MSTP(61)=1      ! Parton showering on or off'),
       parameterSets = cms.vstring('pythiaUESettings',
           'processParameters')
   )
)

process.options.SkipEvent = cms.untracked.vstring('ProductNotFound')
process.configurationMetadata = cms.untracked.PSet(
   version = cms.untracked.string('$Revision: 1.303 $'),
   annotation = cms.untracked.string('GEN-fragment nevts:1'),
   name = cms.untracked.string('PyReleaseValidation')
)


process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
   eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
   outputCommands = process.AODSIMEventContent.outputCommands,
   fileName = cms.untracked.string(OUPTUTFILE),
   dataset = cms.untracked.PSet(
       filterName = cms.untracked.string(''),
       dataTier = cms.untracked.string('AODSIM')
   ),
   SelectEvents = cms.untracked.PSet(
       SelectEvents = cms.vstring('generation_step')
   )
)

process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.simulation = cms.Sequence(process.simulationWithFamos)
process.HLTEndSequence = cms.Sequence(process.reconstructionWithFamos)
process.Realistic7TeV2011CollisionVtxSmearingParameters.type = cms.string("BetaFunc")
process.famosSimHits.VertexGenerator = process.Realistic7TeV2011CollisionVtxSmearingParameters
process.famosPileUp.VertexGenerator = process.Realistic7TeV2011CollisionVtxSmearingParameters
process.mix.playback = True
process.GlobalTag.globaltag = 'START42_V11::All'

process.generation_step = cms.Path(process.generator)
process.reconstruction = cms.Path(process.reconstructionWithFamos)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.validation_step = cms.EndPath(process.validation_prod)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.reconstruction,process.validation_step,process.endjob_step,process.AODSIMoutput_step])

