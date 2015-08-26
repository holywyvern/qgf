#include "new_project_window.hpp"
#include "project_manager.hpp"
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include "window_manager.hpp"
#include "i18n.hpp"

NewProjectDialog::NewProjectDialog(  wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	base_projects = std::vector<wxString>();

	StoredProject = NULL;
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxGridBagSizer* main_sizer;
	main_sizer = new wxGridBagSizer( 0, 0 );
	main_sizer->SetFlexibleDirection( wxBOTH );
	main_sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	label_project_name = new wxStaticText( this, wxID_ANY, __I18N("windows:new_project:name"), wxDefaultPosition, wxDefaultSize, 0 );
	label_project_name->Wrap( -1 );
	main_sizer->Add( label_project_name, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	text_name = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	main_sizer->Add( text_name, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	label_project_location = new wxStaticText( this, wxID_ANY, __I18N("windows:new_project:folder"), wxDefaultPosition, wxDefaultSize, 0 );
	label_project_location->Wrap( -1 );
	main_sizer->Add( label_project_location, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	dir_picker = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, __I18N("windows:new_project:folder_select"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL );
	main_sizer->Add( dir_picker, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	label_base_prject = new wxStaticText( this, wxID_ANY, __I18N("windows:new_project:base"), wxDefaultPosition, wxDefaultSize, 0 );
	label_base_prject->Wrap( -1 );
	main_sizer->Add( label_base_prject, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	choice_base_project = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, this->AddBaseProjects(), 0 );
	choice_base_project->SetSelection( 0 );
	main_sizer->Add( choice_base_project, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* button_sizer;
	button_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	button_create = new wxButton( this, wxID_ANY, __I18N("windows:new_project:create"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_create, 0, wxALL, 5 );
	
	button_cancel = new wxButton( this, wxID_ANY, __I18N("windows:new_project:cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_cancel, 0, wxALL, 5 );
	
	
	main_sizer->Add( button_sizer, wxGBPosition( 3, 1 ), wxGBSpan( 1, 2 ), wxALIGN_RIGHT, 5 );
	
	
	main_sizer->AddGrowableCol( 1 );
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	//wxFileName file = wxFileName(wxStandardPaths::Get().GetDocumentsDir());
	wxFileName file = wxFileName(wxStandardPaths::Get().GetAppDocumentsDir());
	file.AppendDir(wxT("QGF"));
	file.AppendDir(wxT("my projects"));
	dir_picker->SetDirName(file);

	this->SetTitle(__I18N("windows:main:new_project"));

	this->SetSize(480, 210);

	// Connect Events
	button_create->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewProjectDialog::OnCreateProject ), NULL, this );
	button_cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewProjectDialog::OnCancel ), NULL, this );
}

NewProjectDialog::~NewProjectDialog()
{
	// Disconnect Events
	button_create->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewProjectDialog::OnCreateProject ), NULL, this );
	button_cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewProjectDialog::OnCancel ), NULL, this );
}

void 
NewProjectDialog::OnCreateProject() {

	StoredProject = ProjectManager::MakeProjectFromValues(text_name->GetLineText(0), dir_picker->GetPath(), choice_base_project->GetString(choice_base_project->GetSelection()));
	if (StoredProject) {
		this->Hide();
	}
}

void 
NewProjectDialog::OnCancel() {
	StoredProject = NULL;
	this->Hide();
}

Project*
NewProjectDialog::MakeProject() {
	return StoredProject;
}

wxArrayString
NewProjectDialog::AddBaseProjects() {
	wxString path = wxGetCwd() + wxFILE_SEP_PATH + wxT("Project templates");
    wxDir dir(path);

	if ( !wxDir::Exists(path) || !dir.IsOpened() ) {
		WindowManager::ShowError(__I18N("errors:no_base_projects"));
        return wxArrayString();
    }

	wxArrayString dirList = wxArrayString();
	wxString name;
    bool cont = dir.GetFirst(&name, wxEmptyString, wxDIR_DIRS);
    if ( !cont ) {
		WindowManager::ShowError(__I18N("errors:no_base_projects"));
        return wxArrayString();
    }
    while ( cont )
    {
        dirList.Add( name );
        cont = dir.GetNext(&name);
    }

	return dirList;
}