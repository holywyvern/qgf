///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "text_input_window.hpp"
#include "i18n.hpp"
#include "qgf_info.hpp"
#include "window_manager.hpp"

///////////////////////////////////////////////////////////////////////////

TextInputDialog::TextInputDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->invalid_chars = wxArrayString();
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	text_value = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	main_sizer->Add( text_value, 0, wxALL|wxEXPAND, 5 );
	
	button_ok = new wxButton( this, wxID_ANY, wxT("Aceptar"), wxDefaultPosition, wxDefaultSize, 0 );
	main_sizer->Add( button_ok, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TextInputDialog::OnCancel ) );
	button_ok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextInputDialog::OnAccept ), NULL, this );
}

TextInputDialog::~TextInputDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TextInputDialog::OnCancel ) );
	button_ok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextInputDialog::OnAccept ), NULL, this );
	
}

void 
TextInputDialog::OnCancel() {
	this->acepted = false;
}

void 
TextInputDialog::OnAccept() {
	this->acepted = true;
	if (!IsValidText()) {
		WindowManager::ShowError(this->failure_caption);
		return;
	}
	this->Hide();
}

void
TextInputDialog::SetInvalidCharacters(wxArrayString invalid_chars, wxString failure_caption) {
	this->failure_caption = failure_caption;
	this->invalid_chars = invalid_chars;
}

bool
TextInputDialog::IsValidText() {
	wxString text = this->text_value->GetLineText(0);
	if (text.IsEmpty()) return false;
	wxString ivc[] = QGF_INVALID_FOLDER_CHARS;
	wxArrayString invalid_chars = wxArrayString(QGF_INVALID_CHAR_SIZE, ivc);
	for (wxArrayString::iterator it = invalid_chars.begin(); it != invalid_chars.end(); ++it) {
		if (text.Contains(*it)) {
			return false;
		}
	}
	return true;
}

wxString 
TextInputDialog::GetInputText() {
	if (!acepted) return wxT("");
	return this->text_value->GetLineText(0);
}