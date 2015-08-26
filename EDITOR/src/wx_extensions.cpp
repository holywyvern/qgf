#include "wx_extensions.hpp"

#include <wx/dir.h>

bool wxCopyDir(wxString sFrom, wxString sTo)
{
    if (sFrom[sFrom.Len() - 1] != '\\' && sFrom[sFrom.Len() - 1] != '/') sFrom += wxFILE_SEP_PATH;
    if (sTo[sTo.Len() - 1] != '\\' && sTo[sTo.Len() - 1] != '/') sTo += wxFILE_SEP_PATH;

    if (!wxDirExists(sFrom)) {
        wxLogError(wxT("%s does not exist!\r\nCan not copy directory"), sFrom.c_str());
        return false;
    }
    if (!wxDirExists(sTo)) {
        if (!wxMkdir(sTo)) {
            wxLogError(wxT("%s could not be created!"), sTo.c_str());
            return false;
        }
    }
    wxDir fDir(sFrom);
    wxString sNext = wxEmptyString;
    bool bIsFile = fDir.GetFirst(&sNext);
    while (bIsFile) {
        const wxString sFileFrom = sFrom + sNext;
        const wxString sFileTo = sTo + sNext;
        if (::wxDirExists(sFileFrom)) {
            wxCopyDir(sFileFrom, sFileTo);
        }
        else {
            if (!::wxFileExists(sFileTo)) {
                if (!::wxCopyFile(sFileFrom, sFileTo)) {
                    return false;
                }
            }
        }
        bIsFile = fDir.GetNext(&sNext);
    }
    return true;
}