#include "Data.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"
#include "TGraph2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TNtuple.h"
#include "TROOT.h"
#include "TTree.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <functional>

using namespace std;

Data::Data()
{}

Data::Data(const char* filename)
{
    c1 = 0;
    rootFile = 0;

    LoadData(filename);
}



void Data::LoadData(const char* filename)
{
    rootFile = TFile::Open(filename);
    if (!rootFile) {
        string msg = "Unable to open "; msg += filename;
        throw runtime_error(msg.c_str());
    }

    for (int i = 0; i < sizeof(fLegs) / sizeof(fLegs[0]); ++i) {
      fLegs[i] = nullptr;
    }

    cout << "loading " << filename << endl;
    tAtarHits_ = rootFile->Get<TTree>("atarHits");
    tAtarHits_->SetBranchAddress("np", &fAtarHit.np);
    tAtarHits_->SetBranchAddress("pdgid", fAtarHit.pdgid);
    tAtarHits_->SetBranchAddress("x", &fAtarHit.x);
    tAtarHits_->SetBranchAddress("y", &fAtarHit.y);
    tAtarHits_->SetBranchAddress("z", &fAtarHit.z);
    tAtarHits_->SetBranchAddress("dedx", &fAtarHit.dedx);
    tAtarHits_->GetEntry(0);
    std::cout << fAtarHit.np << std::endl;
}

void Data::DrawAtarHits(int ipad)
{
  TVirtualPad *pad = c1->cd(ipad);

  if (fLegs[ipad]) delete fLegs[ipad];
  fLegs[ipad] = new TLegend(0.75, 0.9 - 0.06 * fAtarHit.np, 0.9, 0.9);
  std::map<int, TGraph2D *> graphs;

  for (int i = 0; i < fAtarHit.np; ++i) {
    const auto np = fAtarHit.x->at(i).size();
    std::string name = ::Form("hAtarHit%dPad1", fAtarHit.pdgid[i]);
    TGraph2D *g = (TGraph2D*)gROOT->FindObject(name.c_str());
    if (g) {
      delete g;
    }
    if (np == 0) continue;
    g = new TGraph2D(np);
    g->SetName(name.c_str());
    for (size_t j = 0; j < np; ++j) {
      g->SetPoint(j, fAtarHit.x->at(i).at(j),
                  fAtarHit.y->at(i).at(j), fAtarHit.z->at(i).at(j));
    }
    graphs.insert({fAtarHit.pdgid[i], g});
  }

  auto hist = (TH2F*) graphs.begin()->second->GetHistogram();
  hist->GetXaxis()->SetLimits(-1.5, 1.5);
  hist->GetYaxis()->SetLimits(-1.5, 1.5);
  hist->GetZaxis()->SetLimits(-1, 7.5);
  hist->SetTitle("ATAR Hits;X;Y;Z");
  graphs.begin()->second->Draw("P");

  std::map<int, Color_t> colors;
  colors[211] = kRed;
  colors[-11] = kBlue;
  colors[22] = kGreen+3;

  for (auto it = graphs.begin(); it != graphs.end(); ++it) {
    it->second->SetMarkerColor(colors[it->first]);
    fLegs[ipad]->AddEntry(it->second,
                          std::to_string(it->first).c_str(), "p");
    if (it == graphs.begin())
      continue;
    it->second->Draw("PSAME");
  }
  fLegs[ipad]->Draw();
  pad->SetGridx();
  pad->SetGridy();
  pad->Modified();
  pad->Update();
}
void Data::Draw1(int n)
{
    DrawAtarHits(n);
}


Data::~Data()
{
    delete rootFile;
}
