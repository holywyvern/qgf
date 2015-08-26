#include "qgf/graphics/bitmap.h"
#include "qgf/graphics/color.h"
#include "qgf/tools/func.h"
#include "qgf/tools/file_finder.h"
#include "qgf/tools/text.h"
#include "qgf/ruby.h"
#include "SOIL.h"

typedef struct {
	int width;
	int height;
	uint32_t* pixels;
} qgf_simple_bitmap;

	qgf_simple_bitmap*
	qgf_simple_bitmap_resample(qgf_simple_bitmap* org, int dst_w, int dst_h, TRect* src_rect) {
		float zoom_x = (float)(dst_w) / src_rect->width;
		float zoom_y = (float)(dst_h) / src_rect->height;
		qgf_simple_bitmap* dst = (qgf_simple_bitmap*)malloc(sizeof(qgf_simple_bitmap));
		if (dst_w < 1) dst_w = 1;
		if (dst_h < 1) dst_h = 1;
		dst->width = dst_w;
		dst->height = dst_h;
		dst->pixels = (uint32_t*)malloc(sizeof(uint32_t) * dst_w * dst_h);
		for (int i = 0; i < dst_w * dst_h; ++i) {
			dst->pixels[i] = 0;
		}
		uint8_t* src_pixels = (uint8_t*)&org->pixels[0];
		uint8_t* dst_pixels = (uint8_t*)(&dst->pixels[0]);

		int row = org->width * 4;

		for (int yy = 0; yy < dst_h; yy++) {
			int nearest_matchy = (int)(src_rect->y + (int)(yy / zoom_y)) * row;
			for (int xx = 0; xx < dst_w; xx++) {
				int nearest_match = nearest_matchy + (int)(src_rect->x + (int)(xx / zoom_x)) * 4;
				dst_pixels[0] = src_pixels[nearest_match];
				dst_pixels[1] = src_pixels[nearest_match + 1];
				dst_pixels[2] = src_pixels[nearest_match + 2];
				dst_pixels[3] = src_pixels[nearest_match + 3];
				dst_pixels += 4;
			}
		}

		return dst;
	}

	void 
	qgf_rect_adjust(TRect* rect, int width, int height) {
		if (rect->x < 0) {
			rect->width += rect->x;
			rect->x = 0;
		}
		if (rect->y < 0) {
			rect->height += rect->y;
			rect->y = 0;
		}
		if (rect->x < width && rect->y < height) {
			if (width < rect->x + rect->width) rect->width    = width - rect->x;
			if (height < rect->y + rect->height) rect->height = height - rect->y;
		}
	}

	int
	qgf_rect_isOutOfBounds(TRect* rect, int width, int height) {
		if (rect->width <= 0 || rect->height <= 0) return TRUE;
		if (rect->x >= width || rect->y >= height) return TRUE;
		if (rect->x + rect->width <= 0 || rect->y + rect->height <= 0) return TRUE;
		return FALSE;
	}

	void
	qgf_simple_bitmap_blit(qgf_simple_bitmap* dst, int x, int y, qgf_simple_bitmap* src, TRect* rect, int opacity) {
		if (src->width == 0 || src->height == 0 || dst->width == 0 || dst->height == 0) return;
		if (x >= dst->width || y >= dst->height) return;

		if (x < 0) {
			rect->x -= x;
			x = 0;
		}
		if (y < 0) {
			rect->y -= y;
			y = 0;
		}
		qgf_rect_adjust(rect, src->width, src->height);
		if (qgf_rect_isOutOfBounds(rect, src->width, src->height)) return;
		int src_width = (int)rect->width;
		int src_height = (int)rect->height;
		if (x + src_width > dst->width) src_width = dst->width - x;
		if (y + src_height > dst->height) src_height = dst->height - y;
		if (src_width <= 0 || src_height <= 0) return;

		int src_row = src->width * 4;
		int dst_row = dst->width * 4;
		int rect_x = (int)rect->x;
		int rect_y = (int)rect->y;
		const uint8_t* src_pixels = ((uint8_t*)(&src->pixels[0])) + (rect_x + rect_y * src->width) * 4;
		uint8_t* dst_pixels = ((uint8_t*)(&dst->pixels[0])) + (x + y * dst->width) * 4;
		if (opacity > 255) opacity = 255;
		if (opacity > 0) {
			for (int i = 0; i < src_height; ++i) {
				for (int j = 0; j < src_width; ++j) {
					const uint8_t* src = src_pixels + j * 4;
					uint8_t* dst = dst_pixels + j * 4;
					uint8_t srca = (uint8_t)(src[3] * opacity / 255);
					dst[0] = (dst[0] * (255 - srca) + src[0] * srca) / 255;
					dst[1] = (dst[1] * (255 - srca) + src[1] * srca) / 255;
					dst[2] = (dst[2] * (255 - srca) + src[2] * srca) / 255;
					dst[3] = dst[3] * (255 - srca) / 255 + srca;
				}
				src_pixels += src_row;
				dst_pixels += dst_row;
			}
		}
	}

	void
	qgf_simple_bitmap_blendOverBitmap(qgf_simple_bitmap* fnt_bmp, TBitmap* dst_bmp,
										int x, int y, int width, int height, int align) {
		qgf_simple_bitmap* resampled = NULL;
		qgf_simple_bitmap* src_bmp;
		src_bmp = fnt_bmp;
		if (fnt_bmp->width > width) {
			int stretch = (int)(fnt_bmp->width * 0.4);
			if (width >= stretch) {
				stretch = width;
			} else {
				stretch = fnt_bmp->width;
			}
			TRect resample_rect;
			resample_rect.x = 0;
			resample_rect.y = 0;
			resample_rect.width  = fnt_bmp->width;
			resample_rect.height = fnt_bmp->height;
			resampled = qgf_simple_bitmap_resample(fnt_bmp, stretch, fnt_bmp->height, &resample_rect);	
			src_bmp = resampled;
		}
		if (height > src_bmp->height) y += ((height - src_bmp->height) / 2);
		if (width > src_bmp->width) {
			if (align == 1) {
				x += (width - src_bmp->width) / 2;
			} else if (align == 2) {
				x += width - src_bmp->width;
			}
		}
		TRect src_rect;
		src_rect.x = src_rect.y = 0;
		src_rect.width = width;
		src_rect.height = height;
		qgf_simple_bitmap org_bmp;
		org_bmp.width = dst_bmp->width;
		org_bmp.height = dst_bmp->height;
		org_bmp.pixels = dst_bmp->pixels;
		qgf_simple_bitmap_blit(&org_bmp, x, y, src_bmp, &src_rect, 255);
		if (resampled) {
			free(resampled->pixels);
			free(resampled);
		}
	}

	void
	qgf_simple_bitmap_applyVerticalGradientMask(qgf_simple_bitmap* bmp, TColor* org_c, TColor* c1, TColor* c2) {
		int val = bmp->height;
		uint32_t* dst_pixels = bmp->pixels;
		uint8_t r = (uint8_t)c1->red;
		uint8_t g = (uint8_t)c1->green;
		uint8_t b = (uint8_t)c1->blue;
		for (int y = 0; y < bmp->height; y++) {
			r = (uint8_t)((r * (val - 1) + c2->red) / val);
			g = (uint8_t)((g * (val - 1) + c2->green) / val);
			b = (uint8_t)((b  * (val - 1) + c2->blue)  / val);
			for (int x = 0; x < bmp->width; x++) {
				long pos = x + y * bmp->width;
				uint8_t a = (dst_pixels[pos] >> 24) & 0xFF;
				if (a) {
					dst_pixels[pos] = qgf_color_to_rgba(r, g, b, a);
				}
			}
			val--;
		}
	}

	void
	qgf_simple_bitmap_applyHorizontalGradientMask(qgf_simple_bitmap* bmp, TColor* org_c, TColor* c1, TColor* c2) {
		int val = bmp->width;
		uint32_t* dst_pixels = bmp->pixels;
		uint8_t r = (uint8_t)c1->red;
		uint8_t g = (uint8_t)c1->green;
		uint8_t b = (uint8_t)c1->blue;
		for (int x = 0; x < bmp->width; x++) {
			r = (uint8_t)((r * (val - 1) + c2->red) / val);
			g = (uint8_t)((g * (val - 1) + c2->green) / val);
			b = (uint8_t)((b  * (val - 1) + c2->blue)  / val);
			for (int y = 0; y < bmp->height; y++) {
				long pos = x + y * bmp->width;
				uint8_t a = (dst_pixels[pos] >> 24) & 0xFF;
				if (a) {
					dst_pixels[pos] = qgf_color_to_rgba(r, g, b, a);
				}
			}
			val--;
		}
	}

	void
	qgf_simple_bitmap_applyGradientMask(qgf_simple_bitmap* bmp, TColor* org_c, TColor* c1, TColor* c2, int vertical) {
		if (vertical) {
			qgf_simple_bitmap_applyVerticalGradientMask(bmp, org_c, c1, c2);
		} else {
			qgf_simple_bitmap_applyHorizontalGradientMask(bmp, org_c, c1, c2);
		}
	}

	ruby_value
	rb_gc_cBitmap_alloc(ruby_value klass) {
		TBitmap* bmp;
		bmp = ALLOC(TBitmap);
		bmp->font = Qnil;
		bmp->width = 0;
		bmp->height = 0;
		bmp->flags = 0;
		bmp->pixels = NULL;
		bmp->texture = 0;
		bmp->flash_texture = 0;
		return Data_Wrap_Struct(klass, rb_gc_cBitmap_mark, rb_gc_cBitmap_free, bmp);
	}

	void 
	rb_gc_cBitmap_free(TBitmap* bitmap) {
		if (bitmap->texture > 0) glDeleteTextures(1, &(bitmap->texture));
		if (bitmap->flash_texture > 0) glDeleteTextures(1, &(bitmap->flash_texture));
		free(bitmap);
	}

	void 
	rb_gc_cBitmap_mark(TBitmap* bitmap) {
		rb_gc_mark(bitmap->font);
	}

	ruby_value 
	rb_cBitmap_createByFilename(ruby_value self, char* filename) {
		TBitmap* bmp;
		unsigned char* load_pixels;
		int width;
		int height;
		int channels;
		Data_Get_Struct(self, TBitmap, bmp);
		load_pixels = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_RGBA);
		bmp->width  = width;
		bmp->height = height;
		bmp->pixels = ALLOC_N(uint32_t, width * height);
		memcpy(&(bmp->pixels[0]), load_pixels, width * height * 4);
		SOIL_free_image_data(load_pixels);
		return self;
	}

	ruby_value 
	rb_cBitmap_createBySize(ruby_value self, uint32_t width, uint32_t height) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		bmp->width  = width;
		bmp->height = height;
		bmp->pixels = ALLOC_N(uint32_t, width * height);
		for (int i = 0; i < width * height; ++i) {
			bmp->pixels[i] = 0;
		}
		return self;
	}

	ruby_value 
	rb_cBitmap_initialize(int size, ruby_value* args, ruby_value self) {
		if (size == 1) {
			ruby_value v = rb_funcall(args[0], rb_intern("to_s"), 0);
			char* file = qgf_tools_file_finder_find_image(StringValuePtr(v));
			if (!file) RAISE_FILE_NOT_FOUND(StringValuePtr(v));
			rb_cBitmap_createByFilename(self, file);
			if (file) free(file);
		} else if (size == 2) {
			rb_cBitmap_createBySize(self, NUM2ULONG(args[0]), NUM2ULONG(args[1]));
		} else {
			RAISE_ARGN_S(size, "1 or 2");
		}
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		bmp->font = rb_font_new();
		bmp->flags = BITMAP_FLAG_CHANGED;
		return self;
	}

	ruby_value 
	rb_cBitmap_dispose(ruby_value self) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (bmp->texture > 0) glDeleteTextures(1, &(bmp->texture));
		bmp->flags = bmp->flags | BITMAP_FLAG_DISPOSED;
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_disposedQ(ruby_value self) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		return BITMAP_DISPOSED_P(bmp) ? Qtrue : Qfalse;
	}

	ruby_value 
	rb_cBitmap_width(ruby_value self) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		return ULONG2NUM(bmp->width);
	}
	
	ruby_value 
	rb_cBitmap_height(ruby_value self) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		return ULONG2NUM(bmp->height);
	}

	ruby_value 
	rb_cBitmap_rect(ruby_value self) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		return rb_rect_new(0, 0, (float)bmp->width, (float)bmp->height);
	}

	ruby_value 
	rb_cBitmap_blt(int size, ruby_value* args, ruby_value self) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		TRect* src_rect;
		int x, y;
		TBitmap* src;
		uint8_t opacity;
		if (size == 4) {
			opacity = 255;
		} else if (size == 5) {
			int o = NUM2INT(args[4]);
			opacity = ADJUST_BEETWEEN(o, 0, 255);
		} else {
			RAISE_ARGN_S(size, "4 or 5");
		}
		x = NUM2INT(args[0]);
		y = NUM2INT(args[1]);
		Data_Get_Struct(args[2], TBitmap, src);
		Data_Get_Struct(args[3], TRect, src_rect);
		qgf_simple_bitmap dst_bmp, src_bmp;
		TRect src_r;
		dst_bmp.width = bmp->width;
		dst_bmp.height = bmp->height;
		dst_bmp.pixels = bmp->pixels;
		src_bmp.width = src->width;
		src_bmp.height = src->height;
		src_bmp.pixels = (uint32_t*)malloc(src->width * src->height * 4);
		memcpy(src_bmp.pixels, src->pixels, src->width * src->height * 4);
		src_r.x = src_rect->x;
		src_r.y = src_rect->y;
		src_r.width = src_rect->width;
		src_r.height = src_rect->height;
		qgf_simple_bitmap_blit(&dst_bmp, x, y, &src_bmp, &src_r, opacity);
		free(src_bmp.pixels);
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_stretch_blt(int size, ruby_value* args, ruby_value self) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		uint8_t opacity;
		if ( size == 3) {
			opacity = 255;
		} else if (size == 4) {
			int o = NUM2INT(args[3]);
			opacity = ADJUST_BEETWEEN(o, 0, 255);
		} else {
			RAISE_ARGN_S(size, "3 or 4");
		}
		TRect* src_rect;
		TRect* dst_rect;
		TBitmap* src_bmp;
		Data_Get_Struct(args[0], TRect, dst_rect);
		Data_Get_Struct(args[1], TBitmap, src_bmp);
		Data_Get_Struct(args[2], TRect, src_rect);
		TRect s_rect;
		s_rect.x = src_rect->x;
		s_rect.y = src_rect->y;
		s_rect.width = src_rect->width;
		s_rect.height = src_rect->height;
		qgf_rect_adjust(&s_rect, src_bmp->width, src_bmp->height);
		if (qgf_rect_isOutOfBounds(&s_rect, src_bmp->width, src_bmp->height)) return Qnil;
		qgf_simple_bitmap src;
		src.width = src_bmp->width;
		src.height = src_bmp->height;
		src.pixels = src_bmp->pixels;
		qgf_simple_bitmap* stretched = qgf_simple_bitmap_resample(&src, (int)dst_rect->width, (int)dst_rect->height, &s_rect);
		qgf_simple_bitmap dst;
		dst.width = bmp->width;
		dst.height = bmp->height;
		dst.pixels = bmp->pixels;
		s_rect.x = 0;
		s_rect.y = 0;
		s_rect.width = dst_rect->width;
		s_rect.height = dst_rect->height;
		qgf_simple_bitmap_blit(&dst, (int)dst_rect->x, (int)dst_rect->y, stretched, &s_rect, opacity);
		free(stretched->pixels);
		free(stretched);
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_fill_rect(int size, ruby_value* args, ruby_value self) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		TRect rect;
		TColor* c;
		if (size == 2) {
			TRect* rt;
			Data_Get_Struct(args[0], TRect, rt);
			rect.x = rt->x;
			rect.y = rt->y;
			rect.width = rt->width;
			rect.height = rt->height;
			Data_Get_Struct(args[1], TColor, c);
		} else if (size == 5) {
			rect.x = NUM2INT(args[0]);
			rect.y = NUM2INT(args[1]);;
			rect.width = NUM2INT(args[2]);
			rect.height = NUM2INT(args[3]);
			Data_Get_Struct(args[4], TColor, c);
		} else {
			RAISE_ARGN_S(size, "2 or 5");
		}
		long pos;
		uint32_t color = qgf_color_to_rgba(c);
		int dst_x = (int)(rect.width + rect.x);
		int dst_y = (int)(rect.height + rect.y);
		for (int i = (int)rect.x; i < dst_x; i++) {
			for (int j = (int)rect.y; j < dst_y; j++) {
				pos = i + j * bmp->width;
				if (i >= 0 && i < bmp->width && j >= 0 && j < bmp->height) {
					bmp->pixels[pos] = color;
				}
			}
		}
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_gradient_fill_rect(int size, ruby_value* args, ruby_value self) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		TRect rect;
		int vertical;
		TColor* c1;
		TColor* c2;
		if (size == 3) {
			TRect* r;
			Data_Get_Struct(args[0], TRect, r);
			Data_Get_Struct(args[1], TColor, c1);
			Data_Get_Struct(args[2], TColor, c2);
			rect.x = r->x;
			rect.y = r->y;
			rect.width = r->width;
			rect.height = r->height;
			vertical = FALSE;
		} else if ( size == 4 ) {
			TRect* r;
			Data_Get_Struct(args[0], TRect, r);
			Data_Get_Struct(args[1], TColor, c1);
			Data_Get_Struct(args[2], TColor, c2);
			rect.x = r->x;
			rect.y = r->y;
			rect.width = r->width;
			rect.height = r->height;
			vertical = RTEST(args[3]);
		} else if ( size == 6 ) {
			rect.x = NUM2INT(args[0]);
			rect.y = NUM2INT(args[1]);
			rect.width = NUM2INT(args[2]);
			rect.height = NUM2INT(args[3]);
			Data_Get_Struct(args[4], TColor, c1);
			Data_Get_Struct(args[5], TColor, c2);
			vertical = FALSE;
		} else if ( size == 7 ) {
			rect.x = NUM2INT(args[0]);
			rect.y = NUM2INT(args[1]);
			rect.width = NUM2INT(args[2]);
			rect.height = NUM2INT(args[3]);
			Data_Get_Struct(args[4], TColor, c1);
			Data_Get_Struct(args[5], TColor, c2);
			vertical = RTEST(args[6]);
		} else {
			RAISE_ARGN_S(3, "4, 5, 6, or 7");
		}
		int line;
		int r, g, b, a;
		long pos;
		long limit = bmp->width * bmp->height;
		r = (int)c1->red;
		g = (int)c1->green;
		b = (int)c1->blue;
		a = (int)c1->alpha;
		int dst_x = (int)(rect.width + rect.x);
		int dst_y = (int)(rect.height + rect.y);
		if (vertical) {
			line = (int)rect.height;
			for (int y = (int)rect.y; y < dst_y; y++) {
				r = (int)(r * (line - 1) + c2->red)   / line;
				g = (int)(g * (line - 1) + c2->green) / line;
				b = (int)(b * (line - 1) + c2->blue)  / line;
				a = (int)(a * (line - 1) + c2->alpha) / line;
				for (int x = (int)rect.x; x < dst_x; x++) {
					if (x >= 0 && x < bmp->width && y >= 0 && y < bmp->height) {
						pos = x + y * bmp->width;
						bmp->pixels[pos] = qgf_color_to_rgba(r, g, b, a);
					}
				}
				line--;
			} 
		} else {
			line = (int)rect.width;
			for (int x = (int)rect.x; x < dst_x; x++) {
				r = (int)(r * (line - 1) + c2->red)   / line;
				g = (int)(g * (line - 1) + c2->green) / line;
				b = (int)(b * (line - 1) + c2->blue)  / line;
				a = (int)(a * (line - 1) + c2->alpha) / line;
				for (int y = (int)rect.y; y < dst_y; y++) {
					if (x >= 0 && x < bmp->width && y >= 0 && y < bmp->height) {
						pos = x + y * bmp->width;
						bmp->pixels[pos] = qgf_color_to_rgba(r, g, b, a);
					}
				}
				line--;
			} 
		}
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_clear(ruby_value self) {
		ruby_value rect = rb_funcall(self, rb_intern("rect"), 0);
		return rb_funcall(self, rb_intern("clear_rect"), 1, rect);
	}

	ruby_value 
	rb_cBitmap_clear_rect(int size, ruby_value* args, ruby_value self) {
		ruby_value c = rb_color_new(0, 0, 0, 0);
		if (size == 1) {
			return rb_funcall(self, rb_intern("fill_rect"), 2, args[0], c);
		} else if (size == 4) {
			return rb_funcall(self, rb_intern("fill_rect"), 5, args[0], args[1], args[2], args[3], c);
		} else {
			RAISE_ARGN_S(size, "1 or 4");
		}
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_get_pixel(ruby_value self, ruby_value x, ruby_value y) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		int px, py;
		px = NUM2INT(x);
		py = NUM2INT(y);
		if (px < 0 || px >= bmp->width || py < 0 || py >= bmp->height) {
			return rb_color_new(0, 0, 0, 0);
		}
		return qgf_color_from_rgba(bmp->pixels[px % bmp->width + py / bmp->width]);
	}

	ruby_value 
	rb_cBitmap_set_pixel(ruby_value self, ruby_value x, ruby_value y, ruby_value color) {
		TBitmap* bmp;
		TColor* c;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		int px, py;
		px = NUM2INT(x);
		py = NUM2INT(y);
		if (px < 0 || px >= bmp->width || py < 0 || py >= bmp->height) {
			return color;
		}
		Data_Get_Struct(color, TColor, c);
		uint32_t rgba = qgf_color_to_rgba((uint8_t)c->red, (uint8_t)c->green, (uint8_t)c->blue, (uint8_t)c->alpha);
		bmp->pixels[px % bmp->width + py / bmp->width] = rgba;
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		return color;
	}

	ruby_value 
	rb_cBitmap_hue_change(ruby_value self, ruby_value hue) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		int h = NUM2INT(hue);
		for (int i = 0; i < bmp->width; i++) {
			for (int j = 0; j < bmp->height; j++) {
				long pos = i + j * bmp->width;
				bmp->pixels[pos] = qgf_color_adjustHSL(bmp->pixels[pos], h, 0, 100);
			}
		}
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_lum_change(ruby_value self, ruby_value lum) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		int l = NUM2INT(lum);
		for (int i = 0; i < bmp->width; i++) {
			for (int j = 0; j < bmp->height; j++) {
				long pos = i + j * bmp->width;
				bmp->pixels[pos] = qgf_color_adjustHSL(bmp->pixels[pos], 0, 0, l);
			}
		}
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_sat_change(ruby_value self, ruby_value sat) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		int s = NUM2INT(sat);
		for (int i = 0; i < bmp->width; i++) {
			for (int j = 0; j < bmp->height; j++) {
				long pos = i + j * bmp->width;
				bmp->pixels[pos] = qgf_color_adjustHSL(bmp->pixels[pos], 0, s, 100);
			}
		}
		return Qnil;
	}


	ruby_value 
	rb_cBitmap_hsl_change(ruby_value self, ruby_value hue, ruby_value sat, ruby_value lum) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		int h = NUM2INT(hue);
		int s = NUM2INT(sat);
		int l = NUM2INT(lum);
		for (int i = 0; i < bmp->width; i++) {
			for (int j = 0; j < bmp->height; j++) {
				long pos = i + j * bmp->width;
				bmp->pixels[pos] = qgf_color_adjustHSL(bmp->pixels[pos], h, s, l);
			}
		}
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_blur(int size, ruby_value* args, ruby_value self) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		long pos;
		int dst;
		int opacity;
		if (size == 0) {
			dst = 2;
			opacity = 50;
		} else if (size == 1) {
			dst = NUM2INT(args[0]);
			opacity = 50;
		} else if (size == 2) {
			dst = NUM2INT(args[0]);
			opacity = NUM2INT(args[1]);
		} else {
			RAISE_ARGN_S(size, "0, 1 or 2");
		}
		if (dst < 1) dst = 1;
		for (long x = 0; x < bmp->width; x++) {
			for (long y = 0; y < bmp->height; y++) {
				if (x >= 0 && x < bmp->width && y >= 0 && y < bmp->height) {
					uint32_t c = bmp->pixels[x + y * bmp->width];
					long x1, x2, y1, y2;
					x1 = x - dst;
					x2 = x + dst;
					y1 = y - dst;
					y2 = y + dst;
					long pos;
					for (long i = x1; i < x2; i++) {
						for (long j = y1; j < y2; j++) {
							if (i >= 0 && i < bmp->width && j >= 0 && j < bmp->height) {
								pos = i + j * bmp->width;
								bmp->pixels[pos] = qgf_color_blend(c, bmp->pixels[pos], opacity);
							}
						}
					} 
				} 
			}
		}
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_radial_blur(ruby_value self, ruby_value angle, ruby_value division) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_draw_text(int size, ruby_value* args, ruby_value self) {
		int x, y, width, height, align;
		ruby_value text;
		TRect* r;
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		
		if (size == 2) {
			Data_Get_Struct(args[0], TRect, r);
			x = (int)r->x;
			y = (int)r->y;
			width = (int)r->width;
			height = (int)r->height;
			text = args[1];
			align = 0;
		} else if (size == 3) {
			Data_Get_Struct(args[0], TRect, r);
			x = (int)r->x;
			y = (int)r->y;
			width = (int)r->width;
			height = (int)r->height;
			text = args[1];
			align = NUM2INT(args[2]);
		} else if (size == 5) {
			x      = NUM2INT(args[0]);
			y      = NUM2INT(args[1]);
			width  = NUM2INT(args[2]);
			height = NUM2INT(args[3]);
			text = args[4];
			align = 0;
		} else if (size == 6) {
			x      = NUM2INT(args[0]);
			y      = NUM2INT(args[1]);
			width  = NUM2INT(args[2]);
			height = NUM2INT(args[3]);
			text = args[4];
			align = NUM2INT(args[5]);
		} else {
			RAISE_ARGN_S(size, "2, 3, 5 or 6");
		}
		if (TYPE(text) != T_STRING) {
			text = rb_funcall(text, rb_intern("to_s"), 0);
		}
		TFont* font;
		TColor* c;
		int w, h;
		Data_Get_Struct(bmp->font, TFont, font);
		qgf_simple_bitmap sdw_bmp;
		qgf_simple_bitmap out_bmp;
		qgf_simple_bitmap fnt_bmp;

		if (FONT_SHADOW_P(font)) {
			Data_Get_Struct(font->shadow_color, TColor, c);
			sdw_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &w, &h);
			sdw_bmp.width = w;
			sdw_bmp.height = h;
			free(sdw_bmp.pixels);
			sdw_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &w, &h);
			sdw_bmp.width = w;
			sdw_bmp.height = h;
			qgf_simple_bitmap_blendOverBitmap(&sdw_bmp, bmp, x+2, y+2, width, height, align);
			free(sdw_bmp.pixels);
		}
		if (FONT_OUTLINE_P(font)) {
			
			Data_Get_Struct(font->out_color, TColor, c);
			out_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &(out_bmp.width), &(out_bmp.height));
			free(out_bmp.pixels);
			out_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &(out_bmp.width), &(out_bmp.height));
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x-1, y, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x+1, y, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x, y-1, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x, y-1, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x-1, y-1, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x-1, y+1, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x+1, y-1, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x+1, y+1, width, height, align);
			free(out_bmp.pixels);
		}
		Data_Get_Struct(font->color, TColor, c);
		fnt_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &(fnt_bmp.width), &(fnt_bmp.height));
		qgf_simple_bitmap_blendOverBitmap(&fnt_bmp, bmp, x, y, width, height, align);
		fnt_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &(fnt_bmp.width), &(fnt_bmp.height));
		qgf_simple_bitmap_blendOverBitmap(&fnt_bmp, bmp, x, y, width, height, align);
		free(fnt_bmp.pixels);
		if (FONT_UNDERLINE_P(font)) {
		}
		return Qnil;
	}

	ruby_value 
	rb_cBitmap_gradient_draw_text(int size, ruby_value* args, ruby_value self) {
		int x, y, width, height, align, vertical_gradient;
		ruby_value text;
		TRect* r;
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->flags = bmp->flags | BITMAP_FLAG_CHANGED;
		vertical_gradient = FALSE;
		if (size == 2) {
			Data_Get_Struct(args[0], TRect, r);
			x = (int)r->x;
			y = (int)r->y;
			width = (int)r->width;
			height = (int)r->height;
			text = args[1];
			align = 0;
		} else if (size == 3) {
			Data_Get_Struct(args[0], TRect, r);
			x = (int)r->x;
			y = (int)r->y;
			width = (int)r->width;
			height = (int)r->height;
			text = args[1];
			align = NUM2INT(args[2]);
		} else if (size == 4) {
			Data_Get_Struct(args[0], TRect, r);
			x = (int)r->x;
			y = (int)r->y;
			width = (int)r->width;
			height = (int)r->height;
			text = args[1];
			align = NUM2INT(args[2]);
			vertical_gradient = RTEST(args[3]);
		} else if (size == 5) {
			x      = NUM2INT(args[0]);
			y      = NUM2INT(args[1]);
			width  = NUM2INT(args[2]);
			height = NUM2INT(args[3]);
			text = args[4];
			align = 0;
		} else if (size == 6) {
			x      = NUM2INT(args[0]);
			y      = NUM2INT(args[1]);
			width  = NUM2INT(args[2]);
			height = NUM2INT(args[3]);
			text = args[4];
			align = NUM2INT(args[5]);
		} else if (size == 7) {
			x      = NUM2INT(args[0]);
			y      = NUM2INT(args[1]);
			width  = NUM2INT(args[2]);
			height = NUM2INT(args[3]);
			text = args[4];
			align = NUM2INT(args[5]);
			vertical_gradient = RTEST(args[6]);
		} else {
			RAISE_ARGN_S(size, "2, 3, 5 or 6");
		}
		if (TYPE(text) != T_STRING) {
			text = rb_funcall(text, rb_intern("to_s"), 0);
		}
		TFont* font;
		TColor* c;
		int w, h;
		Data_Get_Struct(bmp->font, TFont, font);
		qgf_simple_bitmap sdw_bmp;
		qgf_simple_bitmap out_bmp;
		qgf_simple_bitmap fnt_bmp;
		if (FONT_SHADOW_P(font)) {
			Data_Get_Struct(font->shadow_color, TColor, c);
			sdw_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &w, &h);
			sdw_bmp.width = w;
			sdw_bmp.height = h;
			free(sdw_bmp.pixels);
			sdw_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &w, &h);
			sdw_bmp.width = w;
			sdw_bmp.height = h;
			qgf_simple_bitmap_blendOverBitmap(&sdw_bmp, bmp, x+2, y+2, width, height, align);
			free(sdw_bmp.pixels);
		}
		if (FONT_OUTLINE_P(font)) {
			
			Data_Get_Struct(font->out_color, TColor, c);
			out_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &(out_bmp.width), &(out_bmp.height));
			free(out_bmp.pixels);
			out_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &(out_bmp.width), &(out_bmp.height));
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x-1, y, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x+1, y, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x, y-1, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x, y-1, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x-1, y-1, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x-1, y+1, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x+1, y-1, width, height, align);
			qgf_simple_bitmap_blendOverBitmap(&out_bmp, bmp, x+1, y+1, width, height, align);
			free(out_bmp.pixels);
		}
		TColor* c1;
		TColor* c2;
		Data_Get_Struct(font->gradient_start_color, TColor, c1);
		Data_Get_Struct(font->gradient_end_color, TColor, c2);
		Data_Get_Struct(font->color, TColor, c);
		fnt_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &(fnt_bmp.width), &(fnt_bmp.height));
		free(fnt_bmp.pixels);
		fnt_bmp.pixels = qgf_tools_text_textBitmap(font, c, StringValuePtr(text), &(fnt_bmp.width), &(fnt_bmp.height));
		qgf_simple_bitmap_applyGradientMask(&fnt_bmp, c, c1, c2, vertical_gradient);
		qgf_simple_bitmap_blendOverBitmap(&fnt_bmp, bmp, x, y, width, height, align);
		free(fnt_bmp.pixels);
		if (FONT_UNDERLINE_P(font)) {
		}
		return Qnil;
	}


	ruby_value 
	rb_cBitmap_text_size(ruby_value self, ruby_value str) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		if (RTEST(bmp->font)) {
			return rb_funcall(bmp->font, rb_intern("text_size"), 1, str);
		}
		return rb_rect_new();
	}

	ruby_value 
	rb_cBitmap_GETfont(ruby_value self) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		return bmp->font;
	}

	ruby_value 
	rb_cBitmap_SETfont(ruby_value self, ruby_value value) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		bmp->font = value;
		return value;
	}

	ruby_value 
	rb_cBitmap_to_bmp(ruby_value self, ruby_value filename) {
		TBitmap* bmp;
		Data_Get_Struct(self, TBitmap, bmp);
		if (BITMAP_DISPOSED_P(bmp)) RAISE_DISPOSED_BITMAP();
		//SOIL_save_image(StringValuePtr(filename), SOIL_SAVE_TYPE_BMP, bmp->width, bmp->height, 4, bmp->data);
		SOIL_save_image(StringValuePtr(filename), SOIL_SAVE_TYPE_BMP, bmp->width, bmp->height, 4, (unsigned char*)bmp->pixels);
		return Qnil;
	}	

	void
	Init_QGF_Graphics_Bitmap() {

		rb_cBitmap = rb_define_class("Bitmap", rb_cObject);
		rb_define_alloc_func(rb_cBitmap, rb_gc_cBitmap_alloc);

		rb_define_method(rb_cBitmap, "initialize", RUBY_METHOD_FUNC(rb_cBitmap_initialize), -1);

		rb_define_method(rb_cBitmap, "dispose", RUBY_METHOD_FUNC(rb_cBitmap_dispose), 0);
		rb_define_method(rb_cBitmap, "disposed?", RUBY_METHOD_FUNC(rb_cBitmap_disposedQ), 0);
		rb_define_method(rb_cBitmap, "width", RUBY_METHOD_FUNC(rb_cBitmap_width), 0);
		rb_define_method(rb_cBitmap, "height", RUBY_METHOD_FUNC(rb_cBitmap_height), 0);
		rb_define_method(rb_cBitmap, "rect", RUBY_METHOD_FUNC(rb_cBitmap_rect), 0);
		rb_define_method(rb_cBitmap, "blt", RUBY_METHOD_FUNC(rb_cBitmap_blt), -1);
		rb_define_method(rb_cBitmap, "stretch_blt", RUBY_METHOD_FUNC(rb_cBitmap_stretch_blt), -1);
		rb_define_method(rb_cBitmap, "fill_rect", RUBY_METHOD_FUNC(rb_cBitmap_fill_rect), -1);
		rb_define_method(rb_cBitmap, "gradient_fill_rect", RUBY_METHOD_FUNC(rb_cBitmap_gradient_fill_rect), -1);
		rb_define_method(rb_cBitmap, "clear", RUBY_METHOD_FUNC(rb_cBitmap_clear), 0);
		rb_define_method(rb_cBitmap, "clear_rect", RUBY_METHOD_FUNC(rb_cBitmap_clear_rect), -1);
		rb_define_method(rb_cBitmap, "get_pixel", RUBY_METHOD_FUNC(rb_cBitmap_get_pixel), 2);
		rb_define_method(rb_cBitmap, "set_pixel", RUBY_METHOD_FUNC(rb_cBitmap_set_pixel), 3);
		rb_define_method(rb_cBitmap, "hue_change", RUBY_METHOD_FUNC(rb_cBitmap_hue_change), 1);
		rb_define_method(rb_cBitmap, "sat_change", RUBY_METHOD_FUNC(rb_cBitmap_sat_change), 1);
		rb_define_method(rb_cBitmap, "lum_change", RUBY_METHOD_FUNC(rb_cBitmap_lum_change), 1);
		rb_define_method(rb_cBitmap, "hsl_change", RUBY_METHOD_FUNC(rb_cBitmap_hsl_change), 3);
		rb_define_method(rb_cBitmap, "blur", RUBY_METHOD_FUNC(rb_cBitmap_blur), -1);
		rb_define_method(rb_cBitmap, "radial_blur", RUBY_METHOD_FUNC(rb_cBitmap_radial_blur), 2);
		rb_define_method(rb_cBitmap, "draw_text", RUBY_METHOD_FUNC(rb_cBitmap_draw_text), -1);
		rb_define_method(rb_cBitmap, "gradient_draw_text", RUBY_METHOD_FUNC(rb_cBitmap_gradient_draw_text), -1);
		rb_define_method(rb_cBitmap, "text_size", RUBY_METHOD_FUNC(rb_cBitmap_text_size), 1);
		rb_define_method(rb_cBitmap, "font", RUBY_METHOD_FUNC(rb_cBitmap_GETfont), 0);
		rb_define_method(rb_cBitmap, "font=", RUBY_METHOD_FUNC(rb_cBitmap_SETfont), 1);
		rb_define_method(rb_cBitmap, "to_bmp", RUBY_METHOD_FUNC(rb_cBitmap_to_bmp), 1);
		

	}
