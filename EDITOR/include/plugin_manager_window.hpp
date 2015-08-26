#ifndef __PLUGIN_MANAGER_WINDOW_H__
#define __PLUGIN_MANAGER_WINDOW_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PluginManagerDialog
///////////////////////////////////////////////////////////////////////////////
class PluginManagerDialog : public wxDialog 
{
	private:
		bool plugins_changed;
		void OnDetectPlugins();
		void OnAddPlugin();
		void OnRemovePlugin();
		void OnConfigurePlugins();
		void OnClose();
	protected:
		wxListBox* list_detected;
		wxButton* button_detect;
		wxButton* button_add;
		wxButton* button_remove;
		wxListBox* list_selected;
		wxButton* button_configure;
		wxButton* button_close;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnDetectPlugins( wxCommandEvent& event ) { OnDetectPlugins(); event.Skip(); }
		virtual void OnAddPlugin( wxCommandEvent& event ) { OnAddPlugin(); event.Skip(); }
		virtual void OnRemovePlugin( wxCommandEvent& event ) { OnRemovePlugin(); event.Skip(); }
		virtual void OnConfigurePlugins( wxCommandEvent& event ) { OnConfigurePlugins(); event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { OnClose(); event.Skip(); }
		virtual void OnClose( wxCloseEvent& event ) { OnClose(); event.Skip(); }
	
	public:
		
		PluginManagerDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Plugin Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 557,353 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~PluginManagerDialog();
	
};

#endif //__PLUGIN_MANAGER_WINDOW_H__