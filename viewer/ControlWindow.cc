#include "ControlWindow.h"

#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TGButtonGroup.h"
#include "TGListBox.h"
#include "TGLabel.h"
// #include "TGDoubleSlider.h"
#include "TString.h"

#include <GuiTypes.h>
#include <TGLayout.h>
#include <iostream>
using namespace std;

ControlWindow::ControlWindow(const TGWindow *p, int w, int h)
    :TGHorizontalFrame(p, w, h)
{
    TGGroupFrame *group_general = new TGGroupFrame(this, "General", kHorizontalFrame);
    group_general->SetTitlePos(TGGroupFrame::kLeft);
    AddFrame(group_general, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 1, 1));

    group_general->AddFrame(
        new TGLabel(group_general, "cluster index: "),
        new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 1, 1));

        clusterEntry =
        new TGNumberEntry(group_general, 0, 5, -1, TGNumberFormat::kNESInteger,
                          TGNumberFormat::kNEANonNegative,
                          TGNumberFormat::kNELLimitMinMax, 0, 2000000);

    group_general->AddFrame(clusterEntry, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    group_general->AddFrame(new TGLabel(group_general, "cluster id: "), new TGLayoutHints(kLHintsTop | kLHintsLeft,  2, 2, 1, 1));
    clusterIdEntry = new TGNumberEntry(group_general, 0, 5, -1,
        TGNumberFormat::kNESInteger,
        TGNumberFormat::kNEANonNegative,
        TGNumberFormat::kNELLimitMinMax,
        0, 1000);
    group_general->AddFrame(clusterIdEntry, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    allClusterButton = new TGCheckButton(group_general, "all clusters  ");
    allClusterButton->SetState(kButtonDown);
    group_general->AddFrame(allClusterButton, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    // badChanelButton = new TGCheckButton(group_general, "bad ch  ");
    // badChanelButton->SetState(kButtonUp);
    // group_general->AddFrame(badChanelButton, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    // drawTrackButton = new TGCheckButton(group_general, "draw track  ");
    // drawTrackButton->SetState(kButtonDown);
    // group_general->AddFrame(drawTrackButton, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    TGGroupFrame *group_misc = new TGGroupFrame(this, "Zoom", kHorizontalFrame);
    group_misc->SetTitlePos(TGGroupFrame::kLeft);
    AddFrame(group_misc, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 1, 1));

    group_misc->AddFrame(new TGLabel(group_misc, "point index: "), new TGLayoutHints(kLHintsTop | kLHintsLeft,  2, 2, 1, 1));
    pointIndexEntry = new TGNumberEntry(group_misc, 2, 5, -1,
        TGNumberFormat::kNESInteger,
        TGNumberFormat::kNEANonNegative,
        TGNumberFormat::kNELLimitMinMax,
        1, 1000);
    group_misc->AddFrame(pointIndexEntry, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    group_misc->AddFrame(new TGLabel(group_misc, "zoom +- 10x: "), new TGLayoutHints(kLHintsTop | kLHintsLeft,  2, 2, 1, 1));
    zoomEntry = new TGNumberEntry(group_misc, 2, 5, -1,
        TGNumberFormat::kNESInteger,
        TGNumberFormat::kNEANonNegative,
        TGNumberFormat::kNELLimitMinMax,
        1, 100);
    group_misc->AddFrame(zoomEntry, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));


    unZoomButton = new TGTextButton(group_misc, "Play");
    group_misc->AddFrame(unZoomButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));

    TGGroupFrame *group_range = new TGGroupFrame(this, "Range", kHorizontalFrame);
    group_range->SetTitlePos(TGGroupFrame::kLeft);
    AddFrame(group_range,
             new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 1, 1));

    const int groupid[5] = {1, 1, 0, 0, 0};
    const TString str[5] = {"time: ", "dE: ", "X: ", "Y: ", "Z: "};
    const double min_val[5] = {-300, 0, -1.5, -1.5, -1.};
    const double max_val[5] = {1500, 100, 1.5, 1.5, 7.};

    for (unsigned int i = 0; i < 5; i++) {
      group_range->AddFrame(
          new TGLabel(group_range, str[i].Data()),
          new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 1, 1));


      minEntry[i] = new TGNumberEntry(group_range, min_val[i], 5, -1,
                                      TGNumberFormat::kNESReal,
                                      TGNumberFormat::kNEAAnyNumber,
                                      TGNumberFormat::kNELLimitMinMax,
                                      min_val[i], max_val[i]);

      group_range->AddFrame(
          minEntry[i], new TGLayoutHints(kLHintsTop | kLHintsLeft, 1, 1, 1, 1));

      maxEntry[i] = new TGNumberEntry(group_range, max_val[i], 5, -1,
                                      TGNumberFormat::kNESReal,
                                      TGNumberFormat::kNEAAnyNumber,
                                      TGNumberFormat::kNELLimitMinMax,
                                      min_val[i], max_val[i]);
      group_range->AddFrame(
          maxEntry[i], new TGLayoutHints(kLHintsTop | kLHintsLeft, 1, 1, 1, 1));
    }

    rangeZoomButton = new TGTextButton(group_range, "Zoom");
    group_range->AddFrame(rangeZoomButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));

    keepRangeButton = new TGCheckButton(group_range, "keep");
    keepRangeButton->SetState(kButtonUp);
    group_range->AddFrame(keepRangeButton, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

}

ControlWindow::~ControlWindow()
{
}
