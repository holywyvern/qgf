//////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __QGF_WINDOWS_IMAGE_PREVIEW_H
#define __QGF_WINDOWS_IMAGE_PREVIEW_H 1

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ImagePreviewDialog
///////////////////////////////////////////////////////////////////////////////
class ImagePreviewDialog : public wxDialog 
{
	private:
		void OnClose();
	protected:
		wxScrolledWindow* scroll_panel;
		wxStaticBitmap* preview_bitmap;
		wxButton* close_button;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCommandEvent& event ) { OnClose(); event.Skip(); }
		
	
	public:
		
		ImagePreviewDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ImagePreviewDialog();
	
		void SetPreview(wxString file);
};

#endif 
