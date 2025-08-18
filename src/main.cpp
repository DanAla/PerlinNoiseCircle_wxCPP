#include <wx/wx.h>
#include "MainFrame.h"

class DefaultApp : public wxApp {
public:
    virtual bool OnInit() override {
        if (!wxApp::OnInit()) {
            return false;
        }
        
        MainFrame* frame = new MainFrame();
        frame->Show(true);
        
        return true;
    }
};

wxIMPLEMENT_APP(DefaultApp);
