#include "qgf/tools/text.h"
#include "qgf/tools/file_finder.h"
#include "qgf/tools/encoding.h"
#include "qgf/ruby.h"
#include <ft2build.h>
#include <map>
#include <vector>
#include <string>
#include FT_FREETYPE_H
#include FT_GLYPH_H
static FT_Library qgf_ft_lib;
static std::map<std::string, FT_Face> qgf_fonts_map;

FT_Face
qgf_tools_text_getFont(char* name, int bold, int italic) {
	std::string key = std::string(name);
	if (qgf_fonts_map.count(key) == 0) {
		char* font = qgf_tools_file_finder_find_font(name, bold, italic);
		FT_Error err = FT_New_Face(qgf_ft_lib, std::string(font).c_str(), 0, &(qgf_fonts_map[key]));
		free(font);
		if (err) {
			return NULL;
		}
	}
	FT_Face result = qgf_fonts_map[key];
	if (bold) {
		result->style_flags = result->style_flags | FT_STYLE_FLAG_BOLD;
	} else {
		result->style_flags = result->style_flags & ~FT_STYLE_FLAG_BOLD;
	}
	if (italic) {
		result->style_flags = result->style_flags | FT_STYLE_FLAG_ITALIC;	
	} else {
		result->style_flags = result->style_flags & ~FT_STYLE_FLAG_ITALIC;
	}
	return qgf_fonts_map[key];
}

FT_Face
qgf_tools_text_getFT_Face(TFont* font) {
	FT_Face face;
	if (TYPE(font->name) == T_STRING) {
		face = qgf_tools_text_getFont(StringValuePtr(font->name), FONT_BOLD_P(font), FONT_ITALIC_P(font));
		if (face == NULL) {
			RAISE_FONT_NOT_FOUND(StringValuePtr(font->name));
		}
	} else if (TYPE(font->name) == T_ARRAY) {
		long size = NUM2LONG(rb_funcall(font->name, rb_intern("size"), 0));
		for (long i = 0; i < size; i++) {
			ruby_value ary_ent = rb_ary_entry(font->name, i);
			face = qgf_tools_text_getFont(StringValuePtr(ary_ent), FONT_BOLD_P(font), FONT_ITALIC_P(font));
			if (face) return face;
		}
	} else {
		ruby_value value = rb_funcall(font->name, rb_intern("to_s"), 0);
		face = qgf_tools_text_getFont(StringValuePtr(font->name), FONT_BOLD_P(font), FONT_ITALIC_P(font));
	}
	if (face == NULL) {
		ruby_value str = rb_funcall(font->name, rb_intern("inspect"), 0);
		RAISE_FONT_NOT_FOUND(StringValuePtr(str));
		return NULL;
	}
	return face;
}

uint32_t*
qgf_tools_text_textBitmap(TFont* font, TColor* color, char* txt, int* width, int* height) {
	uint32_t* pixels;
	std::string text = std::string(txt);
	FT_Face face = qgf_tools_text_getFT_Face(font);
	FT_Error err;
	err = FT_Set_Pixel_Sizes(face, font->size, font->size);
	if (err) {
		RAISE_FONT_SIZE_ERROR(font->name, font->size, err);
	}
	err = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	if (err) {
		RAISE_FONT_CHARMAP_ERROR(font->name, err);
	}
	FT_Matrix matrix;
	matrix.xx = 0x10000;
	matrix.xy = 0;
	matrix.yx = 0;
	matrix.yy = 0x10000;

	FT_Vector pen;
	pen.x = 0;
	pen.y = 0;
	std::vector<FT_BitmapGlyph> glyphs;
	FT_ULong* utf32 = UTF8_TO_UTF32((char*)text.c_str());
	unsigned long i = 0;
	while (utf32[i] != '\0') {
		FT_ULong ch = utf32[i];//FT_Get_Char_Index(face, utf32[i]);
		FT_Set_Transform(face, &matrix, &pen);
		err = FT_Load_Char(face, ch, FT_LOAD_TARGET_NORMAL);
		if (err) {
			RAISE_FONT_CHAR_LOAD_ERROR(font->name, ch, err);
		}
		FT_Glyph glyph;
		err = FT_Get_Glyph(face->glyph, &glyph);
		if (err) {
			RAISE_FONT_CHAR_LOAD_ERROR(font->name, ch, err);
		}
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		glyphs.push_back((FT_BitmapGlyph)glyph);
		pen.x += face->glyph->advance.x;
		i++;
	}
	free(utf32);
	int w, h;
	w = 0;
	h = 0;
	for (unsigned int i = 0; i < glyphs.size(); i++) {
		if (w < glyphs[i]->bitmap.width + glyphs[i]->left) w = glyphs[i]->bitmap.width + glyphs[i]->left;
		if (h < glyphs[i]->bitmap.rows + glyphs[i]->top) h = glyphs[i]->bitmap.rows + glyphs[i]->top;
	}
	int min_y = h;
	for (unsigned int i = 0; i < glyphs.size(); i++) {
		if (min_y > h - glyphs[i]->top) min_y = h - glyphs[i]->top;
	}
	min_y = (int)(((float)min_y + 0.5f) / 2.0f);
	pixels = (uint32_t*)malloc(sizeof(uint32_t) * w * h);
	for (int i = 0; i < w * h; i++) {
		pixels[i] = 0;
	} 
	uint8_t* dst_pixels = (uint8_t*)pixels;

	int red = 255;
	int green = 255;
	int blue = 255;
	int alpha = 255;
	if (color) {
		red   = (int)color->red;
		green = (int)color->green;
		blue  = (int)color->blue;
		alpha = (int)color->alpha;
	}

	for (unsigned int i = 0; i < glyphs.size(); i++) {
		
		long glyph_pixel = 0;

		int left = glyphs[i]->left;
		int top = (*height) - glyphs[i]->top;

		for (int y = 0; y < glyphs[i]->bitmap.rows; y++) {
			for (int x = 0; x < glyphs[i]->bitmap.width; x++) {
				if (top + y - min_y < 0 || left + x < 0) continue;
				if (top + y - min_y > h || left + x > w) continue;
				if ((((left + x) + (top + y - min_y) * w) * 4) >= w * h * 4) continue;
				int a = glyphs[i]->bitmap.buffer[x + y * glyphs[i]->bitmap.width];
				long pixel = ((left + x) + (top + y - min_y) * w) * 4;
				dst_pixels[pixel + 0] = (uint8_t)red;
				dst_pixels[pixel + 1] = (uint8_t)green;
				dst_pixels[pixel + 2] = (uint8_t)blue;
				
				dst_pixels[pixel + 3] = (uint8_t)(a * alpha / 255);
			}
			glyph_pixel += glyphs[i]->bitmap.pitch;
		}
	}

	for (unsigned int i = 0; i < glyphs.size(); i++) {
		FT_Done_Glyph((FT_Glyph)glyphs[i]);
	}
	*width = w;
	*height = h;
	return pixels;
}

void 
qgf_tools_text_textSize(TFont* font, char* txt, int* width, int* height) {
	std::string text = std::string(txt);
	FT_Face face = qgf_tools_text_getFT_Face(font);
	FT_Error err;
	err = FT_Set_Pixel_Sizes(face, font->size, font->size);
	if (err) {
		RAISE_FONT_SIZE_ERROR(font->name, font->size, err);
	}
	err = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	if (err) {
		RAISE_FONT_CHARMAP_ERROR(font->name, err);
	}
	int w = 0;
	int h = 0;

	FT_Matrix matrix;
	matrix.xx = 0x10000;
	matrix.xy = 0;
	matrix.yx = 0;
	matrix.yy = 0x10000;

	FT_Vector pen;
	pen.x = 0;
	pen.y = 0;

	std::vector<FT_BitmapGlyph> glyphs;
	FT_ULong* utf32 = UTF8_TO_UTF32((char*)text.c_str());
	unsigned long i = 0;
	while (utf32[i] != '\0') {
		FT_ULong ch = utf32[i];//FT_Get_Char_Index(face, utf32[i]);
		FT_Set_Transform(face, &matrix, &pen);
		err = FT_Load_Char(face, ch, FT_LOAD_TARGET_NORMAL);
		if (err) {
			RAISE_FONT_CHAR_LOAD_ERROR(font->name, ch, err);
		}
		FT_Glyph glyph;
		err = FT_Get_Glyph(face->glyph, &glyph);
		if (err) {
			RAISE_FONT_CHAR_LOAD_ERROR(font->name, ch, err);
		}
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		glyphs.push_back((FT_BitmapGlyph)glyph);
		pen.x += face->glyph->advance.x;
		i++;
	}
	free(utf32);
	for (unsigned int i = 0; i < glyphs.size(); i++) {
		if (w < glyphs[i]->bitmap.width + glyphs[i]->left) w = glyphs[i]->bitmap.width + glyphs[i]->left;
		if (h < glyphs[i]->bitmap.rows + glyphs[i]->top) h = glyphs[i]->bitmap.rows + glyphs[i]->top;
		FT_Done_Glyph((FT_Glyph)glyphs[i]);
	}

	*width = w;
	*height = h;
}

void
qgf_tools_text_initLibrary() {
	FT_Error err = FT_Init_FreeType(&qgf_ft_lib);
	if (err) { exit(1); }
}
