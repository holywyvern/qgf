#include "qgf/graphics/viewport.h"
#include "qgf/tools/array.h"
#include "qgf/ruby.h"
#include "qgf/tools/func.h"
#include "qgf/graphics.h"

	static ruby_value
	rb_gc_cViewport_alloc(ruby_value klass) {
		TViewport* viewport;
		TZObject* obj;
		obj      = ALLOC(TZObject);
		viewport = ALLOC(TViewport);
		viewport->rect = Qnil;
		viewport->ox = 0;
		viewport->oy = 0;
		viewport->objects.capa = 0;
		viewport->objects.size = 0;
		viewport->objects.values = NULL;
		obj->object = viewport;
		obj->on_draw = QGF_DRAW_FUNC(rb_cViewport_draw);
		obj->visible = TRUE;
		obj->z = 0;
		obj->disposed = FALSE;
		return Data_Wrap_Struct(klass, rb_gc_cViewport_mark, rb_gc_cViewport_free, obj);
	}

	static void 
	rb_gc_cViewport_free(TZObject* obj) {
		TViewport* viewport = (TViewport*) obj->object;
		qgf_array_free(&(viewport->objects));
		if (viewport->flash_texture > 0) glDeleteTextures(1, &(viewport->flash_texture));
		free(viewport);
		free(obj);
	}

	static void 
	rb_gc_cViewport_mark(TZObject* obj) {
		TViewport* viewport = (TViewport*)obj->object;
		rb_gc_mark(viewport->color);
		rb_gc_mark(viewport->flash_color);
		rb_gc_mark(viewport->tone);
		rb_gc_mark(viewport->rect);
		qgf_array_gc_mark(&(viewport->objects));
	}

	static ruby_value 
	rb_cViewport_initialize(int size, ruby_value* args, ruby_value self) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		viewport = (TViewport*) obj->object;
		if (size == 0) {
			viewport->rect = rb_rect_new();
		} else if (size == 1) {
			viewport->rect = rb_rect_new(args[0]);
		} else if (size == 4) {
			viewport->rect = rb_rect_new((float)NUM2DBL(args[0]), (float)NUM2DBL(args[1]), (float)NUM2DBL(args[2]), (float)NUM2DBL(args[3]));
		} else {
			RAISE_ARGN_S(size, "0, 1 or 4");
		}
		viewport->color = rb_color_new(0, 0, 0, 0);
		viewport->flash_color = rb_color_new(0, 0, 0, 0);
		viewport->flash_duration = 0;
		viewport->tone  = rb_tone_new();
		qgf_graphics_add_object(self);
		return self;
	}

	static ruby_value 
	rb_cViewport_dispose(ruby_value self) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		obj->disposed = TRUE;
		qgf_graphics_remove_object(self);
		return Qnil;
	}

	static ruby_value 
	rb_cViewport_disposedQ(ruby_value self) {
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		return obj->disposed ? Qtrue : Qfalse;
	}

	static ruby_value 
	rb_cViewport_flash(ruby_value self, ruby_value color, ruby_value duration) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		viewport->flash_color = color;
		viewport->flash_duration = NUM2INT(duration);
		return Qnil;
	}

	static ruby_value 
	rb_cViewport_update(ruby_value self) {
		TZObject* obj;
		TViewport* viewport;
		TColor* color;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		if (viewport->flash_duration > 0 && RTEST(viewport->flash_color)) {
			TColor* o;
			Data_Get_Struct(viewport->color, TColor, o);
			Data_Get_Struct(viewport->flash_color, TColor, color);
			color->red   = (color->red   * (viewport->flash_duration - 1) + o->red)   / viewport->flash_duration;
			color->green = (color->green * (viewport->flash_duration - 1) + o->green) / viewport->flash_duration;
			color->blue  = (color->blue  * (viewport->flash_duration - 1) + o->blue)  / viewport->flash_duration;
			color->alpha = (color->alpha * (viewport->flash_duration - 1) + o->alpha) / viewport->flash_duration;
			viewport->flash_duration = viewport->flash_duration - 1;
		}
		return Qnil;
	}

	static ruby_value 
	rb_cViewport_GETrect(ruby_value self) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		return viewport->rect;
	}

	static ruby_value 
	rb_cViewport_SETrect(ruby_value self, ruby_value value) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		viewport->rect = value;
		return value;
	}

	static ruby_value 
	rb_cViewport_GETvisible(ruby_value self) {
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		return obj->visible ? Qtrue : Qfalse;
	}

	static ruby_value 
	rb_cViewport_SETvisible(ruby_value self, ruby_value value) {
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		obj->visible = RTEST(value);
		return value;
	}

	static ruby_value 
	rb_cViewport_GETz(ruby_value self) {
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		return DBL2NUM(obj->z);
	}

	static ruby_value 
	rb_cViewport_SETz(ruby_value self, ruby_value value) {
		TZObject* obj;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		obj->z = (float)NUM2DBL(value);
		return value;
	}

	static ruby_value 
	rb_cViewport_GETox(ruby_value self) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		return DBL2NUM(viewport->ox);
	}

	static ruby_value 
	rb_cViewport_SETox(ruby_value self, ruby_value value) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		viewport->ox = (float)NUM2DBL(value);
		return value;
	}

	static ruby_value 
	rb_cViewport_GEToy(ruby_value self) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		return DBL2NUM(viewport->oy);
	}

	static ruby_value 
	rb_cViewport_SEToy(ruby_value self, ruby_value value) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		viewport->oy = (float)NUM2DBL(value);
		return value;
	}

	static ruby_value 
	rb_cViewport_GETcolor(ruby_value self) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		return viewport->color;
	}

	static ruby_value 
	rb_cViewport_SETcolor(ruby_value self, ruby_value value) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		viewport->color = value;
		return value;
	}

	static ruby_value 
	rb_cViewport_GETtone(ruby_value self) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		return viewport->tone;
	}

	static ruby_value 
	rb_cViewport_SETtone(ruby_value self, ruby_value value) {
		TZObject* obj;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, obj);
		if (obj->disposed) RAISE_DISPOSED_VIEWPORT();
		viewport = (TViewport*) obj->object;
		viewport->tone = value;
		return value;
	}

	void
	Init_QGF_Graphics_Viewport() {
		rb_cViewport = rb_define_class("Viewport", rb_cObject);
		rb_define_alloc_func(rb_cViewport, rb_gc_cViewport_alloc);

		rb_define_method(rb_cViewport, "dispose", RUBY_METHOD_FUNC(rb_cViewport_dispose), 0);
		rb_define_method(rb_cViewport, "initialize", RUBY_METHOD_FUNC(rb_cViewport_initialize), -1);
		rb_define_method(rb_cViewport, "flash", RUBY_METHOD_FUNC(rb_cViewport_flash), 2);
		rb_define_method(rb_cViewport, "update", RUBY_METHOD_FUNC(rb_cViewport_update), 0);

		rb_define_method(rb_cViewport, "z", RUBY_METHOD_FUNC(rb_cViewport_GETz), 0);
		rb_define_method(rb_cViewport, "z=", RUBY_METHOD_FUNC(rb_cViewport_SETz), 1);
		rb_define_method(rb_cViewport, "visible", RUBY_METHOD_FUNC(rb_cViewport_GETvisible), 0);
		rb_define_method(rb_cViewport, "visible=", RUBY_METHOD_FUNC(rb_cViewport_SETvisible), 1);
		rb_define_method(rb_cViewport, "ox", RUBY_METHOD_FUNC(rb_cViewport_GETox), 0);
		rb_define_method(rb_cViewport, "ox=", RUBY_METHOD_FUNC(rb_cViewport_SETox), 1);
		rb_define_method(rb_cViewport, "oy", RUBY_METHOD_FUNC(rb_cViewport_GEToy), 0);
		rb_define_method(rb_cViewport, "oy=", RUBY_METHOD_FUNC(rb_cViewport_SEToy), 1);
		rb_define_method(rb_cViewport, "rect", RUBY_METHOD_FUNC(rb_cViewport_GETrect), 0);
		rb_define_method(rb_cViewport, "rect=", RUBY_METHOD_FUNC(rb_cViewport_SETrect), 1);
		rb_define_method(rb_cViewport, "color", RUBY_METHOD_FUNC(rb_cViewport_GETcolor), 0);
		rb_define_method(rb_cViewport, "color=", RUBY_METHOD_FUNC(rb_cViewport_SETcolor), 1);
		rb_define_method(rb_cViewport, "tone", RUBY_METHOD_FUNC(rb_cViewport_GETtone), 0);
		rb_define_method(rb_cViewport, "tone=", RUBY_METHOD_FUNC(rb_cViewport_SETtone), 1);
	}

	ruby_value
	rb_viewport_new(float x, float y, float width, float height) {
		ruby_value args[4];
		args[0] = DBL2NUM(x);
		args[1] = DBL2NUM(y);
		args[2] = DBL2NUM(width);
		args[3] = DBL2NUM(height);
		return rb_class_new_instance(4, args, rb_cViewport);
	}

	ruby_value 
	rb_viewport_new(ruby_value rect) {
		return rb_class_new_instance(1, &rect, rb_cViewport);
	}

	ruby_value
	rb_viewport_new() {
		return rb_class_new_instance(0, NULL, rb_cViewport);
	}

	void
	rb_viewport_add_object(ruby_value self, ruby_value obj) {
		TZObject* object;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, object);
		viewport = (TViewport*)object->object;
		qgf_array_add(&(viewport->objects), obj);
	}

	void
	rb_viewport_remove_object(ruby_value self, ruby_value obj) {
		TZObject* object;
		TViewport* viewport;
		Data_Get_Struct(self, TZObject, object);
		viewport = (TViewport*)object->object;
		qgf_array_remove(&(viewport->objects), obj);
	}
