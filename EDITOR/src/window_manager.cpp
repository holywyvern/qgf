#include "window_manager.hpp"
#include "project_manager.hpp"
#include "image_preview_window.hpp"
#include "new_folder_window.hpp"
#include "help_window.hpp"
#include "about_window.hpp"
#include "script_manager_window.hpp"
#include "plugin_manager_window.hpp"
#include "resource_manager_window.hpp"
#include "new_project_window.hpp"
#include "i18n.hpp"
#include "text_input_window.hpp"
#include "export_window.hpp"

static MainFrame* qgf_main_window = NULL;

static ScriptManagerDialog*   script_window = NULL;
static PluginManagerDialog*   plugin_window = NULL;
static ResourceManagerDialog* resource_window = NULL;
static NewProjectDialog*      new_project_window = NULL;
static HelpDialog*            help_window        = NULL;
static AboutDialog*           about_window       = NULL;
static ImagePreviewDialog*    image_preview_window = NULL;
static NewFolderDialog*       new_folder_window  = NULL;
static TextInputDialog*       text_input_window = NULL;
static ExportDialog*          export_window = NULL;
void 
WindowManager::SetMainWindow(MainFrame* mainWindow) {
	qgf_main_window = mainWindow;
}

MainFrame* 
WindowManager::GetMainWindow() {
	return qgf_main_window;
}

int
WindowManager::ShowConfirmation(wxString message, wxString caption) {
	wxMessageDialog* d = new wxMessageDialog(qgf_main_window, message, I18n::Translate(caption), wxYES_NO|wxCANCEL|wxCENTRE|wxYES_DEFAULT|wxICON_QUESTION|wxSTAY_ON_TOP);
	int result = d->ShowModal();
	delete d;
	return result;
}

int
WindowManager::ShowConfirmationNoCancel(wxString message, wxString caption) {
	wxMessageDialog* d = new wxMessageDialog(qgf_main_window, message, I18n::Translate(caption), wxYES_NO|wxCENTRE|wxYES_DEFAULT|wxICON_QUESTION|wxSTAY_ON_TOP);
	int result = d->ShowModal();
	delete d;
	return result;
}

int  
WindowManager::ShowOkCancelConfirmation(wxString message, wxString caption) {
	wxMessageDialog* d = new wxMessageDialog(qgf_main_window, message, I18n::Translate(caption), wxOK|wxCANCEL|wxCENTRE|wxOK_DEFAULT|wxICON_QUESTION|wxSTAY_ON_TOP);
	int result = d->ShowModal();
	delete d;
	return result;
}

void 
WindowManager::ShowError(wxString message, wxString caption) {
	wxMessageDialog* d = new wxMessageDialog(qgf_main_window, message, I18n::Translate(caption), wxOK|wxCENTRE|wxOK_DEFAULT|wxICON_ERROR|wxSTAY_ON_TOP);
	d->ShowModal();
	delete d;
}

void 
WindowManager::ShowInfo(wxString message, wxString caption) {
	wxMessageDialog* d = new wxMessageDialog(qgf_main_window, message, I18n::Translate(caption), wxOK|wxCENTRE|wxOK_DEFAULT|wxICON_INFORMATION|wxSTAY_ON_TOP);
	d->ShowModal();
	delete d;
}

Project*
WindowManager::ShowNewProjectWindow() {
	WindowManager::DeleteAllDialogs();
	new_project_window = new NewProjectDialog(qgf_main_window, 4); 
	new_project_window->ShowModal();
	Project* p = new_project_window->MakeProject();
	return p;
}

void 
WindowManager::ShowPluginManagerWindow() {
	WindowManager::DeleteAllDialogs();
	plugin_window = new PluginManagerDialog(qgf_main_window, 2);
	plugin_window->ShowModal();
}

void 
WindowManager::ShowResourceWindow() {
	WindowManager::DeleteAllDialogs();
	resource_window = new ResourceManagerDialog(qgf_main_window, 3);
	resource_window->ShowModal();
}

void
WindowManager::ShowScriptManagerWindow() {
	WindowManager::DeleteAllDialogs();
	script_window = new ScriptManagerDialog(qgf_main_window, 1);
	script_window->ReloadScripts();
	script_window->ShowModal();
}

void 
WindowManager::ShowContents() {
	WindowManager::DeleteAllDialogs();
	help_window = new HelpDialog(qgf_main_window, 1);
	help_window->ShowModal();
}

void 
WindowManager::ShowAbout() {
	WindowManager::DeleteAllDialogs();
	about_window = new AboutDialog(qgf_main_window, 1);
	about_window->ShowModal();
}

void
WindowManager::ShowImagePreview(wxString filename) {
	WindowManager::DeleteAllDialogs();
	image_preview_window = new ImagePreviewDialog(qgf_main_window, 1);
	image_preview_window->SetPreview(filename);
	image_preview_window->ShowModal();
}

Folder
WindowManager::ShowNewFolderWindow() {
	WindowManager::DeleteAllDialogs();
	new_folder_window = new NewFolderDialog(qgf_main_window, 1);
	new_folder_window->ShowModal();
	if (!new_folder_window->FolderCreated()) return Folder(QGF_FOLDER_TYPE_OTHER, wxT(""), wxT(""));
	return Folder(new_folder_window->GetFolderType(), new_folder_window->GetFolderName(), wxT(""));
}

void 
WindowManager::DeleteAllDialogs() {
	script_window = NULL;
	plugin_window = NULL;
	resource_window = NULL;
	new_project_window = NULL;
	help_window = NULL;
	about_window = NULL;
	image_preview_window = NULL;
	new_folder_window = NULL;
	export_window = NULL;
}

void
WindowManager::RecreateMainWindow() {
	Project* current_p = ProjectManager::GetCurrent();
	WindowManager::DeleteAllDialogs();
	qgf_main_window->Destroy();
	if (current_p) current_p->StopAllPlugins();
	MainFrame* window = new MainFrame(NULL);
	WindowManager::SetMainWindow(window);
	window->Show(true);
	window->Centre();
	if (current_p) current_p->StartAllPlugins();
}

void 
WindowManager::ShowExportWindow() {
	WindowManager::DeleteAllDialogs();
	export_window = new ExportDialog(qgf_main_window);
	export_window->ShowModal();
}

wxString 
WindowManager::InputTextDialog(wxString caption, wxArrayString invalid_chars, wxString error_message) {
	text_input_window = new TextInputDialog(qgf_main_window);
	text_input_window->SetInvalidCharacters(invalid_chars, error_message);
	text_input_window->SetTitle(caption);
	text_input_window->ShowModal();
	return text_input_window->GetInputText();
}