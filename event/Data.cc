#include "Data.h"

#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TROOT.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

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
    cout << "loading " << filename << endl;

}


void Data::Draw1(int n)
{    
    TH1F *h = (TH1F*)gROOT->FindObject("h1");
    if (h) {
        delete h;
    }
    TVirtualPad *pad = c1->GetPad(1);
    c1->cd(1);
    h = new TH1F("h1", "h1", 100, -3, 3);
    h->FillRandom("gaus", n);
    h->Draw();

    pad->SetGridx();
    pad->SetGridy();
    pad->Modified();
    pad->Update();
}


Data::~Data()
{
    delete rootFile;
}
