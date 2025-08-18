#pragma once
#include <wx/wx.h>

enum {
    ID_HELLO = 1000
};

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    
    wxDECLARE_EVENT_TABLE();
};
