#include "api_manager.hpp"
#include "qgf_info.hpp"
#include "window_manager.hpp"
#include "i18n.hpp"

#include <map>

typedef std::map<wxString, QGF_API_LIB_TYPE> LibMap;
static LibMap qgf_lib_map = LibMap();



QGF_API_LIB_TYPE 
qgf::ApiManager::LoadLib(wxString lib_path) {
	lib_path = lib_path + wxT(QGF_PLUGIN_EXT_B);
	if (qgf_lib_map.count(lib_path) > 0) return qgf_lib_map.find(lib_path)->second;
	QGF_API_LIB_TYPE result;
#ifdef _WIN32
	result = LoadLibrary(lib_path);
	if (result == NULL) {
		WindowManager::ShowError(__I18N("errors:cannot_load_plugin"));
		return NULL;
	}
#endif
	qgf_lib_map.insert(std::make_pair(lib_path, result));
	return result;
}

QGF_API_LIB_FUNC 
qgf::ApiManager::LoadFunction(QGF_API_LIB_TYPE handle, wxString func_name) {
	return GetProcAddress(handle, func_name);
}

void            
qgf::ApiManager::UnloadLib(wxString lib_path) {
	lib_path = lib_path + wxT(QGF_PLUGIN_EXT_B);
#ifdef _WIN32
	if (qgf_lib_map.count(lib_path) > 0) {
		FreeLibrary(qgf_lib_map.find(lib_path)->second);
	}
#endif
	if (qgf_lib_map.count(lib_path) > 0) qgf_lib_map.erase(lib_path);
}