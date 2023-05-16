import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonExercise1")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit = cms.untracked.int32(0)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False),
SkipEvent = cms.untracked.vstring('ProductNotFound'))

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('file:/eos/home-c/cmsdas/2023/short-ex-muon/dymm.root'))

process.demo = cms.EDAnalyzer('MuonExercise1')


process.TFileService = cms.Service("TFileService",
          fileName = cms.string('histos1.root')
)

process.p = cms.Path(process.demo)
