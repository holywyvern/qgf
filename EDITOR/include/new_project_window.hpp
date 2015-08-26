

#ifndef __NEW_PROJECT_WINDOW_H__
#define __NEW_PROJECT_WINDOW_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/filepicker.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/gbsizer.h>
#include <wx/dialog.h>

#include "project.hpp"

#include <vector>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class NewProjectDialog
///////////////////////////////////////////////////////////////////////////////
class NewProjectDialog : public wxDialog 
{
	private:
		void OnCreateProject();
		void OnCancel();
		Project* StoredProject;
		std::vector<wxString> base_projects;
	protected:
		wxStaticText* label_project_name;
		wxTextCtrl* text_name;
		wxStaticText* label_project_location;
		wxDirPickerCtrl* dir_picker;
		wxStaticText* label_base_prject;
		wxChoice* choice_base_project;
		wxButton* button_create;
		wxButton* button_cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCreateProject( wxCommandEvent& event ) { OnCreateProject(); event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { OnCancel(); event.Skip(); }
		wxArrayString AddBaseProjects();
	
	public:
		
		NewProjectDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("NewProject"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 480, 250 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~NewProjectDialog();
	
		Project* MakeProject();
};

#endif //__NEW_PROJECT_WINDOW_H__
