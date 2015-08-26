///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "about_window.hpp"
#include "i18n.hpp"
#include "qgf_info.hpp"

///////////////////////////////////////////////////////////////////////////

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	logo_bitmap = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("icons/logo.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER|wxFULL_REPAINT_ON_RESIZE );
	main_sizer->Add( logo_bitmap, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* label_sizer;
	label_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	left_scroll = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxSIMPLE_BORDER|wxVSCROLL );
	left_scroll->SetScrollRate( 5, 5 );
	wxBoxSizer* left_label_sizer;
	left_label_sizer = new wxBoxSizer( wxVERTICAL );
	
	version_label = new wxStaticText( left_scroll, wxID_ANY, __I18N("windows:about:version"), wxDefaultPosition, wxDefaultSize, 0 );
	version_label->Wrap( -1 );
	left_label_sizer->Add( version_label, 0, wxALL, 5 );
	
	autor_label = new wxStaticText( left_scroll, wxID_ANY, __I18N("windows:about:author") + wxT(" Ramiro Rojo"), wxDefaultPosition, wxDefaultSize, 0 );
	autor_label->Wrap( -1 );
	left_label_sizer->Add( autor_label, 0, wxALL, 5 );
	
	
	left_label_sizer->Add( 0, 16, 0, wxEXPAND, 5 );
	
	wxString lang = __I18N("windows:about:lang") + wxT(" ") + __I18N("windows:about:lang_name");

	lang_label = new wxStaticText( left_scroll, wxID_ANY, lang, wxDefaultPosition, wxDefaultSize, 0 );
	lang_label->Wrap( -1 );
	left_label_sizer->Add( lang_label, 0, wxALL, 5 );
	
	lang_ver_label = new wxStaticText( left_scroll, wxID_ANY, (__I18N("windows:about:version") + wxT(" ") + wxT(QGF_VERSION_STRING)), wxDefaultPosition, wxDefaultSize, 0 );
	lang_ver_label->Wrap( -1 );
	left_label_sizer->Add( lang_ver_label, 0, wxALL, 5 );
	
	lang_author_label = new wxStaticText( left_scroll, wxID_ANY, __I18N("windows:about:author") + wxT(" ") + __I18N("info:author"), wxDefaultPosition, wxDefaultSize, 0 );
	lang_author_label->Wrap( -1 );
	left_label_sizer->Add( lang_author_label, 0, wxALL, 5 );	

	left_scroll->SetSizer( left_label_sizer );
	left_scroll->Layout();
	left_label_sizer->Fit( left_scroll );
	label_sizer->Add( left_scroll, 1, wxEXPAND | wxALL, 5 );
	
	right_scroll = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxSIMPLE_BORDER|wxVSCROLL );
	right_scroll->SetScrollRate( 5, 5 );
	wxBoxSizer* right_label_sizer;
	right_label_sizer = new wxBoxSizer( wxVERTICAL );
	
	ruby_version_label = new wxStaticText( right_scroll, wxID_ANY, wxT("Ruby 2.1.0"), wxDefaultPosition, wxDefaultSize, 0 );
	ruby_version_label->Wrap( -1 );
	right_label_sizer->Add( ruby_version_label, 0, wxALL, 5 );
	
	ruby_copyright_label = new wxStaticText( right_scroll, wxID_ANY, wxT("(C) 1993-2014 Yukihiro Matsumoto"), wxDefaultPosition, wxDefaultSize, 0 );
	ruby_copyright_label->Wrap( -1 );
	right_label_sizer->Add( ruby_copyright_label, 0, wxALL, 5 );
	
	ruby_hyperlink = new wxHyperlinkCtrl( right_scroll, wxID_ANY,__I18N("windows:about:website"), wxT("https://www.ruby-lang.org/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	right_label_sizer->Add( ruby_hyperlink, 0, wxALL, 5 );
	
	
	right_label_sizer->Add( 0, 16, 1, wxEXPAND, 5 );
	
	wxwidgets_label = new wxStaticText( right_scroll, wxID_ANY, wxT("wxWidgets 3.0.0"), wxDefaultPosition, wxDefaultSize, 0 );
	wxwidgets_label->Wrap( -1 );
	right_label_sizer->Add( wxwidgets_label, 0, wxALL, 5 );
	
	wx_widgets_link = new wxHyperlinkCtrl( right_scroll, wxID_ANY, __I18N("windows:about:website"), wxT("https://www.wxwidgets.org"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	right_label_sizer->Add( wx_widgets_link, 0, wxALL, 5 );
	
	
	right_label_sizer->Add( 0, 16, 1, wxEXPAND, 5 );
	
	audiere_label = new wxStaticText( right_scroll, wxID_ANY, wxT("Audiere 1.9.4"), wxDefaultPosition, wxDefaultSize, 0 );
	audiere_label->Wrap( -1 );
	right_label_sizer->Add( audiere_label, 0, wxALL, 5 );
	
	audiere_link = new wxHyperlinkCtrl( right_scroll, wxID_ANY, __I18N("windows:about:website"), wxT("http://audiere.sourceforge.net/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	right_label_sizer->Add( audiere_link, 0, wxALL, 5 );
	
	
	right_label_sizer->Add( 0, 16, 1, wxEXPAND, 5 );
	
	freetype_label = new wxStaticText( right_scroll, wxID_ANY, wxT("Freetype 2.5.3"), wxDefaultPosition, wxDefaultSize, 0 );
	freetype_label->Wrap( -1 );
	right_label_sizer->Add( freetype_label, 0, wxALL, 5 );
	
	freetype_link = new wxHyperlinkCtrl( right_scroll, wxID_ANY, __I18N("windows:about:website"), wxT("http://www.freetype.org/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	right_label_sizer->Add( freetype_link, 0, wxALL, 5 );
	
	right_label_sizer->Add( 0, 16, 1, wxEXPAND, 5 );
	
	pugg_label = new wxStaticText( right_scroll, wxID_ANY, wxT("Pugg 0.60"), wxDefaultPosition, wxDefaultSize, 0 );
	pugg_label->Wrap( -1 );
	right_label_sizer->Add( pugg_label, 0, wxALL, 5 );
	
	pugg_link = new wxHyperlinkCtrl( right_scroll, wxID_ANY, __I18N("windows:about:website"), wxT("http://pugg.sourceforge.net/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	right_label_sizer->Add( pugg_link, 0, wxALL, 5 );	

	right_scroll->SetSizer( right_label_sizer );
	right_scroll->Layout();
	right_label_sizer->Fit( right_scroll );
	label_sizer->Add( right_scroll, 1, wxEXPAND | wxALL, 5 );
	
	
	main_sizer->Add( label_sizer, 1, wxEXPAND, 5 );
	
	button_close = new wxButton( this, wxID_ANY, __I18N("windows:close"), wxDefaultPosition, wxDefaultSize, 0 );
	main_sizer->Add( button_close, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	this->SetTitle(__I18N("windows:main:about"));

	// Connect Events
	button_close->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialog::OnClose ), NULL, this );
}

AboutDialog::~AboutDialog()
{
	// Disconnect Events
	button_close->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialog::OnClose ), NULL, this );
	
}

void 
AboutDialog::OnClose() {
	this->Hide();
}