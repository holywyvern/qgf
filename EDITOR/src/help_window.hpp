///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __QGF_WINDOWS_HELP_H
#define __QGF_WINDOWS_HELP_H 1

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/srchctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/webview.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class HelpDialog
///////////////////////////////////////////////////////////////////////////////
class HelpDialog : public wxDialog 
{
	private:
		void OnSearchStart(wxString value);
		void OnClose();
		void OnOpenPage(wxString url);
	protected:
		wxStaticText* label_search;
		wxSearchCtrl* web_search;
		wxButton* button_close;
		wxWebView* html_page;

		// Virtual event handlers, overide them in your derived class
		virtual void OnSearchStart( wxCommandEvent& event ) { OnSearchStart(event.GetString()); event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { OnClose(); event.Skip(); }
		
		virtual void OnOpenPage(wxWebViewEvent& event) { OnOpenPage(event.GetURL()); event.Skip(); }
	
	public:
		
		HelpDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("QGF User Manual"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1024,700 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~HelpDialog();
	
};

#endif 
