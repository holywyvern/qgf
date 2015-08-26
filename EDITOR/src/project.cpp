#include "project.hpp"
#include "qgf_info.hpp"
#include "window_manager.hpp"
#include "folder.hpp"
#include <algorithm>
#include "plugin_manager.hpp"
#include "qgf_api_implementation.hpp"

Project::Project() {
	this->name = wxString();
	this->folders = FolderSet();
}

Project::Project(wxString path) {
	this->name = wxString();
	this->folder = path;
	this->folders = FolderSet();
	this->OpenFromXML();
}


void        
Project::SetName(wxString folder) {
	this->name = wxString(folder);
}

wxString
Project::GetFolder() {
	return this->folder;
}

void        
Project::SetFolder(wxString folder) {
	this->folder = folder;
}

void        
Project::Modify(bool modify) {
	this->modified = modify;
}

bool        
Project::IsModified() {
	return this->modified;
}

wxString 
Project::GetName() {
	return this->name;
}

bool
Project::Save() {
	if (!this->SaveToXML()) return false;
	this->modified = false;
	return true;
}

void 
Project::OpenFromXML() {
	wxString project_config = this->folder + wxFILE_SEP_PATH + wxT(QGF_PROJECT_CONFIG_NAME);
	tinyxml2::XMLDocument doc;
	doc.LoadFile(project_config.ToUTF8());
	if (doc.ErrorID()) {
		project_config.Replace(wxT("\\"), wxT("/"), true);
		doc.LoadFile(project_config.ToUTF8());
		if (doc.ErrorID()) {
			return;
		}
	}
	tinyxml2::XMLElement* root = doc.FirstChildElement(QGF_CONFIG_ROOT_TAG);
	if (root == NULL) return;
	this->OpenXMLInfo(root->FirstChildElement(QGF_CONFIG_INFO_TAG));
	this->OpenXMLFolders(root->FirstChildElement(QGF_CONFIG_FOLDERS_TAG));
	this->OpenXMLPlugins(root->FirstChildElement(QGF_CONFIG_PLUGINS_TAG));
	WindowManager::RecreateMainWindow();
}

void 
Project::OpenXMLInfo(tinyxml2::XMLElement* info) {
	if (info == NULL) return;
	tinyxml2::XMLElement* name = info->FirstChildElement(QGF_CONFIG_INFO_NAME_TAG);
	this->name = wxString::FromUTF8(name->GetText());
}

FolderType
FolderTypeFromString(wxString str) {
	if (str == wxT(QGF_CONFIG_FOLDERS_FOLDER_AUDIO_TAG)) {
		return QGF_FOLDER_TYPE_AUDIO;
	} else if (str == wxT(QGF_CONFIG_FOLDERS_FOLDER_IMAGE_TAG)) {
		return QGF_FOLDER_TYPE_IMAGE;
	} else if (str == wxT(QGF_CONFIG_FOLDERS_FOLDER_FONT_TAG)) {
		return QGF_FOLDER_TYPE_FONT;
	} else if (str == wxT(QGF_CONFIG_FOLDERS_FOLDER_SCRIPT_TAG)) {
		return QGF_FOLDER_TYPE_SCRIPT;
	} else if (str == wxT(QGF_CONFIG_FOLDERS_FOLDER_MUSIC_TAG)) {
		return QGF_FOLDER_TYPE_MUSIC;
	} else if (str == wxT(QGF_CONFIG_FOLDERS_FOLDER_SFX_TAG)) {
		return QGF_FOLDER_TYPE_SFX;
	} else if (str == wxT(QGF_CONFIG_FOLDERS_FOLDER_VIDEO_TAG)) {
		return QGF_FOLDER_TYPE_VIDEO;
	}
	return QGF_FOLDER_TYPE_OTHER;
}

void
Project::OpenXMLFolders(tinyxml2::XMLElement* folders) {

	for (tinyxml2::XMLElement* e = folders->FirstChildElement(QGF_CONFIG_FOLDERS_FOLDER_TAG);
		e != NULL; e = e->NextSiblingElement(QGF_CONFIG_FOLDERS_FOLDER_TAG)) {
			FolderType type = FolderTypeFromString(wxString::FromUTF8(e->Attribute(QGF_CONFIG_FOLDERS_FOLDER_ATT_TYPE_TAG)));
			wxString name = wxString::FromUTF8(e->Attribute(QGF_CONFIG_FOLDERS_FOLDER_ATT_NAME_TAG));
			Folder folder(type, name, name);
			ReadFolderAndSub(&folder, e, name); 
			this->folders.GetFolders()->push_back(folder);
	}
}

void
Project::ReadFolderAndSub(Folder* f, tinyxml2::XMLElement* folder, wxString full_path) {
	for (tinyxml2::XMLElement* e = folder->FirstChildElement(QGF_CONFIG_FOLDERS_FOLDER_TAG);
		e != NULL; e = e->NextSiblingElement(QGF_CONFIG_FOLDERS_FOLDER_TAG)) {
			FolderType type = FolderTypeFromString(wxString::FromUTF8(e->Attribute(QGF_CONFIG_FOLDERS_FOLDER_ATT_TYPE_TAG)));
			wxString name = wxString::FromUTF8(e->Attribute(QGF_CONFIG_FOLDERS_FOLDER_ATT_NAME_TAG));
			wxString new_full_path = full_path + wxFILE_SEP_PATH + name;
			Folder sub_folder(type, name, new_full_path);
			ReadFolderAndSub(&sub_folder, e, new_full_path); 
			f->GetSubFolders()->push_back(sub_folder);
	}
}


void
Project::OpenXMLPlugins(tinyxml2::XMLElement* plugins) {
	for (tinyxml2::XMLElement* e = plugins->FirstChildElement(QGF_CONFIG_PLUGINS_PLUGIN_TAG);
		e != NULL; e = e->NextSiblingElement(QGF_CONFIG_PLUGINS_PLUGIN_TAG)) {
		wxString plugin_name = wxString::FromUTF8(e->GetText());
		qgf::Plugin* plugin = qgf::PluginManager::LoadPlugin(plugin_name);
		this->AddPlugin(plugin_name, plugin);
	}
}

bool
Project::SaveToXML() {
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	tinyxml2::XMLElement* root = doc->NewElement(QGF_CONFIG_ROOT_TAG);
	wxString project_config = this->folder + wxFILE_SEP_PATH + wxT(QGF_PROJECT_CONFIG_NAME);
	tinyxml2::XMLElement* info    = doc->NewElement(QGF_CONFIG_INFO_TAG);
	tinyxml2::XMLElement* folders = doc->NewElement(QGF_CONFIG_FOLDERS_TAG);
	tinyxml2::XMLElement* plugins = doc->NewElement(QGF_CONFIG_PLUGINS_TAG);
	WriteXMLInfo(doc, info);
	WriteXMLFolders(doc, folders);
	WriteXMLPlugins(doc, plugins);
	root->InsertEndChild(info);
	root->InsertEndChild(folders);
	root->InsertEndChild(plugins);
	doc->InsertEndChild(root);
	if (doc->SaveFile(project_config.ToUTF8())) {
		delete doc;
		return false;
	}
	delete doc;
	return true;
}

void
Project::WriteXMLInfo(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* info) {
	tinyxml2::XMLElement* name       = doc->NewElement(QGF_CONFIG_INFO_NAME_TAG);
	tinyxml2::XMLElement* version    = doc->NewElement(QGF_CONFIG_INFO_VERSION_TAG);
	name->SetText(this->name.ToUTF8());
	version->SetText(QGF_VERSION);
	info->InsertEndChild(name);
	info->InsertEndChild(version);
}

void
Project::WriteXMLPlugins(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* plugins) {
	for (PluginSet::iterator it = this->plugins.begin(); it != this->plugins.end(); ++it) {
		tinyxml2::XMLElement* plugin = doc->NewElement(QGF_CONFIG_PLUGINS_PLUGIN_TAG);
		plugin->SetText(it->first.ToUTF8());
		plugins->InsertEndChild(plugin);
	}
}

wxString 
FolderTypeToString(FolderType type) {
	switch (type) {
	case QGF_FOLDER_TYPE_AUDIO:  return wxT(QGF_CONFIG_FOLDERS_FOLDER_AUDIO_TAG);
	case QGF_FOLDER_TYPE_IMAGE:  return wxT(QGF_CONFIG_FOLDERS_FOLDER_IMAGE_TAG);
	case QGF_FOLDER_TYPE_FONT:   return wxT(QGF_CONFIG_FOLDERS_FOLDER_FONT_TAG);
	case QGF_FOLDER_TYPE_MUSIC:  return wxT(QGF_CONFIG_FOLDERS_FOLDER_MUSIC_TAG);
	case QGF_FOLDER_TYPE_SFX:    return wxT(QGF_CONFIG_FOLDERS_FOLDER_SFX_TAG);
	case QGF_FOLDER_TYPE_VIDEO:  return wxT(QGF_CONFIG_FOLDERS_FOLDER_VIDEO_TAG);
	case QGF_FOLDER_TYPE_SCRIPT: return wxT(QGF_CONFIG_FOLDERS_FOLDER_SCRIPT_TAG);
	}
	return wxT("?");
}

tinyxml2::XMLElement*
LinkFolder(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* e, Folder* folder) {
	tinyxml2::XMLElement* new_folder = doc->NewElement(QGF_CONFIG_FOLDERS_FOLDER_TAG);
	new_folder->SetAttribute(QGF_CONFIG_FOLDERS_FOLDER_ATT_TYPE_TAG, FolderTypeToString(folder->GetType()).ToUTF8());
	new_folder->SetAttribute(QGF_CONFIG_FOLDERS_FOLDER_ATT_NAME_TAG, folder->GetName().ToUTF8());
	e->InsertEndChild(new_folder);
	return new_folder;
}

void
LinkSubFolders(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* e, Folder* folder) {
	std::vector<Folder>* f = folder->GetSubFolders();
	for (std::vector<Folder>::iterator i = f->begin(); i != f->end(); ++i) {
		tinyxml2::XMLElement* new_element = LinkFolder(doc, e, &*i);
		LinkSubFolders(doc, new_element, &*i);
	} 
}

void
Project::WriteXMLFolders(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* folders) {
	std::vector<Folder>* f = this->folders.GetFolders();
	for (std::vector<Folder>::iterator i = f->begin(); i != f->end(); ++i) {
		tinyxml2::XMLElement* new_element = LinkFolder(doc, folders, &*i);
		LinkSubFolders(doc, new_element, &*i);
	} 
}

void        
Project::MakeFromNew() {
	wxString name = this->name;
	this->OpenFromXML();
	this->name = name;
	this->SaveToXML();
}

void       
Project::AddPlugin(wxString plugin_name, qgf::Plugin* plugin) {
	this->plugins.insert(std::pair<wxString, qgf::Plugin*>(plugin_name, plugin));
}

void
Project::StartAllPlugins() {
	for(std::map<wxString, qgf::Plugin*>::iterator iter = this->plugins.begin(); iter != this->plugins.end(); ++iter) {
		wxString k =  iter->first;
		qgf::Plugin* plugin = qgf::PluginManager::LoadPlugin(k);
		MainFrame* main_window = WindowManager::GetMainWindow();
		iter->second = plugin;
		plugin->OnStartup(Api::Instance(), main_window, main_window->MenuEdit(), main_window->MenuTools(), main_window->Toolbar(), main_window->MainPanel());
	}
	this->modified = true;
}

void
Project::StopAllPlugins() {
	for(std::map<wxString, qgf::Plugin*>::iterator iter = this->plugins.begin(); iter != this->plugins.end(); ++iter) {
		wxString k =  iter->first;
		iter->second->OnDestruction(Api::Instance());
		qgf::PluginManager::UnloadPlugin(k);
	}
}

void       
Project::RemovePlugin(wxString plugin_name) {
	this->plugins.erase(plugin_name);
}

bool
Project::ContainsPlugin(wxString plugin_name) {
	return this->plugins.count(plugin_name) > 0;
}

PluginSet* 
Project::GetPluginList() {
	return &this->plugins;
}

qgf::Plugin*   
Project::FindPlugin(wxString plugin_name) {
	std::map<wxString, qgf::Plugin*>::iterator iterator = this->plugins.find(plugin_name);
	if (iterator == this->plugins.end()) return NULL;
	return iterator->second;
}

FolderSet* 
Project::GetFolders() {
	return &this->folders;
}

bool
Project::RemoveFolderByName(wxString folder_name) {
	wxArrayString folders = wxSplit(folder_name, wxFILE_SEP_PATH);
	std::vector<Folder>* folder_vec = this->folders.GetFolders();
	wxString folder_path = wxT("");
	Folder folder(QGF_FOLDER_TYPE_OTHER, wxT(""), wxT(""));
	for (wxArrayString::iterator it = folders.begin() ; it != folders.end(); ++it) {
		wxString new_full_path = (folder_path == wxT("")) ? *it : folder_path + wxFILE_SEP_PATH + *it;
		folder = Folder(QGF_FOLDER_TYPE_OTHER, *it, new_full_path);
		std::vector<Folder>::iterator pos = std::find(folder_vec->begin(), folder_vec->end(), folder);
		if (pos == folder_vec->end()) return false;
		if (pos->GetFullPath() == folder_name) {
			folder_vec->erase(pos);
			return true;
		}
		folder_vec = pos->GetSubFolders();
		folder_path = new_full_path;
	}
	return false;
}

bool 
Project::AddFolderByName(wxString folder_name, FolderType type) {
	wxArrayString folders = wxSplit(folder_name, wxFILE_SEP_PATH);
	std::vector<Folder>* folder_vec = this->folders.GetFolders();
	wxString folder_path = wxT("");
	Folder folder(QGF_FOLDER_TYPE_SCRIPT, wxT(""), wxT(""));
	if (folders.Count() == 1) {
		folder_vec->push_back(Folder(type, folders.Last(), folders.Last()));
		return true;
	}
	for (wxArrayString::iterator it = folders.begin() ; it != folders.end(); ++it) {
		wxString new_full_path = (folder_path == wxT("")) ? *it : folder_path + wxFILE_SEP_PATH + *it;
		folder = Folder(QGF_FOLDER_TYPE_OTHER, *it, new_full_path);
		std::vector<Folder>::iterator pos = std::find(folder_vec->begin(), folder_vec->end(), folder);
		if (pos == folder_vec->end()) return false;
		wxString full_path = pos->GetFullPath() + wxFILE_SEP_PATH + folders.Last();
		if (full_path == folder_name) {
			pos->GetSubFolders()->push_back(Folder(type, folders.Last(), full_path));
			return true;
		}

		folder_vec = pos->GetSubFolders();
		folder_path = new_full_path;
	}
	return false;
}