#include <wx/wx.h>
#include <wx/utils.h>
#include <wx/sysopt.h>
#include "MainFrame.h"

class PerlinNoiseApp : public wxApp {
public:
    virtual bool OnInit() override {
        // Suppress manifest warning on Windows
        wxSystemOptions::SetOption("msw.no-manifest-check", 1);
        
        if (!wxApp::OnInit()) {
            return false;
        }
        
        MainFrame* frame = new MainFrame();
        frame->Show(true);
        
        return true;
    }
};

wxIMPLEMENT_APP(PerlinNoiseApp);
