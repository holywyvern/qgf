///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "image_preview_window.hpp"
#include "i18n.hpp"

///////////////////////////////////////////////////////////////////////////

ImagePreviewDialog::ImagePreviewDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	scroll_panel = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxALWAYS_SHOW_SB|wxDOUBLE_BORDER|wxHSCROLL|wxVSCROLL );
	scroll_panel->SetScrollRate( 5, 5 );
	scroll_panel->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	
	wxBoxSizer* scroll_sizer;
	scroll_sizer = new wxBoxSizer( wxVERTICAL );
	
	preview_bitmap = new wxStaticBitmap( scroll_panel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	scroll_sizer->Add( preview_bitmap, 0, wxALL, 5 );
	
	
	scroll_panel->SetSizer( scroll_sizer );
	scroll_panel->Layout();
	scroll_sizer->Fit( scroll_panel );
	main_sizer->Add( scroll_panel, 1, wxEXPAND | wxALL, 5 );
	
	close_button = new wxButton( this, wxID_ANY, wxT("Cerrar"), wxDefaultPosition, wxDefaultSize, 0 );
	main_sizer->Add( close_button, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	this->SetTitle(__I18N("windows:resource:preview_title"));

	// Connect Events
	close_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImagePreviewDialog::OnClose ), NULL, this );
}

ImagePreviewDialog::~ImagePreviewDialog()
{
	// Disconnect Events
	close_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImagePreviewDialog::OnClose ), NULL, this );
	
}

void 
ImagePreviewDialog::OnClose() {
	this->Hide();
}

void 
ImagePreviewDialog::SetPreview(wxString file) {
	this->preview_bitmap->SetBitmap(wxBitmap(file, wxBITMAP_TYPE_ANY));
	this->SetTitle(__I18N("windows:resource:preview_title") + file);
}