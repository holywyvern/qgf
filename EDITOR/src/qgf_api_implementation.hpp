#ifndef __QGF_API_IMPLEMENTATION_H
#define __QGF_API_IMPLEMENTATION_H 1

#include "qgf_api.hpp"

class Api : public qgf::API {
public:
		static qgf::API* Instance();

		virtual qgf::Project* GetProject();
		virtual wxArrayString GetImageFolderLocations();
		virtual wxArrayString GetScriptFolderLocations();
		virtual wxArrayString GetVideoFolderLocations();
		virtual wxArrayString GetMusicFolderLocations();
		virtual wxArrayString GetSFXFolderLocations();
		virtual wxArrayString GetFontFolderLocations();

		virtual wxString Translate(wxString key, wxString plugin_name);
		virtual wxString Translate(wxString key);
};

#endif