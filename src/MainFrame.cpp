#include "MainFrame.h"
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <fstream>
#include <sstream>
#include <iomanip>

#if defined(USE_SINGLE_HEADER_JSON) || defined(USE_JSON)
#ifdef USE_SINGLE_HEADER_JSON
#include <json.hpp>
#else
#include <nlohmann/json.hpp>
#endif
using json = nlohmann::json;
#define JSON_SUPPORT_ENABLED
#endif

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_CLOSE(MainFrame::OnClose)
    EVT_SHOW(MainFrame::OnShow)
    EVT_BUTTON(ID_SAVE_SVG, MainFrame::OnSaveSVG)
    EVT_BUTTON(ID_SAVE_GCODE, MainFrame::OnSaveGCode)
    EVT_BUTTON(ID_SAVE_PARAMS, MainFrame::OnSaveParams)
    EVT_BUTTON(ID_LOAD_PARAMS, MainFrame::OnLoadParams)
wxEND_EVENT_TABLE()

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "Perlin Noise Circles wxGUI", 
              wxDefaultPosition, wxSize(1024, 768)),
      currentParams(GetDefaultParams()) {
    
    LoadDefaultParams();
    LoadState();
    CreateControls();
}

MainFrame::~MainFrame() {
    SaveState();
}

void MainFrame::LoadDefaultParams() {
    try {
        std::ifstream file(GetDefaultParamsFile());
        if (file.is_open()) {
            json j;
            file >> j;
            
            if (j.contains("startRadius")) currentParams.startRadius = j["startRadius"];
            if (j.contains("maxCircles")) currentParams.maxCircles = j["maxCircles"];
            if (j.contains("resolution")) currentParams.resolution = j["resolution"];
            if (j.contains("dRadius")) currentParams.dRadius = j["dRadius"];
            if (j.contains("rdn")) currentParams.rdn = j["rdn"];
            if (j.contains("xOffset")) currentParams.xOffset = j["xOffset"];
            if (j.contains("yOffset")) currentParams.yOffset = j["yOffset"];
            if (j.contains("nSeed")) currentParams.nSeed = j["nSeed"];
            if (j.contains("penWidth")) currentParams.penWidth = j["penWidth"];
            if (j.contains("lineDistance")) currentParams.lineDistance = j["lineDistance"];
        }
    } catch (const std::exception& e) {
        // Use built-in defaults if file reading fails
    }
}

void MainFrame::LoadState() {
    try {
        std::ifstream file(GetConfigFile());
        if (file.is_open()) {
            json j;
            file >> j;
            
            if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
                SetSize(j["size"][0], j["size"][1]);
            }
            
            if (j.contains("pos") && j["pos"].is_array() && j["pos"].size() == 2) {
                SetPosition(wxPoint(j["pos"][0], j["pos"][1]));
            }
            
            if (j.contains("max") && j["max"].get<bool>()) {
                Maximize(true);
            }
            
            if (j.contains("params")) {
                const auto& params = j["params"];
                if (params.contains("startRadius")) currentParams.startRadius = params["startRadius"];
                if (params.contains("maxCircles")) currentParams.maxCircles = params["maxCircles"];
                if (params.contains("resolution")) currentParams.resolution = params["resolution"];
                if (params.contains("dRadius")) currentParams.dRadius = params["dRadius"];
                if (params.contains("rdn")) currentParams.rdn = params["rdn"];
                if (params.contains("xOffset")) currentParams.xOffset = params["xOffset"];
                if (params.contains("yOffset")) currentParams.yOffset = params["yOffset"];
                if (params.contains("nSeed")) currentParams.nSeed = params["nSeed"];
                if (params.contains("penWidth")) currentParams.penWidth = params["penWidth"];
                if (params.contains("lineDistance")) currentParams.lineDistance = params["lineDistance"];
            }
        }
    } catch (const std::exception& e) {
        // Use defaults if state loading fails
    }
}

void MainFrame::CreateControls() {
    // Create splitter window
    splitter = new wxSplitterWindow(this);
    
    // Create canvas
    canvas = new Canvas(splitter);
    
    // Create parameter panel
    paramPanel = new ParamPanel(splitter, 
        [this](const NoiseParams& params) { OnCanvasValuesChanged(params); },
        currentParams);
    
    // Split the window
    splitter->SplitVertically(paramPanel, canvas, 220);
    
    // Create buttons
    btnSaveSVG = new wxButton(this, ID_SAVE_SVG, "Save SVG");
    btnSaveGCode = new wxButton(this, ID_SAVE_GCODE, "Save G-code");
    btnSaveParams = new wxButton(this, ID_SAVE_PARAMS, "Save Params");
    btnLoadParams = new wxButton(this, ID_LOAD_PARAMS, "Load Params");
    
    // Layout buttons
    auto* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSizer->Add(btnSaveSVG, 0, wxALL, 5);
    btnSizer->Add(btnSaveGCode, 0, wxALL, 5);
    btnSizer->Add(btnSaveParams, 0, wxALL, 5);
    btnSizer->Add(btnLoadParams, 0, wxALL, 5);
    
    // Main layout
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(splitter, 1, wxEXPAND);
    mainSizer->Add(btnSizer, 0, wxALIGN_CENTER);
    
    SetSizer(mainSizer);
    
    // Set initial values
    canvas->SetValues(currentParams);
}

void MainFrame::OnCanvasValuesChanged(const NoiseParams& params) {
    currentParams = params;
    canvas->SetValues(params);
}

void MainFrame::OnShow(wxShowEvent& event) {
    event.Skip();
    if (event.IsShown()) {
        // Center the canvas view
        canvas->Scroll((CANVAS_SIZE - VIEWPORT_SIZE) / 2,
                      (CANVAS_SIZE - VIEWPORT_SIZE) / 2);
    }
}

void MainFrame::OnSaveSVG(wxCommandEvent& event) {
    auto paths = canvas->GetPaths();
    if (paths.empty()) {
        return;
    }
    
    wxFileDialog dlg(this, "Save SVG", "", "", "SVG files (*.svg)|*.svg",
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    if (dlg.ShowModal() == wxID_OK) {
        std::ofstream file(dlg.GetPath().ToStdString());
        if (file.is_open()) {
            file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            file << "<svg width=\"" << CANVAS_SIZE << "\" height=\"" << CANVAS_SIZE << "\" ";
            file << "xmlns=\"http://www.w3.org/2000/svg\">\n";
            file << "<g transform=\"translate(" << (CANVAS_SIZE/2 + currentParams.xOffset) << ", ";
            file << (CANVAS_SIZE/2 + currentParams.yOffset) << ")\">\n";
            file << "<polyline points=\"";
            
            for (size_t i = 0; i < paths.size(); ++i) {
                if (i > 0) file << " ";
                file << paths[i].first << "," << paths[i].second;
            }
            
            file << "\" stroke=\"blue\" stroke-width=\"" << currentParams.penWidth;
            file << "\" fill=\"none\"/>\n";
            file << "</g>\n</svg>";
        }
    }
}

void MainFrame::OnSaveGCode(wxCommandEvent& event) {
    auto paths = canvas->GetPaths();
    if (paths.empty()) {
        return;
    }
    
    wxFileDialog dlg(this, "Save G-code", "", "", "G-code files (*.nc)|*.nc",
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    if (dlg.ShowModal() == wxID_OK) {
        std::ofstream file(dlg.GetPath().ToStdString());
        if (file.is_open()) {
            // Header with parameters
            file << "( Noise circles )\n";
            file << "( startRadius " << currentParams.startRadius 
                 << "  maxCircles " << currentParams.maxCircles
                 << "  resolution " << currentParams.resolution
                 << "  dRadius " << currentParams.dRadius
                 << "  rdn " << currentParams.rdn
                 << "  xOffset " << currentParams.xOffset
                 << "  yOffset " << currentParams.yOffset
                 << "  nSeed " << currentParams.nSeed
                 << "  penWidth " << currentParams.penWidth
                 << "  lineDistance " << currentParams.lineDistance << " )\n";
            
            file << "G54\nG90\nG00 Z5\nT01 M06\nG01 F1500\n";
            
            // Move to start position and lower pen
            file << std::fixed << std::setprecision(3);
            file << "G00 X" << paths[0].first << " Y" << paths[0].second << "\n";
            file << "G01 Z-3\n";
            
            // Draw path
            for (size_t i = 1; i < paths.size(); ++i) {
                file << "G01 X" << paths[i].first << " Y" << paths[i].second << "\n";
            }
            
            // Lift pen and end
            file << "G00 Z2\nT00 M06\nM30\n";
        }
    }
}

void MainFrame::OnSaveParams(wxCommandEvent& event) {
    wxFileDialog dlg(this, "Save parameters", "", "", "JSON files (*.json)|*.json",
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    if (dlg.ShowModal() == wxID_OK) {
        json j = {
            {"startRadius", currentParams.startRadius},
            {"maxCircles", currentParams.maxCircles},
            {"resolution", currentParams.resolution},
            {"dRadius", currentParams.dRadius},
            {"rdn", currentParams.rdn},
            {"xOffset", currentParams.xOffset},
            {"yOffset", currentParams.yOffset},
            {"nSeed", currentParams.nSeed},
            {"penWidth", currentParams.penWidth},
            {"lineDistance", currentParams.lineDistance}
        };
        
        std::ofstream file(dlg.GetPath().ToStdString());
        if (file.is_open()) {
            file << j.dump(2);
        }
    }
}

void MainFrame::OnLoadParams(wxCommandEvent& event) {
    wxFileDialog dlg(this, "Load parameters", "", "", "JSON files (*.json)|*.json",
                     wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (dlg.ShowModal() == wxID_OK) {
        try {
            std::ifstream file(dlg.GetPath().ToStdString());
            if (file.is_open()) {
                json j;
                file >> j;
                
                NoiseParams newParams = currentParams;
                if (j.contains("startRadius")) newParams.startRadius = j["startRadius"];
                if (j.contains("maxCircles")) newParams.maxCircles = j["maxCircles"];
                if (j.contains("resolution")) newParams.resolution = j["resolution"];
                if (j.contains("dRadius")) newParams.dRadius = j["dRadius"];
                if (j.contains("rdn")) newParams.rdn = j["rdn"];
                if (j.contains("xOffset")) newParams.xOffset = j["xOffset"];
                if (j.contains("yOffset")) newParams.yOffset = j["yOffset"];
                if (j.contains("nSeed")) newParams.nSeed = j["nSeed"];
                if (j.contains("penWidth")) newParams.penWidth = j["penWidth"];
                if (j.contains("lineDistance")) newParams.lineDistance = j["lineDistance"];
                
                paramPanel->SetAllValues(newParams);
                currentParams = newParams;
            }
        } catch (const std::exception& e) {
            wxMessageBox("Failed to load parameters: " + wxString(e.what()), 
                        "Error", wxOK | wxICON_ERROR);
        }
    }
}

void MainFrame::OnClose(wxCloseEvent& event) {
    SaveState();
    event.Skip();
}

void MainFrame::SaveState() {
    json j = {
        {"size", {GetSize().x, GetSize().y}},
        {"pos", {GetPosition().x, GetPosition().y}},
        {"max", IsMaximized()},
        {"params", {
            {"startRadius", currentParams.startRadius},
            {"maxCircles", currentParams.maxCircles},
            {"resolution", currentParams.resolution},
            {"dRadius", currentParams.dRadius},
            {"rdn", currentParams.rdn},
            {"xOffset", currentParams.xOffset},
            {"yOffset", currentParams.yOffset},
            {"nSeed", currentParams.nSeed},
            {"penWidth", currentParams.penWidth},
            {"lineDistance", currentParams.lineDistance}
        }}
    };
    
    std::ofstream file(GetConfigFile());
    if (file.is_open()) {
        file << j.dump(2);
    }
}
