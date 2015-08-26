#include "folder.hpp"

Folder::Folder(FolderType type, wxString name, wxString full_path) {
	this->full_path = full_path;
	this->name = name;
	this->type = type;
	this->subFolders = std::vector<Folder>();
}

wxString            
Folder::GetName() {
	return this->name;
}

void 
Folder::SetFullPath(wxString full_path) {
	this->full_path = full_path;
}

wxString 
Folder::GetFullPath() {
	return this->full_path;
}

void           
Folder::SetName(wxString name) {
	this->name = name;
}

std::vector<Folder>*
Folder::GetSubFolders() {
	return &(this->subFolders);
}

FolderType
Folder::GetType() {
	return this->type;
}

wxArrayString 
Folder::GetFileTypesByType(FolderType type) {
		wxArrayString result = wxArrayString();

		switch (type) {
		case QGF_FOLDER_TYPE_AUDIO: 
			break;
		case QGF_FOLDER_TYPE_IMAGE:
			result.Add(wxT(".bmp"));
			result.Add(wxT(".jpg"));
			result.Add(wxT(".jpeg"));
			result.Add(wxT(".png"));
			result.Add(wxT("bmp"));
			result.Add(wxT("jpg"));
			result.Add(wxT("jpeg"));
			result.Add(wxT("png"));
			break;
		case QGF_FOLDER_TYPE_FONT:
			result.Add(wxT(".ttf"));
			result.Add(wxT("ttf"));
			break;
		case QGF_FOLDER_TYPE_MUSIC: 
			result.Add(wxT(".ogg"));
			result.Add(wxT(".wav"));
			result.Add(wxT(".mp3"));
			result.Add(wxT(".mid"));
			result.Add(wxT("ogg"));
			result.Add(wxT("wav"));
			result.Add(wxT("mp3"));
			result.Add(wxT("mid"));
			break;
		case QGF_FOLDER_TYPE_SFX:  
			result.Add(wxT(".ogg"));
			result.Add(wxT(".wav"));
			result.Add(wxT(".mp3"));
			result.Add(wxT(".mid"));
			result.Add(wxT("ogg"));
			result.Add(wxT("wav"));
			result.Add(wxT("mp3"));
			result.Add(wxT("mid"));
			break;
		case QGF_FOLDER_TYPE_VIDEO: 
			result.Add(wxT(".ogv"));
			result.Add(wxT("ogv"));
			break;
		case QGF_FOLDER_TYPE_SCRIPT: 
			result.Add(wxT(".rb"));
			result.Add(wxT(".rbw"));
			result.Add(wxT("rb"));
			result.Add(wxT("rbw"));
			break;
		default:
			result.Add(wxT(".*"));
			result.Add(wxT("*"));
		}
		return result;
}