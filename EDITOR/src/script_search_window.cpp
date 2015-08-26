///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "script_search_window.hpp"
#include "script_manager_window.hpp"
#include "qgf_api_implementation.hpp"
#include "i18n.hpp"

#include "project_manager.hpp"
#include "qgf_info.hpp"

#include <wx/textfile.h>
#include <wx/dir.h>



///////////////////////////////////////////////////////////////////////////

ScriptSearchDialog::ScriptSearchDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	search_label = new wxStaticText( this, wxID_ANY, __I18N("windows:script_search:frase"), wxDefaultPosition, wxDefaultSize, 0 );
	search_label->Wrap( -1 );
	main_sizer->Add( search_label, 0, wxALL, 5 );
	
	text_search = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	main_sizer->Add( text_search, 0, wxALL|wxEXPAND, 10 );
	
	label_location = new wxStaticText( this, wxID_ANY, __I18N("windows:script_search:location"), wxDefaultPosition, wxDefaultSize, 0 );
	label_location->Wrap( -1 );
	main_sizer->Add( label_location, 0, wxALL, 5 );
	
	wxString m_choice3Choices[] = { __I18N("windows:script_search:selected_script"), __I18N("windows:script_search:opened_script"), __I18N("windows:script_search:all_script") };
	int m_choice3NChoices = sizeof( m_choice3Choices ) / sizeof( wxString );
	search_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice3NChoices, m_choice3Choices, 0 );
	search_choice->SetSelection( 0 );
	main_sizer->Add( search_choice, 0, wxALL|wxEXPAND, 10 );
	
	button_search = new wxButton( this, wxID_ANY, __I18N("windows:script_search:start_search"), wxDefaultPosition, wxDefaultSize, 0 );
	main_sizer->Add( button_search, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	list_result = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB|wxLB_SINGLE ); 
	main_sizer->Add( list_result, 1, wxALL|wxEXPAND, 5 );
	
	this->SetTitle(__I18N("windows:search"));
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	button_search->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptSearchDialog::OnSearchStart ), NULL, this );
	list_result->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ScriptSearchDialog::OnLookToScript ), NULL, this );
}

ScriptSearchDialog::~ScriptSearchDialog()
{
	// Disconnect Events
	button_search->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptSearchDialog::OnSearchStart ), NULL, this );
	list_result->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ScriptSearchDialog::OnLookToScript ), NULL, this );
	
}

std::vector<ScriptSearchResult>   
ScriptSearchDialog::SearchOnFile(wxString filename) {
	std::vector<ScriptSearchResult>  result = std::vector<ScriptSearchResult>();
	wxTextFile      file;
	file.Open(ProjectManager::GetCurrent()->GetFolder() + filename);
	wxString search_text = this->text_search->GetLineText(0);
	// read the first line
	wxString str = file.GetFirstLine();
	int line = 1;
	if (str.Contains(search_text)) result.push_back(ScriptSearchResult(filename, line, str));

	while(!file.Eof()) {
		++line;
		str = file.GetNextLine();
		if (str.Contains(search_text)) result.push_back(ScriptSearchResult(filename, line, str));
	}
	return result;
}

std::vector<ScriptSearchResult>  
ScriptSearchDialog::SearchOnFolder(wxString folder) {
	wxString path = ProjectManager::GetCurrent()->GetFolder() + folder;
    wxDir dir(path);
	std::vector<ScriptSearchResult>  result = std::vector<ScriptSearchResult>();
	wxString name;
    bool cont = dir.GetFirst(&name, wxT("*.*"), wxDIR_FILES);
    while ( cont ) {
		wxFileName filename = wxFileName(name);
		if (filename.GetExt() == wxT(QGF_SCRIPT_EXT_A) || wxFileName(name).GetExt() == wxT(QGF_SCRIPT_EXT_B) ||
			filename.GetExt() == wxT(QGF_SCRIPT_EXT_C) || wxFileName(name).GetExt() == wxT(QGF_SCRIPT_EXT_D)) {
			std::vector<ScriptSearchResult> search = SearchOnFile(folder + wxFILE_SEP_PATH + filename.GetFullName());
			for (std::vector<ScriptSearchResult>::iterator i = search.begin(); i != search.end(); ++i) {
				result.push_back(*i);
			}
		}
        cont = dir.GetNext(&name);
    }
	return result;
}

std::vector<ScriptSearchResult> 
ScriptSearchDialog::SearchOnAllScripts(wxArrayString script_list) {
	std::vector<ScriptSearchResult>  result = std::vector<ScriptSearchResult>();
	for (wxArrayString::iterator i = script_list.begin(); i != script_list.end(); ++i) {
		std::vector<ScriptSearchResult> sr = this->SearchOnFile(*i);
		for (std::vector<ScriptSearchResult>::iterator i2 = sr.begin(); i2 != sr.end(); ++i2) {
			result.push_back(*i2);
		}
	}
	return result;
}

std::vector<ScriptSearchResult> 
ScriptSearchDialog::SearchAnywhere() {
	wxArrayString folders = Api::Instance()->GetScriptFolderLocations();
	std::vector<ScriptSearchResult>  result = std::vector<ScriptSearchResult>();
	for (wxArrayString::iterator i = folders.begin(); i != folders.end(); ++i) {
		std::vector<ScriptSearchResult> sr = this->SearchOnFolder(*i);
		for (std::vector<ScriptSearchResult>::iterator i2 = sr.begin(); i2 != sr.end(); ++i2) {
			result.push_back(*i2);
		}
	}
	return result;
}

void 
ScriptSearchDialog::OnSearchStart() {
	this->list_result->Clear();
	ScriptManagerDialog* parent = (ScriptManagerDialog*)this->GetParent();
	int sel = this->search_choice->GetSelection();
	if (sel == 0) {
		wxString script = parent->GetCurrentScriptName();
		if (script.IsEmpty()) {
			return;
		}
		ShowSearchResults(this->SearchOnFile(script));
	}else if (sel == 1) {
		wxArrayString opened = parent->GetOpenedScriptNames();
		ShowSearchResults(this->SearchOnAllScripts(opened));
	} else {
		ShowSearchResults(this->SearchAnywhere());
	};
}

void 
ScriptSearchDialog::OnLookToScript() {
	ScriptSearchResult* result = (ScriptSearchResult*)this->list_result->GetClientObject(this->list_result->GetSelection());
	if (!result) return;
	ScriptManagerDialog* parent = (ScriptManagerDialog*)this->GetParent();
	parent->OpenScript(result->filename, result->line);
}

void
ScriptSearchDialog::SetSearchMode(int mode) {
	this->search_choice->SetSelection(mode);
}

void 
ScriptSearchDialog::ShowSearchResults(std::vector<ScriptSearchResult> results) {
	this->list_result->Clear();
	for (std::vector<ScriptSearchResult>::iterator i = results.begin(); i != results.end(); ++i) {
		unsigned int n = this->list_result->Append(wxString::Format(__I18N("windows:script_search:result_format"), i->filename, i->line));
		this->list_result->SetClientObject(n, new ScriptSearchResult(i->filename, i->line, i->text));
	}
}