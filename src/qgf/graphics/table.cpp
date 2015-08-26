#include "qgf/graphics/table.h"
#include "qgf/ruby.h"

	ruby_value
	rb_gc_cTable_alloc(ruby_value klass) {
		TTable* table;
		table = ALLOC(TTable);
		table->xsize = 0;
		table->ysize = 0;
		table->zsize = 0;
		table->data = NULL;
		return Data_Wrap_Struct(klass, rb_gc_cTable_mark, rb_gc_cTable_free, table);
	}

	void 
	rb_gc_cTable_free(TTable*  obj) {
		free(obj->data);
		free(obj);
	}

	void 
	rb_gc_cTable_mark(TTable*  obj) {
	}

	ruby_value 
	rb_cTable_initialize(int size, ruby_value* args, ruby_value self) {
		long xsize, ysize, zsize;
		xsize = 0;
		ysize = 0;
		zsize = 0;
		if (size == 1) {
			xsize = NUM2LONG(args[0]);		
		} else if (size == 2) {
			xsize = NUM2LONG(args[0]);
			ysize = NUM2LONG(args[1]);
		} else if (size == 3) {
			xsize = NUM2LONG(args[0]);
			ysize = NUM2LONG(args[1]);
			zsize = NUM2LONG(args[2]);
		} else {
			RAISE_ARGN_S(size, "1, 2 or 3");
		}
		if (xsize < 1) xsize = 1;
		if (ysize < 1) ysize = 1;
		if (zsize < 1) zsize = 1;
		TTable* table;
		Data_Get_Struct(self, TTable, table);
		long s = xsize * ysize * zsize;
		table->data = ALLOC_N(long, s);
		table->xsize = xsize;
		table->ysize = ysize;
		table->zsize = zsize;
		for (long i = 0; i < s; i++) {
			table->data[i] = 0;
		}
		return self;
	}

	ruby_value 
	rb_cTable_resize(int size, ruby_value* args, ruby_value self) {
		long xsize, ysize, zsize;
		xsize = 0;
		ysize = 0;
		zsize = 0;
		if (size == 1) {
			xsize = NUM2LONG(args[0]);		
		} else if (size == 2) {
			xsize = NUM2LONG(args[0]);
			ysize = NUM2LONG(args[1]);
		} else if (size == 3) {
			xsize = NUM2LONG(args[0]);
			ysize = NUM2LONG(args[1]);
			zsize = NUM2LONG(args[2]);
		} else {
			RAISE_ARGN_S(size, "1, 2 or 3");
		}
		if (xsize < 1) xsize = 1;
		if (ysize < 1) ysize = 1;
		if (zsize < 1) zsize = 1;
		TTable* table;
		Data_Get_Struct(self, TTable, table);
		long s = xsize * ysize * zsize;
		long* newdata = ALLOC_N(long, s);
		for (long i = 0; i < s; i++) {
			newdata[i] = 0;
		}
		long m_x, m_y, m_z;
		m_x = min(xsize, table->xsize);
		m_y = min(ysize, table->ysize);
		m_z = min(zsize, table->zsize);
		for (int x = 0; x < m_x; x++) {
			for (int y = 0; y < m_y; y++) {
				for (int z = 0; z < m_z; z++) {
					long np = x + y * xsize + z * xsize * ysize;
					newdata[np] = table->data[x + y * table->xsize + z * table->xsize * table->ysize];
				}
			}
		} 
		table->xsize = xsize;
		table->ysize = ysize;
		table->zsize = zsize;
		free(table->data);
		table->data = newdata;
		return Qnil;
	}

	ruby_value 
	rb_cTable_GETKEY(int size, ruby_value* args, ruby_value self) {
		TTable* table;
		long x, y, z;
		Data_Get_Struct(self, TTable, table);
		x = 0;
		y = 0;
		z = 0;
		if (size == 1) {
			x = NUM2LONG(args[0]);
		} else if (size == 2) {
			x = NUM2LONG(args[0]);
			y = NUM2LONG(args[1]);
		} else if (size == 3) {
			x = NUM2LONG(args[0]);
			y = NUM2LONG(args[1]);
			z = NUM2LONG(args[2]);
		} else {
			RAISE_ARGN_S(size, "1, 2 or 3");
		}
		if (x < 0 || x >= table->xsize) return Qnil;
		if (y < 0 || y >= table->ysize) return Qnil;
		if (z < 0 || z >= table->zsize) return Qnil;
		return LONG2NUM(table->data[x + y * table->xsize + z * table->xsize * table->ysize]);
	}

	ruby_value 
	rb_cTable_SETKEY(int size, ruby_value* args, ruby_value self) {
		TTable* table;
		long x, y, z;
		ruby_value value;
		Data_Get_Struct(self, TTable, table);
		x = 0;
		y = 0;
		z = 0;
		if (size == 2) {
			x = NUM2LONG(args[0]);
			value = args[1];
		} else if (size == 3) {
			x = NUM2LONG(args[0]);
			y = NUM2LONG(args[1]);
			value = args[2];
		} else if (size == 4) {
			x = NUM2LONG(args[0]);
			y = NUM2LONG(args[1]);
			z = NUM2LONG(args[2]);
			value = args[3];
		} else {
			RAISE_ARGN_S(size, "2, 3 or 4");
		}
		if (x < 0 || x >= table->xsize) return value;
		if (y < 0 || y >= table->ysize) return value;
		if (z < 0 || z >= table->zsize) return value;
		table->data[x + y * table->xsize + z * table->xsize * table->ysize] = NUM2LONG(value);
		return value;
	}

	ruby_value 
	rb_cTable_xsize(ruby_value self) {
		TTable* table;
		Data_Get_Struct(self, TTable, table);
		return LONG2NUM(table->xsize);
	}

	ruby_value 
	rb_cTable_ysize(ruby_value self) {
		TTable* table;
		Data_Get_Struct(self, TTable, table);
		return LONG2NUM(table->ysize);
	}

	ruby_value 
	rb_cTable_zsize(ruby_value self) {
		TTable* table;
		Data_Get_Struct(self, TTable, table);
		return LONG2NUM(table->zsize);
	}

	ruby_value 
	rb_cTable__dump(ruby_value self) {
		TTable* table;
		ruby_value data, arr, str;
		Data_Get_Struct(self, TTable, table);
		unsigned long items = NUM2LONG(table->xsize) * NUM2LONG(table->ysize) * NUM2LONG(table->zsize);
		data = rb_ary_new();
		for (unsigned long i = 0; i < items; i++) {
			rb_ary_push(data, LONG2NUM(table->data[i]));
		}
		str = rb_str_new2("");
		arr = rb_ary_new3(5, INT2NUM(3), LONG2NUM(table->xsize), LONG2NUM(table->ysize), LONG2NUM(table->zsize), ULONG2NUM(items));
		rb_str_concat(str, rb_funcall(arr, rb_intern("pack"), 1, rb_str_new2("L5")));
		rb_str_concat(str, rb_funcall(data, rb_intern("pack"), 1, rb_str_times(rb_str_new2("S"), ULONG2NUM(items))));
		return str;
	}

	ruby_value 
	rb_cTableCLASS__load(ruby_value self, ruby_value str) {
		VALUE arr = rb_funcall(str, rb_intern("unpack"), 1, rb_str_new2("L5"));
		//int dim = NUM2INT(rb_ary_entry(arr, 0));
		long items = NUM2LONG(rb_ary_entry(arr, 4));
		VALUE args[3] = {rb_ary_entry(arr, 1), rb_ary_entry(arr, 2), rb_ary_entry(arr, 3)};
		VALUE table = rb_class_new_instance(3, args, rb_cTable);
		VALUE data = rb_funcall(rb_str_substr(str, 20, items * 2), rb_intern("unpack"), 1, rb_str_times(rb_str_new2("S"), ULONG2NUM(items)));
		TTable* t;
		Data_Get_Struct(table, TTable, t);
		ruby_value entry;
		ID to_i = rb_intern("to_i");
		for (long i = 0; i < items; i++) {
			entry = rb_ary_entry(data, i);
			if (TYPE(entry) == T_STRING) {
				t->data[i] = NUM2LONG(rb_funcall(entry, to_i, 0));
			} else {
				t->data[i] = NUM2LONG(entry);
			}
			
		}
		return table;
	}

	void
	Init_QGF_Graphics_Table() {
		rb_cTable = rb_define_class("Table", rb_cObject);
		rb_define_alloc_func(rb_cTable, rb_gc_cTable_alloc);

		rb_define_method(rb_cTable, "initialize", RUBY_METHOD_FUNC(rb_cTable_initialize), -1);

		rb_define_method(rb_cTable, "resize", RUBY_METHOD_FUNC(rb_cTable_resize), -1);

		rb_define_method(rb_cTable, "[]", RUBY_METHOD_FUNC(rb_cTable_GETKEY), -1);
		rb_define_method(rb_cTable, "[]=", RUBY_METHOD_FUNC(rb_cTable_SETKEY), -1);

		rb_define_method(rb_cTable, "xsize", RUBY_METHOD_FUNC(rb_cTable_xsize), 0);
		rb_define_method(rb_cTable, "ysize", RUBY_METHOD_FUNC(rb_cTable_ysize), 0);
		rb_define_method(rb_cTable, "zsize", RUBY_METHOD_FUNC(rb_cTable_zsize), 0);

		rb_define_method(rb_cTable, "_dump", RUBY_METHOD_FUNC(rb_cTable__dump), 0);
		rb_define_singleton_method(rb_cTable, "_load", RUBY_METHOD_FUNC(rb_cTableCLASS__load), 1);
	}
