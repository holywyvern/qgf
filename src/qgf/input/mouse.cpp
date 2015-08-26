#include "qgf/input/mouse.h"
#ifdef _WIN32
#include "qgf/input/keyboard.h"
#endif
#include "qgf/graphics.h"
#include "qgf/tools/window.h"
#include "qgf/ruby.h"

	static int    qgf_mouse_swap;
	static int    qgf_mouse_primary;
	static int    qgf_mouse_secondary;
	static POINT  qgf_mouse_point;
	static long   qgf_mouse_screen_x;
	static long   qgf_mouse_screen_y;
	static long   qgf_mouse_x;
	static long   qgf_mouse_y;

	ruby_value 
    rb_mMouseCLASS_screen_x(ruby_value self) {
		return LONG2NUM(qgf_mouse_screen_x);
	}

	ruby_value 
    rb_mMouseCLASS_screen_y(ruby_value self) {
		return LONG2NUM(qgf_mouse_screen_y);
	}

	ruby_value 
    rb_mMouseCLASS_screen_position(ruby_value self) {
		return rb_ary_new3(2, LONG2NUM(qgf_mouse_screen_x), LONG2NUM(qgf_mouse_screen_y));
	}

	ruby_value
    rb_mMouseCLASS_x(ruby_value self) {
		return LONG2NUM(qgf_mouse_x);
	}

	ruby_value 
    rb_mMouseCLASS_y(ruby_value self) {
		return LONG2NUM(qgf_mouse_y);
	}

	ruby_value 
    rb_mMouseCLASS_window_position(ruby_value self) {
		return rb_ary_new3(2, LONG2NUM(qgf_mouse_x), LONG2NUM(qgf_mouse_y));
	}

#ifdef _WIN32
	ruby_value
	qgf_mouse_key_value(ruby_value sym) {
		switch (NUM2INT(sym)) {
		case 0:
			return INT2NUM(qgf_mouse_primary);
		case 1:
			return INT2NUM(qgf_mouse_secondary);
		default:
			return sym;
		}
	}
#endif

	ruby_value 
    rb_mMouseCLASS_triggerQ(ruby_value self, ruby_value sym) {
#ifdef _WIN32
		ruby_value key = qgf_mouse_key_value(sym);
		return rb_mKeyboardCLASS_triggerQ(self, key);
#endif
	}

	ruby_value 
    rb_mMouseCLASS_pressQ(ruby_value self, ruby_value sym) {
#ifdef _WIN32
		ruby_value key = qgf_mouse_key_value(sym);
		return rb_mKeyboardCLASS_pressQ(self, key);
#endif
	}

	ruby_value 
    rb_mMouseCLASS_repeatQ(ruby_value self, ruby_value sym) {
#ifdef _WIN32
		ruby_value key = qgf_mouse_key_value(sym);
		return rb_mKeyboardCLASS_repeatQ(self, key);
#endif
	}

	ruby_value 
    rb_mMouseCLASS_releaseQ(ruby_value self, ruby_value sym) {
#ifdef _WIN32
		ruby_value key = qgf_mouse_key_value(sym);
		return rb_mKeyboardCLASS_releaseQ(self, key);
#endif
	}

	ruby_value
    rb_mMouseCLASS_update(ruby_value self) {
#ifdef _WIN32
		long dclick_time = qgf_graphics_frame_rate_func() * (GetDoubleClickTime() / 10) / 100;
		qgf_mouse_swap = SwapMouseButton(0);
		qgf_mouse_primary   = 1 + qgf_mouse_swap;
		qgf_mouse_secondary = 2 - qgf_mouse_swap;
		GetCursorPos(&qgf_mouse_point);
		qgf_mouse_screen_x  = qgf_mouse_point.x;
		qgf_mouse_screen_y  = qgf_mouse_point.y;
		ScreenToClient(qgf_tools_windowHWND(), &qgf_mouse_point);
		qgf_mouse_x  = qgf_mouse_point.x;
		qgf_mouse_y  = qgf_mouse_point.y;
#endif
		return Qnil;
	}

	ruby_value 
    rb_mMouseCLASS_in_areaQ(int size, ruby_value* args, ruby_value self) {
		double x, y, w, h;
		if (size == 1) {
			TRect* rect;
			Data_Get_Struct(args[0], TRect, rect);
			x = rect->x;
			y = rect->y;
			w = rect->width;
			h = rect->height;
		} else if (size == 4) {
			x = NUM2DBL(args[0]);
			y = NUM2DBL(args[1]);
			w = NUM2DBL(args[2]);
			h = NUM2DBL(args[3]);
		} else {
			RAISE_ARGN_S(1, "1 or 4");
		}
		if (qgf_mouse_x < x) return Qfalse;
		if (qgf_mouse_y < y) return Qfalse;
		if (qgf_mouse_x > (x + w)) return Qfalse;
		if (qgf_mouse_y > (y + h)) return Qfalse;
		return Qtrue;
	}

	void
	Init_QGF_Input_Mouse() {
		rb_mMouse = rb_define_module("Mouse");
#ifdef _WIN32
		rb_define_const(rb_mMouse, "PRIMARY", INT2NUM(0));
		rb_define_const(rb_mMouse, "SECONDARY", INT2NUM(1));
		rb_define_const(rb_mMouse, "MIDDLE", INT2NUM(4));
		rb_define_const(rb_mMouse, "MOUSE4", INT2NUM(5));
		rb_define_const(rb_mMouse, "MOUSE5", INT2NUM(6));
#endif
		rb_define_singleton_method(rb_mMouse, "update", RUBY_METHOD_FUNC(rb_mMouseCLASS_update), 0);
		rb_define_singleton_method(rb_mMouse, "screen_x", RUBY_METHOD_FUNC(rb_mMouseCLASS_screen_x), 0);
		rb_define_singleton_method(rb_mMouse, "screen_y", RUBY_METHOD_FUNC(rb_mMouseCLASS_screen_y), 0);
		rb_define_singleton_method(rb_mMouse, "screen_position", RUBY_METHOD_FUNC(rb_mMouseCLASS_screen_position), 0);
		rb_define_singleton_method(rb_mMouse, "x", RUBY_METHOD_FUNC(rb_mMouseCLASS_x), 0);
		rb_define_singleton_method(rb_mMouse, "y", RUBY_METHOD_FUNC(rb_mMouseCLASS_y), 0);
		rb_define_singleton_method(rb_mMouse, "window_position", RUBY_METHOD_FUNC(rb_mMouseCLASS_window_position), 0);
		rb_define_singleton_method(rb_mMouse, "trigger?", RUBY_METHOD_FUNC(rb_mMouseCLASS_triggerQ), 1);
		rb_define_singleton_method(rb_mMouse, "press?", RUBY_METHOD_FUNC(rb_mMouseCLASS_pressQ), 1);
		rb_define_singleton_method(rb_mMouse, "repeat?", RUBY_METHOD_FUNC(rb_mMouseCLASS_repeatQ), 1);
		rb_define_singleton_method(rb_mMouse, "release?", RUBY_METHOD_FUNC(rb_mMouseCLASS_releaseQ), 1);
		rb_define_singleton_method(rb_mMouse, "in_area?", RUBY_METHOD_FUNC(rb_mMouseCLASS_in_areaQ), -1);
	}