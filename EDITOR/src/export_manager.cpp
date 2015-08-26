#include "export_manager.hpp"
#include "api_manager.hpp"
#include "qgf_info.hpp"
#include "project_manager.hpp"
#include "window_manager.hpp"

#include <wx/dir.h>
#include <wx/filename.h>

wxArrayString 
qgf::ExportManager::GetExporters() {
	wxString path = wxGetCwd() + wxFILE_SEP_PATH + wxT("exporters");
    wxDir dir(path);
	if ( !wxDir::Exists(path) || !dir.IsOpened() ) {
        return wxArrayString();
    }
	wxArrayString exporterList = wxArrayString();
	wxString name;
    bool cont = dir.GetFirst(&name, wxT("*.*"), wxDIR_FILES);
    while ( cont ) {
		wxFileName filename = wxFileName(name);
		if (filename.GetExt() == wxT(QGF_PLUGIN_EXT_A) || wxFileName(name).GetExt() == wxT(QGF_PLUGIN_EXT_B)) {
			exporterList.Add(filename.GetName());
		}
        cont = dir.GetNext(&name);
    }
	return exporterList;
}
	
qgf::Exporter* 
qgf::ExportManager::LoadExporter(wxString exporter_name) {
	wxString path = wxGetCwd() + wxFILE_SEP_PATH + wxT("exporters") + wxFILE_SEP_PATH + exporter_name;
	QGF_API_LIB_TYPE handle = qgf::ApiManager::LoadLib(path);
	LoadExporterFuncPtr load_func = (LoadExporterFuncPtr)qgf::ApiManager::LoadFunction(handle, "CreateExporter");
	if (!load_func) {
		return NULL;
	}
	return load_func();
}