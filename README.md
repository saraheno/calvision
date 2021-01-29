
# Calvision
GEANT4-based simulation of energy deposition in a large crystal.

## Installing
On UMD T3 cluster:
```bash
source g4env.sh
cmake -DGeant4_DIR=/cvmfs/geant4.cern.ch/geant4/10.5/x86_64-slc6-gcc63-opt/lib64/GEANT4-10.5.0
cmake --build .
```

## Events running
For the interactive runing:
Check the geometry through a detector viewing window
```bash
./Calo -c template.cfg -u Xm  
```

`-c template.cfg` is the config file mainly for the DetectorConstruction

`-m run.mac` is the macro file for the initialization of G4GeneralParticleSource

`-o test` mean the results will be saved as `test.root`

Simulations can be run in this way:
```bash
./CEPC_CaloTiming -c template.cfg -m run.mac -o test
```

## Tools
`plot_Eion.C` in the `./tool` directory can be used to generate (1) fraction of ionizing enegry vs. partcile velocity for different particles and (2) number of inelastic interactions vs. proton ionizing energy deposition. 

You only need to change the root file in this line:
```C++
TFile *ffile = new TFile("test.root");
```

Other files in `./tool` are used to help generating condor files for massive simulations. 
Do:
```bash
source generate_temp.sh
```
it will generate files for job submission. 

More Branches could be added in `CreateTree.cc` and filled in `SteppingAction.cc`. 



