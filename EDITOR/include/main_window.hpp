

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:

		bool OnMainWindowClose();
		void OnNewProject();
		void OnOpenProject();
		void OnSaveProject();
		void OnCloseProject();
		void OnExportProject();
		void OnExitApplication();
		void OnPluginManager();
		void OnResourceEdit();
		void OnScriptEdit();
		void OnMenuContents();
		void OnMenuAbout();
		void OnPlayTest();
	protected:
		wxMenuBar* main_menu;
		wxMenu* menu_file;
		wxMenu* menu_edit;
		wxMenu* menu_tools;
		wxMenu* menu_help;
		wxToolBar* tool_main;
		wxPanel* main_panel;
		

		wxMenuItem* menu_new_project;
		wxMenuItem* menu_open_project;
		wxMenuItem* menu_save_project;
		wxMenuItem* menu_close_project;
		wxMenuItem* menu_export;
		wxMenuItem* menu_exit;
		wxMenuItem* menu_plugin_manager;
		wxMenuItem* menu_resource_manager;
		wxMenuItem* menu_script_manager;
		wxMenuItem* menu_playtest;
		wxMenuItem* menu_contents;
		wxMenuItem* menu_about;

		wxToolBarToolBase* tool_new;
		wxToolBarToolBase* tool_open;
		wxToolBarToolBase* tool_save;
		wxToolBarToolBase* tool_plugin;
		wxToolBarToolBase* tool_resource;
		wxToolBarToolBase* tool_scripts;
		wxToolBarToolBase* tool_playtest;

		// Virtual event handlers, overide them in your derived class
		virtual void OnMainWindowClose( wxCloseEvent& event ) { bool veto = OnMainWindowClose(); if(veto) event.Veto(veto); else event.Skip(); }
		virtual void OnNewProject( wxCommandEvent& event ) { OnNewProject(); event.Skip(); }
		virtual void OnOpenProject( wxCommandEvent& event ) { OnOpenProject(); event.Skip(); }
		virtual void OnSaveProject( wxCommandEvent& event ) { OnSaveProject(); event.Skip(); }
		virtual void OnCloseProject( wxCommandEvent& event ) { OnCloseProject(); event.Skip(); }
		virtual void OnExportProject( wxCommandEvent& event ) { OnExportProject(); event.Skip(); }
		virtual void OnExitApplication( wxCommandEvent& event ) { OnExitApplication(); event.Skip(); }
		virtual void OnPluginManager( wxCommandEvent& event ) { OnPluginManager(); event.Skip(); }
		virtual void OnResourceEdit( wxCommandEvent& event ) { OnResourceEdit(); event.Skip(); }
		virtual void OnScriptEdit( wxCommandEvent& event ) { OnScriptEdit(); event.Skip(); }
		virtual void OnMenuContents( wxCommandEvent& event ) { OnMenuContents(); event.Skip(); }
		virtual void OnMenuAbout( wxCommandEvent& event ) { OnMenuAbout(); event.Skip(); }
		virtual void OnPlayTest( wxCommandEvent& event ) { OnPlayTest(); event.Skip(); }
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("QGF - Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 669,395 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrame();
	
		void ActivateProjectFunctions();
		void DeactivateProjectFunctions();

		wxMenu* MenuEdit()   { return this->menu_edit;  };
		wxMenu* MenuTools()  { return this->menu_tools; };
		wxPanel* MainPanel() { return this->main_panel; };
		wxToolBar* Toolbar() { return this->tool_main;  };

};

#endif //__MAIN_WINDOW_H__