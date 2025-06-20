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

bool areVectorsDifferentIgnoreOrder(vector<int> vec1, vector<int> vec2)
{
  if (vec1.size() != vec2.size()) return true;
  sort(vec1.begin(), vec1.end());
  sort(vec2.begin(), vec2.end());
  return vec1 != vec2;
}

vector<int> relevantPDGs = {211,  321};

int main(int argc, char *argv[])
{

  if(argc!=2)
    {
      cout << "[e] Provide output file name" << endl;
      return 0;
    }
  
  // Interface for conversion from Pythia8::Event to HepMC
  // event. Specify file where HepMC events will be stored.
  
  Pythia8::Pythia8ToHepMC OutFile(argv[1]);

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;
  pythia.readFile("config_5x41_Q2min1.cmnd");
  pythia.readString("Print:quiet = on"); // Set Pythia to quiet mode
  pythia.init();

  // Begin event loop. Generate event. Skip if error.
  int Mothercount = 0;
  int EventsCount = 0;
  const int Total_Count = 2.5e6;

  while (EventsCount < Total_Count)
    {
      if (!pythia.next()) continue;
      Pythia8::Event &event = pythia.event;
      bool hasMother = false;
      EventsCount++;

      for (int i=0; i<event.size(); i++)
	{
	  Pythia8::Particle &currentParticle = event[i];
	  if (abs(currentParticle.id()) != 421) continue;
	  vector<int> daughters = currentParticle.daughterList();
	  if (daughters.size() != 2) continue;
	  vector<int> daughtersID = {abs(pythia.event[daughters[0]].id()), abs(pythia.event[daughters[1]].id())};
	  if (areVectorsDifferentIgnoreOrder(daughtersID, relevantPDGs)) continue;
	     
	  printf("Mother %s, Daughters: %s %s\n",
		 pythia.particleData.name(currentParticle.id()).c_str(),
		 pythia.particleData.name(pythia.event[daughters[0]].id()).c_str(),
		 pythia.particleData.name(pythia.event[daughters[1]].id()).c_str());
	  
	  Mothercount++;
	  hasMother = true;
	  break;
	}
      
    // Construct new empty HepMC event, fill it and write it out.
    if (hasMother)
      {
	OutFile.writeNextEvent(pythia);
      } 
    // End of event loop. Statistics. Histogram.
    }
  pythia.stat();

  cout<<"Total events scanned = "<<EventsCount<<", D0 found = " << Mothercount << endl;
  // Done.
  return 0;
}
