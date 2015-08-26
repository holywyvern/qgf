
#include "qgf/kernel.h"
#include "qgf/exception.h"
#include "qgf/tools/window.h"
#include <stdlib.h>
#include <string>

	ruby_value
	rb_mKernel_rgss_run_block(VALUE block) {
		return rb_funcall(block,rb_intern("call"), 0);
	}

	ruby_value
	rb_mKernel_rgss_reset_block(VALUE block) {
		return rb_rescue2(RUBY_METHOD_FUNC(rb_mKernel_rgss_run_block), block,
			RUBY_METHOD_FUNC(rb_mKernel_rgss_reset_block), block, rb_const_get(rb_cObject, rb_intern("RGSSReset"))); 
	}

	ruby_value
	rb_mKernel_rgss_main(ruby_value self) {
		VALUE graphics = rb_const_get(rb_cObject, rb_intern("Graphics"));
		long width = NUM2LONG(rb_funcall(graphics, rb_intern("width"), 0));
		long height = NUM2LONG(rb_funcall(graphics, rb_intern("height"), 0));
		ruby_value title = rb_funcall(graphics, rb_intern("window_title"), 0);
		if (!qgf_tools_window_open(width, height, StringValuePtr(title))) {
			rb_raise(rb_eRGSSError, "Can't Open Window.");
		}
		if (!rb_block_given_p())
			rb_raise(rb_eRGSSError, "No block given to RGSS function.");
		return rb_funcall(rb_block_proc(), rb_intern("call"), 0);
		//return rb_mKernel_rgss_reset_block(rb_block_proc());
	}

	ruby_value
	rb_mKernel_rgss_stop(ruby_value self) {
		return rb_eval_string("loop { Graphics.update }");
	}

	ruby_value
	rb_mKernel_load_data(ruby_value self, ruby_value filename) {
		ruby_value file;
		ruby_value obj;
		file = rb_funcall(rb_const_get(rb_cObject, rb_intern("File")), rb_intern("open"), 2, filename, rb_str_new2("rb"));
		obj = rb_funcall(rb_const_get(rb_cObject, rb_intern("Marshal")), rb_intern("load"), 1, file);
		rb_funcall(file, rb_intern("close"), 0);
		return obj;
	}

	ruby_value
	rb_mKernel_save_data(ruby_value self, ruby_value object, ruby_value filename) {
		ruby_value file;
		file = rb_funcall(rb_const_get(rb_cObject, rb_intern("File")), rb_intern("open"), 2, filename, rb_str_new2("wb"));
		rb_funcall(rb_const_get(rb_cObject, rb_intern("Marshal")), rb_intern("dump"), 2, object, file);
		rb_funcall(file, rb_intern("close"), 0);
		return Qnil;
	}

	ruby_value
	rb_mKernel_msgbox(int size, ruby_value* args, ruby_value self) {
		int tsize = 0;
		std::string result("");
		for (int i = 0; i < size; i++) {
			ruby_value obj = rb_funcall(args[i], rb_intern("to_s"), 0);
			result.append(StringValuePtr(obj));
		}
		qgf_tools_window_show_message((char*)result.c_str());
		return Qnil;
	}

	ruby_value
	rb_mKernel_msgbox_p(int size, ruby_value* args, ruby_value self) {
		int tsize = 0;
		std::string result("");
		for (int i = 0; i < size; i++) {
			ruby_value obj = rb_funcall(args[i], rb_intern("inspect"), 0);
			result.append(StringValuePtr(obj));
			if (i != size - 1) result.append("\n");
		}
		qgf_tools_window_show_message((char*)result.c_str());
		return Qnil;
	}

void
Init_QGF_Kernel() {
	rb_define_global_function("rgss_main", RUBY_METHOD_FUNC(rb_mKernel_rgss_main), 0);
	rb_define_global_function("rgss_stop", RUBY_METHOD_FUNC(rb_mKernel_rgss_stop), 0);
	rb_define_global_function("qgf_main", RUBY_METHOD_FUNC(rb_mKernel_rgss_main), 0);
	rb_define_global_function("qgf_stop", RUBY_METHOD_FUNC(rb_mKernel_rgss_stop), 0);
	rb_define_global_function("load_data", RUBY_METHOD_FUNC(rb_mKernel_load_data), 1);
	rb_define_global_function("save_data", RUBY_METHOD_FUNC(rb_mKernel_save_data), 2);
	rb_define_global_function("msgbox", RUBY_METHOD_FUNC(rb_mKernel_msgbox), -1);
	rb_define_global_function("msgbox_p", RUBY_METHOD_FUNC(rb_mKernel_msgbox_p), -1);
}
