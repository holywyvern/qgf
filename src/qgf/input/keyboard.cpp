#include "qgf/input/keyboard.h"	
#include "qgf/tools/encoding.h"
#include "qgf/graphics.h"
#define KEYBOARD_SIZE 256

#ifdef _WIN32
#include <Windows.h>
#include <string>
static BYTE qgf_input_kb_state[KEYBOARD_SIZE];
static HKL qgf_kb_hkl;
#define DOWN_STATE_MASK  0x80
#define DEAD_KEY_MASK    0x80000000

#ifdef _UNICODE
typedef std::wstring string;
#else
typedef std::string string;
#endif
#endif

static uint8_t  qgf_input_kb_flags[KEYBOARD_SIZE];
static char*    qgf_input_kb_character;
static long     qgf_kb_char_refresh_rate;

#define QGF_INPUT_KB_TRIGGER  1
#define QGF_INPUT_KB_PRESSED  2
#define QGF_INPUT_KB_REPEATED 4
#define QGF_INPUT_KB_RELEASED 8

#define KB_TRIGGERED_P(x) (qgf_input_kb_flags[x] & QGF_INPUT_KB_TRIGGER)
#define KB_PRESSED_P(x)   (qgf_input_kb_flags[x] & QGF_INPUT_KB_PRESSED)
#define KB_REPEATED_P(x)  (qgf_input_kb_flags[x] & QGF_INPUT_KB_REPEATED)
#define KB_RELEASED_P(x)  (qgf_input_kb_flags[x] & QGF_INPUT_KB_RELEASED)

	ruby_value
	rb_mKeyboardCLASS_triggerQ(ruby_value self, ruby_value key) {
		int x = NUM2INT(key);
		if (x < 0 || x >= KEYBOARD_SIZE) return Qfalse;
		return KB_TRIGGERED_P(x) ? Qtrue : Qfalse;
	}

	ruby_value 
	rb_mKeyboardCLASS_pressQ(ruby_value self, ruby_value key) {
		int x = NUM2INT(key);
		if (x < 0 || x >= KEYBOARD_SIZE) return Qfalse;
		return KB_PRESSED_P(x) ? Qtrue : Qfalse;
	}

	ruby_value 
	rb_mKeyboardCLASS_repeatQ(ruby_value self, ruby_value key) {
		int x = NUM2INT(key);
		if (x < 0 || x >= KEYBOARD_SIZE) return Qfalse;
		return KB_REPEATED_P(x) ? Qtrue : Qfalse;
	}

	ruby_value 
	rb_mKeyboardCLASS_releaseQ(ruby_value self, ruby_value key) {
		int x = NUM2INT(key);
		if (x < 0 || x >= KEYBOARD_SIZE) return Qfalse;
		return KB_RELEASED_P(x) ? Qtrue : Qfalse;
	}

#ifdef _WIN32
        void 
		qgf_keyboard_clear_buffer(UINT vk,UINT sc, HKL hkl)
        {
			WCHAR c[5] = {0, 0, 0, 0, 0};
            BYTE lpKeyStateNull[KEYBOARD_SIZE];
            int rc;
            do {
                rc = ToUnicodeEx(vk, sc, lpKeyStateNull, c, 5, 0, hkl);
            } while(rc < 0);
        }
#endif

#ifdef _WIN32

	static TCHAR* qgf_kb_buffed_char;

	void
	qgf_input_get_character(UINT vk, string* str) {
		UINT c = MapVirtualKeyEx(vk, 2, qgf_kb_hkl);
		if ((c < 32) && (c & DEAD_KEY_MASK != DEAD_KEY_MASK)) return;
		UINT vsc = MapVirtualKeyEx(vk, 0, qgf_kb_hkl);
		TCHAR result[5];
		for (int i = 0; i < 5; i++) {
			result[i] = '\0';
		}
		int l = ToUnicodeEx(vk, vsc, qgf_input_kb_state, result, 5, 0,  qgf_kb_hkl);
		if ((l == 2) && (result[0] == result[1])) {result[1] = '\0';}
		if (l > 0)str->append(result);
		qgf_keyboard_clear_buffer(vk, vsc, qgf_kb_hkl);
	}
#endif

	ruby_value 
	rb_mKeyboardCLASS_update(ruby_value self) {

#ifdef _WIN32
                string chr(TEXT(""));
		qgf_kb_hkl = GetKeyboardLayout(0);
		GetKeyboardState(qgf_input_kb_state);
#endif
		for (int i = 0; i < KEYBOARD_SIZE; i++) {
#ifdef _WIN32
			if ( qgf_input_kb_state[i] & DOWN_STATE_MASK ) {
				qgf_input_kb_flags[i] = qgf_input_kb_flags[i] & ~QGF_INPUT_KB_RELEASED;
				if (!KB_PRESSED_P(i)) {
					qgf_input_kb_flags[i] = qgf_input_kb_flags[i] | QGF_INPUT_KB_PRESSED;
					qgf_input_kb_flags[i] = qgf_input_kb_flags[i] | QGF_INPUT_KB_TRIGGER;
					qgf_input_kb_flags[i] = qgf_input_kb_flags[i] & ~QGF_INPUT_KB_REPEATED;
				} else {
					qgf_input_kb_flags[i] = qgf_input_kb_flags[i] & ~QGF_INPUT_KB_TRIGGER;
				}
				if (KB_REPEATED_P(i)) {
					qgf_input_kb_flags[i] = qgf_input_kb_flags[i] & ~QGF_INPUT_KB_REPEATED;
				} else {
					qgf_input_kb_flags[i] = qgf_input_kb_flags[i] | QGF_INPUT_KB_REPEATED;
				}
        qgf_input_get_character(i, &chr);
			} else if (KB_PRESSED_P(i) && !KB_RELEASED_P(i)) {
				qgf_input_kb_flags[i] = QGF_INPUT_KB_RELEASED;
			} else {
				qgf_input_kb_flags[i] = 0;
			}
#endif
		}
#ifdef _WIN32 
		if (qgf_input_kb_character) {
			free(qgf_input_kb_character);
			qgf_input_kb_character = NULL;
		}
		if (chr.length() > 0) {
			std::string r = T2CA_CPP(chr.c_str(), CP_UTF8);
            char* buf = (char*)malloc(sizeof(char) * (r.length() + 1));
            memcpy(buf, r.c_str(), r.length());
			buf[r.length()] = '\0';
			qgf_input_kb_character = buf;  
		} else {
			qgf_input_kb_character = 0;
		}
#endif

		return Qnil;
	}

	ruby_value 
	rb_mKeyboardCLASS_character(ruby_value self) {
		return qgf_input_kb_character ? rb_str_new2(qgf_input_kb_character) : Qnil;
	}
  
	void
	Init_QGF_Input_Keyboard() {
		qgf_kb_char_refresh_rate = 0;
#ifdef _WIN32
				qgf_kb_buffed_char = 0;
                qgf_input_kb_character = 0;
#endif
                for (int i = 0; i < KEYBOARD_SIZE; i++) {
#ifdef _WIN32
			qgf_input_kb_state[i] = 0;
#endif
			qgf_input_kb_flags[i] = 0;
		}

                rb_mKeyboard = rb_define_module("Keyboard");



                rb_define_singleton_method(rb_mKeyboard, "update", RUBY_METHOD_FUNC(rb_mKeyboardCLASS_update), 0);
                rb_define_singleton_method(rb_mKeyboard, "press?", RUBY_METHOD_FUNC(rb_mKeyboardCLASS_pressQ), 1);
                rb_define_singleton_method(rb_mKeyboard, "repeat?", RUBY_METHOD_FUNC(rb_mKeyboardCLASS_repeatQ), 1);
                rb_define_singleton_method(rb_mKeyboard, "trigger?", RUBY_METHOD_FUNC(rb_mKeyboardCLASS_triggerQ), 1);
                rb_define_singleton_method(rb_mKeyboard, "release?", RUBY_METHOD_FUNC(rb_mKeyboardCLASS_releaseQ), 1);
                rb_define_singleton_method(rb_mKeyboard, "character", RUBY_METHOD_FUNC(rb_mKeyboardCLASS_character), 0);
	}
