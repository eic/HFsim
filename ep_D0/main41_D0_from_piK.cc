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


//#include "Pythia8Plugins/ROOT.h"


#include "Pythia8/Pythia.h"
#ifndef HEPMC2
#include "Pythia8Plugins/HepMC3.h"
#else
#include "Pythia8Plugins/HepMC2.h"
#endif


#include <chrono>
#include <iostream>
#include <cstdio>  // For std::rename


using namespace Pythia8;
using namespace std::chrono;


bool areVectorsDifferentIgnoreOrder(vector<int> vec1, vector<int> vec2) {
  //if (vec1.size() != vec2.size()) return true;
  sort(vec1.begin(), vec1.end());
  sort(vec2.begin(), vec2.end());
  return vec1 != vec2;
}


int main() {


  // Start measuring time
  auto start = high_resolution_clock::now();


  // Interface for conversion from Pythia8::Event to HepMC
  // event. Specify file where HepMC events will be stored.
  Pythia8::Pythia8ToHepMC FirstFile("output/hepmc_D01.dat");
  Pythia8::Pythia8ToHepMC SecondFile("output/hepmc_D02.dat");
  Pythia8::Pythia8ToHepMC ThirdFile("output/hepmc_D03.dat");
  Pythia8::Pythia8ToHepMC FourthFile("output/hepmc_D04.dat");
  Pythia8::Pythia8ToHepMC FifthFile("output/hepmc_D05.dat");
  Pythia8::Pythia8ToHepMC SixthFile("output/hepmc_D06.dat");
  Pythia8::Pythia8ToHepMC SeventhFile("output/hepmc_D07.dat");
  Pythia8::Pythia8ToHepMC EighthFile("output/hepmc_D08.dat");
  Pythia8::Pythia8ToHepMC NinthFile("output/hepmc_D09.dat");
  Pythia8::Pythia8ToHepMC TenthFile("output/hepmc_D10.dat");


  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;
  pythia.readFile("config_10x100_Q2min1.cmnd");
  pythia.readString("Print:quiet = on"); // Set Pythia to quiet mode
  pythia.init();


  // Begin event loop. Generate event. Skip if error.
  int Mothercount = 0;
  int EventsCount = 0;
  const int Total_Count = 1000000;


  while (Mothercount < Total_Count) {
    if (!pythia.next()) continue;
    Pythia8::Event &event = pythia.event;


    bool hasMother = false;
    EventsCount++;


    vector<int> daughterPDGs = {211, 321};
    const int MotherPDG = 421;
    float EtaMax = 3.5;


    for (int i = 0; i < event.size(); i++) {
      Pythia8::Particle &currentParticle = event[i];
      if (abs(currentParticle.id()) != MotherPDG) continue;
      vector<int> daughters = currentParticle.daughterList();
      if (daughters.size() != daughterPDGs.size()) continue;
      vector<int> daughtersID = {abs(pythia.event[daughters[0]].id()), abs(pythia.event[daughters[1]].id())};
      if (areVectorsDifferentIgnoreOrder(daughtersID, daughterPDGs)) continue;
      bool DaughterOutOfRange = false;
      for (int &daughter : daughters) {
        if (fabs(pythia.event[daughter].eta()) > EtaMax) {
          DaughterOutOfRange = true;
          break;
        }
      }
      if (DaughterOutOfRange) continue;
      Mothercount++;
      hasMother = true;
    }


    // Construct new empty HepMC event, fill it and write it out.
    if (hasMother) {
      if (Mothercount % 10 == 0) FirstFile.writeNextEvent(pythia);
      if (Mothercount % 10 == 1) SecondFile.writeNextEvent(pythia);
      if (Mothercount % 10 == 2) ThirdFile.writeNextEvent(pythia);
      if (Mothercount % 10 == 3) FourthFile.writeNextEvent(pythia);
      if (Mothercount % 10 == 4) FifthFile.writeNextEvent(pythia);
      if (Mothercount % 10 == 5) SixthFile.writeNextEvent(pythia);
      if (Mothercount % 10 == 6) SeventhFile.writeNextEvent(pythia);
      if (Mothercount % 10 == 7) EighthFile.writeNextEvent(pythia);
      if (Mothercount % 10 == 8) NinthFile.writeNextEvent(pythia);
      if (Mothercount % 10 == 9) TenthFile.writeNextEvent(pythia);
    }


    if (EventsCount % 1770130 == 0) cout << "Mother count = " << Mothercount << endl;
  }


  // End of event loop. Statistics. Histogram.
  pythia.stat();
  cout << "Total events needed = " << EventsCount << endl;


  // Stop measuring time and calculate the elapsed time
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<seconds>(stop - start);
  int hours = duration.count() / 3600;
  int minutes = (duration.count() % 3600) / 60;
  int seconds = duration.count() % 60;


  // Print the elapsed time
  std::cout << "Time taken by function: "
            << hours << " hours, "
            << minutes << " minutes, "
            << seconds << " seconds" << std::endl;


  // Rename the output files
  for (int i = 1; i <= 10; ++i) {
    char oldFilename[50];
    snprintf(oldFilename, sizeof(oldFilename), "output/hepmc_D%02d.dat", i);


    char newFilename[256];
    snprintf(newFilename, sizeof(newFilename),
             "output/hepmc_file%02d_D0_from_piK_PYTHIA8_ep_NC_10x100_Q2min1_EtaMax3p5_%dh%dm%ds_%dEvents.dat",
             i, hours, minutes, seconds, EventsCount);


    if (std::rename(oldFilename, newFilename) == 0) {
      std::cout << "File renamed to: " << newFilename << std::endl;
    } else {
      std::cerr << "Error renaming file: " << oldFilename << std::endl;
    }
  }


  // Done.
  return 0;
}