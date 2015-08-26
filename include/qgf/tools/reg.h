#ifndef QGF_TOOLS_REG_H
#define QGF_TOOLS_REG_H 1

#ifdef _WIN32

#include <Windows.h>

char*
qgf_tools_reg_read_str_value(HKEY hkey, const char* key, const char* val);

#endif

#endif
