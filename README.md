# EventGeneration
Scripts for generating data and producing a ROOT file with 4-momentum information for partons, gen jets and reco jets from the generated events.
Requires a working CMSSW environment (tested on CMSSW_10_2_24). Generated events used for project described [here](https://github.com/jblue1/falcon).

### Use
#### Event generation
Event generation is based on a standard recipe from [runTheMatrix.py](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFastSimulationExamples). The cmsDriver
commands used to create the configuration files can be seen in genData/example/makeConfigs.sh. These config files were then edited in order to be run in parallel, which
are done in batches of 2000. For example, to generate 10,000 events, make the working directory genData/example/ and run ```source parallelDriver.sh 5``` (the number of jobs
is an argument passed to the bash script). The random seeds used for each step of the generation process is stored in seeds.txt. ROOT files produced by the 
intermediate steps of the generation process are deleted, and the final output will be a set of ROOT files in AODSIM format.

#### Producing a smaller ROOT file
The rest of this repo is a CMSSW module for producing small ROOT files containing the 4-momenta of the partons, gen jets, and reco jets from the generated events. To use
the module, first run ```scram b``` in the top level directory. Then move into the /python directory, and run ```touch filelist.txt```. Open this new file in your favorite
text editor, and add the files generated in the above step in the format ```file:path/to/your/file.root``` with each file on its own line. Then run
```cmsRun ConfFile_cfg.py``` which will produce a file out.root, containing a ROOT tree ("eventTree") with branches containing various variables (transverse momentum,
psuedo-rapidity, etc) of the jets and partons in the generated events. Each entry represents a single event. 
