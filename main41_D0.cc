// main41.cc is a part of the PYTHIA event generator.
// Copyright (C) 2021 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Authors: Mikhail Kirsanov <Mikhail.Kirsanov@cern.ch>.

// Keywords: basic usage; hepmc;

// This program illustrates how HepMC can be interfaced to Pythia8.
// It studies the charged multiplicity distribution at the LHC.
// HepMC events are output to the hepmcout41.dat file.

// WARNING: typically one needs 25 MB/100 events at the LHC.
// Therefore large event samples may be impractical.

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
  Pythia8::Pythia8ToHepMC FirstFile("hepmc_D01.dat");
  Pythia8::Pythia8ToHepMC SecondFile("hepmc_D02.dat");
  Pythia8::Pythia8ToHepMC ThirdFile("hepmc_D03.dat");
  Pythia8::Pythia8ToHepMC FourthFile("hepmc_D04.dat");

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
  int D0count = 0;
  //for (int iEvent = 0; iEvent < 10000; ++iEvent) {
  while (D0count < 10){
    if (!pythia.next()) continue;
    Pythia8::Event &event = pythia.event;

    bool hasD0 = false;

    for (int i=0; i<event.size(); i++){
        Pythia8::Particle &currentParticle = event[i];
        if (abs(currentParticle.id()) == 421){
            hasD0 = true;
            D0count++;
            //break;
        }
    }

    // Construct new empty HepMC event, fill it and write it out.
    if (hasD0){
      if (D0count %4 == 0) FirstFile.writeNextEvent(pythia);
      if (D0count %4 == 1) SecondFile.writeNextEvent(pythia);
      if (D0count %4 == 2) ThirdFile.writeNextEvent(pythia);
      if (D0count %4 == 3) FourthFile.writeNextEvent(pythia);
    } 


  // End of event loop. Statistics. Histogram.
  }
  pythia.stat();
  if (D0count %100000 == 0) cout<<"D0 count = "<<D0count<<endl;

  // Done.
  return 0;
}
