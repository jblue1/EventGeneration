import FWCore.ParameterSet.Config as cms
#import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# load filelist
#fileList = FileUtils.loadListFromFile("filelist.txt")
prefix = '../../../genData/ttbar/run7'
process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_1.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_2.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_3.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_4.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_5.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_6.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_7.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_8.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_9.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_10.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_11.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_12.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_13.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_14.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_15.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_16.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_17.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_18.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_19.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_20.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_21.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_22.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_23.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_24.root',
                                    'file:' + prefix + '/TTbar_13TeV_TuneCUETP8M1_RECOSIM_25.root',
                                )
                                #fileNames = cms.untracked.vstring(
                                #'file:TTbar_13TeV_TuneCUETP8M1_2018_RECOSIM_2_inAODSIM.root')
                            )
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

process.demo = cms.EDAnalyzer('MyAnalyzer',
    ak4PFJets=cms.InputTag("ak4PFJets"),
    ak4PFJetsCHS=cms.InputTag("ak4PFJetsCHS"),
    ak4GenJets=cms.InputTag("ak4GenJets"),
    genParticles=cms.InputTag("genParticles"))

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("out.root")
    )

process.p = cms.Path(process.demo)
