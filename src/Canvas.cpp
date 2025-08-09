#include "Canvas.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <cmath>
#include <random>

wxBEGIN_EVENT_TABLE(Canvas, wxScrolledWindow)
    EVT_PAINT(Canvas::OnPaint)
wxEND_EVENT_TABLE()

Canvas::Canvas(wxWindow* parent)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(VIEWPORT_SIZE, VIEWPORT_SIZE),
                       wxBORDER_NONE),
      values(GetDefaultParams()),
      buffer(CANVAS_SIZE, CANVAS_SIZE) {
    
    SetScrollbars(1, 1, CANVAS_SIZE, CANVAS_SIZE, 0, 0);
    SetScrollRate(1, 1);
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    
    Redraw();
}

void Canvas::SetValues(const NoiseParams& params) {
    values = params;
    
    // If seed is 0, generate a random seed
    if (std::abs(values.nSeed) < 1e-6) {
        std::random_device rd;
        values.nSeed = rd() % 10000 + 1;
    }
    
    Redraw();
}

void Canvas::OnPaint(wxPaintEvent& event) {
    wxBufferedPaintDC dc(this, buffer, wxBUFFER_VIRTUAL_AREA);
}

void Canvas::Redraw() {
    GeneratePattern();
    
    wxMemoryDC dc(buffer);
    dc.SetBackground(wxBrush(wxColour(255, 255, 255)));
    dc.Clear();
    
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (!gc) {
        return;
    }
    
    // Set pen color and width
    gc->SetPen(wxGraphicsRenderer::GetDefaultRenderer()->CreatePen(
        wxGraphicsPenInfo(wxColour(0, 0, 200)).Width(values.penWidth)));
    
    // Draw the path
    wxGraphicsPath path = gc->CreatePath();
    if (!currentPaths.empty()) {
        auto it = currentPaths.begin();
        path.MoveToPoint(it->first, it->second);
        ++it;
        
        for (; it != currentPaths.end(); ++it) {
            path.AddLineToPoint(it->first, it->second);
        }
    }
    
    gc->StrokePath(path);
    delete gc;
    
    Refresh();
}

void Canvas::GeneratePattern() {
    currentPaths.clear();
    
    PerlinNoise noise(static_cast<unsigned int>(values.nSeed));
    
    double radius = values.startRadius;
    double x1 = values.startRadius;
    double y1 = 0.0;
    
    currentPaths.push_back({x1, y1});
    
    int step = std::max(1, static_cast<int>(std::round(1.0 / values.resolution)));
    double scale = CANVAS_SIZE / 2.0 / 350.0;
    double ld = values.lineDistance;
    
    for (int i = 0; i <= values.maxCircles; ++i) {
        for (int j = 1; j < 360; j += step) {
            double x = (radius - values.rdn) * std::cos(j * M_PI / 180.0);
            double y = (radius - values.rdn) * std::sin(j * M_PI / 180.0);
            
            double n1 = noise.noise2D((radius - values.rdn) / 25.0, j / 25.0);
            double nf1 = noise.noise2D(x / 250.0 + n1, (y * j) / 15500.0);
            double vval = std::pow((radius - values.rdn) * 1.5 + 55, 2) / 40000.0;
            double nf2 = noise.noise2D(x / 30.0, y / 30.0) * std::pow(radius - values.rdn, 1.75) / 3500.0;
            
            // Ensure nf2 is real before using in radians
            double nf2_real = std::abs(nf2);
            double angle1 = (j + nf2_real * 40) * M_PI / 180.0;
            double angle2 = (j + nf2_real * 50) * M_PI / 180.0;
            
            double x2 = values.startRadius * std::cos(angle1) + 
                       radius * std::cos(angle1) * nf1 * vval;
            double y2 = values.startRadius * std::sin(angle2) + 
                       radius * std::sin(angle2) * nf1 * vval;
            
            double sx2 = CANVAS_SIZE / 2 + values.xOffset * scale + x2 * scale;
            double sy2 = CANVAS_SIZE / 2 + values.yOffset * scale + y2 * scale;
            
            currentPaths.push_back({sx2, sy2});
            
            x1 = x2;
            y1 = y2;
            radius += ld * values.dRadius / (360.0 * values.resolution);
        }
    }
}

std::vector<std::pair<double, double>> Canvas::GetPaths() const {
    // Return the raw paths without screen scaling for export
    std::vector<std::pair<double, double>> rawPaths;
    
    PerlinNoise noise(static_cast<unsigned int>(values.nSeed));
    
    double radius = values.startRadius;
    rawPaths.push_back({values.startRadius, 0.0});
    
    int step = std::max(1, static_cast<int>(std::round(1.0 / values.resolution)));
    double ld = values.lineDistance;
    
    for (int i = 0; i <= values.maxCircles; ++i) {
        for (int j = 1; j < 360; j += step) {
            double angle = j * M_PI / 180.0;
            double cx = (radius - values.rdn) * std::cos(angle);
            double cy = (radius - values.rdn) * std::sin(angle);
            
            double n1 = noise.noise2D((radius - values.rdn) / 25.0, j / 25.0);
            double nf1 = noise.noise2D(cx / 250.0 + n1, (cy * j) / 15500.0);
            double vval = std::pow((radius - values.rdn) * 1.5 + 55, 2) / 40000.0;
            double nf2 = noise.noise2D(cx / 30.0, cy / 30.0) * std::pow(radius - values.rdn, 1.75) / 3500.0;
            
            double nf2_real = std::abs(nf2);
            double angle1 = angle + nf2_real * 40 * M_PI / 180.0;
            double angle2 = angle + nf2_real * 50 * M_PI / 180.0;
            
            double x2 = values.startRadius * std::cos(angle1) + 
                       radius * std::cos(angle1) * nf1 * vval;
            double y2 = values.startRadius * std::sin(angle2) + 
                       radius * std::sin(angle2) * nf1 * vval;
            
            rawPaths.push_back({x2, y2});
            radius += ld * values.dRadius / (360.0 * values.resolution);
        }
    }
    
    return rawPaths;
}
