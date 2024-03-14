#include "GuiController.h"
#include "MainWindow.h"
#include "ViewWindow.h"
#include "ControlWindow.h"
#include "Data.h"

#include "TApplication.h"
#include "TSystem.h"
#include "TExec.h"
#include "TROOT.h"
#include "TMath.h"
#include "TGFileDialog.h"

#include "TGMenu.h"
#include "TGNumberEntry.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TH2I.h"
#include "TH1I.h"
#include "TBox.h"
#include "TLine.h"
#include "TColor.h"
#include "TLatex.h"

#include <iostream>
#include <vector>
using namespace std;

GuiController::GuiController(const TGWindow *p, int w, int h, const char* fn)
{
    mw = new MainWindow(p, w, h);
    vw = mw->fViewWindow;
    cw = mw->fControlWindow;

    TString filename;
    if (!fn) {
        cout << "no filename given, terminating" << endl;
        gApplication->Terminate(0);
    }
    else {
        filename = fn;
    }
    data = new Data(filename.Data());
    data->c1 = vw->can;

    InitConnections();

    data->DrawHitXYvsT(1);
    data->DrawHitXZvsT(2);
    data->DrawHitYZvsT(3);
    data->DrawHitXYvsDE(4);
    data->DrawHitXZvsDE(5);
    data->DrawHitYZvsDE(6);
}

GuiController::~GuiController()
{
  gApplication->Terminate(0);
}

void GuiController::InitConnections()
{
  mw->fMenuFile->Connect("Activated(int)", "GuiController", this,
                         "HandleMenu(int)");

  mw->Connect("CloseWindow()", "GuiController", this, "DoExit()");
  mw->DontCallClose();


  cw->clusterEntry->SetLimitValues(0, data->GetNCluster()-1);
  cw->clusterEntry->Connect("ValueSet(Long_t)", "GuiController", this,
                            "ClusterChanged(int)");

  // cw->clusterIdEntry->SetNumber(data->rec_cluster_id->at(currentCluster));
  // cw->clusterIdEntry->Connect("ValueSet(Long_t)", "GuiController", this, "ClusterIdChanged(int)");

  // cw->pointIndexEntry->Connect("ValueSet(Long_t)", "GuiController", this, "ZoomChanged()");

  // cw->zoomEntry->Connect("ValueSet(Long_t)", "GuiController", this, "ZoomChanged()");
  // cw->badChanelButton->Connect("Clicked()", "GuiController", this, "ToggleBadChannel()");
  // cw->drawTrackButton->Connect("Clicked()", "GuiController", this, "ToggleDrawTrack()");
  cw->allClusterButton->Connect("Clicked()", "GuiController", this, "ToggleAllCluster()");

    cw->unZoomButton->Connect("Clicked()", "GuiController", this, "Play()");
    cw->rangeZoomButton->Connect("Clicked()", "GuiController", this, "RangeZoom()");

    vw->can->Connect(
        "ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
        "GuiController",
        this,
        "ProcessCanvasEvent(Int_t,Int_t,Int_t,TObject*)"
    );
}

void GuiController::ProcessCanvasEvent(Int_t ev, Int_t x, Int_t y, TObject* selected)
{
    if (ev == 1) { // clicked
        // if (!(selected->IsA() == TGraph::Class()
        //     || selected->IsA() == TH2F::Class()
        //     // || selected->IsA() == TLine::Class()
        // )) return;
        TVirtualPad* pad = vw->can->GetClickSelectedPad();
        int padNo = pad->GetNumber();
        double xx = pad->AbsPixeltoX(x);
        double yy = pad->AbsPixeltoY(y);
        // int ci = data->FindClusterIndex(xx, yy);
        // int pi = data->FindPointIndex(xx, yy);
        cout << "pad " << padNo << ": (" << xx << ", " << yy << ")" 
            // << "; cluster index: " << ci
            // << "; point index: " << pi
            << endl;
    }
}

void GuiController::ClusterChanged(int i)
{
  int n = cw->clusterEntry->GetNumber();
  data->SetClusterIdx(n);

  cw->allClusterButton->SetState(kButtonUp, kTRUE);

  data->DrawHitXYvsT(1);
  data->DrawHitXZvsT(2);
  data->DrawHitYZvsT(3);
  data->DrawHitXYvsDE(4);
  data->DrawHitXZvsDE(5);
  data->DrawHitYZvsDE(6);
}

void GuiController::ToggleAllCluster()
{
  data->SetDrawAll(cw->allClusterButton->IsDown());
  data->DrawHitXYvsT(1);
  data->DrawHitXZvsT(2);
  data->DrawHitYZvsT(3);
  data->DrawHitXYvsDE(4);
  data->DrawHitXZvsDE(5);
  data->DrawHitYZvsDE(6);
}

void GuiController::RangeZoom()
{
  data->SetTMin(cw->minEntry[0]->GetNumber());
  data->SetTMax(cw->maxEntry[0]->GetNumber());
  data->SetDEMin(cw->minEntry[1]->GetNumber());
  data->SetDEMax(cw->maxEntry[1]->GetNumber());
  data->SetXMin(cw->minEntry[2]->GetNumber());
  data->SetXMax(cw->maxEntry[2]->GetNumber());
  data->SetYMin(cw->minEntry[3]->GetNumber());
  data->SetYMax(cw->maxEntry[3]->GetNumber());
  data->SetZMin(cw->minEntry[4]->GetNumber());
  data->SetZMax(cw->maxEntry[4]->GetNumber());

  data->DrawHitXYvsT(1);
  data->DrawHitXZvsT(2);
  data->DrawHitYZvsT(3);
  data->DrawHitXYvsDE(4);
  data->DrawHitXZvsDE(5);
  data->DrawHitYZvsDE(6);
}

void GuiController::Play()
{
  cout << "chao clicked this button" << endl;
}


void GuiController::HandleMenu(int id)
{
    switch (id) {
        case M_FILE_EXIT:
            gApplication->Terminate(0);
            break;
    }
}

void GuiController::DoExit()
{
  gApplication->Terminate(0);
}
