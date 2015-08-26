#include "qgf/graphics/font.h"	
#include "qgf/tools/func.h"
#include "qgf/tools/file_finder.h"
#include "qgf/tools/text.h"

#include "qgf/ruby.h"

	static int qgf_font_default_size;
	static uint8_t qgf_font_default_flags;

	ruby_value
	rb_gc_cFont_alloc(ruby_value klass) {
		TFont* font = ALLOC(TFont);
		font->name = Qnil;
		font->size = 0;
		font->flags = 0;
		font->color = Qnil;
		font->out_color = Qnil;
		font->shadow_color = Qnil;
		font->gradient_start_color = Qnil;
		font->gradient_end_color = Qnil;
		return Data_Wrap_Struct(klass, rb_gc_cFont_mark, rb_gc_cFont_free, font);
	}

	void 
	rb_gc_cFont_free(TFont* font) {
		free(font);
	}

	void 
	rb_gc_cFont_mark(TFont* font) {
		rb_gc_mark(font->name);
		rb_gc_mark(font->color);
		rb_gc_mark(font->out_color);
		rb_gc_mark(font->shadow_color);
		rb_gc_mark(font->gradient_start_color);
		rb_gc_mark(font->gradient_end_color);
	}

	ruby_value 
	rb_cFont_initialize(int size, ruby_value* args, ruby_value self) {
		int fsize;
		ruby_value name;
		TFont* font;
		if (size == 0) {
			fsize = qgf_font_default_size;
			name = rb_cv_get(rb_cFont, "@@default_name");
		} else if (size == 1) {
			fsize = qgf_font_default_size;
			name = args[0];
		} else if (size == 2) {
			name = args[0];
			fsize = NUM2INT(args[1]);
		} else {
			RAISE_ARGN_S(size, "1 or 2");
		}
		Data_Get_Struct(self, TFont, font);
		font->name = name;
		font->size = fsize;
		font->flags = qgf_font_default_flags;
		font->color = rb_color_new(rb_cv_get(rb_cFont, "@@default_color"));
		font->out_color = rb_color_new(rb_cv_get(rb_cFont, "@@default_out_color"));
		font->shadow_color = rb_color_new(rb_cv_get(rb_cFont, "@@default_shadow_color"));
		font->gradient_start_color = rb_color_new(rb_cv_get(rb_cFont, "@@default_color"));
		TColor* c;
		Data_Get_Struct(font->color, TColor, c);
		font->gradient_end_color = rb_color_new(c->red / 2.0f, c->green / 2.0f, c->blue / 2.0f, c->alpha);
		return self;
	}

	ruby_value 
	rb_cFontCLASS_existQ(ruby_value self, ruby_value name) {
		char* n = qgf_tools_file_finder_find_font(StringValuePtr(name), 0, 0);
		if (n) {
			free(n);
			return Qtrue;
		}
		return Qfalse;
	}

	ruby_value 
	rb_cFont_GETname(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return font->name;
	}
	ruby_value 
	rb_cFont_SETname(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		font->name = value;
		return value;
	}

	ruby_value 
	rb_cFont_GETsize(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return INT2NUM(font->size);
	}
	ruby_value 
	rb_cFont_SETsize(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		font->size = NUM2INT(value);
		return value;
	}

	ruby_value 
	rb_cFont_GETbold(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return FONT_BOLD_P(font) ? Qtrue : Qfalse;
	}
	ruby_value 
	rb_cFont_SETbold(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		if (RTEST(value)) {
			font->flags = font->flags|FONT_FLAG_BOLD;
		} else {
			font->flags = font->flags & ~FONT_FLAG_BOLD;
		}
		return Qnil;
	}

	ruby_value 
	rb_cFont_GETitalic(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return FONT_ITALIC_P(font) ? Qtrue : Qfalse;
	}
	ruby_value 
	rb_cFont_SETitalic(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		if (RTEST(value)) {
			font->flags = font->flags|FONT_FLAG_ITALIC;
		} else {
			font->flags = font->flags & ~FONT_FLAG_ITALIC;
		}
		return value;
	}

	ruby_value 
	rb_cFont_GEToutline(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return FONT_OUTLINE_P(font) ? Qtrue : Qfalse;
	}
	ruby_value 
	rb_cFont_SEToutline(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		if (RTEST(value)) {
			font->flags = font->flags|FONT_FLAG_OUTLINE;
		} else {
			font->flags = font->flags & ~FONT_FLAG_OUTLINE;
		}
		return value;
	}

	ruby_value 
	rb_cFont_GETshadow(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return FONT_SHADOW_P(font) ? Qtrue : Qfalse;
	}
	ruby_value 
	rb_cFont_SETshadow(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		if (RTEST(value)) {
			font->flags = font->flags|FONT_FLAG_SHADOW;
		} else {
			font->flags = font->flags & ~FONT_FLAG_SHADOW;
		}
		return value;
	}

	ruby_value 
	rb_cFont_GETunderline(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return FONT_UNDERLINE_P(font) ? Qtrue : Qfalse;
	}
	ruby_value 
	rb_cFont_SETunderline(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		if (RTEST(value)) {
			font->flags = font->flags|FONT_FLAG_UNDERLINE;
		} else {
			font->flags = font->flags & ~FONT_FLAG_UNDERLINE;
		}
		return value;
	}

	ruby_value 
	rb_cFont_GETcolor(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return font->color;
	}
	ruby_value 
	rb_cFont_SETcolor(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		font->color = value;
		return value;
	}

	ruby_value 
	rb_cFont_GETout_color(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return font->out_color;
	}
	ruby_value 
	rb_cFont_SETout_color(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		font->out_color = value;
		return value;
	}

	ruby_value 
	rb_cFont_GETshadow_color(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return font->shadow_color;
	}

	ruby_value 
	rb_cFont_SETshadow_color(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		font->shadow_color = value;
		return value;
	}

	ruby_value 
	rb_cFont_GETgradient_start_color(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return font->gradient_start_color;
	}

	ruby_value 
	rb_cFont_SETgradient_start_color(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		font->gradient_start_color = value;
		return value;
	}

	ruby_value 
	rb_cFont_GETgradient_end_color(ruby_value self) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		return font->gradient_end_color;
	}

	ruby_value 
	rb_cFont_SETgradient_end_color(ruby_value self, ruby_value value) {
		TFont* font;
		Data_Get_Struct(self, TFont, font);
		font->gradient_end_color = value;
		return value;
	}


	ruby_value 
	rb_cFontCLASS_GETdefault_name(ruby_value self) {
		return rb_cv_get(self, "@@default_name");
	}

	ruby_value 
	rb_cFontCLASS_SETdefault_name(ruby_value self, ruby_value value) {
		rb_cv_set(self, "@@default_name", value);
		return value;
	}

	ruby_value 
	rb_cFontCLASS_GETdefault_size(ruby_value self) {
		return INT2NUM(qgf_font_default_size);
	}

	ruby_value
	rb_cFontCLASS_SETdefault_size(ruby_value self, ruby_value value) {
		qgf_font_default_size = NUM2INT(value);
		return value;
	}

	ruby_value 
	rb_cFontCLASS_GETdefault_bold(ruby_value self) {
		return qgf_font_default_flags & FONT_FLAG_BOLD ? Qtrue : Qfalse;
	}

	ruby_value 
	rb_cFontCLASS_SETdefault_bold(ruby_value self, ruby_value value) {
		if (RTEST(value)) {
			qgf_font_default_flags = qgf_font_default_flags | FONT_FLAG_BOLD;
		} else {
			qgf_font_default_flags = qgf_font_default_flags & ~FONT_FLAG_BOLD;
		}
		return value;
	}

	ruby_value 
	rb_cFontCLASS_GETdefault_italic(ruby_value self) {
		return qgf_font_default_flags & FONT_FLAG_ITALIC ? Qtrue : Qfalse;
	}

	ruby_value 
	rb_cFontCLASS_SETdefault_italic(ruby_value self, ruby_value value) {
		if (RTEST(value)) {
			qgf_font_default_flags = qgf_font_default_flags | FONT_FLAG_ITALIC;
		} else {
			qgf_font_default_flags = qgf_font_default_flags & ~FONT_FLAG_ITALIC;
		}
		return value;
	}

	ruby_value 
	rb_cFontCLASS_GETdefault_shadow(ruby_value self) {
		return qgf_font_default_flags & FONT_FLAG_SHADOW ? Qtrue : Qfalse;
	}

	ruby_value 
	rb_cFontCLASS_SETdefault_shadow(ruby_value self, ruby_value value) {
		if (RTEST(value)) {
			qgf_font_default_flags = qgf_font_default_flags | FONT_FLAG_SHADOW;
		} else {
			qgf_font_default_flags = qgf_font_default_flags & ~FONT_FLAG_SHADOW;
		}
		return value;
	}

	ruby_value 
	rb_cFontCLASS_GETdefault_underline(ruby_value self) {
		return qgf_font_default_flags & FONT_FLAG_UNDERLINE ? Qtrue : Qfalse;
	}

	ruby_value 
	rb_cFontCLASS_SETdefault_underline(ruby_value self, ruby_value value) {
		if (RTEST(value)) {
			qgf_font_default_flags = qgf_font_default_flags | FONT_FLAG_UNDERLINE;
		} else {
			qgf_font_default_flags = qgf_font_default_flags & ~FONT_FLAG_UNDERLINE;
		}
		return value;
	}

	ruby_value 
	rb_cFontCLASS_GETdefault_outline(ruby_value self) {
		return qgf_font_default_flags & FONT_FLAG_OUTLINE ? Qtrue : Qfalse;
	}

	ruby_value 
	rb_cFontCLASS_SETdefault_outline(ruby_value self, ruby_value value) {
		if (RTEST(value)) {
			qgf_font_default_flags = qgf_font_default_flags | FONT_FLAG_OUTLINE;
		} else {
			qgf_font_default_flags = qgf_font_default_flags & ~FONT_FLAG_OUTLINE;
		}
		return value;
	}

	ruby_value 
	rb_cFontCLASS_GETdefault_color(ruby_value self) {
		return rb_cv_get(self, "@@default_color");
	}

	ruby_value 
	rb_cFontCLASS_SETdefault_color(ruby_value self, ruby_value value) {
		rb_cv_set(self, "@@default_color", value);
		return value;
	}

	ruby_value 
	rb_cFontCLASS_GETdefault_out_color(ruby_value self) {
		return rb_cv_get(self, "@@default_out_color");
	}

	ruby_value 
	rb_cFontCLASS_SETdefault_out_color(ruby_value self, ruby_value value) {
		rb_cv_set(self, "@@default_out_color", value);
		return value;
	}

	ruby_value 
	rb_cFontCLASS_GETdefault_shadow_color(ruby_value self) {
		return rb_cv_get(self, "@@default_shadow_color");
	}

	ruby_value 
	rb_cFontCLASS_SETdefault_shadow_color(ruby_value self, ruby_value value) {
		rb_cv_set(self, "@@default_shadow_color", value);
		return value;
	}

	ruby_value 
	rb_cFont_text_size(ruby_value self, ruby_value str) {
		TFont* fnt;
		Data_Get_Struct(self, TFont, fnt);
		int width, height;
		qgf_tools_text_textSize(fnt, StringValuePtr(str), &width, &height);
		return rb_rect_new(0, 0, width, height);
	}

	void
	Init_QGF_Graphics_Font() {
		qgf_font_default_size  = 24;
		qgf_font_default_flags = FONT_FLAG_SHADOW;
		rb_cFont = rb_define_class("Font", rb_cObject);

		rb_define_alloc_func(rb_cFont, rb_gc_cFont_alloc);

		rb_cv_set(rb_cFont, "@@default_name", rb_str_new2("Arial"));
		rb_cv_set(rb_cFont, "@@default_color", rb_color_new(255, 255, 255));
		rb_cv_set(rb_cFont, "@@default_out_color", rb_color_new(0, 0, 0));
		rb_cv_set(rb_cFont, "@@default_shadow_color", rb_color_new(50, 50, 50));

		rb_define_method(rb_cFont, "initialize", RUBY_METHOD_FUNC(rb_cFont_initialize), -1);

		rb_define_method(rb_cFont, "name", RUBY_METHOD_FUNC(rb_cFont_GETname), 0);
		rb_define_method(rb_cFont, "name=", RUBY_METHOD_FUNC(rb_cFont_SETname), 1);
		rb_define_method(rb_cFont, "size", RUBY_METHOD_FUNC(rb_cFont_GETsize), 0);
		rb_define_method(rb_cFont, "size=", RUBY_METHOD_FUNC(rb_cFont_SETsize), 1);
		rb_define_method(rb_cFont, "bold", RUBY_METHOD_FUNC(rb_cFont_GETbold), 0);
		rb_define_method(rb_cFont, "bold=", RUBY_METHOD_FUNC(rb_cFont_SETbold), 1);
		rb_define_method(rb_cFont, "italic", RUBY_METHOD_FUNC(rb_cFont_GETitalic), 0);
		rb_define_method(rb_cFont, "italic=", RUBY_METHOD_FUNC(rb_cFont_SETitalic), 1);
		rb_define_method(rb_cFont, "underline", RUBY_METHOD_FUNC(rb_cFont_GETunderline), 0);
		rb_define_method(rb_cFont, "underline=", RUBY_METHOD_FUNC(rb_cFont_SETunderline), 1);
		rb_define_method(rb_cFont, "outline", RUBY_METHOD_FUNC(rb_cFont_GEToutline), 0);
		rb_define_method(rb_cFont, "outline=", RUBY_METHOD_FUNC(rb_cFont_SEToutline), 1);
		rb_define_method(rb_cFont, "shadow", RUBY_METHOD_FUNC(rb_cFont_GETshadow), 0);
		rb_define_method(rb_cFont, "shadow=", RUBY_METHOD_FUNC(rb_cFont_SETshadow), 1);
		rb_define_method(rb_cFont, "color", RUBY_METHOD_FUNC(rb_cFont_GETcolor), 0);
		rb_define_method(rb_cFont, "color=", RUBY_METHOD_FUNC(rb_cFont_SETcolor), 1);
		rb_define_method(rb_cFont, "out_color", RUBY_METHOD_FUNC(rb_cFont_GETout_color), 0);
		rb_define_method(rb_cFont, "out_color=", RUBY_METHOD_FUNC(rb_cFont_SETout_color), 1);
		rb_define_method(rb_cFont, "shadow_color", RUBY_METHOD_FUNC(rb_cFont_GETshadow_color), 0);
		rb_define_method(rb_cFont, "shadow_color=", RUBY_METHOD_FUNC(rb_cFont_SETshadow_color), 1);

		rb_define_singleton_method(rb_cFont, "exist?", RUBY_METHOD_FUNC(rb_cFontCLASS_existQ), 1);

		rb_define_singleton_method(rb_cFont, "default_name", RUBY_METHOD_FUNC(rb_cFontCLASS_GETdefault_name), 0);
		rb_define_singleton_method(rb_cFont, "default_name=", RUBY_METHOD_FUNC(rb_cFontCLASS_SETdefault_name), 1);
		rb_define_singleton_method(rb_cFont, "default_size", RUBY_METHOD_FUNC(rb_cFontCLASS_GETdefault_size), 0);
		rb_define_singleton_method(rb_cFont, "default_size=", RUBY_METHOD_FUNC(rb_cFontCLASS_SETdefault_size), 1);
		rb_define_singleton_method(rb_cFont, "default_bold", RUBY_METHOD_FUNC(rb_cFontCLASS_GETdefault_bold), 0);
		rb_define_singleton_method(rb_cFont, "default_bold=", RUBY_METHOD_FUNC(rb_cFontCLASS_SETdefault_bold), 1);
		rb_define_singleton_method(rb_cFont, "default_italic", RUBY_METHOD_FUNC(rb_cFontCLASS_GETdefault_italic), 0);
		rb_define_singleton_method(rb_cFont, "default_italic=", RUBY_METHOD_FUNC(rb_cFontCLASS_SETdefault_italic), 1);
		rb_define_singleton_method(rb_cFont, "default_underline", RUBY_METHOD_FUNC(rb_cFontCLASS_GETdefault_underline), 0);
		rb_define_singleton_method(rb_cFont, "default_underline=", RUBY_METHOD_FUNC(rb_cFontCLASS_SETdefault_underline), 1);
		rb_define_singleton_method(rb_cFont, "default_shadow", RUBY_METHOD_FUNC(rb_cFontCLASS_GETdefault_shadow), 0);
		rb_define_singleton_method(rb_cFont, "default_shadow=", RUBY_METHOD_FUNC(rb_cFontCLASS_SETdefault_shadow), 1);
		rb_define_singleton_method(rb_cFont, "default_outline", RUBY_METHOD_FUNC(rb_cFontCLASS_GETdefault_outline), 0);
		rb_define_singleton_method(rb_cFont, "default_outline=", RUBY_METHOD_FUNC(rb_cFontCLASS_SETdefault_outline), 1);
		rb_define_singleton_method(rb_cFont, "default_color", RUBY_METHOD_FUNC(rb_cFontCLASS_GETdefault_color), 0);
		rb_define_singleton_method(rb_cFont, "default_color=", RUBY_METHOD_FUNC(rb_cFontCLASS_SETdefault_color), 1);
		rb_define_singleton_method(rb_cFont, "default_out_color", RUBY_METHOD_FUNC(rb_cFontCLASS_GETdefault_out_color), 0);
		rb_define_singleton_method(rb_cFont, "default_out_color=", RUBY_METHOD_FUNC(rb_cFontCLASS_SETdefault_out_color), 1);
		rb_define_singleton_method(rb_cFont, "default_shadow_color", RUBY_METHOD_FUNC(rb_cFontCLASS_GETdefault_shadow_color), 0);
		rb_define_singleton_method(rb_cFont, "default_shadow_color=", RUBY_METHOD_FUNC(rb_cFontCLASS_SETdefault_shadow_color), 1);

		rb_define_method(rb_cFont, "text_size", RUBY_METHOD_FUNC(rb_cFont_text_size), 1);

		rb_define_method(rb_cFont, "gradient_start_color", RUBY_METHOD_FUNC(rb_cFont_GETgradient_start_color), 0);
		rb_define_method(rb_cFont, "gradient_start_color=", RUBY_METHOD_FUNC(rb_cFont_SETgradient_start_color), 1);
		rb_define_method(rb_cFont, "gradient_end_color", RUBY_METHOD_FUNC(rb_cFont_GETgradient_end_color), 0);
		rb_define_method(rb_cFont, "gradient_end_color=", RUBY_METHOD_FUNC(rb_cFont_SETgradient_end_color), 1);

	}

	ruby_value
	rb_font_new(const char* name, int size) {
		VALUE ary[2];
		ary[0] = rb_str_new2(name);
		ary[1] = INT2NUM(size);
		return rb_class_new_instance(2, ary, rb_cFont);
	}

	ruby_value
	rb_font_new(const char* name) {
		VALUE a = rb_str_new2(name);
		return rb_class_new_instance(1, &a, rb_cFont);
	}

	ruby_value 
	rb_font_new(void) {
		return rb_class_new_instance(0, NULL, rb_cFont);
	}
