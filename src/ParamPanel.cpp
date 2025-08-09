#include "ParamPanel.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

wxBEGIN_EVENT_TABLE(ParamPanel, wxPanel)
    EVT_SPINCTRL(wxID_ANY, ParamPanel::OnSpinChanged)
    EVT_SPINCTRLDOUBLE(wxID_ANY, ParamPanel::OnSpinDoubleChanged)
    EVT_TIMER(ID_TIMER, ParamPanel::OnTimer)
wxEND_EVENT_TABLE()

ParamPanel::ParamPanel(wxWindow* parent, std::function<void(const NoiseParams&)> onChanged, 
                       const NoiseParams& initial)
    : wxPanel(parent), onChanged(onChanged), values(initial) {
    
    timer = new wxTimer(this, ID_TIMER);
    CreateControls();
}

void ParamPanel::CreateControls() {
    // Create a 2-column grid sizer: labels, controls (matching Python: cols=2, vgap=4, hgap=8)
    auto* grid = new wxFlexGridSizer(2, 4, 8);
    
    // Structure to define all parameters
    struct ParamInfo {
        const char* label;
        double minVal, maxVal, inc;
        bool isFloat;
        int eventId;
    };
    
    ParamInfo params[] = {
        {"startRadius", 0.01, 1000.0, 0.01, true, ID_START_RADIUS},
        {"maxCircles", 1, 10000, 1, false, ID_MAX_CIRCLES},
        {"resolution", 1, 100, 1, false, ID_RESOLUTION},
        {"dRadius", 0.01, 100.0, 0.01, true, ID_D_RADIUS},
        {"rdn", 0.01, 100.0, 0.01, true, ID_RDN},
        {"xOffset", -1000.0, 1000.0, 0.1, true, ID_X_OFFSET},
        {"yOffset", -1000.0, 1000.0, 0.1, true, ID_Y_OFFSET},
        {"nSeed", 0, 100000, 1, false, ID_N_SEED},
        {"penWidth", 0.01, 10.0, 0.01, true, ID_PEN_WIDTH},
        {"lineDistance", 0.01, 10.0, 0.01, true, ID_LINE_DISTANCE}
    };
    
    for (const auto& param : params) {
        // Create label with 80px width + right alignment style
        auto* label = new wxStaticText(this, wxID_ANY, param.label);
        label->SetMinSize(wxSize(80, -1)); // 80px width as requested
        label->SetWindowStyle(wxALIGN_RIGHT); // KEY: Set right alignment on the label itself!
        
        // Create control
        if (param.isFloat) {
            auto* ctrl = new wxSpinCtrlDouble(this, param.eventId, wxEmptyString,
                                            wxDefaultPosition, wxDefaultSize,
                                            wxSP_ARROW_KEYS, param.minVal, param.maxVal, 
                                            GetDoubleValue(param.label), param.inc);
            ctrl->SetDigits(2);
            doubleCtrls[param.label] = ctrl;
        } else {
            auto* ctrl = new wxSpinCtrl(this, param.eventId, wxEmptyString,
                                      wxDefaultPosition, wxDefaultSize,
                                      wxSP_ARROW_KEYS, static_cast<int>(param.minVal), 
                                      static_cast<int>(param.maxVal), GetIntValue(param.label));
            intCtrls[param.label] = ctrl;
        }
        
        // Add label (right-aligned in cell with 4px right margin, like Python)
        grid->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxRIGHT, 4);
        
        // Add control (expand in cell, like Python: sc, 1, wx.EXPAND)
        auto* ctrl = param.isFloat ? static_cast<wxWindow*>(doubleCtrls[param.label]) 
                                   : static_cast<wxWindow*>(intCtrls[param.label]);
        grid->Add(ctrl, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    }
    
    // Make the control column growable (like Python: grid.AddGrowableCol(1, 1))
    grid->AddGrowableCol(1, 1);
    
    SetSizer(grid);
}

double ParamPanel::GetDoubleValue(const std::string& key) const {
    if (key == "startRadius") return values.startRadius;
    if (key == "dRadius") return values.dRadius;
    if (key == "rdn") return values.rdn;
    if (key == "xOffset") return values.xOffset;
    if (key == "yOffset") return values.yOffset;
    if (key == "penWidth") return values.penWidth;
    if (key == "lineDistance") return values.lineDistance;
    return 0.0;
}

int ParamPanel::GetIntValue(const std::string& key) const {
    if (key == "maxCircles") return values.maxCircles;
    if (key == "resolution") return values.resolution;
    if (key == "nSeed") return values.nSeed;
    return 0;
}

void ParamPanel::OnSpinChanged(wxSpinEvent& event) {
    int id = event.GetId();
    int value = event.GetInt();
    
    switch (id) {
        case ID_MAX_CIRCLES: values.maxCircles = value; break;
        case ID_RESOLUTION: values.resolution = value; break;
        case ID_N_SEED: values.nSeed = value; break;
    }
    
    StartRegenTimer();
}

void ParamPanel::OnSpinDoubleChanged(wxSpinDoubleEvent& event) {
    int id = event.GetId();
    double value = event.GetValue();
    
    switch (id) {
        case ID_START_RADIUS: values.startRadius = value; break;
        case ID_D_RADIUS: values.dRadius = value; break;
        case ID_RDN: values.rdn = value; break;
        case ID_X_OFFSET: values.xOffset = value; break;
        case ID_Y_OFFSET: values.yOffset = value; break;
        case ID_PEN_WIDTH: values.penWidth = value; break;
        case ID_LINE_DISTANCE: values.lineDistance = value; break;
    }
    
    StartRegenTimer();
}

void ParamPanel::StartRegenTimer() {
    timer->Stop();
    timer->StartOnce(50); // Reduced from 300ms - C++ performance allows much faster updates
}

void ParamPanel::OnTimer(wxTimerEvent& event) {
    onChanged(values);
}

void ParamPanel::SetAllValues(const NoiseParams& params) {
    values = params;
    
    // Update all controls
    if (intCtrls["maxCircles"]) intCtrls["maxCircles"]->SetValue(values.maxCircles);
    if (intCtrls["resolution"]) intCtrls["resolution"]->SetValue(values.resolution);
    if (intCtrls["nSeed"]) intCtrls["nSeed"]->SetValue(values.nSeed);
    
    if (doubleCtrls["startRadius"]) doubleCtrls["startRadius"]->SetValue(values.startRadius);
    if (doubleCtrls["dRadius"]) doubleCtrls["dRadius"]->SetValue(values.dRadius);
    if (doubleCtrls["rdn"]) doubleCtrls["rdn"]->SetValue(values.rdn);
    if (doubleCtrls["xOffset"]) doubleCtrls["xOffset"]->SetValue(values.xOffset);
    if (doubleCtrls["yOffset"]) doubleCtrls["yOffset"]->SetValue(values.yOffset);
    if (doubleCtrls["penWidth"]) doubleCtrls["penWidth"]->SetValue(values.penWidth);
    if (doubleCtrls["lineDistance"]) doubleCtrls["lineDistance"]->SetValue(values.lineDistance);
    
    onChanged(values);
}
