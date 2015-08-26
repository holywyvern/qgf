#ifndef __RESOURCE_MANAGER_WINDOW_H__
#define __RESOURCE_MANAGER_WINDOW_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <vector>
#include "folder.hpp"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ResourceManagerDialog
///////////////////////////////////////////////////////////////////////////////
class ResourceManagerDialog : public wxDialog 
{
	private:
		void OnFolderSelection();
		void OnNewFolder();
		void OnNewSubFolder();
		void OnResourceSelected(); 
		void OnImportResource();
		void OnExportResource();
		void OnPreviewResource();
		void OnDeleteResource();
		void OnClose();
		void OnDeleteFolder();
		void AppendTreeNodes(wxString old_path, wxTreeItemId node, std::vector<Folder>* folders, Folder* parent);
		void MakeImageList();
	protected:
		wxTreeCtrl* tree_folders;
		wxButton* button_new_folder;
		wxButton* button_delete_folder;
		wxButton* button_new_subfolder;
		wxTreeCtrl* tree_resources;
		wxButton* button_add;
		wxButton* button_remove;
		wxButton* button_preview;
		wxButton* button_delete;
		wxButton* button_close;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFolderSelection( wxTreeEvent& event ) { OnFolderSelection(); event.Skip(); }
		virtual void OnNewFolder( wxCommandEvent& event ) { OnNewFolder(); event.Skip(); }
		virtual void OnNewSubFolder( wxCommandEvent& event ) { OnNewSubFolder(); event.Skip(); }
		virtual void OnDeleteFolder( wxCommandEvent& event ) { OnDeleteFolder(); event.Skip(); }
		virtual void OnResourceSelected( wxTreeEvent& event ) { OnResourceSelected(); event.Skip(); }
		virtual void OnImportResource( wxCommandEvent& event ) { OnImportResource(); event.Skip(); }
		virtual void OnExportResource( wxCommandEvent& event ) { OnExportResource(); event.Skip(); }
		virtual void OnPreviewResource( wxCommandEvent& event ) { OnPreviewResource(); event.Skip(); }
		virtual void OnDeleteResource( wxCommandEvent& event ) { OnDeleteResource(); event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { OnClose(); event.Skip(); }
	
	public:
		
		ResourceManagerDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Resource Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,480 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~ResourceManagerDialog();
	
		void ReadFoldersAndFiles();

};

#endif //__RESOURCE_MANAGER_WINDOW_H__