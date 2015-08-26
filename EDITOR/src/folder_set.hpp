#ifndef __QGF_FOLDER_SET_H
#define __QGF_FOLDER_SET_H 1

#include <vector>
#include "folder.hpp"

class FolderSet {
private:
	std::vector<Folder> folders;
public:
	FolderSet();
	std::vector<Folder>* GetFolders();
};

#endif