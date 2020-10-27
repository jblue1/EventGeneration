import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
                                'file:TTbar_13TeV_TuneCUETP8M1_2018_RECOSIM_2_inAODSIM.root')
                            )

process.demo = cms.EDAnalyzer('MyAnalyzer', 
    ak4PFJets=cms.InputTag("ak4PFJets"),
    ak4PFJetsCHS=cms.InputTag("ak4PFJetsCHS"),
    ak4GenJets=cms.InputTag("ak4GenJets"),
    genParticles=cms.InputTag("genParticles"))

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("out.root")
    )

process.p = cms.Path(process.demo)
