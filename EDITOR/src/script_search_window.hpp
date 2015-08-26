///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __QGF_SCRIPT_SEARCH_DIALOG_H
#define __QGF_SCRIPT_SEARCH_DIALOG_H 1

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
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

#include <vector>

///////////////////////////////////////////////////////////////////////////

class ScriptSearchResult : public wxClientData {
public:
	wxString text;
	wxString filename;
	int line;
	ScriptSearchResult(wxString filename, int line, wxString text) { this->filename = filename; this->line = line; this->text = text; }
};


///////////////////////////////////////////////////////////////////////////////
/// Class ScriptSearchDialog
///////////////////////////////////////////////////////////////////////////////
class ScriptSearchDialog : public wxDialog 
{
	private:
			wxString data_name;

		void OnSearchStart();
		void OnLookToScript();
		std::vector<ScriptSearchResult> SearchOnFile(wxString filename);
		std::vector<ScriptSearchResult> SearchOnFolder(wxString filename);
		std::vector<ScriptSearchResult> ScriptSearchDialog::SearchOnAllScripts(wxArrayString script_list);
		std::vector<ScriptSearchResult> SearchAnywhere();
		void ShowSearchResults(std::vector<ScriptSearchResult> results);
	protected:
		wxStaticText* search_label;
		wxTextCtrl* text_search;
		wxStaticText* label_location;
		wxChoice* search_choice;
		wxButton* button_search;
		wxListBox* list_result;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnSearchStart( wxCommandEvent& event ) { OnSearchStart(); event.Skip(); }
		virtual void OnLookToScript( wxCommandEvent& event ) { OnLookToScript(); event.Skip(); }
		
	
	public:
		
		ScriptSearchDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Cuadro de búsqueda"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 578,343 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ScriptSearchDialog();
	
		void SetSearchMode(int mode);
};

#endif 
