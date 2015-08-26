#include "qgf_api_implementation.hpp"
#include "project_manager.hpp"
#include "window_manager.hpp"
#include "i18n.hpp"

static qgf::API* api_instance = NULL;

qgf::API* 
Api::Instance() {
	if (api_instance) return api_instance;
	api_instance = new Api();
	return api_instance;
}

qgf::Project* 
Api::GetProject() {
	return ProjectManager::GetCurrent();
}

wxArrayString LookForFolders(FolderType type, std::vector<Folder>* folders, wxString parent_name) {
	wxArrayString result = wxArrayString();
	for (std::vector<Folder>::iterator it = folders->begin(); it != folders->end(); ++it) {
		wxString full_path = parent_name + wxFILE_SEP_PATH + it->GetName();
		wxArrayString sub_folders = LookForFolders(type, it->GetSubFolders(), full_path);
		if (it->GetType() == type) result.Add(full_path);
		for (wxArrayString::iterator i = sub_folders.begin(); i != sub_folders.end(); ++i) {
			result.Add(*i);
		}
	}
	return result;
}

wxArrayString 
Api::GetImageFolderLocations() {
	return LookForFolders(QGF_FOLDER_TYPE_IMAGE, ProjectManager::GetCurrent()->GetFolders()->GetFolders(), wxT(""));
}

wxArrayString 
Api::GetScriptFolderLocations() {
	return LookForFolders(QGF_FOLDER_TYPE_SCRIPT, ProjectManager::GetCurrent()->GetFolders()->GetFolders(), wxT(""));
}

wxArrayString 
Api::GetVideoFolderLocations() {
	return LookForFolders(QGF_FOLDER_TYPE_VIDEO, ProjectManager::GetCurrent()->GetFolders()->GetFolders(), wxT(""));
}

wxArrayString 
Api::GetMusicFolderLocations() {
	return LookForFolders(QGF_FOLDER_TYPE_MUSIC, ProjectManager::GetCurrent()->GetFolders()->GetFolders(), wxT(""));
}

wxArrayString 
Api::GetSFXFolderLocations() {
	return LookForFolders(QGF_FOLDER_TYPE_SFX, ProjectManager::GetCurrent()->GetFolders()->GetFolders(), wxT(""));
}

wxArrayString 
Api::GetFontFolderLocations() {
	return LookForFolders(QGF_FOLDER_TYPE_FONT, ProjectManager::GetCurrent()->GetFolders()->GetFolders(), wxT(""));
}

wxString 
Api::Translate(wxString key, wxString plugin_name) {
	return I18n::PluginTranslate(key, plugin_name);
}

wxString 
Api::Translate(wxString key) {
	return I18n::Translate(key);
}