#include "qgf/graphics/plane.h"
#include "qgf/tools/func.h"
#include "qgf/ruby.h"
#include "qgf/graphics.h"

	ruby_value
	rb_gc_cPlane_alloc(ruby_value klass) {
		TZObject* obj;
		TPlane* plane;
		obj = ALLOC(TZObject);
		plane = ALLOC(TPlane);
		obj->object = plane;
		obj->on_draw = QGF_DRAW_FUNC(rb_cPlane_draw);
		obj->visible = TRUE;
		obj->z = 0;
		plane->bitmap = Qnil;
		plane->color = Qnil;
		plane->flash_color = Qnil;
		plane->tone = Qnil;
		plane->viewport = Qnil;
		return Data_Wrap_Struct(klass, rb_gc_cPlane_mark, rb_gc_cPlane_free, obj);
	}

	void 
	rb_gc_cPlane_free(TZObject* obj) {
		free(obj->object);
		free(obj);
	}

	void 
	rb_gc_cPlane_mark(TZObject* obj) {
		TPlane* plane;
		plane = (TPlane*)obj->object;
		rb_gc_mark(plane->bitmap);
		rb_gc_mark(plane->color);
		rb_gc_mark(plane->flash_color);
		rb_gc_mark(plane->tone);
		rb_gc_mark(plane->viewport);
	}

	ruby_value 
	rb_cPlane_initialize(int size, ruby_value* args, ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (size == 0) {
			qgf_graphics_add_object(self);
			plane->viewport = Qnil;
		} else if (size == 1) {
			rb_viewport_add_object(args[0], self);
			plane->viewport = args[0];
		} else {
			RAISE_ARGN_S(size, "0 or 1");
		}
		plane->bitmap = Qnil;
		plane->blend_type = 0;
		plane->color = rb_color_new(0, 0, 0, 0);
		plane->flash_color = rb_color_new(0, 0, 0, 0);
		plane->flash_duration = 0;
		plane->opacity = 255.0f;
		plane->ox = 0;
		plane->oy = 0;
		plane->tone = rb_tone_new();
		plane->wave_amp = 0;
		plane->wave_length = 180.0f;
		plane->wave_phase = 0;
		plane->wave_speed = 360.0f;
		plane->zoom_x = 1.0f;
		plane->zoom_y = 1.0f;
		obj->disposed = FALSE;
		obj->visible = TRUE;
		obj->z = 0;
		return self;
	}

	ruby_value 
	rb_cPlane_dispose(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		obj->disposed = TRUE;
		return Qnil;
	}

	ruby_value 
	rb_cPlane_disposedQ(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		return obj->disposed ? Qtrue : Qfalse;
	}

	ruby_value
	rb_cPlane_update(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		if (plane->flash_duration > 0 && RTEST(plane->flash_color)) {
			TColor* c;
			TColor* d;
			Data_Get_Struct(plane->flash_color, TColor, c);
			Data_Get_Struct(plane->color, TColor, d);
			c->red   = (c->red   * (plane->flash_duration - 1) + d->red)   / plane->flash_duration;
			c->green = (c->green * (plane->flash_duration - 1) + d->green) / plane->flash_duration;
			c->blue  = (c->blue  * (plane->flash_duration - 1) + d->blue)  / plane->flash_duration;
			c->alpha = (c->alpha * (plane->flash_duration - 1) + d->alpha) / plane->flash_duration;
			plane->flash_duration = plane->flash_duration - 1;
		}
		plane->wave_phase = (plane->wave_phase + plane->wave_speed / 360.0f);
		while (plane->wave_phase >= 360) {
			plane->wave_phase = plane->wave_phase - 360.0f;
		}
		return Qnil;
	}

	ruby_value
	rb_cPlane_flash(ruby_value self, ruby_value color, ruby_value duration) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		plane->flash_color = color;
		plane->flash_duration = NUM2INT(duration);
		return Qnil;
	}

	ruby_value 
	rb_cPlane_GETbitmap(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return plane->bitmap;
	}

	ruby_value 
	rb_cPlane_SETbitmap(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->bitmap = value;
		return value;
	}

	ruby_value 
	rb_cPlane_GETviewport(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return plane->viewport;
	}

	ruby_value 
	rb_cPlane_SETviewport(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		if (RTEST(plane->viewport)) {
			rb_viewport_remove_object(plane->viewport, self);
		} else {
			qgf_graphics_remove_object(self);
		}
		plane->viewport = value;
		if (RTEST(plane->viewport)) {
			rb_viewport_add_object(plane->viewport, self);
		} else {
			qgf_graphics_add_object(self);
		}
		return value;
	}

	ruby_value 
	rb_cPlane_GETvisible(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return obj->visible ? Qtrue : Qfalse;
	}

	ruby_value 
	rb_cPlane_SETvisible(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		obj->visible = RTEST(value);
		return value;
	}

	ruby_value 
	rb_cPlane_GETz(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return DBL2NUM(obj->z);
	}

	ruby_value 
	rb_cPlane_SETz(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		obj->z = (float)NUM2DBL(value);
		if (RTEST(plane->viewport)) {
			TZObject* zview;
			TViewport* viewport;
			Data_Get_Struct(plane->viewport, TZObject, zview);
			viewport = (TViewport*)zview->object;
			viewport->objects.sorted = FALSE;
		} else {
			qgf_graphics_notify_zObject();
		}
		return value;
	}

	ruby_value 
	rb_cPlane_GETox(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return DBL2NUM(plane->ox);
	}

	ruby_value 
	rb_cPlane_SETox(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->ox = (float)NUM2DBL(value);
		return value;
	}

	ruby_value 
	rb_cPlane_GEToy(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return DBL2NUM(plane->oy);
	}

	ruby_value 
	rb_cPlane_SEToy(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->oy = (float)NUM2DBL(value);
		return value;
	}

	ruby_value 
	rb_cPlane_GETzoom_x(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return DBL2NUM(plane->zoom_x);
	}

	ruby_value 
	rb_cPlane_SETzoom_x(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->zoom_x = (float)NUM2DBL(value);
		return value;
	}

	ruby_value 
	rb_cPlane_GETzoom_y(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return DBL2NUM(plane->zoom_y);
	}

	ruby_value 
	rb_cPlane_SETzoom_y(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->zoom_y = (float)NUM2DBL(value);
		return value;
	}

	ruby_value 
	rb_cPlane_GETopacity(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return DBL2NUM(plane->opacity);
	}

	ruby_value 
	rb_cPlane_SETopacity(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->opacity = (float)NUM2DBL(value);
		return value;
	}

	ruby_value 
	rb_cPlane_GETblend_type(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return INT2NUM(plane->blend_type);
	}

	ruby_value 
	rb_cPlane_SETblend_type(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->blend_type = NUM2INT(value);
		return value;
	}

	ruby_value 
	rb_cPlane_GETcolor(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return plane->color;
	}

	ruby_value 
	rb_cPlane_SETcolor(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->color = value;
		return value;
	}

	ruby_value 
	rb_cPlane_GETtone(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return plane->tone;
	}

	ruby_value 
	rb_cPlane_SETtone(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->tone = value;
		return value;
	}

	ruby_value 
	rb_cPlane_GETwave_amp(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane = (TPlane*)obj->object;
		return DBL2NUM(plane->wave_amp);
	}	

	ruby_value 
	rb_cPlane_SETwave_amp(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->wave_amp = (float)NUM2DBL(value);
		return value;
	}	

	ruby_value 
	rb_cPlane_GETwave_length(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return DBL2NUM(plane->wave_length);
	}	

	ruby_value 
	rb_cPlane_SETwave_length(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->wave_length = (float)NUM2DBL(value);
		return value;
	}	

	ruby_value 
	rb_cPlane_GETwave_speed(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return DBL2NUM(plane->wave_speed);
	}	

	ruby_value 
	rb_cPlane_SETwave_speed(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->wave_speed = (float)NUM2DBL(value);
		return value;
	}	

	ruby_value 
	rb_cPlane_GETwave_phase(ruby_value self) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		return DBL2NUM(plane->wave_phase);
	}	

	ruby_value 
	rb_cPlane_SETwave_phase(ruby_value self, ruby_value value) {
		TPlane* plane;
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		plane = (TPlane*)obj->object;
		if (obj->disposed) RAISE_DISPOSED_PLANE();
		plane->wave_phase = (float)NUM2DBL(value);
		return value;
	}	



	void
	Init_QGF_Graphics_Plane() {
		rb_cPlane = rb_define_class("Plane", rb_cObject);
		rb_define_alloc_func(rb_cPlane, rb_gc_cPlane_alloc);

		rb_define_method(rb_cPlane, "initialize", RUBY_METHOD_FUNC(rb_cPlane_initialize), -1);
		rb_define_method(rb_cPlane, "update", RUBY_METHOD_FUNC(rb_cPlane_update), 0);
		rb_define_method(rb_cPlane, "dispose", RUBY_METHOD_FUNC(rb_cPlane_dispose), 0);
		rb_define_method(rb_cPlane, "disposed?", RUBY_METHOD_FUNC(rb_cPlane_disposedQ), 0);
		rb_define_method(rb_cPlane, "flash", RUBY_METHOD_FUNC(rb_cPlane_flash), 2);

		rb_define_method(rb_cPlane, "z", RUBY_METHOD_FUNC(rb_cPlane_GETz), 0);
		rb_define_method(rb_cPlane, "z=", RUBY_METHOD_FUNC(rb_cPlane_SETz), 1);
		rb_define_method(rb_cPlane, "visible", RUBY_METHOD_FUNC(rb_cPlane_GETvisible), 0);
		rb_define_method(rb_cPlane, "visible=", RUBY_METHOD_FUNC(rb_cPlane_SETvisible), 1);
		rb_define_method(rb_cPlane, "bitmap", RUBY_METHOD_FUNC(rb_cPlane_GETbitmap), 0);
		rb_define_method(rb_cPlane, "bitmap=", RUBY_METHOD_FUNC(rb_cPlane_SETbitmap), 1);
		rb_define_method(rb_cPlane, "viewport", RUBY_METHOD_FUNC(rb_cPlane_GETviewport), 0);
		rb_define_method(rb_cPlane, "viewport=", RUBY_METHOD_FUNC(rb_cPlane_SETviewport), 1);
		rb_define_method(rb_cPlane, "ox", RUBY_METHOD_FUNC(rb_cPlane_GETox), 0);
		rb_define_method(rb_cPlane, "ox=", RUBY_METHOD_FUNC(rb_cPlane_SETox), 1);
		rb_define_method(rb_cPlane, "oy", RUBY_METHOD_FUNC(rb_cPlane_GEToy), 0);
		rb_define_method(rb_cPlane, "oy=", RUBY_METHOD_FUNC(rb_cPlane_SEToy), 1);
		rb_define_method(rb_cPlane, "zoom_x", RUBY_METHOD_FUNC(rb_cPlane_GETzoom_x), 0);
		rb_define_method(rb_cPlane, "zoom_x=", RUBY_METHOD_FUNC(rb_cPlane_SETzoom_x), 1);
		rb_define_method(rb_cPlane, "zoom_y", RUBY_METHOD_FUNC(rb_cPlane_GETzoom_y), 0);
		rb_define_method(rb_cPlane, "zoom_y=", RUBY_METHOD_FUNC(rb_cPlane_SETzoom_y), 1);
		rb_define_method(rb_cPlane, "opacity", RUBY_METHOD_FUNC(rb_cPlane_GETopacity), 0);
		rb_define_method(rb_cPlane, "opacity=", RUBY_METHOD_FUNC(rb_cPlane_SETopacity), 1);
		rb_define_method(rb_cPlane, "blend_type", RUBY_METHOD_FUNC(rb_cPlane_GETblend_type), 0);
		rb_define_method(rb_cPlane, "blend_type=", RUBY_METHOD_FUNC(rb_cPlane_SETblend_type), 1);
		rb_define_method(rb_cPlane, "color", RUBY_METHOD_FUNC(rb_cPlane_GETcolor), 0);
		rb_define_method(rb_cPlane, "color=", RUBY_METHOD_FUNC(rb_cPlane_SETcolor), 1);
		rb_define_method(rb_cPlane, "tone", RUBY_METHOD_FUNC(rb_cPlane_GETtone), 0);
		rb_define_method(rb_cPlane, "tone=", RUBY_METHOD_FUNC(rb_cPlane_SETtone), 1);

		rb_define_method(rb_cPlane, "wave_amp", RUBY_METHOD_FUNC(rb_cPlane_GETwave_amp), 0);
		rb_define_method(rb_cPlane, "wave_amp=", RUBY_METHOD_FUNC(rb_cPlane_SETwave_amp), 1);
		rb_define_method(rb_cPlane, "wave_length", RUBY_METHOD_FUNC(rb_cPlane_GETwave_length), 0);
		rb_define_method(rb_cPlane, "wave_length=", RUBY_METHOD_FUNC(rb_cPlane_SETwave_length), 1);
		rb_define_method(rb_cPlane, "wave_speed", RUBY_METHOD_FUNC(rb_cPlane_GETwave_speed), 0);
		rb_define_method(rb_cPlane, "wave_speed=", RUBY_METHOD_FUNC(rb_cPlane_SETwave_speed), 1);
		rb_define_method(rb_cPlane, "wave_phase", RUBY_METHOD_FUNC(rb_cPlane_GETwave_phase), 0);
		rb_define_method(rb_cPlane, "wave_phase=", RUBY_METHOD_FUNC(rb_cPlane_SETwave_phase), 1);

	}
