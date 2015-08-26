///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "new_folder_window.hpp"
#include "i18n.hpp"
#include "window_manager.hpp"
#include "qgf_info.hpp"

///////////////////////////////////////////////////////////////////////////

NewFolderDialog::NewFolderDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{

	this->folder_created = false;

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* item_sizer;
	item_sizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	item_sizer->AddGrowableCol( 1 );
	item_sizer->SetFlexibleDirection( wxBOTH );
	item_sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	label_name = new wxStaticText( this, wxID_ANY, __I18N("windows:new_folder:name"), wxDefaultPosition, wxDefaultSize, 0 );
	label_name->Wrap( -1 );
	item_sizer->Add( label_name, 0, wxALL, 5 );
	
	text_name = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	item_sizer->Add( text_name, 1, wxALL|wxEXPAND, 5 );
	
	label_type = new wxStaticText( this, wxID_ANY, __I18N("windows:new_folder:type"), wxDefaultPosition, wxDefaultSize, 0 );
	label_type->Wrap( -1 );
	item_sizer->Add( label_type, 0, wxALL, 5 );
	
	wxString choice_typeChoices[] = { __I18N("data:folders:audio"), __I18N("data:folders:image"), __I18N("data:folders:font"), __I18N("data:folders:music"), __I18N("data:folders:sfx"), __I18N("data:folders:script"), __I18N("data:folders:video"), __I18N("data:folders:other") };
	int choice_typeNChoices = sizeof( choice_typeChoices ) / sizeof( wxString );
	choice_type = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choice_typeNChoices, choice_typeChoices, 0 );
	choice_type->SetSelection( 0 );
	item_sizer->Add( choice_type, 1, wxALL|wxEXPAND, 5 );
	
	
	main_sizer->Add( item_sizer, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* button_sizer;
	button_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	button_ok = new wxButton( this, wxID_ANY, __I18N("windows:ok"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_ok, 0, wxALL, 5 );
	
	button_cancel = new wxButton( this, wxID_ANY, __I18N("windows:cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	button_sizer->Add( button_cancel, 0, wxALL, 5 );
	
	
	main_sizer->Add( button_sizer, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( NewFolderDialog::OnClose ) );
	button_ok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewFolderDialog::OnOk ), NULL, this );
	button_cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewFolderDialog::OnCancel ), NULL, this );
}

NewFolderDialog::~NewFolderDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( NewFolderDialog::OnClose ) );
	button_ok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewFolderDialog::OnOk ), NULL, this );
	button_cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewFolderDialog::OnCancel ), NULL, this );
	
}

void 
NewFolderDialog::OnOk() {
	wxString folder_name = this->text_name->GetLineText(0);
	if (folder_name.IsEmpty()) { 
		WindowManager::ShowError(__I18N("errors:folder_name_empty"));
		return;
	}
	wxString ivc[] = QGF_INVALID_FOLDER_CHARS;
	wxArrayString invalid_chars = wxArrayString(QGF_INVALID_CHAR_SIZE, ivc);
	for (wxArrayString::iterator it = invalid_chars.begin(); it != invalid_chars.end(); ++it) {
		if (folder_name.Contains(*it)) {
			WindowManager::ShowError(__I18N("errors:folder_name_invalid"));
			return;
		}
	}
	this->folder_created = true;
	this->Hide();
}

void 
NewFolderDialog::OnCancel() {
	this->folder_created = false;
	this->Hide();
}

bool 
NewFolderDialog::FolderCreated() {
	return this->folder_created;
}

wxString 
NewFolderDialog::GetFolderName() {
	return this->text_name->GetLineText(0);
}

FolderType 
NewFolderDialog::GetFolderType() {
	return (FolderType)this->choice_type->GetSelection();
}