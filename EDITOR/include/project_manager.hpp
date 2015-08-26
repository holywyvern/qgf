#include "project.hpp"

#ifndef __PROJECT_MANAGER_H
#define __PROJECT_MANAGER_H 1

namespace ProjectManager {
	void New();
	void Open();
	void Close();
	void Modify();
	void Save();
	void Compile();
	bool Exit();
	bool ConfirmWhenOpenProject();
	bool MustSaveProject();
	void PlayTest();
	Project* GetCurrent();

	void PluginModify();
	void ResourceModify();
	void ScriptModify();

	bool ProjectOpened();
	bool ProjectCreated();

	wxString MakeTemporaryProject();

	Project* MakeProjectFromValues(wxString name, wxString path, wxString base_project);

};

#endif