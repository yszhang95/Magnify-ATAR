#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "TGFrame.h"
#include "RQ_OBJECT.h"
#include "TString.h"

class MainWindow;
class ViewWindow;
class ControlWindow;
class Data;

class GuiController
{
    RQ_OBJECT("GuiController")
public:
    GuiController(const TGWindow *p, int w, int h, const char* filename=0);
    virtual ~GuiController();

    void InitConnections();
    void HandleMenu(int id);
  void DoExit();
    // void ClusterChanged(int i);
    // void ClusterIdChanged(int i);
    // // void ZRangeChanged();
    // // void ChannelChanged();
    // void ZoomChanged();
    // void ToggleBadChannel();
    // void ToggleDrawTrack();
    // void ToggleAllCluster();
    // void UnZoom();
  void RangeZoom();
    // void SetRangeEntries();
    void Play();
    void ClusterChanged(int i);
  void ToggleAllCluster();
    void ProcessCanvasEvent(Int_t ev, Int_t x, Int_t y, TObject* selected);

    MainWindow *mw;
    ViewWindow *vw;
    ControlWindow *cw;
    Data *data;

};

#endif
