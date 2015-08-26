///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __QGF_ABOUT_WINDOW_H
#define __QGF_ABOUT_WINDOW_H 1

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/hyperlink.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialog
///////////////////////////////////////////////////////////////////////////////
class AboutDialog : public wxDialog 
{
	private:
		void OnClose();
	protected:
		wxStaticBitmap* logo_bitmap;
		wxScrolledWindow* left_scroll;
		wxStaticText* version_label;
		wxStaticText* autor_label;
		wxStaticText* lang_label;
		wxStaticText* lang_ver_label;
		wxStaticText* lang_author_label;
		wxScrolledWindow* right_scroll;
		wxStaticText* ruby_version_label;
		wxStaticText* ruby_copyright_label;
		wxHyperlinkCtrl* ruby_hyperlink;
		wxStaticText* wxwidgets_label;
		wxHyperlinkCtrl* wx_widgets_link;
		wxStaticText* audiere_label;
		wxHyperlinkCtrl* audiere_link;
		wxStaticText* freetype_label;
		wxHyperlinkCtrl* freetype_link;
		wxStaticText* pugg_label;
		wxHyperlinkCtrl* pugg_link;
		wxButton* button_close;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCommandEvent& event ) { OnClose(); event.Skip(); }
		
	
	public:
		
		AboutDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 627,423 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~AboutDialog();
	
};

#endif 
