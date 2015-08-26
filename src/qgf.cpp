#include "qgf.h"

void
Init_QGF() {
	Init_QGF_Exception();
	Init_QGF_Kernel();
	Init_QGF_Audio();
	Init_QGF_Graphics();
	Init_QGF_Input();
}
