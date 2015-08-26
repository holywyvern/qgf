#include "ruby.h"

#include "qgf/tools/reg.h"
#include "qgf/tools/encoding.h"

#include <string>

#ifdef _WIN32

char*
qgf_tools_reg_read_str_value(HKEY hkey, const char* key, const char* val) {
	char value[1024];
	DWORD size = 1024;
	DWORD type = REG_SZ;
	HKEY key_handle;
	if (RegOpenKeyEx(hkey, CA2T(key, CP_UTF8), 0, KEY_QUERY_VALUE, &key_handle)) {
		return "";
	}
	if (RegQueryValueEx(key_handle, CA2T(val, CP_UTF8), NULL, &type, (LPBYTE)&value, &size)) {
		return "";
	}
	std::string string_value = "";
	for (unsigned int i = 0; i < size; i++) {
		if (value[i] != '\0' ) {
			string_value += value[i];
		}
	}
	char* f = (char*)malloc((string_value.size() + 1) * sizeof(char));
	strcpy(f, string_value.c_str());
	RegCloseKey(key_handle);
	
	return f;
}

#endif
