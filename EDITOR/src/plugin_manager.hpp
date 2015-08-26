#ifndef __QGF_PLUGIN_MANAGER_H
#define __QGF_PLUGIN_MANAGER_H 1

#include <wx/wx.h>
#include <vector>

#include "plugin.hpp"

namespace qgf {

namespace PluginManager {
	qgf::Plugin* LoadPlugin(wxString plugin_name);
	bool UnloadPlugin(wxString plugin_name);
	void ConfigurePlugin(wxWindow* parent, wxString project_path, wxString plugin_name);
	wxArrayString DetectPlugins();

}

}

#endif