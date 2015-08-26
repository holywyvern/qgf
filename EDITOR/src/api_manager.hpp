#ifndef __QGF_API_MANAGER_H
#define __QGF_API_MANAGER_H 1

#include <wx/wx.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _WIN32
typedef HINSTANCE QGF_API_LIB_TYPE;
typedef FARPROC   QGF_API_LIB_FUNC;
#endif

namespace qgf {

	namespace ApiManager {
		QGF_API_LIB_TYPE LoadLib(wxString lib_path);
		void             UnloadLib(wxString lib_path);
		QGF_API_LIB_FUNC LoadFunction(QGF_API_LIB_TYPE handle, wxString func_name);
	};

};

#endif
