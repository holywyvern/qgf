#ifndef __QGF_MODELS_PROJECT_H

#define __QGF_MODELS_PROJECT_H 1

#include <wx/wx.h>
#include <string>
#include "folder_set.hpp"
#include "tinyxml2.h"
#include "plugin.hpp"
#include <map>
#include "qgf_project.hpp"

typedef std::map<wxString, qgf::Plugin*> PluginSet;

class Project : public qgf::Project {
private:
	wxString name;
	wxString folder;
	bool modified;
	FolderSet folders;
	PluginSet plugins;
protected:
	void OpenFromXML();
	bool SaveToXML();
	void OpenXMLInfo(tinyxml2::XMLElement* info);
	void OpenXMLFolders(tinyxml2::XMLElement* folders);
	void OpenXMLPlugins(tinyxml2::XMLElement* plugins);
	void WriteXMLInfo(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* info);
	void WriteXMLPlugins(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* plugins);
	void WriteXMLFolders(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* folders);
	void ReadFolderAndSub(Folder* f, tinyxml2::XMLElement* folder, wxString full_path);
public:
	Project();
	Project(wxString path);
	void        SetName(wxString folder);
	void        SetFolder(wxString folder);
	FolderSet*   GetFolders();
	
	void        MakeFromNew();

	void       AddPlugin(wxString plugin_name, qgf::Plugin* plugin);
	void       RemovePlugin(wxString plugin_name);
	void       StartAllPlugins();
	void       StopAllPlugins();
	bool       ContainsPlugin(wxString plugin_name);
	qgf::Plugin* FindPlugin(wxString plugin_name);
	PluginSet* GetPluginList();
	// API functions
	virtual wxString    GetName();
	virtual wxString    GetFolder();
	virtual bool RemoveFolderByName(wxString folder_name);
	virtual bool AddFolderByName(wxString folder_name, FolderType type);
	virtual void Modify(bool modify);
	virtual bool IsModified();
	virtual bool Save();
};

#endif
