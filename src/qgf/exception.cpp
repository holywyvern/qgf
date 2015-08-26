#include "qgf/exception.h"

void
Init_QGF_Exception() {

	rb_eRGSSError = rb_define_class("RGSSError", rb_eException);
	rb_eRGSSReset = rb_define_class("RGSSReset", rb_eException);

	rb_define_class("QGFError", rb_eRGSSError);
	rb_define_class("QGFReset", rb_eRGSSReset);

}
