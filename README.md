# PYTHIA8 D0 Production

This code simulates proton-electron collisions using Pythia8 and saves in HepMC format only the events where a D0 is produced.

## Installation

1) Get and run [eic-shell](https://eic.github.io/tutorial-setting-up-environment/02-eic-shell/index.html)

2) Clone and make pythia<Version>
```
git clone https://gitlab.com/Pythia8/releases.git --branch <pythia_version> <pythia_version>
cd <pythia_version>
./configure --with-hepmc3
make
cd bin
export LD_LIBRARY_PATH=$(./pythia8-config --libdir):${LD_LIBRARY_PATH}
cd ../../
```

3) Clone and compile D0Gen against Pythia
```
git clone https://gitlab.com/eic/D0Gen.git --branch <D0Gen_version> --single-branch
cd D0Gen
g++ main41_D0.cc -o main41_D0 `pythia8-config --cxxflags --libs --hepmc3`
```

For example:
<D0Gen_version> = pythia8.306-1.0 corresponds to <pythia_version> = pythia8306

## Usage

1) Execute D0 generator
```
./main41_D0
```
The code will generate four HepMC files, each containing events with a D0. The code will stop when the desired number of D0s produced (D0count) is reached. You can modify the value of D0count in the code to change the number of events you want to generate.

2) Take the generated outputs and run [afterburner](https://github.com/eic/afterburner) with the right setting for example
```
  abconv -p ip6_hidiv_275x18 hepmc_D01.dat
```
