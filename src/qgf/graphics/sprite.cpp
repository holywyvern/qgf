#include "qgf/graphics/sprite.h"
#include "qgf/ruby.h"
#include "qgf/graphics.h"
#include "qgf/tools/func.h"
#include "qgf/graphics/viewport.h"

	static ruby_value
	rb_gc_cSprite_alloc(ruby_value klass) {
		TSprite* sprite;
		TZObject* zobj;
		sprite = ALLOC(TSprite);
		zobj = ALLOC(TZObject);
		zobj->object = sprite;
		zobj->z = 0;
		zobj->on_draw = QGF_DRAW_FUNC(rb_cSprite_draw);
		zobj->disposed = FALSE;
		sprite->bitmap = Qnil;
		sprite->color = Qnil;
		sprite->flash_color = Qnil;
		sprite->flash_duration = 0;
		sprite->mirror_x = 0;
		sprite->mirror_y = 0;
		sprite->opacity = 255.0f;
		sprite->src_rect = Qnil;
		sprite->tone = Qnil;
		sprite->viewport = Qnil;
		sprite->zoom_x = 1;
		sprite->zoom_y = 1;

		sprite->wave_amp    = 0;
		sprite->wave_length = 0;
		sprite->wave_speed  = 360;
		sprite->wave_phase  = 0;

		return Data_Wrap_Struct(klass, rb_gc_cSprite_mark, rb_gc_cSprite_free, zobj);
	}

	static void 
	rb_gc_cSprite_free(TZObject* obj) {
		free(obj->object);
		free(obj);
	}

	static void 
	rb_gc_cSprite_mark(TZObject* obj) {
		TSprite* spr = (TSprite*)obj->object;
		rb_gc_mark(spr->bitmap);
		rb_gc_mark(spr->src_rect);
		rb_gc_mark(spr->viewport);
		rb_gc_mark(spr->color);
		rb_gc_mark(spr->tone);
		rb_gc_mark(spr->flash_color);
	}

	static ruby_value
	rb_cSprite_initialize(int size, ruby_value* args, ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		sprite = (TSprite*)zobj->object;
		if (size == 0) {
			qgf_graphics_add_object(self);
		} else if (size == 1) {
			rb_viewport_add_object(args[0], self);
			sprite->viewport = args[0];
		} else {
			RAISE_ARGN_S(size, "1 or 2");
		}
		sprite->angle = 0;
		sprite->bitmap = Qnil;
		sprite->blend_type = 0;
		sprite->bush_depth = 0;
		sprite->bush_opacity = 255;
		sprite->color = rb_color_new(0, 0, 0, 0);
		sprite->flash_color = rb_color_new(0, 0, 0, 0);
		sprite->flash_duration = 0;
		sprite->mirror_x = 0;
		sprite->mirror_y = 0;
		sprite->opacity  = 255;
		sprite->ox = 0;
		sprite->oy = 0;
		sprite->src_rect = rb_rect_new();
		sprite->tone = rb_tone_new();
		sprite->wave_amp = 0;
		sprite->wave_length = 180.0f;
		sprite->wave_phase = 0;
		sprite->wave_speed = 360.0f;
		sprite->x = 0;
		sprite->y = 0;
		sprite->zoom_x = 1.0f;
		sprite->zoom_y = 1.0f;
		zobj->z = 0;
		zobj->disposed = FALSE;
		zobj->visible = TRUE;
		return self;
	}	

	static ruby_value 
	rb_cSprite_dispose(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		if (RTEST(sprite->viewport)) {
			rb_viewport_remove_object(sprite->viewport, self);
		} else {
			qgf_graphics_remove_object(self);
		}
		zobj->disposed = TRUE;
		return Qnil;
	}	

	static ruby_value 
	rb_cSprite_disposedQ(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		sprite = (TSprite*)zobj->object;
		return zobj->disposed ? Qtrue : Qfalse;
	}	

	static ruby_value 
	rb_cSprite_flash(ruby_value self, ruby_value color, ruby_value duration) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->flash_color = color;
		sprite->flash_duration = NUM2INT(duration);
		return Qnil;
	}	

	static ruby_value 
	rb_cSprite_update(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		if (sprite->flash_duration > 0 && RTEST(sprite->flash_color)) {
			TColor* c;
			TColor* o;
			Data_Get_Struct(sprite->flash_color, TColor, c);
			Data_Get_Struct(sprite->color, TColor, o);
			c->red   = (c->red   * (sprite->flash_duration - 1) + o->red)   / sprite->flash_duration;
			c->green = (c->green * (sprite->flash_duration - 1) + o->green) / sprite->flash_duration;
			c->blue  = (c->blue  * (sprite->flash_duration - 1) + o->blue ) / sprite->flash_duration;
			c->alpha = (c->alpha * (sprite->flash_duration - 1) + o->alpha) / sprite->flash_duration;
			sprite->flash_duration = sprite->flash_duration - 1;
		}
		sprite->wave_phase = (sprite->wave_phase + sprite->wave_speed / 360.0f);
		while (sprite->wave_phase >= 360.0f) {
			sprite->wave_phase = sprite->wave_phase - 360.0f;
		}
		return Qnil;
	}	

	static ruby_value
	rb_cSprite_width(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return rb_funcall(sprite->src_rect, rb_intern("width"), 0);
	}	

	static ruby_value
	rb_cSprite_height(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return rb_funcall(sprite->src_rect, rb_intern("height"), 0);
	}	

	static ruby_value 
	rb_cSprite_GETbitmap(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return sprite->bitmap;
	}	

	static ruby_value
	rb_cSprite_SETbitmap(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->bitmap = value;
		if (RTEST(value)) {
			sprite->src_rect = rb_funcall(value, rb_intern("rect"), 0);
		}
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETsrc_rect(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return sprite->src_rect;
	}	

	static ruby_value
	rb_cSprite_SETsrc_rect(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->src_rect = value;
		return value;
	}	

	static ruby_value
	rb_cSprite_GETviewport(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return sprite->viewport;
	}	

	static ruby_value 
	rb_cSprite_SETviewport(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		TZObject* zv;
		TViewport* viewport;
		if (RTEST(sprite->viewport)) {
			rb_viewport_remove_object(sprite->viewport, self);
		} else {
			qgf_graphics_remove_object(self);
		}
		if (RTEST(value)) {
			rb_viewport_add_object(value, self);
		} else {
			qgf_graphics_add_object(self);
		}
		sprite->viewport = value;
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETvisible(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return zobj->visible ? Qtrue : Qfalse;
	}	

	static ruby_value 
	rb_cSprite_SETvisible(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		zobj->visible = RTEST(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETx(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->x);
	}	

	static ruby_value 
	rb_cSprite_SETx(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->x = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETy(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->y);
	}	

	static ruby_value 
	rb_cSprite_SETy(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->y = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETz(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(zobj->z);
	}	

	static ruby_value 
	rb_cSprite_SETz(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		zobj->z = (float)NUM2DBL(value);
		if (RTEST(sprite->viewport)) {
			TZObject* zview;
			TViewport* viewport;
			Data_Get_Struct(sprite->viewport, TZObject, zview);
			viewport = (TViewport*)zview->object;
			viewport->objects.sorted = FALSE;
		} else {
			qgf_graphics_notify_zObject();
		}
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETox(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->ox);
	}	

	static ruby_value
	rb_cSprite_SETox(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->ox = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GEToy(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->oy);
	}	

	static ruby_value
	rb_cSprite_SEToy(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->oy = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETzoom_x(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->zoom_x);
	}	

	static ruby_value 
	rb_cSprite_SETzoom_x(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->zoom_x = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETzoom_y(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->zoom_y);
	}	

	static ruby_value 
	rb_cSprite_SETzoom_y(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->zoom_y = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETangle(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->angle);
	}	

	static ruby_value 
	rb_cSprite_SETangle(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->angle = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETwave_amp(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->wave_amp);
	}	

	static ruby_value
	rb_cSprite_SETwave_amp(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->wave_amp = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETwave_length(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->wave_length);
	}	

	static ruby_value
	rb_cSprite_SETwave_length(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->wave_length = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETwave_speed(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->wave_speed);
	}	

	static ruby_value 
	rb_cSprite_SETwave_speed(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->wave_speed = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETwave_phase(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->wave_phase);
	}	

	static ruby_value 
	rb_cSprite_SETwave_phase(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->wave_phase = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETbush_depth(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return INT2NUM(sprite->bush_depth);
	}	

	static ruby_value 
	rb_cSprite_SETbush_depth(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->bush_depth = NUM2INT(value);
		return value;
	}	

	static ruby_value
	rb_cSprite_GETbush_opacity(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return INT2NUM(sprite->bush_opacity);
	}	

	static ruby_value 
	rb_cSprite_SETbush_opacity(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->bush_opacity = NUM2INT(value);
		return value;
	}	

	static ruby_value
	rb_cSprite_GETopacity(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return DBL2NUM(sprite->opacity);
	}	

	static ruby_value
	rb_cSprite_SETopacity(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->opacity = (float)NUM2DBL(value);
		return value;
	}	

	static ruby_value
	rb_cSprite_GETblend_type(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return INT2NUM(sprite->blend_type);
	}	

	static ruby_value
	rb_cSprite_SETblend_type(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->blend_type = NUM2INT(value);
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETcolor(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return sprite->color;
	}	

	static ruby_value 
	rb_cSprite_SETcolor(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->color = value;
		return value;
	}	

	static ruby_value 
	rb_cSprite_GETtone(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return sprite->tone;
	}	

	static ruby_value 
	rb_cSprite_SETtone(ruby_value self, ruby_value value) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		sprite->tone = value;
		return value;
	}	

	static ruby_value 
	rb_cSprite_origin_point(ruby_value self) {
		TSprite* sprite;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		sprite = (TSprite*)zobj->object;
		return Qnil;
	}	

	static ruby_value 
	rb_cSprite_collideQ(ruby_value self, ruby_value sprite) {
		TSprite* spr;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		spr = (TSprite*)zobj->object;
		return Qnil;
	}	

	static ruby_value 
	rb_cSprite_GETmirror_x(ruby_value self) {
		TSprite* spr;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		spr = (TSprite*)zobj->object;
		return spr->mirror_x ? Qtrue : Qfalse;
	}

	static ruby_value 
	rb_cSprite_SETmirror_x(ruby_value self, ruby_value value) {
		TSprite* spr;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		spr = (TSprite*)zobj->object;
		spr->mirror_x = RTEST(value);
		return value;
	}

	static ruby_value 
	rb_cSprite_GETmirror_y(ruby_value self) {
		TSprite* spr;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		spr = (TSprite*)zobj->object;
		return spr->mirror_y ? Qtrue : Qfalse;
	}

	static ruby_value 
	rb_cSprite_SETmirror_y(ruby_value self, ruby_value value) {
		TSprite* spr;
		TZObject* zobj;
		Data_Get_Struct(self, TZObject, zobj);
		if (zobj->disposed) RAISE_DISPOSED_SPRITE();
		spr = (TSprite*)zobj->object;
		spr->mirror_y = RTEST(value);
		return value;
	}

	void
	Init_QGF_Graphics_Sprite() {
		rb_cSprite = rb_define_class("Sprite", rb_cObject);
		rb_define_alloc_func(rb_cSprite, rb_gc_cSprite_alloc);

		rb_define_method(rb_cSprite, "initialize", RUBY_METHOD_FUNC(rb_cSprite_initialize), -1);
		rb_define_method(rb_cSprite, "dispose", RUBY_METHOD_FUNC(rb_cSprite_dispose), 0);
		rb_define_method(rb_cSprite, "disposed?", RUBY_METHOD_FUNC(rb_cSprite_disposedQ), 0);
		rb_define_method(rb_cSprite, "flash", RUBY_METHOD_FUNC(rb_cSprite_flash), 2);
		rb_define_method(rb_cSprite, "update", RUBY_METHOD_FUNC(rb_cSprite_update), 0);
		rb_define_method(rb_cSprite, "width", RUBY_METHOD_FUNC(rb_cSprite_width), 0);
		rb_define_method(rb_cSprite, "height", RUBY_METHOD_FUNC(rb_cSprite_height), 0);

		rb_define_method(rb_cSprite, "bitmap", RUBY_METHOD_FUNC(rb_cSprite_GETbitmap), 0);
		rb_define_method(rb_cSprite, "bitmap=", RUBY_METHOD_FUNC(rb_cSprite_SETbitmap), 1);
		rb_define_method(rb_cSprite, "x", RUBY_METHOD_FUNC(rb_cSprite_GETx), 0);
		rb_define_method(rb_cSprite, "x=", RUBY_METHOD_FUNC(rb_cSprite_SETx), 1);
		rb_define_method(rb_cSprite, "y", RUBY_METHOD_FUNC(rb_cSprite_GETy), 0);
		rb_define_method(rb_cSprite, "y=", RUBY_METHOD_FUNC(rb_cSprite_SETy), 1);
		rb_define_method(rb_cSprite, "viewport", RUBY_METHOD_FUNC(rb_cSprite_GETviewport), 0);
		rb_define_method(rb_cSprite, "viewport=", RUBY_METHOD_FUNC(rb_cSprite_SETviewport), 1);
		rb_define_method(rb_cSprite, "visible", RUBY_METHOD_FUNC(rb_cSprite_GETvisible), 0);
		rb_define_method(rb_cSprite, "visible=", RUBY_METHOD_FUNC(rb_cSprite_SETvisible), 1);
		rb_define_method(rb_cSprite, "ox", RUBY_METHOD_FUNC(rb_cSprite_GETox), 0);
		rb_define_method(rb_cSprite, "ox=", RUBY_METHOD_FUNC(rb_cSprite_SETox), 1);
		rb_define_method(rb_cSprite, "oy", RUBY_METHOD_FUNC(rb_cSprite_GEToy), 0);
		rb_define_method(rb_cSprite, "oy=", RUBY_METHOD_FUNC(rb_cSprite_SEToy), 1);
		rb_define_method(rb_cSprite, "zoom_x", RUBY_METHOD_FUNC(rb_cSprite_GETzoom_x), 0);
		rb_define_method(rb_cSprite, "zoom_x=", RUBY_METHOD_FUNC(rb_cSprite_SETzoom_x), 1);
		rb_define_method(rb_cSprite, "zoom_y", RUBY_METHOD_FUNC(rb_cSprite_GETzoom_y), 0);
		rb_define_method(rb_cSprite, "zoom_y=", RUBY_METHOD_FUNC(rb_cSprite_SETzoom_y), 1);
		rb_define_method(rb_cSprite, "angle", RUBY_METHOD_FUNC(rb_cSprite_GETangle), 0);
		rb_define_method(rb_cSprite, "angle=", RUBY_METHOD_FUNC(rb_cSprite_SETangle), 1);
		rb_define_method(rb_cSprite, "opacity", RUBY_METHOD_FUNC(rb_cSprite_GETopacity), 0);
		rb_define_method(rb_cSprite, "opacity=", RUBY_METHOD_FUNC(rb_cSprite_SETopacity), 1);
		rb_define_method(rb_cSprite, "z", RUBY_METHOD_FUNC(rb_cSprite_GETz), 0);
		rb_define_method(rb_cSprite, "z=", RUBY_METHOD_FUNC(rb_cSprite_SETz), 1);
		rb_define_method(rb_cSprite, "src_rect", RUBY_METHOD_FUNC(rb_cSprite_GETsrc_rect), 0);
		rb_define_method(rb_cSprite, "src_rect=", RUBY_METHOD_FUNC(rb_cSprite_SETsrc_rect), 1);
		rb_define_method(rb_cSprite, "bush_depth", RUBY_METHOD_FUNC(rb_cSprite_GETbush_depth), 0);
		rb_define_method(rb_cSprite, "bush_depth=", RUBY_METHOD_FUNC(rb_cSprite_SETbush_depth), 1);
		rb_define_method(rb_cSprite, "bush_opacity", RUBY_METHOD_FUNC(rb_cSprite_GETbush_opacity), 0);
		rb_define_method(rb_cSprite, "bush_opacity=", RUBY_METHOD_FUNC(rb_cSprite_SETbush_opacity), 1);
		rb_define_method(rb_cSprite, "wave_amp", RUBY_METHOD_FUNC(rb_cSprite_GETwave_amp), 0);
		rb_define_method(rb_cSprite, "wave_amp=", RUBY_METHOD_FUNC(rb_cSprite_SETwave_amp), 1);
		rb_define_method(rb_cSprite, "wave_length", RUBY_METHOD_FUNC(rb_cSprite_GETwave_length), 0);
		rb_define_method(rb_cSprite, "wave_length=", RUBY_METHOD_FUNC(rb_cSprite_SETwave_length), 1);
		rb_define_method(rb_cSprite, "wave_speed", RUBY_METHOD_FUNC(rb_cSprite_GETwave_speed), 0);
		rb_define_method(rb_cSprite, "wave_speed=", RUBY_METHOD_FUNC(rb_cSprite_SETwave_speed), 1);
		rb_define_method(rb_cSprite, "wave_phase", RUBY_METHOD_FUNC(rb_cSprite_GETwave_phase), 0);
		rb_define_method(rb_cSprite, "wave_phase=", RUBY_METHOD_FUNC(rb_cSprite_SETwave_phase), 1);
		rb_define_method(rb_cSprite, "color", RUBY_METHOD_FUNC(rb_cSprite_GETcolor), 0);
		rb_define_method(rb_cSprite, "color=", RUBY_METHOD_FUNC(rb_cSprite_SETcolor), 1);
		rb_define_method(rb_cSprite, "tone", RUBY_METHOD_FUNC(rb_cSprite_GETtone), 0);
		rb_define_method(rb_cSprite, "tone=", RUBY_METHOD_FUNC(rb_cSprite_SETtone), 1);
		rb_define_method(rb_cSprite, "mirror_x", RUBY_METHOD_FUNC(rb_cSprite_GETmirror_x), 0);
		rb_define_method(rb_cSprite, "mirror_x=", RUBY_METHOD_FUNC(rb_cSprite_SETmirror_x), 1);
		rb_define_method(rb_cSprite, "mirror_y", RUBY_METHOD_FUNC(rb_cSprite_GETmirror_y), 0);
		rb_define_method(rb_cSprite, "mirror_y=", RUBY_METHOD_FUNC(rb_cSprite_SETmirror_y), 1);
		rb_define_alias(rb_cSprite, "mirror", "mirror_x");
		rb_define_alias(rb_cSprite, "mirror=", "mirror_x=");
	}
