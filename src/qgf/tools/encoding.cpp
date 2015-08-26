#include "ruby.h"
#include "qgf/tools/encoding.h"
#include <stdint.h>
#include <vector>
#ifdef _WIN32

#ifdef _UNICODE
std::wstring
qgf_tools_encoding_UTF8_2_UTF16(char* text) {
	std::string s(text);
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

std::string
qgf_tools_encoding_UTF16_2_UTF8(wchar_t* text) {
	std::wstring s = std::wstring(text);
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, buf, len, 0, 0);
	std::string r(buf);
	delete[] buf;
	return r;
}

wchar_t*
qgf_tools_encoding_unsafe_UTF8_2_UTF16(char* text) {
	std::string s(text);
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slength, buf, len);
	return buf;
}

char*
qgf_tools_encoding_unsafe_UTF16_2_UTF8(wchar_t* text) {
	std::wstring s = std::wstring(text);
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, buf, len, 0, 0);
	return buf;
}

#else
char*
qgf_tools_encoding_UTF8_2_ANSI(char* text) {
	return std::string(text);
}
#endif

#endif

int UTF8_TO_UTF32_LENGTH(unsigned char ch) {
	int l;
	unsigned char c = (unsigned char)ch;
	c >>= 3;
	// 6 => 0x7e
	// 5 => 0x3e
	if (c == 0x1e) {
		l = 4;
	} else {
		c >>= 1;
		if (c == 0xe) {
			l = 3;
		} else {
			c >>= 1;
			if (c == 0x6) {
				l = 2;
			} else {
				l = 1;
			}
		}
	}
	return l;
}

unsigned long* 
UTF8_TO_UTF32(char *utf8) {
	size_t len = strlen(utf8);
	unsigned char *p = (unsigned char*)utf8;
	unsigned long ch;
	int x = 0;
	int l;
	unsigned long *result = (unsigned long *)malloc(sizeof(unsigned long)*(len+1));
	result[len] = 0;
	unsigned long *r = result;
	if (!result) {
		//ERR("Ran out of memory!");
	}
	while (*p) {
		l = UTF8_TO_UTF32_LENGTH(*p);
		switch (l) {
		case 4:
			ch = (*p ^ 0xf0);
			break;
		case 3:
			ch = (*p ^ 0xe0);
			break;
		case 2:
			ch = (*p ^ 0xc0);
			break;
		case 1:
			ch = *p;
			break;
		default:
			printf("Len: %i\n", l);
		}
		++p;
		int y;
		for (y = l; y > 1; y--) {
			ch <<= 6;
			ch |= (*p ^ 0x80);
			++p;
		}
		x += l;
		*r = ch;
		r++;
	}
	*r = 0x0;
	return result;
}
