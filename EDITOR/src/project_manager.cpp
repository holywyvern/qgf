#ifdef _WIN32
#include <Windows.h>
#endif

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>

#include "window_manager.hpp"
#include "project_manager.hpp"
#include "wx_extensions.hpp"
#include "i18n.hpp"

static Project* currentProject = NULL;

void 
ProjectManager::New() {
	if (currentProject) {
		if (!ProjectManager::ConfirmWhenOpenProject()) {
			return;
		}
	}
	if (ProjectManager::ProjectCreated()) {
		WindowManager::GetMainWindow()->ActivateProjectFunctions();
	}
	WindowManager::RecreateMainWindow();
}

void 
ProjectManager::Open() {
	if (currentProject) {
		if (!ProjectManager::ConfirmWhenOpenProject()) {
			return;
		}
	}
	wxFileDialog FilePicker(WindowManager::GetMainWindow(),__I18N("windows:open_qgf_file"), "", "",
                       __I18N("data:qgf_project") + " (*.qgfproj)|*.qgfproj", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	wxFileName file = wxFileName(wxStandardPaths::Get().GetAppDocumentsDir());
	file.AppendDir(wxT("QGF"));
	file.AppendDir(wxT("my projects"));
	FilePicker.SetDirectory(file.GetFullPath());
	if (FilePicker.ShowModal() == wxID_CANCEL) return;
	wxString path = wxFileName(FilePicker.GetPath()).GetPath();
	currentProject = new Project(path);
	currentProject->SetFolder(path);
	if (ProjectManager::ProjectOpened()) {
		WindowManager::GetMainWindow()->SetTitle(__I18N("windows:main:title_with_project") + currentProject->GetName());
		WindowManager::GetMainWindow()->ActivateProjectFunctions();
		WindowManager::RecreateMainWindow();
	}
}

void
ProjectManager::Close() {
	if (currentProject) {
		if (!ProjectManager::ConfirmWhenOpenProject()) {
			return;
		}
		currentProject->StopAllPlugins();
		delete currentProject;
		currentProject = NULL;
		WindowManager::RecreateMainWindow();
	}
}

bool
ProjectManager::Exit() {
	if (currentProject) {
		if (!ProjectManager::ConfirmWhenOpenProject()) {
			return true;
		}
		currentProject->StopAllPlugins();
		delete currentProject;
		currentProject = NULL;
	}
	WindowManager::GetMainWindow()->Destroy();
	return false;
}

void 
ProjectManager::Modify() {
	currentProject->Modify(true);
}

void 
ProjectManager::Save() {
	currentProject->Modify(false);
	if (!currentProject->Save()) {
		WindowManager::ShowError(__I18N("errors:not_saved"));
	}
}

void 
ProjectManager::Compile() {
	if (!ProjectManager::MustSaveProject()) {
		return;
	}
	WindowManager::ShowExportWindow();
}

bool
ProjectManager::ConfirmWhenOpenProject() {
	if (!currentProject->IsModified()) return true;
	int result = WindowManager::ShowConfirmation(__I18N("windows:confirm:want_save"), __I18N("windows:confirm:info_save"));
	if (result == wxID_CANCEL) {
		return false;
	}
	if (result == wxID_YES) {
		ProjectManager::Save();
	}
	return true;
}

bool 
ProjectManager::MustSaveProject() {
	if (!currentProject->IsModified()) return true;
	int result = WindowManager::ShowOkCancelConfirmation(__I18N("windows:confirm:save_required"), __I18N("windows:confirm:info_save"));
	if (result == wxID_OK) ProjectManager::Save();
	return result == wxID_CANCEL ? false : true;
}

Project*
ProjectManager::GetCurrent() {
	return currentProject;
}

void 
ProjectManager::PluginModify() {
	if (!ProjectManager::MustSaveProject()) {
		return;
	}
	WindowManager::ShowPluginManagerWindow();
}

void ProjectManager::ResourceModify() {
	if (!ProjectManager::MustSaveProject()) {
		return;
	}
	WindowManager::ShowResourceWindow();
}

void 
ProjectManager::ScriptModify() {
	if (!ProjectManager::MustSaveProject()) {
		return;
	}
	WindowManager::ShowScriptManagerWindow();
}

bool 
ProjectManager::ProjectOpened() {
	return currentProject != NULL;
}

bool 
ProjectManager::ProjectCreated() {
	Project* project = WindowManager::ShowNewProjectWindow();
	if (project) {
		if (currentProject) delete currentProject;
		currentProject = project;
		MainFrame* main_w = WindowManager::GetMainWindow();
		main_w->ActivateProjectFunctions();
		main_w->SetTitle(__I18N("windows:main:title_with_project") + project->GetName());
		project->MakeFromNew();
		return true;
	}
	return false;
}

Project*
ProjectManager::MakeProjectFromValues(wxString name, wxString path, wxString base_project) {
	wxString newPath = wxString(path + wxFILE_SEP_PATH + name);
	wxString base_project_path = wxGetCwd() + wxFILE_SEP_PATH + wxT("Project templates") + wxFILE_SEP_PATH + base_project;
	if (!wxDir::Exists(base_project_path)) {
		WindowManager::ShowError(__I18N("errors:base_project_not_exists"));
		return NULL;
	}
	if (base_project.Length() == 0) {
		WindowManager::ShowError(__I18N("errors:no_base_projects"));
		return NULL;
	}
	if (name.Length() == 0) {
		WindowManager::ShowError(__I18N("errors:empty_project_name"));
		return NULL;
	}
	if (path.Length() == 0) {
		WindowManager::ShowError(__I18N("errors:empty_project_folder"));
		return NULL;
	}
	if (wxDir::Exists(newPath)) {
		WindowManager::ShowError(__I18N("errors:repeated_project"));
		return NULL;
	};
	wxDir::Make(newPath, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	if (!wxCopyDir(base_project_path, newPath)) {
		WindowManager::ShowError(__I18N("errors:cant_copy_base_project"));
		return NULL;
	}
	Project* StoredProject = new Project();
	StoredProject->SetName(name);
	StoredProject->SetFolder(newPath);
	return StoredProject;
}

#ifdef _WIN32
#define QGF_BIN_NAME "win32"
#else
#define QGF_BIN_NAME "other"
#endif

wxString
ProjectManager::MakeTemporaryProject() {
	wxString temp_files = wxStandardPaths::Get().GetTempDir();
	temp_files = temp_files + wxFILE_SEP_PATH + wxT("QGF") + wxFILE_SEP_PATH + wxT("PLAYTEST");
	return temp_files;
}

wxString
BinDirectory() {
	return wxGetCwd() + wxFILE_SEP_PATH + wxT("bin") + wxFILE_SEP_PATH + QGF_BIN_NAME;
}

void
ProjectManager::PlayTest() {
	if (!ProjectManager::MustSaveProject()) {
		return;
	}
	wxString dst_path = ProjectManager::MakeTemporaryProject();
	wxString src_path = BinDirectory();
	if (!wxFileName::Rmdir(dst_path, wxPATH_RMDIR_FULL|wxPATH_RMDIR_RECURSIVE)) {
	}
	if (!wxDir::Make(dst_path, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL)) {
		return;
	}
	if (!wxCopyDir(src_path , dst_path)) {
		return;
	}
	if (!wxCopyDir(GetCurrent()->GetFolder(), dst_path)) {
		return;
	}
	wxString cwd = wxGetCwd();
#ifdef _WIN32
	wxString player = dst_path + wxFILE_SEP_PATH + "QGF.exe";
#endif

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );
	wxDialog* panel = new wxDialog(WindowManager::GetMainWindow(), wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300, 100), 0);

	wxStaticText* st = new wxStaticText( panel, wxID_ANY, wxT("Prueba del Proyecto en ejecución"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	st->Wrap( -1 );

	bSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	bSizer->Add( st, 0, wxEXPAND|wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	bSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	panel->SetSizer(bSizer);

	panel->CenterOnParent();
	panel->Show();
	wxSetWorkingDirectory(dst_path);
	wxExecute (player, wxEXEC_SYNC);
	wxSetWorkingDirectory(cwd);
	panel->Hide();

	WindowManager::GetMainWindow()->SetFocus();

}