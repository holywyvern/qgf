#ifndef QGF_TOOLS_OPENGL_H
#define QGF_TOOLS_OPENGL_H 1

#include "qgf/types.h"

#ifdef _WIN32
#include <Windows.h>
#endif

void 
qgf_tools_openGL_initWindow(long width, long height);

char
qgf_tools_openGL_initShaders();

void 
qgf_tools_openGL_updateWindow(long width, long height, TColor* background);

void 
qgf_tools_openGL_bind_texture(TBitmap* bmp);

void
qgf_tools_openGL_finishUpdateWindow();


#endif
