#include "folder_set.hpp"

FolderSet::FolderSet() {
	folders = std::vector<Folder>();
}

std::vector<Folder>*
FolderSet::GetFolders() {
	return &(this->folders);
}