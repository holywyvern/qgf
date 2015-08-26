#include "qgf/graphics/tone.h"
#include "qgf/ruby.h"
#include "qgf/tools/func.h"
#include "qgf/ruby.h"

	ruby_value
	rb_gc_cTone_alloc(ruby_value klass) {
		TTone* tone;
		tone = ALLOC(TTone);
		tone->blue = 0;
		tone->gray = 0;
		tone->green = 0;
		tone->red = 0;
		return Data_Wrap_Struct(klass, rb_gc_cTone_mark, rb_gc_cTone_free, tone);
	}

	void 
	rb_gc_cTone_free(TTone* tone) {
		free(tone);
	}

	void 
	rb_gc_cTone_mark(TTone* tone) {
	}

	ruby_value 
	rb_cTone_initialize(int size, ruby_value* args, ruby_value self) {
		TTone* tone;
		Data_Get_Struct(self, TTone, tone);
		if (size == 0) {
			tone->red   = 0;
			tone->green = 0;
			tone->blue  = 0;
			tone->gray  = 0;
		} else if (size == 1) {
			TTone* other;
			Data_Get_Struct(args[0], TTone, other);
			tone->red   = other->red;
			tone->green = other->green;
			tone->blue  = other->blue;
			tone->gray  = other->gray;
		} else if (size == 3) {
			tone->red   = ADJUST_BEETWEEN((float)NUM2DBL(args[0]), -255.0f, 255.0f);
			tone->green = ADJUST_BEETWEEN((float)NUM2DBL(args[1]), -255.0f, 255.0f);
			tone->blue  = ADJUST_BEETWEEN((float)NUM2DBL(args[2]), -255.0f, 255.0f);
			tone->gray  = 0;
		} else if (size == 4) {
			tone->red   = ADJUST_BEETWEEN((float)NUM2DBL(args[0]), -255.0f, 255.0f);
			tone->green = ADJUST_BEETWEEN((float)NUM2DBL(args[1]), -255.0f, 255.0f);
			tone->blue  = ADJUST_BEETWEEN((float)NUM2DBL(args[2]), -255.0f, 255.0f);
			tone->gray  = ADJUST_BEETWEEN((float)NUM2DBL(args[3]),       0, 255.0f);
		} else {
			RAISE_ARGN_S(size, "1, 3 or 4");
		}
		return self;
	}

	ruby_value 
	rb_cTone_set(int size, ruby_value* args, ruby_value self) {
		TTone* tone;
		Data_Get_Struct(self, TTone, tone);
		if (size == 1) {
			TTone* other;
			Data_Get_Struct(args[0], TTone, other);
			tone->red   = other->red;
			tone->green = other->green;
			tone->blue  = other->blue;
			tone->gray  = other->gray;
		} else if (size == 3) {
			tone->red   = ADJUST_BEETWEEN((float)NUM2DBL(args[0]), -255.0f, 255.0f);
			tone->green = ADJUST_BEETWEEN((float)NUM2DBL(args[1]), -255.0f, 255.0f);
			tone->blue  = ADJUST_BEETWEEN((float)NUM2DBL(args[2]), -255.0f, 255.0f);
			tone->gray  = 0;
		} else if (size == 4) {
			tone->red   = ADJUST_BEETWEEN((float)NUM2DBL(args[0]), -255.0f, 255.0f);
			tone->green = ADJUST_BEETWEEN((float)NUM2DBL(args[1]), -255.0f, 255.0f);
			tone->blue  = ADJUST_BEETWEEN((float)NUM2DBL(args[2]), -255.0f, 255.0f);
			tone->gray  = ADJUST_BEETWEEN((float)NUM2DBL(args[3]),       0, 255.0f);
		} else {
			RAISE_ARGN_S(size, "3 or 4");
		}
		return Qnil;
	}

	ruby_value 
	rb_cTone_GETred(ruby_value self) {
		TTone* tone;
		Data_Get_Struct(self, TTone, tone);
		return DBL2NUM(tone->red);
	}

	ruby_value 
	rb_cTone_SETred(ruby_value self, ruby_value value) {
		TTone* tone;
		Data_Get_Struct(self, TTone, tone);
		tone->red = ADJUST_BEETWEEN((float)NUM2DBL(value), -255.0f, 255.0f);
		return value;
	}

	ruby_value 
	rb_cTone_GETgreen(ruby_value self) {
		TTone* tone;
		Data_Get_Struct(self, TTone, tone);
		return DBL2NUM(tone->green);
	}

	ruby_value 
	rb_cTone_SETgreen(ruby_value self, ruby_value value) {
		TTone* tone;
		Data_Get_Struct(self, TTone, tone);
		tone->green = ADJUST_BEETWEEN((float)NUM2DBL(value), -255.0f, 255.0f);
		return value;
	}

	ruby_value 
	rb_cTone_GETblue(ruby_value self) {
		TTone* tone;
		Data_Get_Struct(self, TTone, tone);
		return DBL2NUM(tone->blue);
	}

	ruby_value 
	rb_cTone_SETblue(ruby_value self, ruby_value value) {
		TTone* tone;
		Data_Get_Struct(self, TTone, tone);
		tone->blue = ADJUST_BEETWEEN((float)NUM2DBL(value), -255.0f, 255.0f);
		return value;
	}

	ruby_value 
	rb_cTone_GETgray(ruby_value self) {
		TTone* tone;
		Data_Get_Struct(self, TTone, tone);
		return DBL2NUM(tone->gray);
	}

	ruby_value 
	rb_cTone_SETgray(ruby_value self, ruby_value value) {
		TTone* tone;
		Data_Get_Struct(self, TTone, tone);
		tone->gray = ADJUST_BEETWEEN((float)NUM2DBL(value), 0, 255.0f);
		return value;
	}

	ruby_value 
	rb_cTone__dump(ruby_value self) {
		ruby_value ary;
		TTone* tone;
		Data_Get_Struct(self, TTone, tone);
		ary = rb_ary_new3(4, DBL2NUM(tone->red), DBL2NUM(tone->green), DBL2NUM(tone->blue), DBL2NUM(tone->gray));
		return rb_funcall(ary, rb_intern("pack"), 1, rb_str_new2("d4"));
	}

	ruby_value 
	rb_cToneCLASS__load(ruby_value self, ruby_value str) {
		ruby_value ary[4];
		ruby_value arr = rb_funcall(str, rb_intern("unpack"), 1, rb_str_new2("d4"));
		ary[0] = rb_ary_entry(arr, 0);
		ary[1] = rb_ary_entry(arr, 1);
		ary[2] = rb_ary_entry(arr, 2);
		ary[3] = rb_ary_entry(arr, 3);
		return rb_class_new_instance(4, ary, rb_cTone);
	}


	void
	Init_QGF_Graphics_Tone() {
		rb_cTone = rb_define_class("Tone", rb_cObject);
		rb_define_alloc_func(rb_cTone, rb_gc_cTone_alloc);
		rb_define_method(rb_cTone, "initialize", RUBY_METHOD_FUNC(rb_cTone_initialize), -1);
		rb_define_method(rb_cTone, "set", RUBY_METHOD_FUNC(rb_cTone_set), -1);

		rb_define_method(rb_cTone, "red", RUBY_METHOD_FUNC(rb_cTone_GETred), 0);
		rb_define_method(rb_cTone, "red=", RUBY_METHOD_FUNC(rb_cTone_SETred), 1);
		rb_define_method(rb_cTone, "green", RUBY_METHOD_FUNC(rb_cTone_GETgreen), 0);
		rb_define_method(rb_cTone, "green=", RUBY_METHOD_FUNC(rb_cTone_SETgreen), 1);
		rb_define_method(rb_cTone, "blue", RUBY_METHOD_FUNC(rb_cTone_GETblue), 0);
		rb_define_method(rb_cTone, "blue=", RUBY_METHOD_FUNC(rb_cTone_SETblue), 1);
		rb_define_method(rb_cTone, "gray", RUBY_METHOD_FUNC(rb_cTone_GETgray), 0);
		rb_define_method(rb_cTone, "gray=", RUBY_METHOD_FUNC(rb_cTone_SETgray), 1);

		rb_define_method(rb_cTone, "_dump", RUBY_METHOD_FUNC(rb_cTone__dump), 0);
		rb_define_singleton_method(rb_cTone, "_load", RUBY_METHOD_FUNC(rb_cToneCLASS__load), 1);
	}

	ruby_value
	rb_tone_new(float red, float green, float blue, float gray) {
		ruby_value args[4];
		args[0] = DBL2NUM(ADJUST_BEETWEEN(red,   -255.0f, 255.0f));
		args[1] = DBL2NUM(ADJUST_BEETWEEN(green, -255.0f, 255.0f));
		args[2] = DBL2NUM(ADJUST_BEETWEEN(blue,  -255.0f, 255.0f));
		args[3] = DBL2NUM(ADJUST_BEETWEEN(gray,        0, 255.0f));
		return rb_class_new_instance(4, args, rb_cTone);
	}

	static ruby_value 
	rb_tone_new(float red, float green, float blue) {
		ruby_value args[3];
		args[0] = DBL2NUM(ADJUST_BEETWEEN(red,   -255.0f, 255.0f));
		args[1] = DBL2NUM(ADJUST_BEETWEEN(green, -255.0f, 255.0f));
		args[2] = DBL2NUM(ADJUST_BEETWEEN(blue,  -255.0f, 255.0f));
		return rb_class_new_instance(3, args, rb_cTone);
	}

	ruby_value 
	rb_tone_new(ruby_value tone) {
		return rb_class_new_instance(1, &tone, rb_cTone);
	}

	ruby_value
	rb_tone_new() {
		return rb_class_new_instance(0, NULL, rb_cTone);
	}
