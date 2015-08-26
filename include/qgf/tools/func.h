#ifndef QGF_TOOLS_FUNC_H
#define QGF_TOOLS_FUNC_H 1

#include "qgf/types.h"

//==============================================================================================
// ** C functions
//----------------------------------------------------------------------------------------------
// Functions for the engine
//----------------------------------------------------------------------------------------------
	// Bitmap c constructors
	ruby_value 
	rb_bitmap_new(int width, int height);
	ruby_value 
	rb_bitmap_new(const char* filename);
	// Color c constructors
	ruby_value
	rb_color_new(float red, float green, float blue, float alpha );
	ruby_value 
	rb_color_new(float red, float green, float blue);
	ruby_value 
	rb_color_new(ruby_value color);
	ruby_value
	rb_color_new();
	// Font c constructors
	ruby_value
	rb_font_new(const char* name, int size);
	ruby_value
	rb_font_new(const char* name);
	ruby_value 
	rb_font_new(void);
	// Plane c constructors
	static ruby_value 
	rb_plane_new(ruby_value viewport);
	ruby_value
	rb_plane_new();
	// Rect c constructors
	ruby_value
	rb_rect_new(float x, float y,float width,float height);
	ruby_value
	rb_rect_new(ruby_value rect);
	ruby_value 
	rb_rect_new();
	// Sprite c constructors
	ruby_value
	rb_sprite_new(ruby_value viewport);
	ruby_value
	rb_sprite_new();
	// Table c constructors
	ruby_value
	rb_table_new(unsigned int xsize,unsigned  int ysize, unsigned int zsize);
	ruby_value 
	rb_table_new(unsigned int xsize,unsigned  int ysize);
	ruby_value
	rb_table_new(unsigned int xsize);
	// TilemapXP c constructors
	ruby_value 
	rb_tilemapxp_new(ruby_value viewport);
	ruby_value 
	rb_tilemapxp_new();
	// TilemapVX c constructors
	ruby_value 
	rb_tilemapvx_new(ruby_value viewport);	
	ruby_value
	rb_tilemapvx_new();
	// Tone c constructors
	ruby_value
	rb_tone_new(float red, float green, float blue, float gray);
	static ruby_value 
	rb_tone_new(float red, float green, float blue);
	ruby_value 
	rb_tone_new(ruby_value tone);
	ruby_value
	rb_tone_new();
	// Viewport c constructors
	ruby_value
	rb_viewport_new(int x, int y, unsigned int width, unsigned int height);
	ruby_value 
	rb_viewport_new(ruby_value rect);
	ruby_value
	rb_viewport_new();
	// WindowXP c constructors
	ruby_value 
	rb_windowxp_new(int x, int y, unsigned int width, unsigned int height);
	ruby_value 
	rb_windowxp_new(int x, int y, unsigned int width);
	ruby_value 
	rb_windowxp_new(int x, int y);
	ruby_value 
	rb_windowxp_new(int x);
	ruby_value 
	rb_windowxp_new();
	// WindowVX c constructors
	ruby_value 
	rb_windowvx_new();
	ruby_value 
	rb_windowvx_new(int x);
	ruby_value 
	rb_windowvx_new(int x, int y);
	ruby_value 
	rb_windowvx_new(int x, int y, unsigned int width);
	ruby_value 
	rb_windowvx_new(int x, int y, unsigned int width, unsigned int height);	

#define ADJUST_BEETWEEN(x, min, max) (x < min ? min : (x > max ? max : x))

	void 
	qgf_color_from_rgba(uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a, uint32_t rgba);

	ruby_value
	qgf_color_from_rgba(uint32_t rgba);

	uint32_t
	qgf_color_to_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	uint32_t
	qgf_color_blend(uint32_t src, uint32_t dst, uint8_t opacity);

	uint32_t
	qgf_color_to_rgba(TColor* c);

	uint32_t
	qgf_color_adjustHSL(uint32_t c, float h, float s, float l);

#endif
