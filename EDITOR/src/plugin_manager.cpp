#include "plugin_manager.hpp"	
#include "plugin.hpp"
#ifdef _WIN32
#include <Windows.h>
#endif
#include "project_manager.hpp"
#include "project_manager.hpp"
#include "window_manager.hpp"
#include <wx/dir.h>
#include "qgf_info.hpp"
#include "i18n.hpp"
#include "qgf_api_implementation.hpp"
#include "api_manager.hpp"

qgf::Plugin*
qgf::PluginManager::LoadPlugin(wxString plugin_name) {
	LoadPluginFuncPtr load_function = NULL;
	wxString lib_path = wxGetCwd() + wxFILE_SEP_PATH + wxT("plugins") + wxFILE_SEP_PATH + plugin_name;
	QGF_API_LIB_TYPE handle = qgf::ApiManager::LoadLib(lib_path);
	if (!handle) {
		WindowManager::ShowError(__I18N("errors:cannot_load_plugin"));
		return NULL;
	}
	load_function = (LoadPluginFuncPtr)qgf::ApiManager::LoadFunction(handle, wxT("ExportPlugin"));
	if (!load_function) {
		WindowManager::ShowError(__I18N("errors:cannot_load_plugin"));
		return NULL;
	}
	qgf::Plugin* plugin = NULL;
	if (load_function) plugin = load_function();
	if (plugin) {
		return plugin;
	}
	WindowManager::ShowError(__I18N("errors:cannot_load_plugin"));
	return NULL;
}

bool 
qgf::PluginManager::UnloadPlugin(wxString plugin_name) {
	return true;
}

wxArrayString 
qgf::PluginManager::DetectPlugins() {
	wxString path = wxGetCwd() + wxFILE_SEP_PATH + wxT("plugins");
    wxDir dir(path);
	if ( !wxDir::Exists(path) || !dir.IsOpened() ) {
        return wxArrayString();
    }
	wxArrayString pluginList = wxArrayString();
	wxString name;
    bool cont = dir.GetFirst(&name, wxT("*.*"), wxDIR_FILES);
    while ( cont ) {
		wxFileName filename = wxFileName(name);
		if (filename.GetExt() == wxT(QGF_PLUGIN_EXT_A) || wxFileName(name).GetExt() == wxT(QGF_PLUGIN_EXT_B)) {
			if (!ProjectManager::GetCurrent()->ContainsPlugin(filename.GetName())) pluginList.Add(filename.GetName());
		}
        cont = dir.GetNext(&name);
    }
	return pluginList;
}

void 
qgf::PluginManager::ConfigurePlugin(wxWindow* parent, wxString project_path, wxString plugin_name) {
	qgf::Plugin* plugin = ProjectManager::GetCurrent()->FindPlugin(plugin_name);
	if (plugin) {
		plugin->OpenConfigurationWindow(Api::Instance(), parent);
	}
}