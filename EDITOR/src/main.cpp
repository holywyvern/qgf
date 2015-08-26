#ifdef _WIN32
#include <Windows.h>
#endif

#include <wx/wx.h>

#include "main_window.hpp"
#include "window_manager.hpp"
#include "i18n.hpp"

class QGFApp : public wxApp
{
  public:
	  virtual bool OnInit() {
		  wxInitAllImageHandlers();
		  I18n::Init();
		  MainFrame* window = new MainFrame(NULL);
		  window->SetIcon(wxIcon(wxT("AAAAAAAAAAAAA")));
		  WindowManager::SetMainWindow(window);
		  window->Show(true);
		  window->Centre();
		  return true;
	  }
};

IMPLEMENT_APP(QGFApp);

