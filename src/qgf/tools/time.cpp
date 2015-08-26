#include "qgf/tools/time.h"

#ifdef _WIN32
#include <Windows.h>

static LARGE_INTEGER frequency;
static BOOL htimer;

#endif


long
qgf_tools_time_getTicks() {
#ifdef _WIN32
		LARGE_INTEGER tick;
		htimer = QueryPerformanceFrequency(&frequency);
		if (htimer) {
			QueryPerformanceCounter(&tick);
			return (long)(((double)tick.QuadPart * 1000.0) / (double)frequency.QuadPart);
		}
		return GetTickCount();
#endif
}

void
qgf_tools_time_sleep(long ms) {
	if (ms <= 0) return;
#ifdef _WIN32
	Sleep((DWORD)ms);
#endif
}
