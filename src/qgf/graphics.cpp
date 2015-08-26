#include "qgf/graphics.h"
#include "qgf/tools/file_finder.h"
#include "qgf/tools/window.h"
#include "qgf/tools/openGL.h"
#include "qgf/tools/array.h"
#include "qgf/tools/func.h"
#include "qgf/tools/time.h"
#include "SOIL.h"
#include <string>

	static long           qgf_graphics_width;
	static long           qgf_graphics_height;
	static int            qgf_graphics_frame_rate;
	static unsigned long  qgf_graphics_frame_count;
	static int            qgf_graphics_brightness;
	static char           qgf_graphics_frozen;
	static char           qgf_graphics_in_transition;
	static int            qgf_graphics_frame_skip;
	       qgf_arr        qgf_graphics_object_array;

        static long            qgf_graphics_tick;
        static long            qgf_graphics_last_ticks;
        static long            qgf_graphics_next_ticks;
        static int             qgf_graphics_needed_frame_skips;
        static long            qgf_graphics_real_frame_rate;
   
	ruby_value 
	rb_mGraphicsCLASS_update(ruby_value self) {
		long   real_ticks;
		long   wait_time;
		qgf_graphics_frame_count++;
		if (qgf_graphics_needed_frame_skips > 0) {
			qgf_graphics_needed_frame_skips--;
			return Qnil;
		}
		qgf_graphics_last_ticks = qgf_tools_time_getTicks();
		qgf_graphics_next_ticks = qgf_graphics_last_ticks + 1000 / qgf_graphics_frame_rate;
		ruby_value back = rb_cv_get(rb_mGraphics, "@@bckgrnd_clr");
		TColor* bg;
		Data_Get_Struct(back, TColor, bg);
		if (!qgf_tools_window_update()) {
			rb_raise(rb_eException, "asd");
		}
		qgf_array_sort(&qgf_graphics_object_array);
		qgf_tools_openGL_updateWindow(qgf_graphics_width, qgf_graphics_height, bg);
		for (long i = 0; i < qgf_graphics_object_array.size; i++) {
			if (RTEST(qgf_graphics_object_array.values[i])) {
				TZObject* obj;
				Data_Get_Struct(qgf_graphics_object_array.values[i], TZObject, obj);
				if (obj->visible && obj->on_draw) {
					obj->on_draw(obj->object, qgf_graphics_width, qgf_graphics_height);
				}
			}
		}
		qgf_tools_openGL_finishUpdateWindow();
		real_ticks = qgf_tools_time_getTicks();
		qgf_graphics_real_frame_rate =  real_ticks - qgf_graphics_last_ticks;
		wait_time = qgf_graphics_next_ticks - real_ticks;
		if (wait_time > 0) { 
			qgf_tools_time_sleep((long)wait_time);
		} else if (wait_time < 0) {
			long v = -wait_time * 1000 / qgf_graphics_frame_rate;
			qgf_graphics_needed_frame_skips =  ADJUST_BEETWEEN(v, 0, qgf_graphics_frame_skip);
		}
		
		return Qnil;
	}

	ruby_value 
	rb_mGraphicsCLASS_wait(ruby_value self, ruby_value time) {
		int i;
		int t;
		t = NUM2INT(time);
		for (i = 0; i < t; ++i) {
			rb_mGraphicsCLASS_update(self);
		}
		return Qnil;
	}

	ruby_value 
	rb_mGraphicsCLASS_fadeout(ruby_value self, ruby_value time) {
		int i;
		i = time;
		while (i) {
			qgf_graphics_brightness = (qgf_graphics_brightness * (i - 1)) / i;
			rb_mGraphicsCLASS_update(self);
			i--;
		}
		return Qnil;
	}

	ruby_value 
	rb_mGraphicsCLASS_fadein(ruby_value self, ruby_value time) {
		int i;
		i = time;
		while (i) {
			qgf_graphics_brightness = (qgf_graphics_brightness * (i - 1) + 255) / i;
			rb_mGraphicsCLASS_update(self);
			i--;
		}
		return Qnil;
	}

	ruby_value 
	rb_mGraphicsCLASS_freeze(ruby_value self) {
		qgf_graphics_frozen = 1;
		return Qnil;
	}

	ruby_value 
	rb_mGraphicsCLASS_transition(int size, ruby_value* args, ruby_value self) {
		int time;
		char* filename;
		int vague;
		qgf_graphics_frozen = 0;
		qgf_graphics_in_transition = 1;
		time = (size > 0) ? NUM2INT(args[0]) : 10;
		filename =  (size > 1) ? qgf_tools_file_finder_find_image(StringValuePtr(args[1])) : NULL;
		vague = (size > 2) ? NUM2INT(args[2]) : 40;
		if (filename) {
		} else {
			rb_mGraphicsCLASS_fadeout(self, time / 2);
			rb_mGraphicsCLASS_fadein(self, time / 2);
		}
		qgf_graphics_in_transition = 0;
		return Qnil;
	}

	ruby_value 
	rb_mGraphicsCLASS_snap_to_bitmap(ruby_value self) {
		return Qnil;
	}

	ruby_value 
	rb_mGraphicsCLASS_frame_reset(ruby_value self) {
		qgf_graphics_next_ticks = qgf_tools_time_getTicks() + 1000 / qgf_graphics_frame_rate;
		return Qnil;
	}

	ruby_value 
	rb_mGraphicsCLASS_width(ruby_value self) {
		return LONG2NUM(qgf_graphics_width);
	}

	ruby_value 
	rb_mGraphicsCLASS_height(ruby_value self) {
		return LONG2NUM(qgf_graphics_height);
	}

	ruby_value 
	rb_mGraphicsCLASS_resize_screen(ruby_value self, ruby_value width, ruby_value height) {
		qgf_graphics_width  = NUM2LONG(width);
		qgf_graphics_height = NUM2LONG(height);
		return Qnil;
	}

	ruby_value 
	rb_mGraphicsCLASS_play_movie(ruby_value self, ruby_value filename) {
		return Qnil;
	}

	ruby_value 
	rb_mGraphicsCLASS_GETframe_rate(ruby_value self) {
		return INT2NUM(qgf_graphics_frame_rate);
	}
	ruby_value 
	rb_mGraphicsCLASS_SETframe_rate(ruby_value self, ruby_value value) {
		int v = NUM2INT(value);
		qgf_graphics_frame_rate = v < 10 ? 10 : (v > 120 ? 120 : v);
		return value;
	}

	ruby_value 
	rb_mGraphicsCLASS_GETframe_count(ruby_value self) {
		return ULONG2NUM(qgf_graphics_frame_count);
	}
	ruby_value 
	rb_mGraphicsCLASS_SETframe_count(ruby_value self, ruby_value value) {
		qgf_graphics_frame_count = NUM2ULONG(value);
		qgf_graphics_frame_skip = qgf_graphics_frame_count / 10;
		return value;
	}

	ruby_value 
	rb_mGraphicsCLASS_GETbrightness(ruby_value self) {
		return INT2NUM(qgf_graphics_brightness);
	}
	ruby_value 
	rb_mGraphicsCLASS_SETbrightness(ruby_value self, ruby_value value) {
		int v = NUM2INT(value);
		qgf_graphics_brightness = v > 255 ? 255 : (v < 0 ? 0 : v);
		return value;
	}

	ruby_value 
	rb_mGraphicsCLASS_GETwindow_title(ruby_value self) {
		return rb_funcall(rb_cv_get(self, "@@wndw_title"), rb_intern("clone"), 0);
	}
	ruby_value 
	rb_mGraphicsCLASS_SETwindow_title(ruby_value self, ruby_value value) {
		rb_cv_set(self, "@@wndw_title", value);
		qgf_tools_window_rename(StringValuePtr(value));
		return value;
	}

	ruby_value 
	rb_mGraphicsCLASS_GETframe_skip(ruby_value self) {
		return INT2NUM(qgf_graphics_frame_skip);
	}
	ruby_value 
	rb_mGraphicsCLASS_SETframe_skip(ruby_value self, ruby_value value) {
		int v = NUM2INT(value);
		qgf_graphics_frame_skip = v > qgf_graphics_frame_rate / 10 ? qgf_graphics_frame_rate / 10 : (v < 0 ? 0 : v);
		return value;
	}

	ruby_value
	rb_mGraphicsCLASS_to_bmp(ruby_value self, ruby_value filename) {
		return SOIL_save_screenshot(StringValuePtr(filename), SOIL_SAVE_TYPE_BMP, 0, 0, qgf_graphics_width, qgf_graphics_height);
	}

	ruby_value
	rb_mGraphicsCLASS_real_frame_rate(ruby_value self) {
		return LONG2NUM((long)qgf_graphics_real_frame_rate);
	}

void
Init_QGF_Graphics() {

	qgf_graphics_width      = QGF_GRAPHICS_DEFAULT_WIDTH;
	qgf_graphics_height     = QGF_GRAPHICS_DEFAULT_HEIGHT;
	qgf_graphics_frame_rate = QGF_GRAPHICS_DEFAULT_FRAME_RATE;
	qgf_graphics_frame_count = 0;
	qgf_graphics_brightness = 255;
	qgf_graphics_frame_skip = qgf_graphics_frame_rate / 10;
	qgf_graphics_needed_frame_skips = 0;
	qgf_graphics_last_ticks = qgf_tools_time_getTicks();
	qgf_graphics_next_ticks = qgf_tools_time_getTicks() + 1000 / qgf_graphics_frame_rate;
	rb_mGraphics = rb_define_module("Graphics");

	rb_cv_set(rb_mGraphics, "@@wndw_title", rb_str_new2("QGF Window"));
	rb_cv_set(rb_mGraphics, "@@drawable_objects", rb_ary_new());

	rb_define_singleton_method(rb_mGraphics, "update", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_update), 0);
	rb_define_singleton_method(rb_mGraphics, "wait", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_wait), 1);
	rb_define_singleton_method(rb_mGraphics, "fadein", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_fadein), 1);
	rb_define_singleton_method(rb_mGraphics, "fadeout", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_fadeout), 1);
	rb_define_singleton_method(rb_mGraphics, "freeze", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_freeze), 0);
	rb_define_singleton_method(rb_mGraphics, "transition", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_transition), -1);
	rb_define_singleton_method(rb_mGraphics, "snap_to_bitmap", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_snap_to_bitmap), 0);
	rb_define_singleton_method(rb_mGraphics, "frame_reset", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_frame_reset), 0);
	rb_define_singleton_method(rb_mGraphics, "width", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_width), 0);
	rb_define_singleton_method(rb_mGraphics, "height", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_height), 0);
	rb_define_singleton_method(rb_mGraphics, "resize_screen", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_resize_screen), 0);
	rb_define_singleton_method(rb_mGraphics, "play_movie", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_play_movie), 0);

	rb_define_singleton_method(rb_mGraphics, "frame_rate", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_GETframe_rate), 0);
	rb_define_singleton_method(rb_mGraphics, "frame_rate=", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_SETframe_rate), 1);

	rb_define_singleton_method(rb_mGraphics, "frame_count", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_GETframe_count), 0);
	rb_define_singleton_method(rb_mGraphics, "frame_count=", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_SETframe_count), 1);

	rb_define_singleton_method(rb_mGraphics, "brightness", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_GETbrightness), 0);
	rb_define_singleton_method(rb_mGraphics, "brightness=", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_SETbrightness), 1);

	rb_define_singleton_method(rb_mGraphics, "window_title", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_GETwindow_title), 0);
	rb_define_singleton_method(rb_mGraphics, "window_title=", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_SETwindow_title), 1);

	rb_define_singleton_method(rb_mGraphics, "frame_skip", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_GETframe_skip), 0);
	rb_define_singleton_method(rb_mGraphics, "frame_skip=", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_SETframe_skip), 1);

	rb_define_singleton_method(rb_mGraphics, "real_frame_rate", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_real_frame_rate), 0);

	rb_define_singleton_method(rb_mGraphics, "to_bmp", RUBY_METHOD_FUNC(rb_mGraphicsCLASS_to_bmp), 1);

	Init_QGF_Graphics_Color();
	Init_QGF_Graphics_Tone();
	Init_QGF_Graphics_Bitmap();
	Init_QGF_Graphics_Font();
	Init_QGF_Graphics_Rect();
	Init_QGF_Graphics_Sprite();
	Init_QGF_Graphics_Plane();
	Init_QGF_Graphics_Table();
	Init_QGF_Graphics_Viewport();

	rb_cv_set(rb_mGraphics, "@@bckgrnd_clr", rb_color_new());

}

void 
qgf_graphics_add_object(ruby_value object) {
	ruby_value arr = rb_cv_get(rb_mGraphics, "@@drawable_objects");
	rb_ary_push(arr, object);
	qgf_array_add(&qgf_graphics_object_array, object);
}

void 
qgf_graphics_remove_object(ruby_value object) {
	ruby_value arr = rb_cv_get(rb_mGraphics, "@@drawable_objects");
	rb_ary_delete(arr, object);
	qgf_array_remove(&qgf_graphics_object_array, object);
}

void 
qgf_graphics_notify_zObject() {
	qgf_graphics_object_array.sorted = FALSE;
}

long
qgf_graphics_keyboard_refresh_rate() {
	long n = qgf_graphics_frame_rate / 20;
	return n > 0 ? n : 1;
}

long
qgf_graphics_frame_rate_func() {
	return qgf_graphics_frame_rate;
}