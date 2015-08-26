#ifndef __QGF_FOLDER_H
#define __QGF_FOLDER_H 1

#include <vector>
#include <wx/wx.h>

#include "folder_type.hpp"

class Folder {
private:
	FolderType type;
	wxString name;
	wxString full_path;
	std::vector<Folder> subFolders;
public:
	Folder(FolderType type, wxString name,wxString full_path);
	wxString             GetName();
	wxString             GetFullPath();
	FolderType           GetType();
	void                 SetName(wxString name);
	void                 SetFullPath(wxString full_path);
	std::vector<Folder>*  GetSubFolders();
	static wxArrayString GetFileTypesByType(FolderType type);

	bool operator==(const Folder& other){ return this->full_path == other.full_path; };

};

#endif