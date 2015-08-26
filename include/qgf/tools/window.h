#ifndef QGF_TOOLS_WINDOW_H
#define QGF_TOOLS_WINDOW_H 1

#include "qgf/types.h"

#ifdef __cplusplus
extern "C"
#endif
int
qgf_tools_window_open(long width, long height, char* window_name);

#ifdef __cplusplus
extern "C"
#endif
void
qgf_tools_window_close(void);

#ifdef __cplusplus
extern "C"
#endif
void
qgf_tools_window_rename(char* window_name);

#ifdef __cplusplus
extern "C"
#endif
void
qgf_tools_window_show_message(char* message);

#ifdef __cplusplus
extern "C"
#endif
void
qgf_tools_window_show_warning(char* message);

#ifdef __cplusplus
extern "C"
#endif
void
qgf_tools_window_show_error(char* message);

#ifdef __cplusplus
extern "C"
#endif
void
qgf_tools_window_show_info(char* message);

#ifdef __cplusplus
extern "C"
#endif
int
qgf_tools_window_update(void);

#endif

#ifdef _WIN32
HWND 
qgf_tools_windowHWND();
#endif