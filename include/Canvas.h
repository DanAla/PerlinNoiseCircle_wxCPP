#pragma once

#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <vector>
#include "Constants.h"
#include "PerlinNoise.h"

class Canvas : public wxScrolledWindow {
public:
    Canvas(wxWindow* parent);
    
    void SetValues(const NoiseParams& params);
    std::vector<std::pair<double, double>> GetPaths() const;
    const NoiseParams& GetValues() const { return values; }

private:
    void OnPaint(wxPaintEvent& event);
    void Redraw();
    void GeneratePattern();
    
    NoiseParams values;
    wxBitmap buffer;
    std::vector<std::pair<double, double>> currentPaths;
    
    DECLARE_EVENT_TABLE()
};
