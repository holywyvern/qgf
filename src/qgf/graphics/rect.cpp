#include "qgf/graphics/rect.h"
#include "qgf/ruby.h"

	static ruby_value
	rb_gc_cRect_alloc(ruby_value klass) {
		TRect* rect;
		rect = ALLOC(TRect);
		rect->x = 0;
		rect->y = 0;
		rect->width = 0;
		rect->height = 0;
		return Data_Wrap_Struct(klass, rb_gc_cRect_mark, rb_gc_cRect_free, rect);
	}

	static void 
	rb_gc_cRect_free(TRect* rect) {
		free(rect);
	}

	static void 
	rb_gc_cRect_mark(TRect* rect) {
	}

	static ruby_value 
	rb_cRect_initialize(int size, ruby_value* args, ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		if (size == 0) {
			rect->x      = 0;
			rect->y      = 0;
			rect->width  = 0;
			rect->height = 0;
		} else if (size == 1) {
			TRect* org;
			Data_Get_Struct(self, TRect, org);
			rect->x      = org->x;
			rect->y      = org->y;
			rect->width  = org->width;
			rect->height = org->height;
		} else if (size == 4) {
			rect->x      = (float)NUM2DBL(args[0]);
			rect->y      = (float)NUM2DBL(args[1]);
			rect->width  = (float)NUM2DBL(args[2]);
			rect->height = (float)NUM2DBL(args[3]);
		} else {
			RAISE_ARGN_S(size, "0, 1 or 4");
		}
		if (rect->width < 0) RAISE_ASSERT_POSITIVE_ARG("width");
		if (rect->height< 0) RAISE_ASSERT_POSITIVE_ARG("height");
		return self;
	}

	static ruby_value 
	rb_cRect_set(int size, ruby_value* args, ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		if (size == 1) {
			TRect* org;
			Data_Get_Struct(self, TRect, org);
			rect->x      = org->x;
			rect->y      = org->y;
			rect->width  = org->width;
			rect->height = org->height;
		} else if (size == 4) {
			rect->x      = (float)NUM2DBL(args[0]);
			rect->y      = (float)NUM2DBL(args[1]);
			rect->width  = (float)NUM2DBL(args[2]);
			rect->height = (float)NUM2DBL(args[3]);
		} else {
			RAISE_ARGN_S(size, "1 or 4");
		}
		if (rect->width < 0) RAISE_ASSERT_POSITIVE_ARG("width");
		if (rect->height< 0) RAISE_ASSERT_POSITIVE_ARG("height");
		return Qnil;
	}

	static ruby_value 
	rb_cRect_empty(ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		rect->x      = 0;
		rect->y      = 0;
		rect->width  = 0;
		rect->height = 0;
		return self;
	}

	static ruby_value 
	rb_cRect_GETx(ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		return DBL2NUM(rect->x);
	}

	static ruby_value 
	rb_cRect_SETx(ruby_value self, ruby_value value) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		rect->x = (float)NUM2DBL(value);
		return value;
	}

	static ruby_value 
	rb_cRect_GETy(ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		return DBL2NUM(rect->y);
	}

	static ruby_value 
	rb_cRect_SETy(ruby_value self, ruby_value value) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		rect->y = (float)NUM2DBL(value);
		return value;
	}

	static ruby_value 
	rb_cRect_GETwidth(ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		return DBL2NUM(rect->width);
	}

	static ruby_value 
	rb_cRect_SETwidth(ruby_value self, ruby_value value) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		rect->width = (float)NUM2DBL(value);
		return value;
	}

	static ruby_value 
	rb_cRect_GETheight(ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		return DBL2NUM(rect->height);
	}

	static ruby_value 
	rb_cRect_SETheight(ruby_value self, ruby_value value) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		rect->height = (float)NUM2DBL(value);
		return value;
	}

	static ruby_value 
	rb_cRect_top_left_point(ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		return Qnil;
	}

	static ruby_value 
	rb_cRect_top_right_point(ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		return Qnil;
	}

	static ruby_value 
	rb_cRect_bottom_left_point(ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		return Qnil;
	}

	static ruby_value 
	rb_cRect_bottom_right_point(ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		return Qnil;
	}

	static ruby_value 
	rb_cRect_center_point(ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		return Qnil;
	}

	static ruby_value 
	rb_cRect__dump(ruby_value self) {
		TRect* rect;
		Data_Get_Struct(self, TRect, rect);
		ruby_value ary = rb_ary_new3(4, DBL2NUM(rect->x), DBL2NUM(rect->y), DBL2NUM(rect->width), DBL2NUM(rect->height));
		return rb_funcall(ary, rb_intern("pack"), 1, rb_str_new2("d4"));
	}

	static ruby_value 
	rb_cRectCLASS__load(ruby_value self, ruby_value str) {
		ruby_value a = rb_funcall(str, rb_intern("unpack"), 1, rb_str_new2("d4"));
		ruby_value ary[4];
		ary[0] = DBL2NUM(rb_ary_entry(0, a));
		ary[1] = DBL2NUM(rb_ary_entry(1, a));
		ary[2] = DBL2NUM(rb_ary_entry(2, a));
		ary[3] = DBL2NUM(rb_ary_entry(3, a));
		return rb_class_new_instance(4, ary, rb_cRect);
	}

	void
	Init_QGF_Graphics_Rect() {
		rb_cRect = rb_define_class("Rect", rb_cObject);
		rb_define_alloc_func(rb_cRect, rb_gc_cRect_alloc);


		rb_define_method(rb_cRect, "initialize", RUBY_METHOD_FUNC(rb_cRect_initialize), -1);
		rb_define_method(rb_cRect, "set", RUBY_METHOD_FUNC(rb_cRect_set), -1);
		rb_define_method(rb_cRect, "empty", RUBY_METHOD_FUNC(rb_cRect_empty), 0);

		rb_define_method(rb_cRect, "x", RUBY_METHOD_FUNC(rb_cRect_GETx), 0);
		rb_define_method(rb_cRect, "x=", RUBY_METHOD_FUNC(rb_cRect_SETx), 1);
		rb_define_method(rb_cRect, "y", RUBY_METHOD_FUNC(rb_cRect_GETy), 0);
		rb_define_method(rb_cRect, "y=", RUBY_METHOD_FUNC(rb_cRect_SETy), 1);
		rb_define_method(rb_cRect, "width", RUBY_METHOD_FUNC(rb_cRect_GETwidth), 0);
		rb_define_method(rb_cRect, "width=", RUBY_METHOD_FUNC(rb_cRect_SETwidth), 1);
		rb_define_method(rb_cRect, "height", RUBY_METHOD_FUNC(rb_cRect_GETheight), 0);
		rb_define_method(rb_cRect, "height=", RUBY_METHOD_FUNC(rb_cRect_SETheight), 1);

		rb_define_method(rb_cRect, "_dump", RUBY_METHOD_FUNC(rb_cRect__dump), 1);
		rb_define_singleton_method(rb_cRect, "_load", RUBY_METHOD_FUNC(rb_cRectCLASS__load), 1);

	}

	ruby_value
	rb_rect_new(float x, float y,float width, float height) {
		ruby_value ary[4];
		ary[0] = DBL2NUM(x);
		ary[1] = DBL2NUM(y);
		ary[2] = DBL2NUM(width);
		ary[3] = DBL2NUM(height);
		return rb_class_new_instance(4, ary, rb_cRect);
	}

	ruby_value
	rb_rect_new(ruby_value rect) {
		return rb_class_new_instance(1, &rect, rb_cRect);
	}

	ruby_value 
	rb_rect_new() {
		return rb_class_new_instance(0, NULL, rb_cRect);
	}
