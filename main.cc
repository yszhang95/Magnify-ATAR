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
  TTree t("atarHits", "atar G4Hits");
  int np = 3; // number of particles in an event
  int pdgid[20] = {0};
  std::vector<std::vector<Float_t > > x;
  std::vector<std::vector<Float_t > > y;
  std::vector<std::vector<Float_t > > z;
  std::vector<std::vector<Float_t > > dedx;

  t.Branch("np", &np, "np/I");
  t.Branch("pdgid", pdgid, "pdgid[np]/I");
  t.Branch("x", &x);
  t.Branch("y", &y);
  t.Branch("z", &z);
  t.Branch("dedx", &dedx);

  std::vector<float> xtemp, ytemp, ztemp, dedxtemp;
  // pion
  pdgid[0] = 211;
  xtemp.clear(); ytemp.clear(); ztemp.clear(); dedxtemp.clear();
  for (int i = 0; i < 30; ++i) {
    xtemp.push_back(i * 0.001);
    ytemp.push_back(i * 0.001);
    ztemp.push_back(i * 0.12);
    std::cout << ztemp.back() << "\n";
    dedxtemp.push_back(20 * 3.8);
  }
  x.push_back(xtemp);
  y.push_back(ytemp);
  z.push_back(ztemp);
  dedx.push_back(dedxtemp);
  // positron
  pdgid[1] = -11;
  xtemp.clear();
  ytemp.clear();
  ztemp.clear();
  dedxtemp.clear();
  for (int i = 0; i < 5; ++i) {
    xtemp.push_back(x.at(0).back() + i * 0.1);
    ytemp.push_back(y.at(0).back() - i * 0.1);
    ztemp.push_back(z.at(0).back() + i * 0.12);
    dedxtemp.push_back(3.8);

        std::cout << ztemp.back() << "\n";
  }
  for (int i = 0; i < 25; ++i) {
    xtemp.push_back(xtemp.back() - 0.1);
    ytemp.push_back(ytemp.back() - 0.05);
    ztemp.push_back(ztemp.back() + 0.12);
    dedxtemp.push_back(3.8);
  }
  x.push_back(xtemp);
  y.push_back(ytemp);
  z.push_back(ztemp);
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
  dedx.push_back(dedxtemp);

  t.Fill();

  f.cd();
  t.Write();
}

int main(int argc, char** argv)
{
  std::cout << "[INFO] Starting Magnify-ATAR\n";
  test_data();

  TApplication theApp("App", &argc, argv);

  GuiController *gc = new GuiController(gClient->GetRoot(), 1600, 900,
                                        "output.root");
  theApp.Run();
  return 0;
}
