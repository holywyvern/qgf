///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __QGF_SCRIPT_PAGE_H
#define __QGF_SCRIPT_PAGE_H 1

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/stc/stc.h>
#include "script_manager_window.hpp"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ScriptPage
///////////////////////////////////////////////////////////////////////////////
class ScriptPage : public wxPanel 
{
	private:
		wxString script_name;
		ScriptManagerDialog* script_window;
		void OnTextChange();
		void OnAnyChange();
		bool modified;
		void ScriptPage::ApplyStyle();
	protected:
		wxStyledTextCtrl*  text_script;
	public:
		
		ScriptPage( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
		~ScriptPage();

		void SetScript(ScriptManagerDialog* window, wxString script_name);
	
		virtual void OnTextChange(wxStyledTextEvent& event) { OnTextChange(); event.Skip(); }
		virtual void OnAnyChange(wxStyledTextEvent& event) { OnAnyChange(); event.Skip(); }

		bool CanCut();
		bool CanCopy();
		bool CanPaste();
		bool CanUndo();
		bool CanRedo();
		bool IsModified();
		void Cut();
		void Copy();
		void Paste();
		void Save();
		void Undo();
		void Redo();

		void GoToLine(int line);

		wxString GetScriptName();
};

#endif //__NONAME_H__
