///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "export_window.hpp"
#include "project_manager.hpp"
#include "export_manager.hpp"
#include "window_manager.hpp"
#include "i18n.hpp"

#include <wx/dir.h>

class ExporterClientData : public wxClientData {
public:
	qgf::Exporter* exporter;
	ExporterClientData(qgf::Exporter* exporter) { this->exporter = exporter; }
};

///////////////////////////////////////////////////////////////////////////

ExportDialog::ExportDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* folder_sizer;
	folder_sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Directorio de salida") ), wxVERTICAL );
	
	folder_picker = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL );
	folder_sizer->Add( folder_picker, 0, wxALL|wxEXPAND, 5 );
	
	
	main_sizer->Add( folder_sizer, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* platform_sizer;
	platform_sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Plataforma") ), wxVERTICAL );
	
	wxArrayString platform_choiceChoices;
	platform_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, platform_choiceChoices, 0 );
	platform_choice->SetSelection( 0 );
	platform_sizer->Add( platform_choice, 0, wxALL|wxEXPAND, 5 );
	
	
	main_sizer->Add( platform_sizer, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* button_sizer;
	button_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	button_export = new wxButton( this, wxID_ANY, wxT("Exportar"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_export, 0, wxALL, 5 );
	
	button_cancel = new wxButton( this, wxID_ANY, wxT("Cancelar"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_cancel, 0, wxALL, 5 );
	
	
	main_sizer->Add( button_sizer, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	this->LoadImporters();

	// Connect Events
	folder_picker->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( ExportDialog::UpdateProjectDir ), NULL, this );
	button_export->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialog::OnExport ), NULL, this );
	button_cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialog::OnCancel ), NULL, this );
}

ExportDialog::~ExportDialog()
{
	// Disconnect Events
	folder_picker->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( ExportDialog::UpdateProjectDir ), NULL, this );
	button_export->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialog::OnExport ), NULL, this );
	button_cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialog::OnCancel ), NULL, this );
	
}

void 
ExportDialog::OnUpdateDir() {
}

void 
ExportDialog::OnExport() {
	wxString path = this->folder_picker->GetPath();
	if (path.IsEmpty()) {
		WindowManager::ShowError(__I18N("errors:empty_export_folder"));
		return;
	}
	if (this->platform_choice->GetSelection() < 0) {
		WindowManager::ShowError(__I18N("errors:no_platform_selected"));
		return;
	}
	Project* project = ProjectManager::GetCurrent();
	wxString dst_path = path + wxFILE_SEP_PATH + wxT("QGF - ") + project->GetName();
	if (wxDir::Exists(dst_path)) {
		if (!wxFileName::Rmdir(dst_path, wxPATH_RMDIR_FULL|wxPATH_RMDIR_RECURSIVE)) {
			return;
		}
	}
	if (!wxDir::Make(dst_path, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL)) {
		return;
	}
	ExporterClientData* data = (ExporterClientData*)this->platform_choice->GetClientObject(this->platform_choice->GetSelection());
	data->exporter->Export(WindowManager::GetMainWindow(), project->GetFolder(), dst_path, wxGetCwd());
	this->Hide();
}

void 
ExportDialog::OnCancel() {
	this->Hide();
}

void 
ExportDialog::LoadImporters() {
	wxArrayString exporters = qgf::ExportManager::GetExporters();
	for (wxArrayString::iterator i = exporters.begin(); i != exporters.end(); ++i) {
		qgf::Exporter* exporter = qgf::ExportManager::LoadExporter(*i);
		int n = this->platform_choice->Append(exporter->GetExporterName());
		this->platform_choice->SetClientObject(n, new ExporterClientData(exporter));
	}
}