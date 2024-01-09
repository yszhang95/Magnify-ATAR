#ifndef DATA_H
#define DATA_H

#include "RtypesCore.h"
#include <vector>

class TFile;
class TCanvas;
class TTree;
class TLegend;

class Data {
public:
  struct HitInfo {
  public:
    int np;
    int pdgid[20];

    std::vector<std::vector<Float_t > >* x;
    std::vector<std::vector<Float_t > >* y;
    std::vector<std::vector<Float_t > >* z;
    std::vector<std::vector<Float_t > >* dedx;
    HitInfo() : np(0), x(0), y(0), z(0), dedx(0) {
      for (int i = 0; i < 20; ++i) {
        pdgid[i] = 0;
      }
    }
  };
    Data();
    Data(const char* filename);

    virtual ~Data();


    TCanvas *c1;
    TFile *rootFile;

    TTree *tAtarHits_;


  void DrawAtarHits(int);

    void Draw1(int n=1);


private:
  void LoadData(const char *filename);
  HitInfo fAtarHit;
  TLegend* fLegs[20];

};

#endif
