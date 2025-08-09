#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <functional>
#include <unordered_map>
#include "Constants.h"

class ParamPanel : public wxPanel {
public:
    ParamPanel(wxWindow* parent, std::function<void(const NoiseParams&)> onChanged, 
               const NoiseParams& initial);
    
    void SetAllValues(const NoiseParams& params);
    NoiseParams GetValues() const { return values; }

private:
    void CreateControls();
    void OnSpinChanged(wxSpinEvent& event);
    void OnSpinDoubleChanged(wxSpinDoubleEvent& event);
    void OnTimer(wxTimerEvent& event);
    void StartRegenTimer();
    double GetDoubleValue(const std::string& key) const;
    int GetIntValue(const std::string& key) const;
    
    std::function<void(const NoiseParams&)> onChanged;
    NoiseParams values;
    
    // UI controls
    std::unordered_map<std::string, wxSpinCtrl*> intCtrls;
    std::unordered_map<std::string, wxSpinCtrlDouble*> doubleCtrls;
    
    wxTimer* timer;
    
    // Event IDs
    enum {
        ID_START_RADIUS = 1000,
        ID_MAX_CIRCLES,
        ID_RESOLUTION,
        ID_D_RADIUS,
        ID_RDN,
        ID_X_OFFSET,
        ID_Y_OFFSET,
        ID_N_SEED,
        ID_PEN_WIDTH,
        ID_LINE_DISTANCE,
        ID_TIMER
    };
    
    DECLARE_EVENT_TABLE()
};
