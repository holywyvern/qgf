#ifndef QGF_TOOLS_ENCODING_H
#define QGF_TOOLS_ENCODING_H 1

#ifdef _WIN32

#include <Windows.h>
#include <string>

#ifdef _UNICODE
std::wstring
qgf_tools_encoding_UTF8_2_UTF16(char* text);
std::string
qgf_tools_encoding_UTF16_2_UTF8(wchar_t* text);

wchar_t*
qgf_tools_encoding_unsafe_UTF8_2_UTF16(char* text);
char*
qgf_tools_encoding_unsafe_UTF16_2_UTF8(wchar_t* text);

#define CA2T(x, mode) (((std::wstring)qgf_tools_encoding_UTF8_2_UTF16((char*)x)).c_str())
#define T2CA(x, mode) (((std::string)qgf_tools_encoding_UTF16_2_UTF8((wchar_t*)x)).c_str())
#define T2CA_CPP(x, mode) ((std::string)qgf_tools_encoding_UTF16_2_UTF8((wchar_t*)x))

#else
char*
qgf_tools_encoding_UTF8_2_ANSI(char* text);
#define CA2T(x, mode) (((std::string)qgf_tools_encoding_UTF8_2_ANSI(x)).c_str())
#define T2CA_CPP(x, mode) ((std::string)qgf_tools_encoding_UTF8_2_ANSI(x))
#endif

#endif

unsigned long* 
UTF8_TO_UTF32(char *utf8);

#endif
