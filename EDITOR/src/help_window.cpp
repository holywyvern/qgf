///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "help_window.hpp"
#include "i18n.hpp"
#include "window_manager.hpp"

///////////////////////////////////////////////////////////////////////////

HelpDialog::HelpDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* top_sizer;
	top_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	label_search = new wxStaticText( this, wxID_ANY, wxT("Search here:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	label_search->Wrap( -1 );
	top_sizer->Add( label_search, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	web_search = new wxSearchCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifndef __WXMAC__
	web_search->ShowSearchButton( true );
	#endif
	web_search->ShowCancelButton( false );
	top_sizer->Add( web_search, 0, wxALL, 5 );
	
	
	main_sizer->Add( top_sizer, 0, wxEXPAND, 5 );
	
	html_page = wxWebView::New( this, wxID_ANY, I18n::ManualFilePath() );
	main_sizer->Add( html_page, 1, wxEXPAND | wxALL, 5 );
	
	button_close = new wxButton( this, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	main_sizer->Add( button_close, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	this->SetTitle(__I18N("windows:main:contents"));

	// Connect Events
	web_search->Connect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( HelpDialog::OnSearchStart ), NULL, this );
	web_search->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( HelpDialog::OnSearchStart ), NULL, this );
	web_search->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( HelpDialog::OnSearchStart ), NULL, this );
	html_page->Connect( wxEVT_WEBVIEW_NEWWINDOW, wxWebViewEventHandler( HelpDialog::OnOpenPage ), NULL, this);
	button_close->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HelpDialog::OnClose ), NULL, this );
}

HelpDialog::~HelpDialog()
{
	// Disconnect Events
	web_search->Disconnect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( HelpDialog::OnSearchStart ), NULL, this );
	web_search->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( HelpDialog::OnSearchStart ), NULL, this );
	web_search->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( HelpDialog::OnSearchStart ), NULL, this );
	html_page->Disconnect( wxEVT_WEBVIEW_NEWWINDOW, wxWebViewEventHandler( HelpDialog::OnOpenPage ), NULL, this);
	button_close->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HelpDialog::OnClose ), NULL, this );
	
}

void 
HelpDialog::OnSearchStart(wxString value) {
	this->html_page->Find(value);
}

void 
HelpDialog::OnClose() {
	this->Hide();
}

void 
HelpDialog::OnOpenPage(wxString url) {
	wxLaunchDefaultBrowser(url);
}