#ifndef __SCRIPT_MANAGER_WINDOW_H__
#define __SCRIPT_MANAGER_WINDOW_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/aui/auibook.h>
#include <wx/menu.h>

#include "script_search_window.hpp"

//#include "script.hpp"

///////////////////////////////////////////////////////////////////////////

enum SearchMode {
	QGF_SCRIPT_SEARCH_MODE_SCRIPT = 0,
	QGF_SCRIPT_SEARCH_MODE_ALL    = 1
};

class ScriptManagerCopyData {
		public:
		wxString name;
		bool folder;
		bool cutted;
		wxTreeItemId id;
		ScriptManagerCopyData(wxString name, bool folder, bool cutted, wxTreeItemId id) {this->name = name; this->folder = folder; this->cutted = cutted; this->id = id; }
};

///////////////////////////////////////////////////////////////////////////////
/// Class ScriptManagerDialog
///////////////////////////////////////////////////////////////////////////////
class ScriptManagerDialog : public wxDialog
{
	private:
		ScriptSearchDialog* search_window;
		ScriptManagerCopyData* copy_data;
		wxBitmap script_normal_bitmap;
		wxBitmap script_modified_bitmap;
		wxToolBarToolBase* tool_cut;
		wxToolBarToolBase* tool_copy;
		wxToolBarToolBase* tool_paste;
		wxToolBarToolBase* tool_undo;
		wxToolBarToolBase* tool_redo;
		wxToolBarToolBase* tool_find;
		wxToolBarToolBase* tool_save;
		wxToolBarToolBase* tool_save_all;
		wxToolBarToolBase* tool_close;
		wxToolBarToolBase* tool_close_all;
		//Script* script;
		void OnScriptClicked();
		void OnOk();
		void OnCancel();
		void OnApply();
		void OnHelp();
		void OnCut();
		void OnCopy();
		void OnPaste();
		void OnUndo();
		void OnRedo();
		void OnSearch();
		void OnScriptMenu(wxTreeItemId item_id);
		void OnDelete();
		void OpenScript(wxString script_name);
		void OnChangePage();
		void OnClosePage();
		//void ShowScript(Script* script);
		void OnSave();
		void OnClose();
		void OnSaveAll();
		void OnCloseAll();

		void CreateImageList();
		void RealizeToolbar();
		void  OnWindowClose();
		void OpenScriptMenu(wxTreeItemId item_id, wxString script_name);
		void OpenFolderMenu(wxTreeItemId item_id, wxString folder_name);
		size_t GetScriptIndex(wxString script_name);
		void OnPopupFolderClick(int id, wxClientData* data);
		void OnPopupScriptClick(int id, wxClientData* data);

		void CreateScript(wxTreeItemId parent_id, wxString folder_name);
		void CreateFolder(wxTreeItemId parent_id, wxString folder_name);
		void RefreshCopyData();
		void CopyFolder(wxTreeItemId item_id, wxString folder_name);
		void CopyScript(wxTreeItemId item_id, wxString script_name);
		void CutFolder(wxTreeItemId item_id, wxString folder_name);
		void CutScript(wxTreeItemId item_id, wxString script_name);
		void DeleteFolder(wxTreeItemId id, wxString folder_name);
		void DeleteScript(wxTreeItemId id, wxString script_name);
		bool CanPaste(wxTreeItemId item_id);
		void Paste(wxTreeItemId id, wxString dst);
		void OpenSearch(SearchMode mode);
		void AddScript(wxString dst);
		void OnAddScriptFromButton();
		void OnAddFolderFromButton();

		void PasteFolder(wxString src_folder, wxString dst_folder);
		bool PasteFile(wxString src_folder, wxString dst_folder);

	protected:
		wxPanel* panel_text_control;
		wxTreeCtrl* script_list;
		wxButton* script_add_button;
		wxButton* folder_add_button;
		wxPanel* panel_script;
		wxToolBar* script_tool;
		wxAuiNotebook* script_notebook;
		wxButton* button_ok;
		wxButton* button_cancel;
		wxButton* button_apply;
		wxButton* button_help;

		// Virtual event handlers, overide them in your derived class
		virtual void OnWindowClose( wxCloseEvent& event ) { OnWindowClose(); event.Skip(); }
		virtual void OnScriptClicked( wxTreeEvent& event ) { OnScriptClicked(); event.Skip(); }
		virtual void OnScriptMenu( wxTreeEvent& event ) { OnScriptMenu(event.GetItem()); event.Skip(); }
		virtual void OnScriptKeydown( wxTreeEvent& event ) { if (event.GetKeyCode() == WXK_DELETE || event.GetKeyCode() == WXK_BACK) OnDelete(); event.Skip(); }
		virtual void OnAddFolder( wxCommandEvent& event ) { OnAddFolderFromButton(); event.Skip(); }
		virtual void OnAddScript( wxCommandEvent& event ) { OnAddScriptFromButton(); event.Skip(); }
		virtual void OnCut( wxCommandEvent& event ) { OnCut(); event.Skip(); }
		virtual void OnCopy( wxCommandEvent& event ) { OnCopy(); event.Skip(); }
		virtual void OnPaste( wxCommandEvent& event ) { OnPaste(); event.Skip(); }
		virtual void OnUndo( wxCommandEvent& event ) { OnUndo(); event.Skip(); }
		virtual void OnRedo( wxCommandEvent& event ) { OnRedo(); event.Skip(); }
		virtual void OnSearch( wxCommandEvent& event ) { OnSearch(); event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { OnSave(); event.Skip(); }
		virtual void OnSaveAll( wxCommandEvent& event ) { OnSaveAll(); event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { OnClose(); event.Skip(); }
		virtual void OnCloseAll( wxCommandEvent& event ) { OnCloseAll(); event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { OnOk(); event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { OnCancel(); event.Skip(); }
		virtual void OnApply( wxCommandEvent& event ) { OnApply(); event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { OnHelp(); event.Skip(); }

		virtual void OnChangePage( wxAuiNotebookEvent& event ) { OnChangePage(); event.Skip(); }
		virtual void OnClosePage( wxAuiNotebookEvent& event ) { OnClosePage(); event.Skip(); }

		virtual void OnPopupFolderClick( wxCommandEvent& event ) { OnPopupFolderClick(event.GetId(), ((wxMenu*)event.GetEventObject())->GetClientObject()); event.Skip(); }
		virtual void OnPopupScriptClick( wxCommandEvent& event ) { OnPopupScriptClick(event.GetId(), ((wxMenu*)event.GetEventObject())->GetClientObject()); event.Skip(); }
	public:

		ScriptManagerDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Script Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1024, 800 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~ScriptManagerDialog();
		virtual void ReloadScripts();
		void OnTextChange();
		wxString GetCurrentScriptName();
		wxArrayString GetOpenedScriptNames();
		void OpenScript(wxString script_name, int line);
};

#endif //__SCRIPT_MANAGER_WINDOW_H__