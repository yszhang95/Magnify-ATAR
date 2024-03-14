#include "Data.h"

#include "TCanvas.h"
#include "TPaletteAxis.h"
#include "TFile.h"
#include "TLegend.h"
#include "TGraph2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TNtuple.h"
#include "TROOT.h"
#include "TTree.h"
#include "TPolyMarker3D.h"

#include <TError.h>
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <functional>

using namespace std;

const map<int, Color_t> Data::colors_ = {};

Float_t Data::HitInfo::GetVal(std::string var, size_t i, size_t j)
{
  std::transform(var.begin(), var.end(), var.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  if (var == "x")
    return x->at(i).at(j);
  if (var == "y")
    return y->at(i).at(j);
  if (var == "z")
    return z->at(i).at(j);
  if (var == "t")
    return t->at(i).at(j);
  if (var == "de")
    return de->at(i).at(j);
  return 0;
}

Data::Data()
    : fXMin(-1.5), fXMax(1.5), fYMin(-1.5), fYMax(1.5), fZMin(-1), fZMax(7),
      fTMin(0), fTMax(2800), fDEMin(0), fDEMax(10), fNCluster(0), fClusterIdx(0), fDrawAll(true)
{
  c1 = 0;
  rootFile = 0;
  for (unsigned int i = 20; i != 20; ++i) {
    fLegs[i] = nullptr;
  }
}

Data::Data(const char *filename) : Data()
{
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

    std::cout << "[INFO] Loading " << filename << ".\n";
    tAtarHits_ = rootFile->Get<TTree>("atarHits");
    tAtarHits_->SetBranchAddress("ncluster", &fAtarHit.ncluster);
    tAtarHits_->SetBranchAddress("clusterid", fAtarHit.clusterid);
    tAtarHits_->SetBranchAddress("x", &fAtarHit.x);
    tAtarHits_->SetBranchAddress("y", &fAtarHit.y);
    tAtarHits_->SetBranchAddress("z", &fAtarHit.z);
    tAtarHits_->SetBranchAddress("t", &fAtarHit.t);
    tAtarHits_->SetBranchAddress("de", &fAtarHit.de);
    tAtarHits_->GetEntry(0);

    fNCluster = fAtarHit.ncluster;
}

Double_t Data::GetMin(std::string var)
{
  std::transform(var.begin(), var.end(), var.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  if (var == "x")
    return fXMin;
  if (var == "y")
    return fYMin;
  if (var == "z")
    return fZMin;
  if (var == "t")
    return fTMin;
  if (var == "de")
    return fDEMin;

  return 0;
}

Double_t Data::GetMax(std::string var)
{
  std::transform(var.begin(), var.end(), var.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  if (var == "x")
    return fXMax;
  if (var == "y")
    return fYMax;
  if (var == "z")
    return fZMax;
  if (var == "t")
    return fTMax;
  if (var == "de")
    return fDEMax;

  return 0;
}

void Data::DrawPoints(int ipad, const std::string x, const std::string y,
                      const std::string z)
{
  TVirtualPad *pad = c1->cd(ipad);
  if (fLegs[ipad]) delete fLegs[ipad];
  fLegs[ipad] = new TLegend(0.75, 0.9 - 0.06 * fAtarHit.ncluster, 0.9, 0.9);

  std::vector<TGraph2D*> graphs;

  for (int i = 0; i < fAtarHit.ncluster; ++i) {
    const auto npts = fAtarHit.x->at(i).size();
    std::string name = ::Form("hHit%s%svs%s_idx%d", x.c_str(), y.c_str(), z.c_str(), i);
    TGraph2D *g = (TGraph2D*)gROOT->FindObject(name.c_str());
    if (g) {
      delete g;
    }

    const bool notselected = !fDrawAll && i != fClusterIdx;
    if (npts == 0 || notselected) {
      g = nullptr;
    } else {
      g = new TGraph2D(npts);
      g->SetName(name.c_str());
      for (size_t j = 0; j < npts; ++j) {
        g->SetPoint(j, fAtarHit.GetVal(x, i, j), fAtarHit.GetVal(y, i, j),
                    fAtarHit.GetVal(z, i, j));
        // set minimum and maximum must be called before set limits...
        g->SetMinimum(GetMin(z));
        g->SetMaximum(GetMax(z));
        g->GetXaxis()->SetLimits(GetMin(x), GetMax(x));
        g->GetYaxis()->SetLimits(GetMin(y), GetMax(y));
      }
    }
    graphs.push_back(g);
  }
  bool framed = false;
  for (const auto g : graphs) {
    if (g) {
      if (framed) {
        g->Draw("PCOL SAME");
      } else {
        g->Draw("PCOL Z");
        g->SetTitle(::Form("Hits;%s;%s;%s;", x.c_str(), y.c_str(), z.c_str()));
        framed = true;
      }
    }
  }
  pad->SetTheta(90-0.0001);
  pad->SetPhi(0+0.0001);
  pad->SetGridx();
  pad->SetGridy();
  pad->Modified();
  pad->Update();
}

void Data::DrawHitXYvsT(int ipad) { DrawPoints(ipad, "X", "Y", "T"); }
void Data::DrawHitXZvsT(int ipad) { DrawPoints(ipad, "X", "Z", "T"); }
void Data::DrawHitYZvsT(int ipad) { DrawPoints(ipad, "Y", "Z", "T"); }
void Data::DrawHitXYvsDE(int ipad) { DrawPoints(ipad, "X", "Y", "dE"); }
void Data::DrawHitXZvsDE(int ipad) { DrawPoints(ipad, "X", "Z", "dE"); }
void Data::DrawHitYZvsDE(int ipad) { DrawPoints(ipad, "Y", "Z", "dE"); }

Data::~Data()
{
    delete rootFile;
}
