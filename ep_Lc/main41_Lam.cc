// main41_Lam.cc is a modified version of main41.cc in the examples of the PYTHIA event generator.
// Copyright (C) 2021 Torbjorn Sjostrand.
// PYTHIA is licensed under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Authors: Enea Prifti, University of Illinois at Chicago.

// Keywords: basic usage; hepmc;

// This program simulates proton-electron collisions using Pythia8.
// It then saves in HepMC format only the events where a Lambda or anti-Lambda is produced.
//It continues to generate events until the desired number of lambdas produced (Lcount) is reached.
// HepMC events are output to 4 separate files to reduce the file size.

#include "Pythia8/Pythia.h"
#ifndef HEPMC2
#include "Pythia8Plugins/HepMC3.h"
#else
#include "Pythia8Plugins/HepMC2.h"
#endif

using namespace Pythia8;

int main() {

  // Interface for conversion from Pythia8::Event to HepMC
  // event. Specify file where HepMC events will be stored.
  Pythia8::Pythia8ToHepMC FirstFile("hepmc_Lambda1.dat");
  Pythia8::Pythia8ToHepMC SecondFile("hepmc_Lambda2.dat");
  Pythia8::Pythia8ToHepMC ThirdFile("hepmc_Lambda3.dat");
  Pythia8::Pythia8ToHepMC FourthFile("hepmc_Lambda4.dat");

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 11");
  pythia.readString("Beams:frameType = 2");
  pythia.readString("Beams:eA = 275");
  pythia.readString("WeakBosonExchange:ff2ff(t:gmZ) = on");
  pythia.readString("SpaceShower:dipoleRecoil = on");
  pythia.readString("Beams:eB = 18");
  pythia.readString("PhaseSpace:Q2min = 100");
  pythia.init();

  // Begin event loop. Generate event. Skip if error.
  int Lcount = 0;
  //for (int iEvent = 0; iEvent < 10000; ++iEvent) {
  while (Lcount < 10){
    if (!pythia.next()) continue;
    Pythia8::Event &event = pythia.event;

    bool hasLambda = false;

    for (int i=0; i<event.size(); i++){
        Pythia8::Particle &currentParticle = event[i];
        if (abs(currentParticle.id()) == 3122){
            hasLambda = true;
            Lcount++;
        }
    }

    // Construct new empty HepMC event, fill it and write it out.
    if (hasLambda){
      if (Lcount %4 == 0) FirstFile.writeNextEvent(pythia);
      if (Lcount %4 == 1) SecondFile.writeNextEvent(pythia);
      if (Lcount %4 == 2) ThirdFile.writeNextEvent(pythia);
      if (Lcount %4 == 3) FourthFile.writeNextEvent(pythia);
    } 


  // End of event loop. Statistics. Histogram.
  }
  pythia.stat();

  // Done.
  return 0;
}
