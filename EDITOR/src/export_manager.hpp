#ifndef __QGF_EXPORT_MANAGER_H
#define __QGF_EXPORT_MANAGER_H 1

#include <wx/wx.h>
#include "qgf_exporter.hpp"

namespace qgf {

	namespace ExportManager {
		wxArrayString GetExporters();
		qgf::Exporter* LoadExporter(wxString exporter_name);
	};

};

#endif