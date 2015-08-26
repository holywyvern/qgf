#ifndef __QGF_I18N_H
#define __QGF_I18N_H 1

#include <wx/wx.h>

namespace I18n {
	void Init();
	wxString Translate(wxString key);
	wxString PluginTranslate(wxString key, wxString plugin_name);
	wxString LocaleFilename();
	wxString ManualFilePath();
}

#define __I18N(X) (I18n::Translate(wxT(X)))

#endif