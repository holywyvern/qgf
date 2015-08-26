#ifndef __WINDOW_MANAGER_H
#define __WINDOW_MANAGER_H 1

#include <wx/wx.h>

#include "main_window.hpp"
#include "project.hpp"

namespace WindowManager {

	void SetMainWindow(MainFrame* window);
	MainFrame* GetMainWindow();
	int  ShowConfirmation(wxString message, wxString caption = wxT("windows:confirmation"));
	int  ShowConfirmationNoCancel(wxString message, wxString caption = wxT("windows:confirmation"));
	int  ShowOkCancelConfirmation(wxString message, wxString caption = wxT("windows:confirmation"));
	void ShowError(wxString message, wxString caption = wxT("windows:error"));
	void ShowInfo(wxString message, wxString caption = wxT("windows:information"));

	Project* ShowNewProjectWindow();
	void ShowPluginManagerWindow();
	void ShowResourceWindow();
	void ShowScriptManagerWindow();
	void ShowContents();
	void ShowAbout();
	Folder ShowNewFolderWindow();
	void DeleteAllDialogs();
	void RecreateMainWindow();
	void ShowImagePreview(wxString filename);

	void ShowExportWindow();

	wxString InputTextDialog(wxString caption, wxArrayString invalid_chars, wxString error_message = wxT("error"));
};

#endif