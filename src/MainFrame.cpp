#include "MainFrame.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_HELLO, MainFrame::OnHello)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Default wxWidgets Application") {
    // Create menu bar
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_HELLO, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    // Create status bar
    CreateStatusBar();
    SetStatusText("Welcome to the Default wxWidgets Application!");
    
    // Set window size and center
    SetSize(800, 600);
    Center();
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a default wxWidgets application template.\n\n"
                 "Built with MinGW-w64 and wxWidgets 3.3.1\n"
                 "Ready for your custom application development!",
                 "About Default App", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnHello(wxCommandEvent& event) {
    wxLogMessage("Hello world from wxWidgets!");
}
