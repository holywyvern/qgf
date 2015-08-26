///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "script_page.hpp"
#include "script_manager_window.hpp"
#include "project_manager.hpp"
#include <wx/textfile.h>

///////////////////////////////////////////////////////////////////////////

class qgfStyledTextCtrl : public wxStyledTextCtrl
{
public:  
  qgfStyledTextCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
          const wxPoint &pos = wxDefaultPosition,
          const wxSize &size = wxDefaultSize,
          long style = wxSUNKEN_BORDER|wxVSCROLL
         );

  void OnCharAdded (wxStyledTextEvent &event);

  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(qgfStyledTextCtrl, wxStyledTextCtrl)
  EVT_STC_CHARADDED (wxID_ANY,  qgfStyledTextCtrl::OnCharAdded)
END_EVENT_TABLE()

qgfStyledTextCtrl::qgfStyledTextCtrl(wxWindow *parent, wxWindowID id,
            const wxPoint &pos,
            const wxSize &size,
            long style)
    : wxStyledTextCtrl (parent, id, pos, size, style) {
}


void 
qgfStyledTextCtrl::OnCharAdded (wxStyledTextEvent &event) {
    char chr = (char)event.GetKey();
    int currentLine = GetCurrentLine();
    // Change this if support for mac files with \r is needed
    if (chr == '\n') {
        int lineInd = 0;
        if (currentLine > 0) {
            lineInd = GetLineIndentation(currentLine - 1);
        }
        if (lineInd == 0) return;
        SetLineIndentation (currentLine, lineInd);
        GotoPos(GetLineIndentPosition(currentLine));
    }
}


///////////////////////////////////////////////////////////////////////////

ScriptPage::ScriptPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->modified = false;
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	text_script = new qgfStyledTextCtrl(this, wxID_ANY);
	
	main_sizer->Add( text_script, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	

	ApplyStyle();

	text_script->Connect(wxEVT_STC_CHANGE, wxStyledTextEventHandler( ScriptPage::OnTextChange ), NULL, this );
	text_script->Connect(wxEVT_STC_UPDATEUI, wxStyledTextEventHandler( ScriptPage::OnAnyChange ), NULL, this );
}

ScriptPage::~ScriptPage() {
	text_script->Disconnect(wxEVT_STC_CHANGE, wxStyledTextEventHandler( ScriptPage::OnTextChange ), NULL, this );
	text_script->Disconnect(wxEVT_STC_UPDATEUI, wxStyledTextEventHandler( ScriptPage::OnAnyChange ), NULL, this );
}

void 
ScriptPage::OnTextChange() {
	modified = true;
	this->script_window->OnTextChange();
}

void 
ScriptPage::OnAnyChange() {
	this->script_window->OnTextChange();
}

enum
{
    MARGIN_LINE_NUMBERS,
    MARGIN_FOLD
};

#define WX_COLOR_RB_NAME wxColor(0x002020)
#define WX_COLOR_RB_KEYWORD wxColor(0x7F3000)
#define WX_COLOR_RB_OPERATOR wxColor(0x7F5000)
#define WX_COLOR_RB_STD wxColor(0xFF8080)
#define WX_COLOR_RB_VAR wxColor(0, 0, 0)
#define WX_COLOR_RB_STRING wxColor(0x7F007F)
#define WX_COLOR_RB_DEF wxColor(0x8F5A00)
void
ScriptPage::ApplyStyle() {
	wxStyledTextCtrl* text = this->text_script;

		text->StyleClearAll();
		text->SetLexer(wxSTC_LEX_RUBY);
		//text->SetIndentationGuides(true);

		wxColour backgroundColor = wxColour (240, 240, 240);
		wxColour color =  wxColour (75, 75, 75);

		text->SetMarginWidth (MARGIN_LINE_NUMBERS, 20);
        text->StyleSetForeground(wxSTC_STYLE_LINENUMBER, color );
        text->StyleSetBackground(wxSTC_STYLE_LINENUMBER, backgroundColor);
        text->SetMarginType (MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

		/*
        // ---- Enable code folding
		text->SetProperty(wxT("fold"), wxT("1"));
		text->SetProperty(wxT("fold.compact"), wxT("1"));
		text->SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);
		int marginNum = 1; // can be 0-5 I believe 
		text->SetMarginType(marginNum, wxSTC_MARGIN_SYMBOL);
		text->SetMarginWidth(marginNum, 16);
		text->SetMarginSensitive(marginNum, true);
		text->SetMarginMask(1, wxSTC_MASK_FOLDERS);
		text->SetFoldMarginColour(true, backgroundColor);
		text->SetFoldMarginHiColour(true, backgroundColor);
		text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS, backgroundColor, color);
		text->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS, backgroundColor, color);
		text->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE, backgroundColor, color);
		text->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER, backgroundColor, color);
		text->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED, backgroundColor, color);
		text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED, backgroundColor, color);
		text->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER, backgroundColor, color);
        // ---- End of code folding part
		*/

		text->StyleSetBackground(wxSTC_RB_ERROR, wxColor(255, 0, 0));
		text->StyleSetForeground(wxSTC_RB_COMMENTLINE, wxColor(0x007F00));
		text->StyleSetForeground(wxSTC_RB_POD, wxColor(0x004000));
		text->StyleSetBackground(wxSTC_RB_POD, wxColor(0xC0FFC0));
		text->StyleSetForeground(wxSTC_RB_NUMBER, wxColor(0x007F7F));
		text->StyleSetForeground(wxSTC_RB_WORD, WX_COLOR_RB_KEYWORD);
		text->StyleSetBold(wxSTC_RB_WORD, true);
		text->StyleSetForeground(wxSTC_RB_STRING, WX_COLOR_RB_STRING);
		text->StyleSetForeground(wxSTC_RB_CHARACTER, WX_COLOR_RB_STRING);
		text->StyleSetForeground(wxSTC_RB_CLASS_VAR, WX_COLOR_RB_VAR);
		text->StyleSetBold(wxSTC_RB_CLASS_VAR, true);
		text->StyleSetForeground(wxSTC_RB_DEFNAME, WX_COLOR_RB_DEF);
		text->StyleSetBold(wxSTC_RB_DEFNAME, true);
		text->StyleSetForeground(wxSTC_RB_OPERATOR, WX_COLOR_RB_OPERATOR);
		text->StyleSetBold(wxSTC_RB_OPERATOR, true);
		text->StyleSetForeground(wxSTC_RB_REGEX,  wxColor(0x1003FF));
		text->StyleSetForeground(wxSTC_RB_GLOBAL, wxColor(0x800080));
		text->StyleSetForeground(wxSTC_RB_SYMBOL, wxColor(0x10A060));
		text->StyleSetForeground(wxSTC_RB_INSTANCE_VAR, WX_COLOR_RB_VAR);
		text->StyleSetForeground(wxSTC_RB_BACKTICKS, wxColor(0xFFFF00));
		text->StyleSetBackground(wxSTC_RB_BACKTICKS, wxColor(0xA08080));
		text->StyleSetForeground(wxSTC_RB_DATASECTION, wxColor(0x600000));
		text->StyleSetBackground(wxSTC_RB_DATASECTION, wxColor(0xFFF0D8));
		text->StyleSetForeground(wxSTC_RB_HERE_DELIM, wxColor(0, 0, 0));
		text->StyleSetBackground(wxSTC_RB_HERE_DELIM, wxColor(0xDDD0DD));
		text->StyleSetForeground(wxSTC_RB_HERE_Q, wxColor(0x7F007F));
		text->StyleSetForeground(wxSTC_RB_HERE_QQ, wxColor(0x7F007F));
		text->StyleSetForeground(wxSTC_RB_HERE_QX, wxColor(0x7F007F));
		text->StyleSetBackground(wxSTC_RB_HERE_Q, wxColor(0xDDD0DD));
		text->StyleSetBackground(wxSTC_RB_HERE_QQ, wxColor(0xDDD0DD));
		text->StyleSetBackground(wxSTC_RB_HERE_QX, wxColor(0xDDD0DD));
		text->StyleSetBold(wxSTC_RB_HERE_Q, false);
		text->StyleSetBold(wxSTC_RB_HERE_QQ, true);
		text->StyleSetItalic(wxSTC_RB_HERE_QX, true);
		text->StyleSetForeground(wxSTC_RB_STRING_Q, WX_COLOR_RB_STRING);
		text->StyleSetBackground(wxSTC_RB_STRING_Q, wxColor(0xDDD0DD));
		text->StyleSetForeground(wxSTC_RB_STRING_QQ, wxColor(0x7F007F));
		text->StyleSetForeground(wxSTC_RB_STRING_QX, wxColor(0xFFFF00));
		text->StyleSetBackground(wxSTC_RB_STRING_QX, wxColor(0xA08080));
		text->StyleSetForeground(wxSTC_RB_STRING_QR, wxColor(0, 0, 0));
		text->StyleSetBackground(wxSTC_RB_STRING_QR, wxColor(0xA0FFA0));
		text->StyleSetForeground(wxSTC_RB_STRING_QW, wxColor(0, 0, 0));
		text->StyleSetBackground(wxSTC_RB_STRING_QW, wxColor(0xFFFFE0));
		text->StyleSetForeground(wxSTC_RB_WORD_DEMOTED, WX_COLOR_RB_KEYWORD);
		text->StyleSetBold(wxSTC_RB_WORD_DEMOTED, true);
		text->StyleSetForeground(wxSTC_RB_STDERR, WX_COLOR_RB_STD);
		text->StyleSetForeground(wxSTC_RB_STDIN, WX_COLOR_RB_STD);
		text->StyleSetForeground(wxSTC_RB_STDOUT, WX_COLOR_RB_STD);

		text->StyleSetForeground(wxSTC_RB_MODULE_NAME, WX_COLOR_RB_NAME);
		text->StyleSetBold(wxSTC_RB_MODULE_NAME, true);
		text->StyleSetForeground(wxSTC_RB_CLASSNAME, WX_COLOR_RB_NAME);
		text->StyleSetBold(wxSTC_RB_CLASSNAME, true);

		/*
		text->StyleSetForeground(wxSTC_RB_IDENTIFIER, RUBY_IDENTIFIER_COLOR);
		text->StyleSetForeground(wxSTC_RB_DEFAULT, wxColor(0, 0, 0));
		text->StyleSetForeground(wxSTC_RB_UPPER_BOUND, wxColor(0, 0, 0));
		*/
 

	wxString keywords = wxString(wxT("alias and begin break case catch class def do elsif else fail ")) + 
						wxString(wxT("ensure for end if in module next not or raise redo rescue ")) + 
						wxString(wxT("retry return then throw super unless undef until when while yield")) +
						wxString(wxT("nil self true false __FILE__ __LINE__"));

	text->SetKeyWords(0, keywords);

}

wxString
ScriptPage::GetScriptName() {
	return this->script_name;
}

void
ScriptPage::SetScript(ScriptManagerDialog* window, wxString script_name) {
	this->script_name = script_name;
	this->script_window = window;
	wxString filename = ProjectManager::GetCurrent()->GetFolder() + script_name;
	if (!wxFileName::Exists(filename)) return;
	wxTextFile      tfile;
	tfile.Open(filename);
	wxString str = tfile.GetFirstLine();
	wxString all_text = str;
	while(!tfile.Eof()) {
		str = tfile.GetNextLine();
		all_text = all_text + wxT("\n") + str;
	}
	this->text_script->SetText(all_text);
	modified = false;
}

bool
ScriptPage::CanCut() {
	return this->text_script->CanCut();
}

bool 
ScriptPage::CanCopy() {
	return this->text_script->CanCopy();
}

bool 
ScriptPage::CanPaste() {
	return this->text_script->CanPaste();
}

bool 
ScriptPage::CanUndo() {
	return this->text_script->CanUndo();
}

bool 
ScriptPage::CanRedo() {
	return this->text_script->CanRedo();
}

bool 
ScriptPage::IsModified() {
	return modified;
}

void
ScriptPage::Save() {
	wxString filename = ProjectManager::GetCurrent()->GetFolder() + script_name;
	wxTextFile file(filename);
	file.Open();
	file.Clear();
	file.AddLine(this->text_script->GetText());
	file.Write();
	file.Close();
	this->modified = false;
}

void
ScriptPage::Undo() {
	this->text_script->Undo();
}

void
ScriptPage::Redo() {
	this->text_script->Redo();
}


void 
ScriptPage::Cut() {
	this->text_script->Cut();
}

void 
ScriptPage::Copy() {
	this->text_script->Copy();
}

void 
ScriptPage::Paste() {
	this->text_script->Paste();
}

void 
ScriptPage::GoToLine(int line) {
	this->text_script->SetSelection(this->text_script->GetLineIndentPosition(line - 1), this->text_script->GetLineEndPosition(line -1));
}