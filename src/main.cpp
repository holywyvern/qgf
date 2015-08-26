// QGF.cpp: define el punto de entrada de la aplicación de consola.
//


#include "qgf.h"

#include "ruby/ruby.h"

#ifdef _WIN32
#include <Windows.h>
#endif
 
int
main(int argc, char **argv)
{
#ifdef RUBY_DEBUG_ENV
    ruby_set_debug_option(getenv("RUBY_DEBUG"));
#endif
#ifdef HAVE_LOCALE_H
    setlocale(LC_CTYPE, "");
#endif
	int argc2 = 3;
	char* argv2[] = {argv[0], "-e", "require './Scripts/main.rb'"};
	qgf_tools_text_initLibrary();
    ruby_sysinit(&argc, &argv);
    {
	RUBY_INIT_STACK;
	ruby_init();
	/* QGF definitions */
	Init_QGF();
	return ruby_run_node(ruby_options(argc2, argv2));
    }
}
