///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __QGF_EXPORT_WINDOW_H
#define __QGF_EXPORT_WINDOW_H 1

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/filepicker.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ExportDialog
///////////////////////////////////////////////////////////////////////////////
class ExportDialog : public wxDialog 
{
	private:
		void OnUpdateDir();
		void OnExport();
		void OnCancel();
		void LoadImporters();
	protected:
		wxDirPickerCtrl* folder_picker;
		wxChoice* platform_choice;
		wxButton* button_export;
		wxButton* button_cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void UpdateProjectDir( wxFileDirPickerEvent& event ) { OnUpdateDir(); event.Skip(); }
		virtual void OnExport( wxCommandEvent& event ) { OnExport(); event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { OnCancel(); event.Skip(); }
		
	
	public:
		
		ExportDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Exportar Proyecto"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 444,199 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ExportDialog();
	
};

#endif