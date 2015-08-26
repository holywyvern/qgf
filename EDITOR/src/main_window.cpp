#include "main_window.hpp"

#include "project_manager.hpp"
#include "window_manager.hpp"
#include "i18n.hpp"
#include <wx/stdpaths.h>

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	main_menu = new wxMenuBar( 0 );
	menu_file = new wxMenu();
	
	menu_new_project = new wxMenuItem( menu_file, 1101, wxString(__I18N("windows:main:new_project")) + __I18N("windows:menu_ext") + wxT('\t') + wxT("CTRL+N"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	menu_new_project->SetBitmaps( wxBitmap( wxT("icons/new.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	menu_new_project->SetBitmap( wxBitmap( wxT("icons/new.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	menu_file->Append( menu_new_project );
	
	
	menu_open_project = new wxMenuItem( menu_file, 1102, wxString(__I18N("windows:main:open")) + __I18N("windows:menu_ext") + wxT('\t') + wxT("CTRL+O"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	menu_open_project->SetBitmaps( wxBitmap( wxT("icons/open.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	menu_open_project->SetBitmap( wxBitmap( wxT("icons/open.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	menu_file->Append( menu_open_project );
	
	
	menu_save_project = new wxMenuItem( menu_file, 1103, wxString( __I18N("windows:main:save") ) + wxT('\t') + wxT("CTRL+S"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	menu_save_project->SetBitmaps( wxBitmap( wxT("icons/save.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	menu_save_project->SetBitmap( wxBitmap( wxT("icons/save.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	menu_file->Append( menu_save_project );
	
	
	menu_close_project = new wxMenuItem( menu_file, 1104, wxString(__I18N("windows:main:close")) + wxT('\t') + wxT("CTRL+C"), wxEmptyString, wxITEM_NORMAL );
	menu_file->Append( menu_close_project );
	
	menu_file->AppendSeparator();
	
	
	menu_export = new wxMenuItem( menu_file, 1105, wxString(__I18N("windows:main:export")) + __I18N("windows:menu_ext") , wxEmptyString, wxITEM_NORMAL );
	menu_file->Append( menu_export );
	
	menu_file->AppendSeparator();
	
	
	menu_exit = new wxMenuItem( menu_file, 1106, wxString(__I18N("windows:main:exit")) + __I18N("windows:menu_ext") , wxEmptyString, wxITEM_NORMAL );
	menu_file->Append( menu_exit );
	
	main_menu->Append( menu_file, __I18N("windows:main:file") ); 
	
	menu_edit = new wxMenu();
	main_menu->Append( menu_edit, __I18N("windows:main:edit") ); 
	
	menu_tools = new wxMenu();
	
	menu_plugin_manager = new wxMenuItem( menu_tools, 1107, wxString( __I18N("windows:main:plugin_manager") + __I18N("windows:menu_ext") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	menu_plugin_manager->SetBitmaps( wxBitmap( wxT("icons/plugin.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	menu_plugin_manager->SetBitmap( wxBitmap( wxT("icons/plugin.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	menu_tools->Append( menu_plugin_manager );
	
	
	menu_resource_manager = new wxMenuItem( menu_tools, 1108, wxString( __I18N("windows:main:resource_manager") + __I18N("windows:menu_ext") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	menu_resource_manager->SetBitmaps( wxBitmap( wxT("icons/resourcemng.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	menu_resource_manager->SetBitmap( wxBitmap( wxT("icons/resourcemng.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	menu_tools->Append( menu_resource_manager );
	
	
	menu_script_manager = new wxMenuItem( menu_tools, 1109, wxString(  __I18N("windows:main:script_manager") + __I18N("windows:menu_ext") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	menu_script_manager->SetBitmaps( wxBitmap( wxT("icons/scripts.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	menu_script_manager->SetBitmap( wxBitmap( wxT("icons/scripts.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	menu_tools->Append( menu_script_manager );
	
	menu_tools->AppendSeparator();

	menu_playtest  = new wxMenuItem( menu_tools, 1300, wxString(  __I18N("windows:main:playtest") + __I18N("windows:menu_ext") + wxT("\tF5") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	menu_playtest->SetBitmaps( wxBitmap( wxT("icons/playtest.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	menu_playtest->SetBitmap( wxBitmap( wxT("icons/playtest.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	menu_tools->Append( menu_playtest  );

	main_menu->Append( menu_tools, __I18N("windows:main:tools") ); 
	
	menu_help = new wxMenu();
	
	menu_contents = new wxMenuItem( menu_help, 1110, wxString( __I18N("windows:main:contents") + __I18N("windows:menu_ext") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	menu_contents->SetBitmaps( wxBitmap( wxT("icons/help.png"), wxBITMAP_TYPE_ANY ), wxBitmap( wxT("icons/help.png"), wxBITMAP_TYPE_ANY ) );
	#elif defined( __WXGTK__ )
	menu_contents->SetBitmap( wxBitmap( wxT("icons/help.png"), wxBITMAP_TYPE_ANY ) );
	#endif
	menu_help->Append( menu_contents );
	
	
	menu_about = new wxMenuItem( menu_help, 1111, wxString( __I18N("windows:main:about") + __I18N("windows:menu_ext") ) , wxEmptyString, wxITEM_NORMAL );
	menu_help->Append( menu_about );
	
	main_menu->Append( menu_help, __I18N("windows:main:help") ); 
	
	this->SetMenuBar( main_menu );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	tool_main = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); 
	tool_new = tool_main->AddTool( 201, wxT("tool"), wxBitmap( wxT("icons/new.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:main:new_project") + wxT('\n') + __I18N("windows:main:new_project_desc"), wxEmptyString, NULL ); 
	
	tool_open = tool_main->AddTool( 202, wxT("tool"), wxBitmap( wxT("icons/open.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:main:open") + wxT("\n") + __I18N("windows:main:open_project_desc"), wxEmptyString, NULL ); 
	
	tool_save = tool_main->AddTool( 203, wxT("tool"), wxBitmap( wxT("icons/save.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:main:save") + wxT("\n") + __I18N("windows:main:save_project_desc"), wxEmptyString, NULL ); 
	
	tool_main->AddSeparator(); 
	
	tool_plugin = tool_main->AddTool( 204, wxT("tool"), wxBitmap( wxT("icons/plugin.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:main:plugin_manager") + wxT("\n") + __I18N("windows:main:plugin_manager_desc"), wxEmptyString, NULL ); 
	
	tool_resource = tool_main->AddTool( 205, wxT("tool"), wxBitmap( wxT("icons/resourcemng.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:main:resource_manager") + wxT("\n") + __I18N("windows:main:resource_manager_desc"), wxEmptyString, NULL ); 
	
	tool_scripts = tool_main->AddTool( 206, wxT("tool"), wxBitmap( wxT("icons/scripts.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:main:script_manager") + wxT("\n") + __I18N("windows:main:script_manager_desc"), wxEmptyString, NULL ); 
	
	tool_main->AddSeparator(); 
	
	tool_playtest = tool_main->AddTool( 300, wxT("tool"), wxBitmap( wxT("icons/playtest.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, __I18N("windows:main:playtest") + wxT("\n") + __I18N("windows:main:playtest_desc"), wxEmptyString, NULL ); 

	tool_main->Realize(); 
	
	main_sizer->Add( tool_main, 0, wxEXPAND, 5 );
	
	main_panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	main_sizer->Add( main_panel, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainWindowClose ) );
	this->Connect( menu_new_project->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnNewProject ) );
	this->Connect( menu_open_project->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnOpenProject ) );
	this->Connect( menu_save_project->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnSaveProject ) );
	this->Connect( menu_close_project->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnCloseProject ) );
	this->Connect( menu_export->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnExportProject ) );
	this->Connect( menu_exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnExitApplication ) );
	this->Connect( menu_plugin_manager->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnPluginManager ) );
	this->Connect( menu_resource_manager->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnResourceEdit ) );
	this->Connect( menu_script_manager->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnScriptEdit ) );
	this->Connect( menu_playtest->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnPlayTest ) );
	this->Connect( menu_contents->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuContents ) );
	this->Connect( menu_about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuAbout ) );
	this->Connect( 201, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnNewProject ) );
	this->Connect( 202, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnOpenProject ) );
	this->Connect( 203, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnSaveProject ) );
	this->Connect( 204, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnPluginManager ) );
	this->Connect( 205, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnResourceEdit ) );
	this->Connect( 206, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnScriptEdit ) );
	this->Connect( 300, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnPlayTest ) );
	
	if (!ProjectManager::GetCurrent()) {
		this->SetTitle(__I18N("windows:main:title_with_no_project"));
		this->DeactivateProjectFunctions();
	} else {
		this->SetTitle(__I18N("windows:main:title_with_project") + ProjectManager::GetCurrent()->GetName());
	}

}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainWindowClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnNewProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnOpenProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnSaveProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnCloseProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnExportProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnExitApplication ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnPluginManager ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnResourceEdit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnScriptEdit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnPlayTest ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuContents ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnMenuAbout ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnNewProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnOpenProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnSaveProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnPluginManager ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnResourceEdit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::OnScriptEdit ) );
	
}

bool
MainFrame::OnMainWindowClose() {
	return ProjectManager::Exit();
}

void 
MainFrame::OnNewProject() {
	ProjectManager::New();
}

void MainFrame::OnOpenProject() {
	ProjectManager::Open();
}

void MainFrame::OnSaveProject() {
	ProjectManager::Save();
}

void MainFrame::OnCloseProject() {
	ProjectManager::Close();
}

void MainFrame::OnExportProject() {
	ProjectManager::Compile();
}

void MainFrame::OnExitApplication() {
	ProjectManager::Exit();
}

void MainFrame::OnPluginManager() {
	ProjectManager::PluginModify();
}

void MainFrame::OnResourceEdit() {
	ProjectManager::ResourceModify();
}

void MainFrame::OnScriptEdit() {
	ProjectManager::ScriptModify();
}

void MainFrame::OnMenuContents() {
	WindowManager::ShowContents();
}

void MainFrame::OnMenuAbout() {
	WindowManager::ShowAbout();

}

void 
MainFrame::DeactivateProjectFunctions() {

		menu_save_project->Enable(false);
		menu_close_project->Enable(false);
		menu_export->Enable(false);
		menu_plugin_manager->Enable(false);
		menu_resource_manager->Enable(false);
		menu_script_manager->Enable(false);
		menu_playtest->Enable(false);

		tool_save->Enable(false);
		tool_plugin->Enable(false);
		tool_resource->Enable(false);
		tool_scripts->Enable(false);
		tool_playtest->Enable(false);

		tool_main->Realize(); 
}

void
MainFrame::ActivateProjectFunctions() {

		menu_save_project->Enable(true);
		menu_close_project->Enable(true);
		menu_export->Enable(true);
		menu_plugin_manager->Enable(true);
		menu_resource_manager->Enable(true);
		menu_script_manager->Enable(true);
		menu_playtest->Enable(true);

		tool_save->Enable(true);
		tool_plugin->Enable(true);
		tool_resource->Enable(true);
		tool_scripts->Enable(true);
		tool_playtest->Enable(true);

		tool_main->Realize(); 
}

void 
MainFrame::OnPlayTest() {
	ProjectManager::PlayTest();

}