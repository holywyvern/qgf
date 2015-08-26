#include "resource_manager_window.hpp"
#include "project_manager.hpp"
#include "i18n.hpp"
#include <wx/dir.h>
#include "window_manager.hpp"
#include <algorithm>
#include "qgf_info.hpp"

///////////////////////////////////////////////////////////////////////////

ResourceManagerDialog::ResourceManagerDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* upper_sizer;
	upper_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* left_sizer;
	left_sizer = new wxBoxSizer( wxVERTICAL );
	
	tree_folders = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_SINGLE|wxTR_HIDE_ROOT|wxTR_TWIST_BUTTONS );
	left_sizer->Add( tree_folders, 1, wxALL|wxEXPAND, 5 );


	wxBoxSizer* left_button_sizer;
	left_button_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	button_new_folder = new wxButton( this, wxID_ANY, __I18N("windows:resource:new_folder") + __I18N("windows:menu_ext"), wxDefaultPosition, wxDefaultSize, 0 );
	left_button_sizer->Add( button_new_folder, 0, wxALL, 5 );
	
	button_new_subfolder = new wxButton( this, wxID_ANY, __I18N("windows:resource:new_subfolder") + __I18N("windows:menu_ext"), wxDefaultPosition, wxDefaultSize, 0 );
	left_button_sizer->Add( button_new_subfolder, 0, wxALL, 5 );

	button_delete_folder = new wxButton( this, wxID_ANY, __I18N("windows:resource:delete_folder") + __I18N("windows:menu_ext"), wxDefaultPosition, wxDefaultSize, 0 );
	left_button_sizer->Add( button_delete_folder, 0, wxALL, 5 );
	
	
	left_sizer->Add( left_button_sizer, 0, wxEXPAND, 5 );
	
	upper_sizer->Add( left_sizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* right_sizer;
	right_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	tree_resources = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_SINGLE|wxTR_HIDE_ROOT|wxTR_NO_LINES|wxTR_ROW_LINES|wxTR_FULL_ROW_HIGHLIGHT|wxTR_TWIST_BUTTONS );
	right_sizer->Add( tree_resources, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* button_sizer;
	button_sizer = new wxBoxSizer( wxVERTICAL );
	
	button_add = new wxButton( this, wxID_ANY, __I18N("windows:resource:import") + __I18N("windows:menu_ext"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_add, 0, wxALL, 5 );
	
	button_remove = new wxButton( this, wxID_ANY, __I18N("windows:resource:export") + __I18N("windows:menu_ext"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_remove, 0, wxALL, 5 );
	
	button_preview = new wxButton( this, wxID_ANY, __I18N("windows:resource:preview") + __I18N("windows:menu_ext"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_preview, 0, wxALL, 5 );
	
	button_delete = new wxButton( this, wxID_ANY, __I18N("windows:resource:delete") + __I18N("windows:menu_ext"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_delete, 0, wxALL, 5 );
	
	
	right_sizer->Add( button_sizer, 0, wxEXPAND, 5 );
	
	
	upper_sizer->Add( right_sizer, 1, wxEXPAND, 5 );
	
	
	main_sizer->Add( upper_sizer, 1, wxEXPAND, 5 );
	
	button_close = new wxButton( this, wxID_ANY, __I18N("windows:close"), wxDefaultPosition, wxDefaultSize, 0 );
	main_sizer->Add( button_close, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	this->SetTitle(__I18N("windows:main:resource_manager"));

	MakeImageList();

	this->ReadFoldersAndFiles();

	// Connect Events
	tree_folders->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( ResourceManagerDialog::OnFolderSelection ), NULL, this );
	button_new_folder->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnNewFolder ), NULL, this );
	button_new_subfolder->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnNewSubFolder ), NULL, this );
	button_delete_folder->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnDeleteFolder ), NULL, this );
	tree_resources->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( ResourceManagerDialog::OnResourceSelected ), NULL, this );
	button_add->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnImportResource ), NULL, this );
	button_remove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnExportResource ), NULL, this );
	button_preview->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnPreviewResource ), NULL, this );
	button_delete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnDeleteResource ), NULL, this );
	button_close->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnClose ), NULL, this );
}

ResourceManagerDialog::~ResourceManagerDialog()
{
	// Disconnect Events
	tree_folders->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( ResourceManagerDialog::OnFolderSelection ), NULL, this );
	button_new_folder->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnNewFolder ), NULL, this );
	button_new_subfolder->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnNewSubFolder ), NULL, this );
	button_delete_folder->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnDeleteFolder ), NULL, this );
	tree_resources->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( ResourceManagerDialog::OnResourceSelected ), NULL, this );
	button_add->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnImportResource ), NULL, this );
	button_remove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnExportResource ), NULL, this );
	button_preview->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnPreviewResource ), NULL, this );
	button_delete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnDeleteResource ), NULL, this );
	button_close->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ResourceManagerDialog::OnClose ), NULL, this );
	
}

void
ResourceManagerDialog::MakeImageList() {
	wxImageList* image_list = new wxImageList(16, 16);
	#if defined(__WXMSW__) || defined( __WXGTK__ )
	image_list->Add(wxBitmap(wxT("icons/resource/audio.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/resource/image.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/resource/font.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/resource/music.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/resource/sfx.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/resource/script.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/resource/video.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/resource/folder.png"), wxBITMAP_TYPE_ANY));
	#endif
	this->tree_folders->SetImageList(image_list);
}

class FolderTreeData : public wxTreeItemData {
public:
	wxString folder_path;

	Folder* folder;
	Folder* parent;
	FolderTreeData(wxString folder_path, Folder* folder, Folder* parent = NULL) {
		this->folder_path = folder_path;
		this->folder = folder;
		this->parent = parent;
	};

	wxArrayString 
	GetFiles() {
		wxDir dir(ProjectManager::GetCurrent()->GetFolder() + this->folder_path);
		dir.Open(ProjectManager::GetCurrent()->GetFolder() + this->folder_path);
		if (!dir.IsOpened()) return wxArrayString();
		wxArrayString extensions = Folder::GetFileTypesByType(this->folder->GetType());
		wxArrayString files = wxArrayString();
		wxString name;
		bool cont = dir.GetFirst(&name, wxT("*.*"), wxDIR_FILES);
		while ( cont ) {
			wxFileName filename = wxFileName(name);
			for (wxArrayString::iterator it = extensions.begin(); it != extensions.end(); ++it) {
				if (filename.GetExt() == *it) files.Add(name);
			}
			cont = dir.GetNext(&name);
		}
		return files;
	}

};

void 
ResourceManagerDialog::OnFolderSelection() {
	this->tree_resources->DeleteAllItems();
	wxTreeItemId item_id = this->tree_folders->GetSelection();
	FolderTreeData* data = (FolderTreeData*)this->tree_folders->GetItemData(item_id);
	wxArrayString files = data->GetFiles();
	wxTreeItemId root = this->tree_resources->AddRoot(wxT(""));
	for (wxArrayString::iterator it = files.begin(); it != files.end(); ++it) {
		this->tree_resources->AppendItem(root, *it, -1, -1, new FolderTreeData(data->folder_path, data->folder));
	}
	this->button_preview->Disable();
	this->button_remove->Disable();
	this->button_delete->Disable();
}

void 
ResourceManagerDialog::OnNewFolder() {
	Folder folder = WindowManager::ShowNewFolderWindow();
	folder.SetFullPath(folder.GetName());
	if (folder.GetName().IsEmpty()) return;
	wxTreeItemId root = this->tree_folders->GetRootItem();
	if (!root.IsOk()) {
		WindowManager::ShowError(__I18N("errors:cannot_add_folder"));
	}
	Project* project = ProjectManager::GetCurrent();
	wxString folder_path = project->GetFolder() + wxFILE_SEP_PATH + folder.GetName();
	if (!wxMkdir(folder_path)) {
		WindowManager::ShowError(__I18N("errors:cannot_add_folder"));
		return;
	}
	std::vector<Folder>* folder_vector = project->GetFolders()->GetFolders();
	folder_vector->push_back(folder);
	this->tree_folders->AppendItem(root, folder.GetName(), folder.GetType(), folder.GetType(), new FolderTreeData(wxFILE_SEP_PATH + folder.GetName(), &*folder_vector->rbegin()));
	ProjectManager::Save();
}

void 
ResourceManagerDialog::OnNewSubFolder() {
	wxTreeItemId item_id = this->tree_folders->GetSelection();
	if (!item_id.IsOk()) return;
	Folder folder = WindowManager::ShowNewFolderWindow();
	if (folder.GetName().IsEmpty()) return;
	FolderTreeData* data = (FolderTreeData*)this->tree_folders->GetItemData(item_id);
	folder.SetFullPath(data->folder->GetName() + wxFILE_SEP_PATH + folder.GetName());
	Project* project = ProjectManager::GetCurrent();
	wxString folder_path = project->GetFolder() + data->folder_path + wxFILE_SEP_PATH + folder.GetName();
	if (!wxMkdir(folder_path)) {
		WindowManager::ShowError(__I18N("errors:cannot_add_folder"));
		return;
	}
	std::vector<Folder>* folder_vector = data->folder->GetSubFolders();
	folder_vector->push_back(folder);
	this->tree_folders->AppendItem(item_id, folder.GetName(), folder.GetType(), folder.GetType(), new FolderTreeData(data->folder_path + wxFILE_SEP_PATH + folder.GetName(), &*folder_vector->rbegin()));
	ProjectManager::Save();
}

void 
ResourceManagerDialog::OnDeleteFolder() {
	wxTreeItemId item_id = this->tree_folders->GetSelection();
	FolderTreeData* data = (FolderTreeData*)this->tree_folders->GetItemData(item_id);
	if (WindowManager::ShowConfirmation(__I18N("windows:resource:delete_folder_confirm")) == wxID_YES) {
		wxString folder_path = ProjectManager::GetCurrent()->GetFolder() + data->folder_path;
		if (!wxFileName::Rmdir(folder_path, wxPATH_RMDIR_FULL|wxPATH_RMDIR_RECURSIVE)) {
			WindowManager::ShowError(__I18N("errors:cannot_delete_folder"));
		}
		std::vector<Folder>* parent_f = NULL; 
		if (data->parent) {
			parent_f = data->parent->GetSubFolders();
		} else {
			parent_f = ProjectManager::GetCurrent()->GetFolders()->GetFolders();
		}
		if (!parent_f) {
			WindowManager::ShowError(__I18N("errors:cannot_delete_folder"));
		}
		Folder f = *data->folder;
		std::vector<Folder>::iterator it = std::find(parent_f->begin(), parent_f->end(), f); 
		if (it != parent_f->end()) {
			parent_f->erase(it);
		}
		this->tree_folders->Delete(item_id);
		ProjectManager::Save();
	};
}

void 
ResourceManagerDialog::OnResourceSelected() {
	wxTreeItemId item_id = this->tree_resources->GetSelection();
	FolderTreeData* data = (FolderTreeData*)this->tree_resources->GetItemData(item_id);
	wxString file = this->tree_resources->GetItemText(item_id);
	this->button_remove->Enable();
	this->button_delete->Enable();
	wxFileName filename = wxFileName(file);
	wxString ext = filename.GetExt();
	if (ext == wxT(".bmp") || ext == wxT(".jpg") || ext == wxT(".jpeg") ||ext == wxT(".png") ||
		ext == wxT("bmp") || ext == wxT("jpg") || ext == wxT("jpeg") ||ext == wxT("png")) {
			this->button_preview->Enable();
	}
}

wxString
FolderStringByType(FolderType type) {
	switch (type) {
	case QGF_FOLDER_TYPE_AUDIO:
	case QGF_FOLDER_TYPE_MUSIC:
    case QGF_FOLDER_TYPE_SFX:
		return __I18N("data:audio_extension") + wxT("(") wxT(QGF_AUDIO_FORMATS) + wxT(")|") + wxT(QGF_AUDIO_FORMATS);
	case QGF_FOLDER_TYPE_IMAGE:
		return __I18N("data:image_extension") + wxT("(") wxT(QGF_IMAGE_FORMATS) + wxT(")|") + wxT(QGF_IMAGE_FORMATS);
	case QGF_FOLDER_TYPE_FONT:
		return __I18N("data:font_extension") + wxT("(") wxT(QGF_FONT_FORMATS) + wxT(")|") + wxT(QGF_FONT_FORMATS);
	case QGF_FOLDER_TYPE_SCRIPT:
		return __I18N("data:script_extension") + wxT("(") wxT(QGF_SCRIPT_FORMATS) + wxT(")|") + wxT(QGF_SCRIPT_FORMATS);
	case QGF_FOLDER_TYPE_VIDEO:
		return __I18N("data:video_extension") + wxT("(") wxT(QGF_VIDEO_FORMATS) + wxT(")|") + wxT(QGF_VIDEO_FORMATS);
	default:
		return __I18N("data:any_extension") + wxT("(*.*)|*.*");
	}
}


void 
ResourceManagerDialog::OnImportResource() {
	wxTreeItemId item_id = this->tree_folders->GetSelection();
	if (!item_id.IsOk()) return;
	FolderTreeData* data = (FolderTreeData*)this->tree_folders->GetItemData(item_id);

	wxString folder_type_filter = FolderStringByType(data->folder->GetType());
	wxFileDialog file_picker(WindowManager::GetMainWindow(),__I18N("windows:import_resource"), "", "",
                       folder_type_filter, wxFD_MULTIPLE|wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if (file_picker.ShowModal() == wxID_CANCEL) return;
	wxArrayString paths;
	file_picker.GetPaths(paths);
	for (wxArrayString::iterator it = paths.begin(); it != paths.end(); ++it) {
		wxString src_path = *it;
		wxFileName file = wxFileName(src_path);
		wxString dst_path = ProjectManager::GetCurrent()->GetFolder() + data->folder_path + wxFILE_SEP_PATH + file.GetFullName();
		if (wxFileName(dst_path).Exists()) {
			int id = WindowManager::ShowConfirmation(wxString::Format(__I18N("questions:override_file"), file.GetFullName()));
			if (id == wxID_CANCEL) {
				return;
			}
			if (src_path.IsSameAs(dst_path, false)) {
				WindowManager::ShowError(__I18N("errors:self_file_copy"));
				continue;
			}
			if (id == wxID_NO) {continue;}
		}
		wxCopyFile(src_path, dst_path, true);
		ResourceManagerDialog::OnFolderSelection();
	}
}

void 
ResourceManagerDialog::OnExportResource() {
	wxTreeItemId folder_id = this->tree_folders->GetSelection();
	if (!folder_id.IsOk()) return;
	wxTreeItemId item_id = this->tree_resources->GetSelection();
	if (!item_id.IsOk()) return;
	FolderTreeData* data = (FolderTreeData*)this->tree_folders->GetItemData(folder_id);
	wxString src_path = ProjectManager::GetCurrent()->GetFolder() + data->folder_path + wxFILE_SEP_PATH + this->tree_resources->GetItemText(item_id);
	if (!wxFileName(src_path).Exists()) return;
	wxString folder_type_filter = FolderStringByType(data->folder->GetType());
	wxFileDialog save_dialog(WindowManager::GetMainWindow(),__I18N("windows:export_resource"), "", "",
                       folder_type_filter, wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if (save_dialog.ShowModal() == wxID_CANCEL) return;
	wxCopyFile(src_path, save_dialog.GetPath(), true);
}

void
ResourceManagerDialog::OnPreviewResource() {
	wxTreeItemId item_id = this->tree_resources->GetSelection();
	FolderTreeData* data = (FolderTreeData*)this->tree_resources->GetItemData(item_id);
	wxString file = ProjectManager::GetCurrent()->GetFolder() + data->folder_path + wxFILE_SEP_PATH + this->tree_resources->GetItemText(item_id);
	if (!wxFileName(file).Exists()) return;
	WindowManager::ShowImagePreview(file);
}

void 
ResourceManagerDialog::OnDeleteResource() {
	wxTreeItemId folder_id = this->tree_folders->GetSelection();
	if (!folder_id.IsOk()) return;
	wxTreeItemId item_id = this->tree_resources->GetSelection();
	if (!item_id.IsOk()) return;
	FolderTreeData* data = (FolderTreeData*)this->tree_folders->GetItemData(folder_id);
	wxString src_path = ProjectManager::GetCurrent()->GetFolder() + data->folder_path + wxFILE_SEP_PATH + this->tree_resources->GetItemText(item_id);
	if (!wxFileName(src_path).Exists()) return;
	wxRemoveFile(src_path);
	ResourceManagerDialog::OnFolderSelection();
}

void 
ResourceManagerDialog::OnClose() {
	this->Hide();
}


void
ResourceManagerDialog::AppendTreeNodes(wxString old_path, wxTreeItemId node, std::vector<Folder>* folders, Folder* parent) {
	for (std::vector<Folder>::iterator it = folders->begin(); it != folders->end(); ++it) {
		wxString new_path = old_path + wxFILE_SEP_PATH + it->GetName();
		FolderTreeData* folder_data = new FolderTreeData(new_path, &*it, parent);
		wxTreeItemId folder = this->tree_folders->AppendItem(node, it->GetName(), it->GetType(), it->GetType(), folder_data);
		AppendTreeNodes(new_path, folder, it->GetSubFolders(), &*it);
	}
}

void
ResourceManagerDialog::ReadFoldersAndFiles() {
	Project* project = ProjectManager::GetCurrent();
	FolderSet* folder_set = project->GetFolders();
	wxTreeItemId root = this->tree_folders->AddRoot(wxT(""));
	AppendTreeNodes(wxT(""), root, folder_set->GetFolders(), NULL);
}

