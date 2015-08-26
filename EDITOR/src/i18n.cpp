#include "i18n.hpp"
#include "tinyxml2.h"
#include "window_manager.hpp"

static tinyxml2::XMLDocument* lang_doc;

void
I18n::Init() {
	lang_doc = new tinyxml2::XMLDocument();
	lang_doc->LoadFile(LocaleFilename());
}

wxString 
I18n::Translate(wxString key) {
	if (!lang_doc) return key;
	wxArrayString keys = wxSplit(key, wxT(':'));
	tinyxml2::XMLElement* e;
	e = lang_doc->FirstChildElement("i18n");
	if (!e) return key;
	for (int i = 0; i < keys.GetCount(); i++) {
		if (!e) return key;
		e = e->FirstChildElement(keys[i].ToUTF8());
	}
	if (!e) return key;
	return wxString::FromUTF8(e->GetText());
}

wxString
PluginFilename(wxString plugin_name) {
	wxString file = wxGetCwd() + wxFILE_SEP_PATH + wxT("plugins") + wxFILE_SEP_PATH + wxT("i18n") + wxFILE_SEP_PATH + plugin_name + wxFILE_SEP_PATH +  wxLocale(wxLocale::GetSystemLanguage()).GetCanonicalName() + wxT(".xml");
	if (!wxFileName::FileExists(file)) {
		file = wxGetCwd() + wxFILE_SEP_PATH + wxT("plugins") + wxFILE_SEP_PATH + wxT("i18n") + wxFILE_SEP_PATH + plugin_name + wxFILE_SEP_PATH +  wxT("es_AR") + wxT(".xml");
	}
	return file;
}

wxString 
I18n::PluginTranslate(wxString key, wxString plugin_name) {
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->LoadFile(PluginFilename(plugin_name));
	wxArrayString keys = wxSplit(key, wxT(':'));
	tinyxml2::XMLElement* e;
	e = doc->FirstChildElement("i18n");
	if (!e) {
		delete doc;
		return key;
	}
	for (int i = 0; i < keys.GetCount(); i++) {
		if (!e) {
			delete doc;
			return key;
		}
		e = e->FirstChildElement(keys[i].ToUTF8());
	}
	if (!e) {
		delete doc;
		return key;
	}
	wxString result = wxString::FromUTF8(e->GetText());
	delete doc;
	return result;
}

wxString 
I18n::LocaleFilename() {
	wxString file = wxGetCwd() + wxFILE_SEP_PATH + wxT("i18n") + wxFILE_SEP_PATH +  wxLocale(wxLocale::GetSystemLanguage()).GetCanonicalName() + wxT(".xml");
	if (!wxFileName::FileExists(file)) {
		file = wxGetCwd() + wxFILE_SEP_PATH + wxT("i18n") + wxFILE_SEP_PATH + wxT("es_AR") + wxT(".xml");
	}
	return file;
}

wxString
I18n::ManualFilePath() {
	wxString path = wxGetCwd();
	path = wxGetCwd() + wxFILE_SEP_PATH + wxT("manuals") + wxFILE_SEP_PATH + wxLocale(wxLocale::GetSystemLanguage()).GetCanonicalName() + wxFILE_SEP_PATH + wxT("index.html");
	if (!wxFileName::FileExists(path)) {
		path = wxGetCwd() + wxFILE_SEP_PATH + wxT("manuals") + wxFILE_SEP_PATH + wxT("es_AR") + wxFILE_SEP_PATH + wxT("index.html");
	}
	path.Replace(wxT('\\'), wxT('/'), true);
	wxString file = wxT("file:///") + path;
	return file;
}