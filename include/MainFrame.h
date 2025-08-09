#pragma once

#include <wx/wx.h>
#include <wx/splitter.h>
#include "Constants.h"
#include "Canvas.h"
#include "ParamPanel.h"

class MainFrame : public wxFrame {
public:
    MainFrame();
    ~MainFrame();

private:
    void CreateMenus();
    void CreateControls();
    void LoadState();
    void SaveState();
    void LoadDefaultParams();
    
    // Event handlers
    void OnClose(wxCloseEvent& event);
    void OnShow(wxShowEvent& event);
    void OnSaveSVG(wxCommandEvent& event);
    void OnSaveGCode(wxCommandEvent& event);
    void OnSaveParams(wxCommandEvent& event);
    void OnLoadParams(wxCommandEvent& event);
    void OnCanvasValuesChanged(const NoiseParams& params);
    
    // UI components
    wxSplitterWindow* splitter;
    Canvas* canvas;
    ParamPanel* paramPanel;
    
    // Buttons
    wxButton* btnSaveSVG;
    wxButton* btnSaveGCode;
    wxButton* btnSaveParams;
    wxButton* btnLoadParams;
    
    // State
    NoiseParams currentParams;
    
    // Event IDs
    enum {
        ID_SAVE_SVG = 2000,
        ID_SAVE_GCODE,
        ID_SAVE_PARAMS,
        ID_LOAD_PARAMS
    };
    
    DECLARE_EVENT_TABLE()
};
