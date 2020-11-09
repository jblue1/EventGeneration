#Based on standard recipe from runTheMatrix.py: 10824.0 TTbar_13TeV_TuneCUETP8M1_2018_GenSimFull+DigiFull_2018+RecoFull_2018+HARVESTFull_2018+ALCAFull_2018+NanoFull_2018 
#[1]: 
#for i in {1..10}
#do
cmsDriver.py TTbar_13TeV_TuneCUETP8M1_cfi  --conditions \
auto:phase1_2018_realistic -n 10 --era Run2_2018 \
--eventcontent FEVTDEBUG -s GEN,SIM --datatier GEN-SIM --beamspot Realistic25ns13TeVEarly2018Collision --geometry DB:Extended \
--fileout file:TTbar_13TeV_TuneCUETP8M1_2018_GEN_SIM.root --no_exec


                                           
#[2]: 
#for i in {1..10}
#do
cmsDriver.py step2  --conditions auto:phase1_2018_realistic -s DIGI:pdigi_valid,L1,DIGI2RAW,HLT:@relval2018 \
--datatier GEN-SIM-DIGI-RAW -n 10 --geometry DB:Extended --era Run2_2018 --eventcontent FEVTDEBUGHLT \
--filein file:TTbar_13TeV_TuneCUETP8M1_2018_GEN_SIM.root --fileout \
file:TTbar_13TeV_TuneCUETP8M1_2018_FEVT.root --no_exec
#done
#wait
#
#[3]: 
#for i in {1..10}
#do
cmsDriver.py step3  --conditions auto:phase1_2018_realistic -n 10 --era Run2_2018 --eventcontent RECOSIM,AODSIM,DQM \
--runUnscheduled  -s RAW2DIGI,L1Reco,RECO,RECOSIM,EI,PAT --datatier \
GEN-SIM-RECO,AODSIM,DQMIO --geometry DB:Extended --filein \
file:TTbar_13TeV_TuneCUETP8M1_2018_FEVT.root --fileout \
file:TTbar_13TeV_TuneCUETP8M1_2018_RECOSIM.root --no_exec
#done
