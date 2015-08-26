#include "ruby.h"
#include "qgf/tools/file_finder.h"
#include <string>
#include "qgf/tools/reg.h"
#include <sys/stat.h>
#include "qgf/tools/encoding.h"

#ifdef _WIN32

#include <Windows.h>

void
qgf_win32_str_slasher(char* filename) {
	for (unsigned int i = 0; i < strlen(filename); i++) {
		if (filename[i] == '/') {
			filename[i] = '\\';
		}
	}
}

static char* qgf_win32_win_font_path;

char*
qgf_win32_font_path() {
	if (qgf_win32_win_font_path) return qgf_win32_win_font_path;
	LPTSTR dir = (LPTSTR)malloc(sizeof(TCHAR) * 256);
	int n = GetWindowsDirectory(dir, 256);
#if defined(_UNICODE) 
	std::wstring fonts_path(TEXT(""));
#else
	std::string fonts_path("");
#endif
	if (n > 0) {
		char* str = (char*)dir;
		for (unsigned int i = 0; i < n * sizeof(dir[0]); i++) {
			if (str[i] != '\0') {
				fonts_path += str[i];
			}
		}
		free(dir);
		fonts_path += TEXT("\\Fonts\\");
		qgf_win32_win_font_path = (char*)malloc((fonts_path.size() + 1) * sizeof(TCHAR));
		strcpy(qgf_win32_win_font_path, T2CA(fonts_path.c_str(), CP_UTF8));
		return qgf_win32_win_font_path;
	}
}

#else

#define X) (X)

#endif

const char* image_extensions[] = {"", ".png", ".jpg", ".jpeg", ".bmp", NULL};
const char* music_extensions[] = {"", ".ogg", ".wav", ".mp3",  ".mid", NULL};
const char* sound_extensions[] = {"", ".ogg", ".wav", ".mp3",  NULL};
const char* video_extensions[] = {"", ".ogv", ".avi", ".mp4",   NULL};
const char* data_extensions[]  = {"", ".qgfdata", ".rvdata2", ".rvdata", ".rxdata", NULL};
const char* font_extensions[]  = {"", ".ttf", ".ttc", ".TTF", NULL};
const char* font_folders[]     = {"", "./Fonts/", NULL};

int qgf_tools_file_finder_file_exists(char* filename) {
#ifdef _WIN32
	qgf_win32_str_slasher(filename);
#endif
	FILE * file = fopen(filename, "r");
	if (file) {
		fclose(file);
		return TRUE;
	} 
	return FALSE;
}

char* qgf_tools_file_finder_find_by_extensions(char* filename, const char* extensions[]) {
#ifdef _WIN32
	qgf_win32_str_slasher(filename);
#endif
	if (qgf_tools_file_finder_file_exists(filename)) { 
		char* fn = (char*)malloc((strlen(filename) + 1) * sizeof(char));
		strcpy(fn, filename);
		return fn;
	}
	for (int i = 0; extensions[i]; i++) {
		std::string file("");
		file.append(filename);
		file.append(extensions[i]);
		if (qgf_tools_file_finder_file_exists((char*)file.c_str())) {
			char* f = (char*)malloc((file.size() + 1) * sizeof(char));
			strcpy(f, file.c_str());
			return f;
		}
	}
	return NULL;
}

char*
qgf_tools_file_finder_find_image(char* filename) {
	return qgf_tools_file_finder_find_by_extensions(filename, image_extensions);
}

char*
qgf_tools_file_finder_find_music(char* filename) {
	return qgf_tools_file_finder_find_by_extensions(filename, music_extensions);
}

char*
qgf_tools_file_finder_find_sound(char* filename) {
	return qgf_tools_file_finder_find_by_extensions(filename, sound_extensions);
}

char*
qgf_tools_file_finder_find_video(char* filename) {
	return qgf_tools_file_finder_find_by_extensions(filename, video_extensions);
}

char*
qgf_tools_file_finder_find_data(char* filename) {
	return qgf_tools_file_finder_find_by_extensions(filename, data_extensions);
}

char*
qgf_tools_file_finder_find_font(char* filename, int bold, int italic) {
#ifdef _WIN32
	qgf_win32_str_slasher(filename);
#endif
	for (int i = 0; font_folders[i]; i++) {
		std::string str("");
		str.append(font_folders[i]);
		str.append(filename);
		char* ex = qgf_tools_file_finder_find_by_extensions((char*)str.c_str(), font_extensions);
		if (ex) {
			return ex;
		}
	}
#ifdef _WIN32
	std::string ff("");

	char* fn = qgf_tools_reg_read_str_value(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", (std::string(filename) + " (TrueType)").c_str());
	if (qgf_tools_file_finder_file_exists(fn)) return fn;
	ff = qgf_win32_font_path();
	ff += fn;
	if (qgf_tools_file_finder_file_exists((char*)ff.c_str())) {
		free(fn);
		fn = (char*)malloc((ff.size() + 1) * sizeof(char));
		strcpy(fn, ff.c_str());
		return fn;
	}
	ff += ".ttf";
	if (qgf_tools_file_finder_file_exists((char*)ff.c_str())) {
		free(fn);
		fn = (char*)malloc((ff.size() + 1) * sizeof(char));
		strcpy(fn, ff.c_str());
		return fn;
	}
	free(fn);
	fn = qgf_tools_reg_read_str_value(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Fonts", (std::string(filename) + " (TrueType)").c_str());
	if (qgf_tools_file_finder_file_exists(fn)) return fn;
	ff = qgf_win32_font_path();
	if (qgf_tools_file_finder_file_exists((char*)ff.c_str())) {
		free(fn);
		fn = (char*)malloc((ff.size() + 1) * sizeof(char));
		strcpy(fn, ff.c_str());
		return fn;
	}
	ff += ".ttf";
	if (qgf_tools_file_finder_file_exists((char*)ff.c_str())) {
		free(fn);
		fn = (char*)malloc((ff.size() + 1) * sizeof(char));
		strcpy(fn, ff.c_str());
		return fn;
	}
	free(fn);

#endif
	return NULL;
}
