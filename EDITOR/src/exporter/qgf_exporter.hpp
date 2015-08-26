#ifndef __QGF_EXPORTER_H
#define __QGF_EXPORTER_H 1

#include <wx\wx.h>

namespace qgf {
	class Exporter {
	public:
		virtual wxString GetExporterName() = 0;
		virtual void Export(wxWindow* main_window, wxString src_path, wxString dst_path, wxString cwd) = 0;
	};
};

typedef qgf::Exporter* (*LoadExporterFuncPtr)();

#endif