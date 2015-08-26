///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __QGF_WINDOWS_NEW_FOLDER_H
#define __QGF_WINDOWS_NEW_FOLDER_H 1

#include "folder.hpp"

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class NewFolderDialog
///////////////////////////////////////////////////////////////////////////////
class NewFolderDialog : public wxDialog 
{
	private:
		void OnOk();
		void OnCancel();
		bool folder_created;
	protected:
		wxStaticText* label_name;
		wxTextCtrl* text_name;
		wxStaticText* label_type;
		wxChoice* choice_type;
		wxButton* button_ok;
		wxButton* button_cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { OnCancel(); event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { OnOk(); event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { OnCancel(); event.Skip(); }
	
	public:
		
		NewFolderDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 371,200 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~NewFolderDialog();
	
		bool FolderCreated();
		wxString GetFolderName();
		FolderType GetFolderType();
};

#endif 
