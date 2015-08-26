#ifndef QGF_TOOLS_TEXT_H
#define QGF_TOOLS_TEXT_H 1

#include "qgf/types.h"


uint32_t*
qgf_tools_text_textBitmap(TFont* font, TColor* color, char* txt, int* width, int* height);

void 
qgf_tools_text_textSize(TFont* font, char* text, int* width, int* height);

void
qgf_tools_text_initLibrary();

#endif
