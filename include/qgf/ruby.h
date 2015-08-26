#ifndef QGF_RUBY_H
#define QGF_RUBY_H 1

#include "ruby.h"

#define RB_INSPECT_OBJ(obj) (rb_funcall(obj, rb_intern("inspect"), 0))

#define RAISE_ARGN(a, n) (rb_raise(rb_eArgError, "wrong number of arguments(%i for %i)", a, n))
#define RAISE_ARGN_S(a, n) (rb_raise(rb_eArgError, "wrong number of arguments(%i for %s)", a, n))

#define RAISE_DISPOSED(x) (rb_raise(rb_const_get(rb_cObject, rb_intern("RGSSError")), "Disposed %s." x))

#define RAISE_ASSERT_POSITIVE_ARG(name) (rb_raise(rb_eArgError, "%s cannot be negative.", name))

#define RAISE_FILE_NOT_FOUND(file) (rb_raise(rb_eArgError, "File not found: %s.", file))

#define RAISE_FONT_NOT_FOUND(font) (rb_raise(rb_eArgError, "Cannot load font: %s.", font))
#define RAISE_FONT_SIZE_ERROR(name, size, code) (rb_raise(rb_eArgError, "Couldn't set font(%s) size(%d).\nCode: %d\n", RB_INSPECT_OBJ(name), size, code))
#define RAISE_FONT_CHARMAP_ERROR(name, code) (rb_raise(rb_eArgError, "Couldn't set font(%s) charmap.\nCode: %d\n", RB_INSPECT_OBJ(name), code))
#define RAISE_FONT_CHAR_LOAD_ERROR(name, c, err) (rb_eArgError, "Couldn't load font(%s) char '%s'.\nCode: %d\n", RB_INSPECT_OBJ(name), text[i], err)


#define RAISE_DISPOSED_BITMAP() (RAISE_DISPOSED("Bitmap"))
#define RAISE_DISPOSED_SPRITE() (RAISE_DISPOSED("Sprite"))
#define RAISE_DISPOSED_PLANE() (RAISE_DISPOSED("Plane"))
#define RAISE_DISPOSED_VIEWPORT() (RAISE_DISPOSED("Viewport"))

#endif
