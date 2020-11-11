#Based on commands for standard recipe from runTheMatrix.py: 10824.0 TTbar_13TeV_TuneCUETP8M1_2018_GenSimFull+DigiFull_2018+RecoFull_2018+HARVESTFull_2018+ALCAFull_2018+NanoFull_2018 
#[1]: 
touch seeds.txt
echo "Running $1 jobs"
for i in {1..$1}
do
    cmsRun TTbar_13TeV_TuneCUETP8M1_cfi_GEN_SIM.py \
    outputFile=TTbar_13TeV_TuneCUETP8M1_GEN_SIM_${i}.root &
done
wait


#[2]: 
for i in {1..$1}
do
    cmsRun step2_DIGI_L1_DIGI2RAW_HLT.py \
    inputFiles=file:TTbar_13TeV_TuneCUETP8M1_GEN_SIM_${i}.root \
    outputFile=TTbar_13TeV_TuneCUETP8M1_FEVT_${i}.root &
done
wait
rm TTbar_13TeV_TuneCUETP8M1_GEN_SIM_*.root

#[3]: 
for i in {1..$1}
do
    cmsRun step3_RAW2DIGI_L1Reco_RECO_RECOSIM_EI_PAT.py \
    inputFiles=file:TTbar_13TeV_TuneCUETP8M1_FEVT_${i}.root \
    outputFile=TTbar_13TeV_TuneCUETP8M1_RECOSIM_${i}.root &
done
wait

rm TTbar_13TeV_TuneCUETP8M1_FEVT*.root
