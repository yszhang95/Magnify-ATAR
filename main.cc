#include <RtypesCore.h>
#include <iostream>
#include "Data.h"
#include "GuiController.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"

void test_data() {
  // https://root-forum.cern.ch/t/fill-a-tbranch-with-a-multi-dimensional-std-vector/33808
  gInterpreter->GenerateDictionary("vector<vector<float>>", "vector");
  TFile f("output.root", "recreate");
  TTree hits("atarHits", "atar G4Hits");
  int np = 3; // number of particles in an event
  int pdgid[20] = {0};
  std::vector<std::vector<Float_t > > x;
  std::vector<std::vector<Float_t > > y;
  std::vector<std::vector<Float_t> > z;
  std::vector<std::vector<Float_t > > t;
  std::vector<std::vector<Float_t > > dedx;

  hits.Branch("ncluster", &np, "ncluster/I");
  hits.Branch("clusterid", pdgid, "clusterid[ncluster]/I");
  hits.Branch("x", &x);
  hits.Branch("y", &y);
  hits.Branch("z", &z);
  hits.Branch("t", &t);
  hits.Branch("de", &dedx);

  std::vector<float> xtemp, ytemp, ztemp, ttemp, dedxtemp;
  // pion
  pdgid[0] = 211;
  xtemp.clear();
  ytemp.clear();
  ztemp.clear();
  dedxtemp.clear();
  for (int i = 0; i < 30; ++i) {
    xtemp.push_back(i * 0.001);
    ytemp.push_back(i * 0.001);
    ztemp.push_back(i * 0.12);
    ttemp.push_back(0.1);
    dedxtemp.push_back(20 * 3.8);
  }
  x.push_back(xtemp);
  y.push_back(ytemp);
  z.push_back(ztemp);
  t.push_back(ttemp);
  dedx.push_back(dedxtemp);
  // positron
  pdgid[1] = -11;
  xtemp.clear();
  ytemp.clear();
  ztemp.clear();
  ttemp.clear();
  dedxtemp.clear();
  for (int i = 0; i < 5; ++i) {
    xtemp.push_back(x.at(0).back() + i * 0.1);
    ytemp.push_back(y.at(0).back() - i * 0.1);
    ztemp.push_back(z.at(0).back() + i * 0.12);
    ttemp.push_back(500);
    dedxtemp.push_back(3.8);
  }
  for (int i = 0; i < 25; ++i) {
    xtemp.push_back(xtemp.back() - 0.1);
    ytemp.push_back(ytemp.back() - 0.05);
    ztemp.push_back(ztemp.back() + 0.12);
    ttemp.push_back(700);
    dedxtemp.push_back(3.8);
  }
  x.push_back(xtemp);
  y.push_back(ytemp);
  z.push_back(ztemp);
  t.push_back(ttemp);
  dedx.push_back(dedxtemp);

  // photon
  pdgid[2] = 22;
  xtemp.clear();
  ytemp.clear();
  ztemp.clear();
  dedxtemp.clear();
  x.push_back(xtemp);
  y.push_back(ytemp);
  z.push_back(ztemp);
  t.push_back(ztemp);
  dedx.push_back(dedxtemp);

  hits.Fill();

  f.cd();
  hits.Write();
}

int main(int argc, char** argv)
{
  std::cout << "[INFO] Starting Magnify-ATAR\n";
  if (argc > 2) {
    std::cerr << "[ERROR] Multiple input files are given."
      " Only the first one will be read.\n";
  }
  if (argc == 1) {
    test_data();
    std::cout << "[INFO] No input is given. Use the test data instead.\n";
  }
  const std::string input = argc == 1 ? "output.root" : argv[1];

  TApplication theApp("App", &argc, argv);

  GuiController *gc = new GuiController(gClient->GetRoot(), 1600, 900,
                                        input.c_str());
  theApp.Run();
  return 0;
}
