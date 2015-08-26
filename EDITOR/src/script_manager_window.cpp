#include "script_manager_window.hpp"
#include "i18n.hpp"
#include "qgf_info.hpp"
#include "qgf_api_implementation.hpp"
#include <wx/dir.h>
#include "qgf_info.hpp"
#include "project_manager.hpp"
#include "window_manager.hpp"
#include "script_page.hpp"
#include <wx/textfile.h>
#include "wx_extensions.hpp"


///////////////////////////////////////////////////////////////////////////

class ScriptTreeCtrl : public wxTreeCtrl {
protected:
	virtual int OnCompareItems(const wxTreeItemId& item1, const wxTreeItemId& item2);
public:
	ScriptTreeCtrl(wxWindow* parent) : wxTreeCtrl(parent, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), wxTR_DEFAULT_STYLE|wxTR_FULL_ROW_HIGHLIGHT|wxTR_HIDE_ROOT|wxTR_NO_LINES|wxTR_SINGLE|wxTR_TWIST_BUTTONS) { };

};

int
ScriptTreeCtrl::OnCompareItems(const wxTreeItemId& item1, const wxTreeItemId& item2) {
	return this->GetItemText(item1).CompareTo(this->GetItemText(item2));
}

ScriptManagerDialog::ScriptManagerDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->search_window = NULL;
	this->copy_data = NULL;
	script_normal_bitmap = wxBitmap(wxBitmap( wxT("icons/scripts/script.png"), wxBITMAP_TYPE_ANY ));
	script_modified_bitmap = wxBitmap(wxBitmap( wxT("icons/scripts/script_edit.png"), wxBITMAP_TYPE_ANY ));
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	panel_text_control = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* script_sizer;
	script_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* scriptbox_sizer;
	scriptbox_sizer = new wxBoxSizer( wxVERTICAL );
	
	script_list = new ScriptTreeCtrl( panel_text_control );
	scriptbox_sizer->Add( script_list, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* button_script_sizer;
	button_script_sizer = new wxBoxSizer( wxVERTICAL );
	
	script_add_button = new wxButton( panel_text_control, wxID_ANY, __I18N("windows:scripts:new_script") + __I18N("windows:menu_ext"), wxDefaultPosition, wxDefaultSize, 0 );
	button_script_sizer->Add( script_add_button, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	folder_add_button = new wxButton( panel_text_control, wxID_ANY, __I18N("windows:scripts:new_folder") + __I18N("windows:menu_ext"), wxDefaultPosition, wxDefaultSize, 0 );
	button_script_sizer->Add( folder_add_button, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );	

	scriptbox_sizer->Add( button_script_sizer, 0, wxEXPAND, 5 );
	
	script_sizer->Add( scriptbox_sizer, 0, wxEXPAND, 5 );
	
	panel_script = new wxPanel( panel_text_control, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* script_nb_sizer;
	script_nb_sizer = new wxBoxSizer( wxVERTICAL );
	
	script_tool = new wxToolBar( panel_script, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); 
	tool_cut = script_tool->AddTool( 101, wxT("tool"), wxBitmap( wxT("icons/cut.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL,  __I18N("windows:cut") + wxT(" (CTR+X)"), wxEmptyString, NULL ); 
	
	tool_copy = script_tool->AddTool( 102, wxT("tool"), wxBitmap( wxT("icons/copy.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:copy") + wxT(" (CTRL+C)"), wxEmptyString, NULL ); 
	
	tool_paste = script_tool->AddTool( 103, wxT("tool"), wxBitmap( wxT("icons/paste.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:paste") + wxT(" (CTRL+V)"), wxEmptyString, NULL ); 
	
	script_tool->AddSeparator(); 
	
	tool_undo = script_tool->AddTool( 104, wxT("tool"), wxBitmap( wxT("icons/arrow_undo.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:undo") + wxT(" (CTRL+Z)"), wxEmptyString, NULL ); 
	
	tool_redo = script_tool->AddTool( 105, wxT("tool"), wxBitmap( wxT("icons/arrow_redo.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:redo") + wxT(" (CTRL+Y)"), wxEmptyString, NULL ); 
	
	script_tool->AddSeparator(); 
	
	tool_find = script_tool->AddTool( 106, wxT("tool"), wxBitmap( wxT("icons/find.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:search") + wxT("(CTRL+F)"), wxEmptyString, NULL ); 
	
	script_tool->AddSeparator(); 
	
	tool_save = script_tool->AddTool( 108, wxT("tool"), wxBitmap( wxT("icons/save.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:save"), wxEmptyString, NULL ); 
	
	tool_save_all = script_tool->AddTool( 109, wxT("tool"), wxBitmap( wxT("icons/save_all.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:save_all"), wxEmptyString, NULL ); 
	
	tool_close = script_tool->AddTool( 110, wxT("tool"), wxBitmap( wxT("icons/close.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:close"), wxEmptyString, NULL ); 
	
	tool_close_all = script_tool->AddTool( 111, wxT("tool"), wxBitmap( wxT("icons/close_all.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:close_all"), wxEmptyString, NULL ); 


	script_tool->Realize(); 
	
	script_nb_sizer->Add( script_tool, 0, wxEXPAND, 5 );
	
	script_notebook = new wxAuiNotebook( panel_script, wxID_ANY );
	
	script_nb_sizer->Add( script_notebook, 1, wxEXPAND | wxALL, 5 );
	
	
	panel_script->SetSizer( script_nb_sizer );
	panel_script->Layout();
	script_nb_sizer->Fit( panel_script );
	script_sizer->Add( panel_script, 1, wxEXPAND | wxALL, 5 );
	
	
	panel_text_control->SetSizer( script_sizer );
	panel_text_control->Layout();
	script_sizer->Fit( panel_text_control );
	main_sizer->Add( panel_text_control, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* button_sizer;
	button_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	button_ok = new wxButton( this, wxID_ANY, __I18N("windows:ok"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_ok, 0, wxALL, 5 );
	
	button_cancel = new wxButton( this, wxID_ANY, __I18N("windows:cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_cancel, 0, wxALL, 5 );
	
	button_apply = new wxButton( this, wxID_ANY, __I18N("windows:apply"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_apply, 0, wxALL, 5 );
	
	button_help = new wxButton( this, wxID_ANY, __I18N("windows:help"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_help, 0, wxALL, 5 );
	
	
	main_sizer->Add( button_sizer, 0, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->SetSize(1024, 600);

	this->Centre( wxBOTH );
	
	this->SetTitle(__I18N("windows:main:script_manager"));

	this->CreateImageList();

	ReloadScripts();

	RealizeToolbar();

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ScriptManagerDialog::OnWindowClose ) );
	script_list->Connect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( ScriptManagerDialog::OnScriptClicked ), NULL, this );
	script_list->Connect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( ScriptManagerDialog::OnScriptMenu ), NULL, this );
	script_list->Connect( wxEVT_COMMAND_TREE_KEY_DOWN, wxTreeEventHandler( ScriptManagerDialog::OnScriptKeydown ), NULL, this );
	folder_add_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnAddFolder ), NULL, this );
	script_add_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnAddScript ), NULL, this );
	this->Connect( 101, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnCut ) );
	this->Connect( 102, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnCopy ) );
	this->Connect( 103, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnPaste ) );
	this->Connect( 104, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnUndo ) );
	this->Connect( 105, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnRedo ) );
	this->Connect( 106, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnSearch ) );
	this->Connect( 108, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnSave ) );
	this->Connect( 109, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnSaveAll ) );
	this->Connect( 110, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnClose ) );
	this->Connect( 111, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnCloseAll ) );
	script_notebook->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( ScriptManagerDialog::OnChangePage ), NULL, this );
	script_notebook->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler( ScriptManagerDialog::OnClosePage ), NULL, this );
	button_ok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnOk ), NULL, this );
	button_cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnCancel ), NULL, this );
	button_apply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnApply ), NULL, this );
	button_help->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnHelp ), NULL, this );
}

ScriptManagerDialog::~ScriptManagerDialog()
{
	if (copy_data) delete copy_data;
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ScriptManagerDialog::OnWindowClose ) );
	script_list->Disconnect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( ScriptManagerDialog::OnScriptClicked ), NULL, this );
	script_list->Disconnect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( ScriptManagerDialog::OnScriptMenu ), NULL, this );
	script_list->Disconnect( wxEVT_COMMAND_TREE_KEY_DOWN, wxTreeEventHandler( ScriptManagerDialog::OnScriptKeydown ), NULL, this );
	folder_add_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnAddFolder ), NULL, this );
	script_add_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnAddScript ), NULL, this );
	this->Disconnect( 101, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnCut ) );
	this->Disconnect( 102, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnCopy ) );
	this->Disconnect( 103, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnPaste ) );
	this->Disconnect( 104, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnUndo ) );
	this->Disconnect( 105, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnRedo ) );
	this->Disconnect( 106, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnSearch ) );
	this->Disconnect( 108, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnSave ) );
	this->Disconnect( 109, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnSaveAll ) );
	this->Disconnect( 110, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnClose ) );
	this->Disconnect( 111, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnCloseAll ) );
	script_notebook->Disconnect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( ScriptManagerDialog::OnChangePage ), NULL, this );
	script_notebook->Disconnect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler( ScriptManagerDialog::OnClosePage ), NULL, this );
	button_ok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnOk ), NULL, this );
	button_cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnCancel ), NULL, this );
	button_apply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnApply ), NULL, this );
	button_help->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptManagerDialog::OnHelp ), NULL, this );
	
}

class ScriptTreeData : public wxTreeItemData {
public:
	wxString folder;
	int type;

	ScriptTreeData(wxString folder, int type) { this->folder = folder; this->type = type; }
	bool isFolder() { return type == 0; }
};

void 
ScriptManagerDialog::OnScriptClicked() {
	wxTreeItemId item_id = this->script_list->GetSelection();
	if (!item_id.IsOk()) return;
	ScriptTreeData* data = (ScriptTreeData*)this->script_list->GetItemData(item_id);
	if (data->isFolder()) return;
	OpenScript(data->folder + wxFILE_SEP_PATH + this->script_list->GetItemText(item_id));
}

void 
ScriptManagerDialog::OnOk() {
	this->OnSaveAll();
	this->RealizeToolbar();
	this->Hide();
	if (search_window) search_window->Hide();
}

void 
ScriptManagerDialog::OnCancel() {
	this->RealizeToolbar();
	this->Hide();
	if (search_window) search_window->Hide();
}

void 
ScriptManagerDialog::OnApply() {
	this->OnSaveAll();
	this->RealizeToolbar();
}

void 
ScriptManagerDialog::OnHelp() {
	WindowManager::ShowContents();
}

void 
ScriptManagerDialog::OnCut() {
	ScriptPage* current_page = (ScriptPage*)this->script_notebook->GetCurrentPage();
	if (current_page) current_page->Cut();
	RealizeToolbar();
}

void 
ScriptManagerDialog::OnCopy() {
	ScriptPage* current_page = (ScriptPage*)this->script_notebook->GetCurrentPage();
	if (current_page) current_page->Copy();
	RealizeToolbar();
}

void
ScriptManagerDialog::OnPaste() {
	ScriptPage* current_page = (ScriptPage*)this->script_notebook->GetCurrentPage();
	if (current_page) current_page->Paste();
	RealizeToolbar();
}

void 
ScriptManagerDialog::OnUndo() {
	ScriptPage* current_page = (ScriptPage*)this->script_notebook->GetCurrentPage();
	if (current_page) current_page->Undo();
	RealizeToolbar();
}

void 
ScriptManagerDialog::OnRedo() {
	ScriptPage* current_page = (ScriptPage*)this->script_notebook->GetCurrentPage();
	if (current_page) current_page->Redo();
	RealizeToolbar();
}

void 
ScriptManagerDialog::OnSearch() {
	this->OpenSearch(QGF_SCRIPT_SEARCH_MODE_SCRIPT);
}

void 
ScriptManagerDialog::OnChangePage() {
	RealizeToolbar();
}

void 
ScriptManagerDialog::OnClosePage() {
	ScriptPage* page = (ScriptPage*)this->script_notebook->GetCurrentPage();
	if (!page) return;
	if (page->IsModified()) {
		int confirm = WindowManager::ShowConfirmationNoCancel(__I18N("questions:save_opened_script"));
		if (confirm == wxID_CANCEL) return;
		if (confirm == wxID_YES) page->Save();
	}
	if (this->script_notebook->GetPageCount() < 1) {
		if (search_window) search_window->Hide();
	}
	RealizeToolbar();
}

void 
ScriptManagerDialog::OnSave() {
	ScriptPage* current_page = (ScriptPage*)this->script_notebook->GetCurrentPage();
	if (current_page) current_page->Save();
	RealizeToolbar();
}

void 
ScriptManagerDialog::OnClose() {
	ScriptPage* page = (ScriptPage*)this->script_notebook->GetCurrentPage();
	if (!page) return;
	if (page->IsModified()) {
		int confirm = WindowManager::ShowConfirmation(__I18N("questions:save_opened_script"));
		if (confirm == wxID_CANCEL) return;
		if (confirm == wxID_YES) page->Save();
	}
	this->script_notebook->DeletePage(this->script_notebook->GetSelection());
	if (this->script_notebook->GetPageCount() < 1) {
		if (search_window) search_window->Hide();
	}
	RealizeToolbar();
}

void 
ScriptManagerDialog::OnSaveAll() {
	int count = this->script_notebook->GetPageCount();
	for (int i = 0; i < count; ++i) {
		ScriptPage* page = (ScriptPage*)this->script_notebook->GetPage(i);
		if (page) page->Save();
	}
	RealizeToolbar();
}

void 
ScriptManagerDialog::OnCloseAll() {
	while (this->script_notebook->GetPageCount()) {
			ScriptPage* page = (ScriptPage*)this->script_notebook->GetCurrentPage();
			if (page->IsModified()) {
				int confirm = WindowManager::ShowConfirmation(__I18N("questions:save_opened_script"));
				if (confirm == wxID_CANCEL) return;
				if (confirm == wxID_YES) page->Save();
			}
			this->script_notebook->DeletePage(this->script_notebook->GetSelection());
	}
	if (this->script_notebook->GetPageCount() < 1) {
		if (search_window) search_window->Hide();
	}
	RealizeToolbar();
}

void 
ScriptManagerDialog::CreateImageList() {
	wxImageList* image_list = new wxImageList(16, 16);
	#if defined(__WXMSW__) || defined( __WXGTK__ )
	image_list->Add(wxBitmap(wxT("icons/scripts/folder.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/scripts/file.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/scripts/folder_fade.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/scripts/file_fade.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/scripts/folder_open.png"), wxBITMAP_TYPE_ANY));
	image_list->Add(wxBitmap(wxT("icons/scripts/folder_open_fade.png"), wxBITMAP_TYPE_ANY));
	#endif
	this->script_list->SetImageList(image_list);
}

wxArrayString
GetScriptsFrom(wxString folder) {
	wxString path = ProjectManager::GetCurrent()->GetFolder() + folder;
    wxDir dir(path);
	wxArrayString result = wxArrayString();
	wxString name;
    bool cont = dir.GetFirst(&name, wxT("*.*"), wxDIR_FILES);
    while ( cont ) {
		wxFileName filename = wxFileName(name);
		if (filename.GetExt() == wxT(QGF_SCRIPT_EXT_A) || wxFileName(name).GetExt() == wxT(QGF_SCRIPT_EXT_B) ||
			filename.GetExt() == wxT(QGF_SCRIPT_EXT_C) || wxFileName(name).GetExt() == wxT(QGF_SCRIPT_EXT_D)) {
			result.Add(filename.GetFullName());
		}
        cont = dir.GetNext(&name);
    }
	return result;
}

wxArrayString 
GetAllScripts() {
	wxArrayString result = wxArrayString();
	wxArrayString folders = folders = Api::Instance()->GetScriptFolderLocations();
	for (wxArrayString::iterator it = folders.begin(); it != folders.end(); ++it) {
		wxArrayString scripts = GetScriptsFrom(*it);
		for (wxArrayString::iterator it2 = scripts.begin(); it2 != scripts.end(); ++it2) {
			result.Add(*it + wxFILE_SEP_PATH + *it2);
		}
	}
	return result;
}


void 
ScriptManagerDialog::OnScriptMenu(wxTreeItemId item_id) {
	if (!item_id.IsOk()) return;
	ScriptTreeData* data = (ScriptTreeData*)this->script_list->GetItemData(item_id);
	if (data->isFolder()) {
		this->OpenFolderMenu(item_id, data->folder);
	} else {
		this->OpenScriptMenu(item_id, data->folder + wxFILE_SEP_PATH + this->script_list->GetItemText(item_id));
	}
}

void 
ScriptManagerDialog::OnDelete() {
	wxTreeItemId item_id = this->script_list->GetSelection();
	if (!item_id.IsOk()) return;
	ScriptTreeData* data = (ScriptTreeData*)this->script_list->GetItemData(item_id);
	if (data->isFolder()) {
		this->DeleteFolder(data->GetId(), data->folder);
	} else {
		this->DeleteScript(data->GetId(), data->folder + wxFILE_SEP_PATH + this->script_list->GetItemText(item_id));
	}
	RealizeToolbar();
}

void
ScriptManagerDialog::OnAddScriptFromButton() {
	wxTreeItemId item_id = this->script_list->GetSelection();
	if (!item_id.IsOk()) return;
	ScriptTreeData* data = (ScriptTreeData*)this->script_list->GetItemData(item_id);
	this->CreateScript(item_id, data->folder);
}

void
ScriptManagerDialog::OnAddFolderFromButton() {
	wxTreeItemId item_id = this->script_list->GetSelection();
	if (!item_id.IsOk()) return;
	ScriptTreeData* data = (ScriptTreeData*)this->script_list->GetItemData(item_id);
	this->CreateFolder(item_id, data->folder);
}

void 
ScriptManagerDialog::OnTextChange() {
	RealizeToolbar();
}

void 
ScriptManagerDialog::RealizeToolbar() {
	size_t count = this->script_notebook->GetPageCount();
	bool save_all_cond = false;
	for (size_t i = 0; i < count; i++) {
		ScriptPage* page = (ScriptPage*)this->script_notebook->GetPage(i);
		if (page && page->IsModified()) {
			save_all_cond = true;
			this->script_notebook->SetPageText(i, page->GetScriptName() + wxT(" *"));
			this->script_notebook->SetPageBitmap(i, script_modified_bitmap);
		} else {
			this->script_notebook->SetPageText(i, page->GetScriptName());
			this->script_notebook->SetPageBitmap(i, script_normal_bitmap);
		}
	}
	ScriptPage* current_page = (ScriptPage*)this->script_notebook->GetCurrentPage();
	this->tool_close->Enable(count > 0);
	this->tool_close_all->Enable(count > 0);
	this->tool_copy->Enable(current_page && current_page->CanCopy());
	this->tool_cut->Enable(current_page && current_page->CanCut());
	this->tool_paste->Enable(current_page && current_page->CanPaste());
	this->tool_find->Enable(count > 0);
	this->tool_undo->Enable(current_page && current_page->CanUndo());
	this->tool_redo->Enable(current_page && current_page->CanRedo());
	this->tool_save->Enable(current_page && current_page->IsModified());
	this->tool_save_all->Enable(save_all_cond);
	this->script_tool->Realize();
}

size_t
ScriptManagerDialog::GetScriptIndex(wxString script_name) {
	for (size_t i = 0; i < this->script_notebook->GetPageCount(); ++i) {
		ScriptPage* page = (ScriptPage*)this->script_notebook->GetPage(i);
		if (page && page->GetScriptName() == script_name) return i;
	}
	return -1;
}

void
ScriptManagerDialog::OpenScript(wxString script_name) {
	OpenScript(script_name, 1);
}

void
ScriptManagerDialog::OpenScript(wxString script_name, int line) {
	wxString script_path = ProjectManager::GetCurrent()->GetFolder() + wxFILE_SEP_PATH + script_name;
	if (!wxFileName::Exists(script_path)) {
		WindowManager::ShowError(__I18N("errors:script_does_not_exists"));
	}
	int index = GetScriptIndex(script_name);
	if (index >= 0) {
		ScriptPage* page = (ScriptPage*)this->script_notebook->GetPage(index);
		page->GoToLine(line);
		this->script_notebook->SetSelection(index);
		return;
	}
	ScriptPage* page = new ScriptPage(this->script_notebook);
	page->SetScript(this, script_name);
	page->GoToLine(line);
	this->script_notebook->AddPage(page, script_name, true);
	RealizeToolbar();
}

void
ScriptManagerDialog::ReloadScripts() {
	this->script_list->DeleteAllItems();
	this->script_notebook->DeleteAllPages();
	wxTreeItemId root = this->script_list->AddRoot("");
	wxArrayString folders = Api::Instance()->GetScriptFolderLocations();
	for (wxArrayString::iterator i = folders.begin(); i != folders.end(); ++i) {
		wxTreeItemId folder_id = this->script_list->AppendItem(root, wxT(".") + *i, 0, 0, new ScriptTreeData(*i, 0));
		wxArrayString scripts = GetScriptsFrom(*i);
			this->script_list->SetItemImage(folder_id, 4, wxTreeItemIcon_Expanded);
			this->script_list->SetItemImage(folder_id, 4, wxTreeItemIcon_SelectedExpanded);
		for (wxArrayString::iterator i2 = scripts.begin(); i2 != scripts.end(); ++i2) {
			wxTreeItemId id = this->script_list->AppendItem(folder_id, *i2, 1, 1, new ScriptTreeData(*i, 1)); 
			this->script_list->SetItemImage(id, 4, wxTreeItemIcon_Expanded);
			this->script_list->SetItemImage(id, 4, wxTreeItemIcon_SelectedExpanded);
		}
		this->script_list->SortChildren(folder_id);
	}
	this->script_list->SortChildren(root);
	RealizeToolbar();
}

void
ScriptManagerDialog::OnWindowClose() {
	while (this->script_notebook->GetPageCount()) {
			ScriptPage* page = (ScriptPage*)this->script_notebook->GetCurrentPage();
			if (page->IsModified()) {
				int confirm = WindowManager::ShowConfirmationNoCancel(__I18N("questions:save_opened_script"));
				if (confirm == wxID_YES) page->Save();
			}
			this->script_notebook->DeletePage(this->script_notebook->GetSelection());
	}
	if (search_window) search_window->Hide();
	RealizeToolbar();
	
}

/*
void 
ScriptManagerDialog::ShowScript(Script* script) {
}
*/

#define ID_NEW_SUB_FOLDER 201
#define ID_NEW_SCRIPT     202
#define ID_COPY           203
#define ID_CUT            204
#define ID_PASTE          205
#define ID_DELETE         206
#define ID_SEARCH         207
#define ID_SEARCH_ALL     208

class MenuScriptData : public wxClientData {
public:
	wxTreeItemId id;
	wxString name;
	MenuScriptData(wxTreeItemId id, wxString name) { this->id = id; this->name = name; }
};

void
ScriptManagerDialog::OpenScriptMenu(wxTreeItemId item_id, wxString script_name) {
 	wxMenu mnu;
	mnu.SetClientObject(new MenuScriptData(item_id, script_name));
	//mnu.Append(ID_SEARCH_ALL, __I18N("windows:scripts:search_anywere") + __I18N("windows:menu_ext"));
	//mnu.AppendSeparator();
 	mnu.Append(ID_CUT, 	__I18N("windows:cut"));
	mnu.Append(ID_COPY, __I18N("windows:copy"));
	mnu.Append(ID_DELETE, __I18N("windows:delete"));

 	mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ScriptManagerDialog::OnPopupScriptClick ), NULL, this);
	this->PopupMenu(&mnu);
}

void
ScriptManagerDialog::OpenFolderMenu(wxTreeItemId item_id, wxString folder_name) {
 	wxMenu mnu;
	mnu.SetClientObject(new MenuScriptData(item_id, folder_name));
	mnu.Append(ID_NEW_SUB_FOLDER, 	__I18N("windows:scripts:new_folder") + __I18N("windows:menu_ext"));
	mnu.Append(ID_NEW_SCRIPT, 	__I18N("windows:scripts:new_script") + __I18N("windows:menu_ext"));
	mnu.AppendSeparator();
	//mnu.Append(ID_SEARCH, __I18N("windows:scripts:search_on_folder") + __I18N("windows:menu_ext"));
	//mnu.Append(ID_SEARCH_ALL, __I18N("windows:scripts:search_anywere") + __I18N("windows:menu_ext"));
	//mnu.AppendSeparator();
 	//mnu.Append(ID_CUT, 	__I18N("windows:cut"));
	//mnu.Append(ID_COPY, __I18N("windows:copy"));
	mnu.Append(ID_PASTE, __I18N("windows:paste"));
	mnu.Append(ID_DELETE, __I18N("windows:delete"));
	mnu.Enable(ID_PASTE, this->CanPaste(item_id));
 	mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ScriptManagerDialog::OnPopupFolderClick), NULL, this);
	this->PopupMenu(&mnu);
}

void
ScriptManagerDialog::OnPopupScriptClick(int id, wxClientData* data) {
	MenuScriptData* i = (MenuScriptData*)data;
	switch(id) {
	case ID_COPY:
		this->CopyScript(i->id, i->name);
		break;
	case ID_CUT:
		this->CutScript(i->id, i->name);
		break;
	case ID_DELETE:
		this->DeleteScript(i->id, i->name);
		break;
	case ID_SEARCH:
		this->script_list->SelectItem(i->id);
		this->OpenScript(i->name);
		OpenSearch(QGF_SCRIPT_SEARCH_MODE_SCRIPT);
		break;
	case ID_SEARCH_ALL:
		this->OpenSearch(QGF_SCRIPT_SEARCH_MODE_ALL);
		break;
	}
}

void
ScriptManagerDialog::OnPopupFolderClick(int id, wxClientData* data) {
	MenuScriptData* i = (MenuScriptData*)data;
	this->script_list->SelectItem(i->id);
	switch(id) {
	case ID_NEW_SUB_FOLDER: 
		CreateFolder(i->id, i->name);
		break;
	case ID_NEW_SCRIPT:		
		CreateScript(i->id, i->name);
		break;
	case ID_COPY:			
		CopyFolder(i->id, i->name);
		break;
	case ID_CUT:			
		CutFolder(i->id, i->name);
		break;
	case ID_PASTE:			
		Paste(i->id, i->name);
		break;
	case ID_DELETE:			
		DeleteFolder(i->id, i->name);
		break;
	case ID_SEARCH_ALL:		
		OpenSearch(QGF_SCRIPT_SEARCH_MODE_ALL);
		break;
	}
}

void 
ScriptManagerDialog::CreateScript(wxTreeItemId parent_id, wxString folder_name) {
	wxString path = ProjectManager::GetCurrent()->GetFolder();
	wxString ivc[] = QGF_INVALID_FOLDER_CHARS;
	wxArrayString invalid_chars = wxArrayString(QGF_INVALID_CHAR_SIZE, ivc);
	wxString dialog = WindowManager::InputTextDialog(__I18N("windows:scripts:add_script_caption"), invalid_chars, __I18N("errors:script_name_invalid"));
	if (dialog.IsEmpty()) return;
	wxString relative_path = folder_name + wxFILE_SEP_PATH + dialog + wxT(".rb");
	wxString new_file = path + relative_path;
	if (wxFileName::FileExists(new_file)) {
		WindowManager::ShowError(__I18N("errors:file_already_exists"));
		return;
	}
	wxTextFile      tfile;
	tfile.Create(new_file);
	tfile.AddLine(wxT("class ") + dialog.Capitalize());
	tfile.AddLine(wxT("\tdef method_sum(num1, num2)"));
	tfile.AddLine(wxT("\t\tnum1 + num2"));
	tfile.AddLine(wxT("\tend"));
	tfile.AddLine(wxT("end")  );
	tfile.Write();
	tfile.Close();
	ScriptTreeData* data = (ScriptTreeData*)this->script_list->GetItemData(parent_id);
	wxTreeItemId root = data->isFolder() ? parent_id : this->script_list->GetItemParent(parent_id);
	wxTreeItemId id = this->script_list->AppendItem(root, dialog + wxT(".rb"), 1, 1, new ScriptTreeData(folder_name, 1)); 
	this->script_list->SetItemImage(id, 4, wxTreeItemIcon_Expanded);
	this->script_list->SetItemImage(id, 4, wxTreeItemIcon_SelectedExpanded);
	this->script_list->SortChildren(root);
	this->OpenScript(relative_path);
	this->RealizeToolbar();
	
}

void 
ScriptManagerDialog::CreateFolder(wxTreeItemId parent_id, wxString folder_name) {
	wxString path = ProjectManager::GetCurrent()->GetFolder();
	wxString ivc[] = QGF_INVALID_FOLDER_CHARS;
	wxArrayString invalid_chars = wxArrayString(QGF_INVALID_CHAR_SIZE, ivc);
	wxString dialog = WindowManager::InputTextDialog(__I18N("windows:scripts:add_folder_caption"), invalid_chars,  __I18N("errors:folder_name_invalid"));
	if (dialog.IsEmpty()) return;
	wxString new_folder = folder_name + wxFILE_SEP_PATH + dialog;
	wxString full_path = path + new_folder;
	if (wxDir::Exists(full_path)) {
		WindowManager::ShowError(__I18N("errors:folder_already_exists"));
		return;
	}
	if (!wxMkdir(full_path)) {
		WindowManager::ShowError(__I18N("errors:folder_could_not_be_created"));
		return;
	}
	if (!ProjectManager::GetCurrent()->AddFolderByName(new_folder.AfterFirst(wxFILE_SEP_PATH), QGF_FOLDER_TYPE_SCRIPT)) {
		WindowManager::ShowError(__I18N("errors:folder_could_not_be_created"));
		return;
	}
	ProjectManager::Save();
	ScriptTreeData* data = (ScriptTreeData*)this->script_list->GetItemData(parent_id);
	wxTreeItemId root = this->script_list->GetRootItem();
	wxTreeItemId id = this->script_list->AppendItem(root, wxT(".")+ new_folder, 0, 0, new ScriptTreeData(new_folder, 0)); 
	this->script_list->SetItemImage(id, 4, wxTreeItemIcon_Expanded);
	this->script_list->SetItemImage(id, 4, wxTreeItemIcon_SelectedExpanded);
	this->script_list->SortChildren(root);
	this->RealizeToolbar();
}

void
ScriptManagerDialog::RefreshCopyData() {
	if (this->copy_data) {
		this->script_list->SetItemImage(copy_data->id, copy_data->folder ? 0 : 1, wxTreeItemIcon_Normal);
		this->script_list->SetItemImage(copy_data->id, copy_data->folder ? 0 : 1, wxTreeItemIcon_Selected);
		this->script_list->SetItemImage(copy_data->id, 4, wxTreeItemIcon_Expanded);
		this->script_list->SetItemImage(copy_data->id, 4, wxTreeItemIcon_SelectedExpanded);
		delete copy_data;
	}
}

void 
ScriptManagerDialog::CopyFolder(wxTreeItemId item_id, wxString folder_name)  {
	RefreshCopyData();
	this->copy_data = new ScriptManagerCopyData(folder_name, true, false, item_id);
	this->script_list->SetItemImage(item_id, 0, wxTreeItemIcon_Normal);
	this->script_list->SetItemImage(item_id, 0, wxTreeItemIcon_Selected);
	this->script_list->SetItemImage(item_id, 4, wxTreeItemIcon_Expanded);
	this->script_list->SetItemImage(item_id, 4, wxTreeItemIcon_SelectedExpanded);
}

void 
ScriptManagerDialog::CopyScript(wxTreeItemId item_id, wxString script_name)  {
	RefreshCopyData();
	this->copy_data = new ScriptManagerCopyData(script_name, false, false, item_id);
	this->script_list->SetItemImage(item_id, 1, wxTreeItemIcon_Normal);
	this->script_list->SetItemImage(item_id, 1, wxTreeItemIcon_Selected);
	this->script_list->SetItemImage(item_id, 4, wxTreeItemIcon_Expanded);
	this->script_list->SetItemImage(item_id, 4, wxTreeItemIcon_SelectedExpanded);
}

void 
ScriptManagerDialog::CutFolder(wxTreeItemId item_id, wxString folder_name)  {
	RefreshCopyData();
	this->copy_data = new ScriptManagerCopyData(folder_name, true, true, item_id);
	this->script_list->SetItemImage(item_id, 2, wxTreeItemIcon_Normal);
	this->script_list->SetItemImage(item_id, 2, wxTreeItemIcon_Selected);
	this->script_list->SetItemImage(item_id, 5, wxTreeItemIcon_Expanded);
	this->script_list->SetItemImage(item_id, 5, wxTreeItemIcon_SelectedExpanded);
}

void 
ScriptManagerDialog::CutScript(wxTreeItemId item_id, wxString script_name)  {
	RefreshCopyData();
	this->copy_data = new ScriptManagerCopyData(script_name, false, true, item_id);
	this->script_list->SetItemImage(item_id, 3, wxTreeItemIcon_Normal);
	this->script_list->SetItemImage(item_id, 3, wxTreeItemIcon_Selected);
	this->script_list->SetItemImage(item_id, 5, wxTreeItemIcon_Expanded);
	this->script_list->SetItemImage(item_id, 5, wxTreeItemIcon_SelectedExpanded);
}

void 
ScriptManagerDialog::DeleteFolder(wxTreeItemId id, wxString folder_name)  {
	Project* current_p = ProjectManager::GetCurrent();
	wxString path = current_p->GetFolder() + folder_name;
	wxFileName filename = wxFileName(path);
	if (filename.Exists()) {
		wxArrayString scripts = GetScriptsFrom(folder_name);
		for (wxArrayString::iterator i = scripts.begin(); i != scripts.end(); ++i) {
			size_t index = this->GetScriptIndex(folder_name + wxFILE_SEP_PATH + *i);
			if (index >= 0 )this->script_notebook->DeletePage(index);
		}
		if (!wxFileName::Rmdir(path, wxPATH_RMDIR_FULL|wxPATH_RMDIR_RECURSIVE)) {
			WindowManager::ShowError(__I18N("errors:cannot_delete_folder"));
			return;
		}
		if (current_p->RemoveFolderByName(folder_name.AfterFirst(wxFILE_SEP_PATH))) {
			ProjectManager::Save();
		}
		this->script_list->Delete(id);
		if (!copy_data) return;
		if (copy_data->name == folder_name) {
			delete copy_data;
			copy_data = NULL;
		}
	} else {
		WindowManager::ShowError(__I18N("errors:folder_dont_exists"));
	}
}

void 
ScriptManagerDialog::DeleteScript(wxTreeItemId id, wxString script_name)  {
	Project* current_p = ProjectManager::GetCurrent();
	wxString path = current_p->GetFolder() + script_name;
	wxFileName filename = wxFileName(path);
	if (!filename.Exists()) {
		WindowManager::ShowError(__I18N("errors:script_dont_exists"));
		return;
	}
	if (!wxRemoveFile(path)) {
		WindowManager::ShowError(__I18N("errors:cannot_delete_scripts"));
		return;
	}
	size_t index = this->GetScriptIndex(script_name);
	if (index >= 0 )this->script_notebook->DeletePage(index);
	this->RealizeToolbar();
	this->script_list->Delete(id);
	if (!copy_data) return;
	if (copy_data->name == script_name) {
		delete copy_data;
		copy_data = NULL;
	}
}

bool 
ScriptManagerDialog::CanPaste(wxTreeItemId item_id) {
	if (!copy_data) return false;
	if (copy_data->cutted) {
		if (item_id == copy_data->id) return false;
		ScriptTreeData* data = (ScriptTreeData*)this->script_list->GetItemData(item_id);
		if (!data->isFolder()) return false;
		if (data->folder.StartsWith(copy_data->name)) return false;
		if (copy_data->name.BeforeLast(wxFILE_SEP_PATH) == data->folder) return false;
	}
	return true;
}

void
ScriptManagerDialog::PasteFolder(wxString src_folder, wxString dst_folder) {
}

bool
ScriptManagerDialog::PasteFile(wxString src_folder, wxString dst_folder) {
		bool existed = false;
		if (!wxFileName::FileExists(src_folder)) {
			WindowManager::ShowError(__I18N("errors:script_src_dont_exists"));
			return existed;
		}
		if (wxFileName::FileExists(dst_folder)) {
			existed = true;
			int id = WindowManager::ShowConfirmationNoCancel(wxString::Format(__I18N("questions:override_file"), dst_folder));
			if (id != wxID_YES) return true;
		} 
		wxCopyFile(src_folder, dst_folder, true);
		if (copy_data->cutted) {
			if (!wxRemoveFile(src_folder)) {
				WindowManager::ShowError(__I18N("errors:cannot_delete_scripts"));
				return false;
			}
		}
		return existed;
}

void 
ScriptManagerDialog::Paste(wxTreeItemId id, wxString dst)  {
	Project* cp = ProjectManager::GetCurrent();
	wxString dst_script = dst + wxFILE_SEP_PATH + copy_data->name.AfterLast(wxFILE_SEP_PATH);
	wxString dst_folder = cp->GetFolder() + dst_script;
	wxString src_folder = cp->GetFolder() + copy_data->name;
	if (src_folder == dst_folder) {
		WindowManager::ShowError("errors:same_paste_src_and_dst");
		return;
	}
	if (copy_data->folder) {
		PasteFolder(src_folder, dst_folder);
	} else {
		bool already = PasteFile(src_folder, dst_folder);
		size_t org = this->GetScriptIndex(dst_script);
		if (org >= 0) this->script_notebook->DeletePage(org);
		this->OpenScript(dst_script);
		wxString name = copy_data->name.AfterLast(wxFILE_SEP_PATH);
		if (!already) {
			this->script_list->AppendItem(id, name, 1, 1, new ScriptTreeData(dst_script.BeforeLast(wxFILE_SEP_PATH), 1));
			this->script_list->SortChildren(id);
			size_t index = this->GetScriptIndex(name);
			if (index >= 0) this->script_notebook->DeletePage(index);
		}
	}
	if (copy_data->cutted) {
		if (!copy_data->folder) {
			size_t index = this->GetScriptIndex(copy_data->name);
			if (index >= 0) this->script_notebook->DeletePage(index);
		}
		this->script_list->Delete(copy_data->id);
		delete copy_data;
		copy_data = NULL;
	}
	this->RealizeToolbar();
	//wxCopyDir(src_folder, dst_folder);
}

		
void 
ScriptManagerDialog::OpenSearch(SearchMode mode)  {
	if (search_window) {
		if (search_window->IsVisible()) {
			search_window->SetSearchMode(mode);
			return;
		}
	}
	search_window = new ScriptSearchDialog(this);
	search_window->Show();
}

wxString 
ScriptManagerDialog::GetCurrentScriptName() {
	ScriptPage* page = (ScriptPage*)this->script_notebook->GetCurrentPage();
	if (!page) return wxT("");
	return page->GetScriptName();
}

wxArrayString 
ScriptManagerDialog::GetOpenedScriptNames() {
	wxArrayString result = wxArrayString();
	int count = this->script_notebook->GetPageCount();
	for (int i = 0; i < count; ++i) {
		ScriptPage* page = (ScriptPage*)this->script_notebook->GetPage(i);
		result.Add(page->GetScriptName());
	}
	return result;
}

void 
ScriptManagerDialog::AddScript(wxString dst) {

}