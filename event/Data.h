#ifndef DATA_H
#define DATA_H

#include "RtypesCore.h"
#include <type_traits>
#include <vector>
#include <map>
#include "TColor.h"
#include "Math/Point3Dfwd.h"
#include "Math/Vector3Dfwd.h"

class TFile;
class TCanvas;
class TTree;
class TLegend;

class Data {
public:
  struct HitInfo {
  public:
    int ncluster;
    int clusterid[80];

    std::vector<std::vector<Float_t > >* x;
    std::vector<std::vector<Float_t > >* y;
    std::vector<std::vector<Float_t > >* z;
    std::vector<std::vector<Float_t > >* t;
    std::vector<std::vector<Float_t > >* de;
    HitInfo() : ncluster(0), x(0), y(0), z(0), t(0), de(0) {
      std::for_each(std::begin(clusterid), std::end(clusterid), [](int& i) { i=0; });
    }

    Float_t GetVal(std::string, size_t i, size_t j);
  };
  Data();
  Data(const char *filename);

  virtual ~Data();

  TCanvas *c1;
  TFile *rootFile;

  TTree *tAtarHits_;

  void SetTMin(Double_t tmin) { fTMin = tmin; }
  void SetTMax(Double_t tmax) { fTMax = tmax; }
  void SetDEMin(Double_t demin) { fDEMin = demin; }
  void SetDEMax(Double_t demax) { fDEMax = demax; }
  void SetXMin(Double_t xmin) { fXMin = xmin; }
  void SetXMax(Double_t xmax) { fXMax = xmax; }
  void SetYMin(Double_t ymin) { fYMin = ymin; }
  void SetYMax(Double_t ymax) { fYMax = ymax; }
  void SetZMin(Double_t zmin) { fZMin = zmin; }
  void SetZMax(Double_t zmax) { fZMax = zmax; }

  void SetClusterIdx(unsigned int idx) { fClusterIdx = idx; }
  void SetDrawAll(bool drawall) { fDrawAll = drawall; }

  void DrawHitXYvsT(int);
  void DrawHitXZvsT(int);
  void DrawHitYZvsT(int);
  void DrawHitXYvsDE(int);
  void DrawHitXZvsDE(int);
  void DrawHitYZvsDE(int);
  void Draw3D(int);
  // void DrawAll();

  const unsigned int GetNCluster() const { return fNCluster; }

private:
  void LoadData(const char *filename);

  void DrawPoints(int, const std::string, const std::string, const std::string);

  Double_t GetMin(std::string var);
  Double_t GetMax(std::string var);

  // void DrawNTuple();

  HitInfo fAtarHit;
  ROOT::Math::XYZPoint* fEStart;
  ROOT::Math::Polar3DVector* fEDirection;
  TLegend *fLegs[20];

  Double_t fXMin;
  Double_t fXMax;
  Double_t fYMin;
  Double_t fYMax;
  Double_t fZMin;
  Double_t fZMax;
  Double_t fTMin;
  Double_t fTMax;
  Double_t fDEMin;
  Double_t fDEMax;

  unsigned int fNCluster;
  unsigned int fClusterIdx;

  bool fDrawAll;

  static const std::map<int, Color_t> colors_;
};

#endif
