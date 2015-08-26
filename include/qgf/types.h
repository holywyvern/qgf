#ifndef QGF_TYPES_H
#define QGF_TYPES_H 1

#ifdef _WIN32
#include "GL/glew.h"
#include "GL/wglew.h"
#endif

#include "qgf/tools/audio_system.h"

#include <ruby.h>
#include <stdint.h>
#include <gl/GL.h>

typedef VALUE ruby_value;

typedef struct {
long size;
long capa;
char sorted;
ruby_value* values;

} qgf_arr;

//==============================================================================================
// ** C structs
//----------------------------------------------------------------------------------------------
// The special structs used as data
//----------------------------------------------------------------------------------------------

#define FONT_FLAG_BOLD        1
#define FONT_FLAG_ITALIC      2
#define FONT_FLAG_UNDERLINE   4
#define FONT_FLAG_OUTLINE     8
#define FONT_FLAG_SHADOW     16

typedef struct {
ruby_value name;
uint8_t size;
uint8_t flags;
ruby_value color;
ruby_value out_color;
ruby_value shadow_color;
ruby_value gradient_start_color;
ruby_value gradient_end_color;
} TFont;

#define FONT_BOLD_P(font)      (font->flags & FONT_FLAG_BOLD)
#define FONT_ITALIC_P(font)    (font->flags & FONT_FLAG_ITALIC)
#define FONT_OUTLINE_P(font)   (font->flags & FONT_FLAG_OUTLINE)
#define FONT_UNDERLINE_P(font) (font->flags & FONT_FLAG_UNDERLINE)
#define FONT_SHADOW_P(font)    (font->flags & FONT_FLAG_SHADOW)

//-----------------------------------------------------------------------------------------
// * Bitmap information Struct
//-----------------------------------------------------------------------------------------

#define BITMAP_FLAG_DISPOSED 1
#define BITMAP_FLAG_CHANGED  2

typedef struct {
ruby_value font;
int width;
int height;
uint32_t* pixels;
uint8_t flags;
GLuint  texture;
GLuint flash_texture;
} TBitmap;

#define BITMAP_DISPOSED_P(x) (x->flags & BITMAP_FLAG_DISPOSED)
#define BITMAP_CHANGED_P(x)  (x->flags & BITMAP_FLAG_CHANGED)	

typedef struct {
float red;
float green;
float blue;
float alpha;
} TColor;

typedef struct {
float red;
float green;
float blue;
float gray;
} TTone;

typedef struct {
float x;
float y;
float width;
float height;
} TRect;

typedef struct {
	long xsize;
	long ysize;
	long zsize;
	long* data;
} TTable;

typedef struct {
float x;
float y;
float z;
} TPoint;


typedef struct {
	qgf_stream* stream;
	ruby_value name;
} TAudioFile;

typedef void (*zobject_funct_ptr)(void*, long, long);

#define QGF_DRAW_FUNC(func) ((zobject_funct_ptr)func)

typedef struct {
float z;
void* object;
char disposed;
uint8_t visible;
zobject_funct_ptr on_draw;
} TZObject;

typedef struct {
ruby_value rect;
qgf_arr objects;
float ox;
float oy;
ruby_value color;
ruby_value flash_color;
ruby_value tone;
int flash_duration;
GLuint flash_texture;
} TViewport;

typedef struct {
	ruby_value flash_color;
	int flash_duration;
	ruby_value bitmap;
	ruby_value src_rect;
	ruby_value viewport;
	uint8_t mirror_x, mirror_y;
	float x, y, ox, oy;
	float zoom_x, zoom_y, angle;
	float wave_amp, wave_length, wave_speed, wave_phase;
	int bush_depth, bush_opacity;
	float opacity;
	int blend_type;
	ruby_value color;
	ruby_value tone;
}  TSprite;

typedef struct {
	float zoom_x;
    float zoom_y;
    float opacity;
	float ox;
	float oy;
	int blend_type;
	ruby_value bitmap;
	ruby_value viewport;
	ruby_value color;
    ruby_value tone;
    int flash_duration;
    ruby_value flash_color;
	float wave_amp, wave_length, wave_speed, wave_phase;
} TPlane;

typedef struct {
	char a;
} TTilemapXP;

typedef struct {
	char a;
} TTilemapVX;

typedef struct {
	char a;
} TWindowXP;

typedef struct {
	char a;
} TWindowVX;

#endif
