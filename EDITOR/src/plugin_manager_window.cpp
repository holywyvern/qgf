#include "plugin_manager_window.hpp"
#include "plugin_manager.hpp"
#include "project_manager.hpp"
#include "i18n.hpp"
#include "window_manager.hpp"

///////////////////////////////////////////////////////////////////////////

PluginManagerDialog::PluginManagerDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{

	plugins_changed = false;
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* plugin_sizer;
	plugin_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* detected_box;
	detected_box = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, __I18N("windows:plugin:detected") ), wxVERTICAL );
	
	list_detected = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB ); 
	detected_box->Add( list_detected, 1, wxALL|wxEXPAND, 5 );
	
	button_detect = new wxButton( this, wxID_ANY, __I18N("windows:plugin:detect"), wxDefaultPosition, wxDefaultSize, 0 );
	detected_box->Add( button_detect, 0, wxALL, 5 );
	
	
	plugin_sizer->Add( detected_box, 1, wxEXPAND|wxALL, 5 );
	
	wxBoxSizer* management_box;
	management_box = new wxBoxSizer( wxVERTICAL );
	
	button_add = new wxButton( this, wxID_ANY, __I18N("windows:plugin:add"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	management_box->Add( button_add, 0, wxALIGN_CENTER|wxALL, 5 );
	
	button_remove = new wxButton( this, wxID_ANY, __I18N("windows:plugin:remove"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	management_box->Add( button_remove, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	plugin_sizer->Add( management_box, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxStaticBoxSizer* selected_box;
	selected_box = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, __I18N("windows:plugin:selected") ), wxVERTICAL );
	
	list_selected = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB ); 
	selected_box->Add( list_selected, 1, wxALL|wxEXPAND, 5 );
	
	button_configure = new wxButton( this, wxID_ANY, __I18N("windows:plugin:configure_plugin"), wxDefaultPosition, wxDefaultSize, 0 );
	selected_box->Add( button_configure, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	plugin_sizer->Add( selected_box, 1, wxEXPAND|wxALL, 5 );
	
	
	main_sizer->Add( plugin_sizer, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	button_close = new wxButton( this, wxID_ANY, __I18N("windows:close"), wxDefaultPosition, wxDefaultSize, 0 );
	main_sizer->Add( button_close, 0, wxALIGN_RIGHT|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );

	this->SetTitle(__I18N("windows:main:plugin_manager"));
	this->OnDetectPlugins();

	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PluginManagerDialog::OnClose ) );

	button_detect->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginManagerDialog::OnDetectPlugins ), NULL, this );
	button_add->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginManagerDialog::OnAddPlugin ), NULL, this );
	button_remove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginManagerDialog::OnRemovePlugin ), NULL, this );
	button_configure->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginManagerDialog::OnConfigurePlugins ), NULL, this );
	button_close->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginManagerDialog::OnClose ), NULL, this );

}

PluginManagerDialog::~PluginManagerDialog()
{
	button_detect->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginManagerDialog::OnDetectPlugins ), NULL, this );
	button_add->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginManagerDialog::OnAddPlugin ), NULL, this );
	button_remove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginManagerDialog::OnRemovePlugin ), NULL, this );
	button_configure->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginManagerDialog::OnConfigurePlugins ), NULL, this );
	button_close->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginManagerDialog::OnClose ), NULL, this );
}

void
PluginManagerDialog::OnDetectPlugins() {
	wxArrayString plugins = qgf::PluginManager::DetectPlugins();
	this->list_detected->Clear();
	if (!plugins.IsEmpty()) this->list_detected->InsertItems(plugins, 0);
	PluginSet* list = ProjectManager::GetCurrent()->GetPluginList();
	this->list_selected->Clear();
	wxArrayString name_list = wxArrayString();
	for (PluginSet::iterator it = list->begin(); it != list->end(); ++it) {
		name_list.Add(it->first);
	}
	if (!name_list.IsEmpty()) this->list_selected->InsertItems(name_list, 0);
}

void 
PluginManagerDialog::OnAddPlugin() {
	int selected = this->list_detected->GetSelection();
	if (selected < 0) return;
	wxString item = this->list_detected->GetString(selected);
	qgf::Plugin* plugin = qgf::PluginManager::LoadPlugin(item);
	if (plugin == NULL) return;
	Project* project = ProjectManager::GetCurrent();
	project->AddPlugin(item, plugin);
	qgf::PluginManager::ConfigurePlugin(this, project->GetFolder(), item);
	plugins_changed = true;
	this->list_selected->InsertItems(1, &item, 0);
	this->list_detected->Delete(selected);
}

void
PluginManagerDialog::OnRemovePlugin() {
	int selected = this->list_selected->GetSelection();
	if (selected < 0) return;
	wxString item = this->list_selected->GetString(selected);
	this->list_detected->InsertItems(1, &item, 0);
	this->list_selected->Delete(selected);
	Project* project = ProjectManager::GetCurrent();
	project->RemovePlugin(item);
	plugins_changed = true;
}

void 
PluginManagerDialog::OnConfigurePlugins() {
	int selected = this->list_selected->GetSelection();
	if (selected < 0) return;
	wxString item = wxString::FromUTF8(this->list_selected->GetString(selected));
	qgf::PluginManager::ConfigurePlugin(this, ProjectManager::GetCurrent()->GetFolder(), item);
	plugins_changed = true;
}

void 
PluginManagerDialog::OnClose() {
	this->Hide();
	if (plugins_changed) {
		ProjectManager::Save();
		WindowManager::RecreateMainWindow();
	}
}
