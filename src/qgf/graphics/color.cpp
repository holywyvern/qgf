#include "qgf/tools/func.h"
#include "qgf/graphics/color.h"	
#include "qgf/ruby.h"

	static ruby_value
	rb_gc_cColor_alloc(ruby_value klass) {
		TColor* color;
		color = ALLOC(TColor);
		color->alpha = 0;
		color->blue = 0;
		color->green = 0;
		color->red = 0;
		return Data_Wrap_Struct(klass, rb_gc_cColor_mark, rb_gc_cColor_free, color);
	}

	static void 
	rb_gc_cColor_free(TColor* color) {
		free(color);
	}

	static void 
	rb_gc_cColor_mark(TColor* color) {
	}

	static ruby_value 
	rb_cColor_initialize(int size, ruby_value* args, ruby_value self) {
		TColor* color;
		Data_Get_Struct(self, TColor, color);
		if (size == 0) {
			color->alpha = 255.0f;
			color->blue = 0;
			color->green = 0;
			color->red = 0;
		} else if (size == 1) {
			TColor* color2;
			Data_Get_Struct(args[0], TColor, color2);
			color->alpha = color2->alpha;
			color->blue = color2->blue;
			color->green = color2->green;
			color->red = color2->red;
		} else if (size == 3) {
			color->red   = ADJUST_BEETWEEN(NUM2INT(args[0]), 0.0f, 255.0f);
			color->green = ADJUST_BEETWEEN(NUM2INT(args[1]), 0.0f, 255.0f);
			color->blue  = ADJUST_BEETWEEN(NUM2INT(args[2]), 0.0f, 255.0f);
			color->alpha = 255;
		} else if (size == 4) {
			color->red   = ADJUST_BEETWEEN(NUM2INT(args[0]), 0.0f, 255.0f);
			color->green = ADJUST_BEETWEEN(NUM2INT(args[1]), 0.0f, 255.0f);
			color->blue  = ADJUST_BEETWEEN(NUM2INT(args[2]), 0.0f, 255.0f);
			color->alpha = ADJUST_BEETWEEN(NUM2INT(args[2]), 0.0f, 255.0f);
		} else {
			RAISE_ARGN_S(size, "0, 1, 3 or 4");
		}
		
		return self;
	}

	static ruby_value 
	rb_cColor_set(int size, ruby_value* args, ruby_value self) {
		TColor* color;
		Data_Get_Struct(self, TColor, color);
		if (size == 1) {
			TColor* color2;
			Data_Get_Struct(self, TColor, color2);
			color->alpha = color2->alpha;
			color->blue = color2->blue;
			color->green = color2->green;
			color->red = color2->red;
		} else if (size == 3) {
			color->red   = ADJUST_BEETWEEN(NUM2INT(args[0]), 0.0f, 255.0f);
			color->green = ADJUST_BEETWEEN(NUM2INT(args[1]), 0.0f, 255.0f);
			color->blue  = ADJUST_BEETWEEN(NUM2INT(args[2]), 0.0f, 255.0f);
			color->alpha = 255;
		} else if (size == 4) {
			color->red   = ADJUST_BEETWEEN(NUM2INT(args[0]), 0.0f, 255.0f);
			color->green = ADJUST_BEETWEEN(NUM2INT(args[1]), 0.0f, 255.0f);
			color->blue  = ADJUST_BEETWEEN(NUM2INT(args[2]), 0.0f, 255.0f);
			color->alpha = ADJUST_BEETWEEN(NUM2INT(args[2]), 0.0f, 255.0f);
		} else {
			RAISE_ARGN_S(size, "1, 3 or 4");
		}
		return self;
	}

	static ruby_value 
	rb_cColor_SETred(ruby_value self, ruby_value value) {
		TColor* color;
		Data_Get_Struct(self, TColor, color);
		color->red = (float)NUM2DBL(value);
		return value;
	}

	static ruby_value 
	rb_cColor_SETgreen(ruby_value self, ruby_value value) {
		TColor* color;
		Data_Get_Struct(self, TColor, color);
		color->green = (float)NUM2DBL(value);
		return value;
	}

	static ruby_value 
	rb_cColor_SETblue(ruby_value self, ruby_value value) {
		TColor* color;
		Data_Get_Struct(self, TColor, color);
		color->blue = (float)NUM2DBL(value);
		return value;
	}

	static ruby_value 
	rb_cColor_SETalpha(ruby_value self, ruby_value value) {
		TColor* color;
		Data_Get_Struct(self, TColor, color);
		color->alpha = (float)NUM2DBL(value);
		return value;
	}

	static ruby_value 
	rb_cColor_GETred(ruby_value self) {
		TColor* color;
		Data_Get_Struct(self, TColor, color);
		return DBL2NUM(color->red);
	}

	static ruby_value 
	rb_cColor_GETgreen(ruby_value self) {
		TColor* color;
		Data_Get_Struct(self, TColor, color);
		return DBL2NUM(color->green);
	}

	static ruby_value 
	rb_cColor_GETblue(ruby_value self) {
		TColor* color;
		Data_Get_Struct(self, TColor, color);
		return DBL2NUM(color->blue);
	}

	static ruby_value 
	rb_cColor_GETalpha(ruby_value self) {
		TColor* color;
		Data_Get_Struct(self, TColor, color);
		return DBL2NUM(color->alpha);
	}

	static ruby_value 
	rb_cColor__dump(ruby_value self) {
		TColor* color;
		Data_Get_Struct(self, TColor, color);
		ruby_value ary;
		ary = rb_ary_new3(4, DBL2NUM(color->red), DBL2NUM(color->green), DBL2NUM(color->blue), DBL2NUM(color->alpha));
		return rb_funcall(ary, rb_intern("pack"), 1, rb_str_new2("d4"));
	}

	static ruby_value 
	rb_cColorCLASS__load(ruby_value self, ruby_value str) {
		ruby_value ary[4];
		VALUE arr = rb_funcall(str, rb_intern("unpack"), 1, rb_str_new2("d4"));
		ary[0] = rb_ary_entry(arr, 0);
		ary[1] = rb_ary_entry(arr, 1);
		ary[2] = rb_ary_entry(arr, 2);
		ary[3] = rb_ary_entry(arr, 3);
		return rb_class_new_instance(4, ary, rb_cColor);
	}

	void
	Init_QGF_Graphics_Color() {
		rb_cColor = rb_define_class("Color", rb_cObject);

		rb_define_alloc_func(rb_cColor, rb_gc_cColor_alloc);

		rb_define_method(rb_cColor, "initialize", RUBY_METHOD_FUNC(rb_cColor_initialize), -1);
		rb_define_method(rb_cColor, "set", RUBY_METHOD_FUNC(rb_cColor_set), -1);
		rb_define_method(rb_cColor, "_dump", RUBY_METHOD_FUNC(rb_cColor__dump), 0);

		rb_define_method(rb_cColor, "red", RUBY_METHOD_FUNC(rb_cColor_GETred), 0);
		rb_define_method(rb_cColor, "red=", RUBY_METHOD_FUNC(rb_cColor_SETred), 1);
		rb_define_method(rb_cColor, "green", RUBY_METHOD_FUNC(rb_cColor_GETgreen), 0);
		rb_define_method(rb_cColor, "green=", RUBY_METHOD_FUNC(rb_cColor_SETgreen), 1);
		rb_define_method(rb_cColor, "blue", RUBY_METHOD_FUNC(rb_cColor_GETblue), 0);
		rb_define_method(rb_cColor, "blue=", RUBY_METHOD_FUNC(rb_cColor_SETblue), 1);
		rb_define_method(rb_cColor, "alpha", RUBY_METHOD_FUNC(rb_cColor_GETalpha), 0);
		rb_define_method(rb_cColor, "alpha=", RUBY_METHOD_FUNC(rb_cColor_SETalpha), 1);

		rb_define_singleton_method(rb_cColor, "_load", RUBY_METHOD_FUNC(rb_cColorCLASS__load), 1);

	}

	ruby_value
	rb_color_new(float red, float green, float blue, float alpha ) {
		VALUE ary[4];
		ary[0] = DBL2NUM(red);
		ary[1] = DBL2NUM(green);
		ary[2] = DBL2NUM(blue);
		ary[3] = DBL2NUM(alpha);
		return rb_class_new_instance(4, ary, rb_cColor);
	}

	ruby_value 
	rb_color_new(float red, float green, float blue) {
		VALUE ary[3];
		ary[0] = DBL2NUM(red);
		ary[1] = DBL2NUM(green);
		ary[2] = DBL2NUM(blue);
		return rb_class_new_instance(3, ary, rb_cColor);
	}

	ruby_value 
	rb_color_new(ruby_value color) {
		return rb_class_new_instance(1, &color, rb_cColor);
	}

	ruby_value
	rb_color_new() {
		return rb_class_new_instance(0, NULL, rb_cColor);
	}

	void 
	qgf_color_from_rgba(uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a, uint32_t rgba) {
		*a = (rgba & 0xFF000000) >> 24;
		*b = (rgba & 0xFF0000) >> 16;
		*g = (rgba & 0xFF00) >> 8;
		*r = (rgba & 0xFF);
	}

	ruby_value
	qgf_color_from_rgba(uint32_t rgba) {
		uint8_t r, g, b, a;
		qgf_color_from_rgba(&r, &g, &b, &a, rgba);
		return rb_color_new(r, g, b, a);
	}

	uint32_t
	qgf_color_to_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return (a << 24) + (b << 16) + (g << 8) + r;
	}

	uint32_t
	qgf_color_to_rgba(TColor* c) {
		return qgf_color_to_rgba((uint8_t)c->red, (uint8_t)c->green, (uint8_t)c->blue, (uint8_t)c->alpha);
	}

	uint32_t
	qgf_color_blend(uint32_t src, uint32_t dst, uint8_t opacity) {
			uint8_t r, g, b, a;
			uint8_t r2, g2, b2, a2;
			qgf_color_from_rgba(&r, &g, &b, &a, src);
			qgf_color_from_rgba(&r2, &g2, &b2, &a2, dst);
			uint8_t srca = (uint8_t)(a2 * opacity / 255);
			r2 = (r2 * (255 - srca) + r * srca) / 255;
			g2 = (g2 * (255 - srca) + g * srca) / 255;
			b2 = (b2 * (255 - srca) + b * srca) / 255;
			a2 = a2 * (255 - srca) / 255 + srca;
			return qgf_color_to_rgba(r2, g2, b2, a2);
	}

	void
	RGB2HSL(float *h, float *s, float *l, int r2, int g2, int b2) {
		float vmin, vmax, delta;
		float dr, dg, db;
		float r, g, b;
		r = r2 / 255.0f;
		g = g2 / 255.0f;
		b = b2 / 255.0f;
		vmin = min(min(r, g), b);
		vmax = max(max(r, g), b);
		delta = vmax - vmin;
		*l = (vmax + vmin) / 2;
		if (delta == 0) {
			*h = 0;
			*s = 0;
		} else {
			if (*l < 0.5) {
				*s = delta / (vmax + vmin);
			} else {
				*s = delta / (2 - vmax - vmin);
			}
			dr = (((vmax - r) / 6) + (delta / 2)) / delta;
			dg = (((vmax - g) / 6) + (delta / 2)) / delta;
			db = (((vmax - b) / 6) + (delta / 2)) / delta;
			if (r == vmax) {
				*h = db - dg;
			} else if (g == vmax) {
				*h = (1.0 / 3) + dr - db;
			} else if (b == vmax) {
				*h = (2.0 / 3) + dg - dr;
			}
		}
	}

	float 
	Hue_2_RGB(double v1, double v2, double vH) {
		if (vH < 0) vH += 1;
		if (vH > 1) vH -= 1;
		if ((6 * vH) < 1) return (v1 + (v2 - v1) * 6 * vH);
		if ((2 * vH) < 1) return (v2);
		if ((3 * vH) < 2) return (v1 + (v2 - v1) * ((2.0 / 3) - vH ) * 6);
		return v1;
	}

	void
	HSL2RGB(uint8_t* r, uint8_t* g, uint8_t* b, float h, float s, float l) {
		double v1, v2;
		if (s == 0) {
			*r = (unsigned char)(l * 255);
			*g = (unsigned char)(l * 255);
			*b = (unsigned char)(l * 255);
		} else {
			if (l < 0.5) {
				v2 = l * (1 + s);
			} else {
				v2 = (l + s) - (s * l);
			}
			v1 = 2 * l - v2;
			*r = (unsigned char)(255 * Hue_2_RGB(v1, v2, h + (1.0 / 3)));
			*g = (unsigned char)(255 * Hue_2_RGB(v1, v2, h));
			*b = (unsigned char)(255 * Hue_2_RGB(v1, v2, h - (1.0 / 3)));
		}
	}

	uint32_t
	qgf_color_adjustHSL(uint32_t c, float h, float s, float l) {
		uint8_t r, g, b, a;
		float h2, s2, l2;
		qgf_color_from_rgba(&r, &g, &b, &a, c);
		RGB2HSL(&h2, &s2, &l2, r, g, b);
		h2 = h2 + h / 360.0;
		while (h2 > 1) h2 -= 1;
		while (h2 < 0) h2 += 1;
		s2 = s2 + s / 100.0f;
		if (s2 > 1) s2 = 1;
		if (s2 < 0) s2 = 0;
		l2 = l2 * (l  / 100.0f);
		if (l2 > 1) l2 = 1;
		if (l2 < 0) l2 = 0;
		HSL2RGB(&r, &g, &b, h2, s2, l2);
		//rb_funcall(rb_cObject, rb_intern("p"), 3, INT2NUM(r), INT2NUM(g), INT2NUM(b));
		return qgf_color_to_rgba(r, g, b, a);
	}
