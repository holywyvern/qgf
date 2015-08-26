///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __QGF_TEXT_INPUT_WINDOW_H
#define __QGF_TEXT_INPUT_WINDOW_H 1

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class TextInputDialog
///////////////////////////////////////////////////////////////////////////////
class TextInputDialog : public wxDialog 
{
	private:
		wxArrayString invalid_chars;
		wxString failure_caption;
		bool acepted;
		bool IsValidText();
		void OnCancel();
		void OnAccept();
	protected:
		wxTextCtrl* text_value;
		wxButton* button_ok;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCancel( wxCloseEvent& event ) { OnCancel();   event.Skip(); }
		virtual void OnAccept( wxCommandEvent& event ) { OnAccept(); event.Skip(); }
		
	
	public:
		
		TextInputDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 233,102 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~TextInputDialog();

		wxString GetInputText();
		void SetInvalidCharacters(wxArrayString invalid_chars, wxString failure_caption);
};

#endif 