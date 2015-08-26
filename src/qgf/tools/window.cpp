#include "qgf/tools/window.h"

#ifdef _WIN32

#include "qgf/tools/openGL.h"
#include "qgf/tools/encoding.h"
#include <Windows.h>


#define QGF_WINDOW_CLASS_NAME "QGF-WINDOW"

static HDC       qgf_window_hdc;
static HGLRC     qgf_window_hglrc;
static HWND      qgf_window_hwnd;
static HINSTANCE qgf_window_hInstance;
static int       qgf_window_fullscreen;

LRESULT CALLBACK 
qgf_tools_win32_WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg)
	{
	case WM_ACTIVATE:
		if (HIWORD(wParam)) {
			/* RESUME */
		} else {
			/* PAUSE */
		}
		break;
	case WM_SETFOCUS:
			/* RESUME */
		break;
	case WM_KILLFOCUS:
			/* PAUSE */
		break;
	case WM_SYSCOMMAND:
		switch (wParam) {
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	case WM_CLOSE:
    /* CLOSE */
		break;
	case WM_CHAR:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int
qgf_tools_win32_registerWindow(WNDCLASSEX* ex) {
    ex->cbSize = sizeof(WNDCLASSEX);
    ex->style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    ex->lpfnWndProc = qgf_tools_win32_WinProc;
    ex->cbClsExtra = 0;
    ex->cbWndExtra = 0;
    ex->hInstance = qgf_window_hInstance;
    ex->hIcon = LoadIcon(NULL, IDI_APPLICATION);
    ex->hCursor = LoadCursor(NULL, IDC_ARROW);
    ex->hbrBackground = NULL;
    ex->lpszMenuName = NULL;
    ex->lpszClassName = TEXT(QGF_WINDOW_CLASS_NAME);
    ex->hIconSm = NULL;
	return RegisterClassEx(ex);
}

void
qgf_tools_win32_closeWindow() {
	if (qgf_window_fullscreen) {
		ChangeDisplaySettings(0, 0);
		ShowCursor(TRUE);
	}
	if (qgf_window_hglrc) {
		wglMakeCurrent(0, 0);
		wglDeleteContext(qgf_window_hglrc);
		qgf_window_hglrc = 0;
	}
	if (qgf_window_hdc && !ReleaseDC(qgf_window_hwnd, qgf_window_hdc)) {
		qgf_window_hdc = 0;
	}
	if (qgf_window_hwnd && !DestroyWindow(qgf_window_hwnd)) {
		qgf_window_hwnd = 0;
	}    
	if (!UnregisterClass(TEXT(QGF_WINDOW_CLASS_NAME), qgf_window_hInstance)) {
            qgf_window_hInstance = 0;
  }
}


int
qgf_tools_win32_updateWindow() {
    MSG msg;
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (!qgf_window_hwnd || !IsWindowVisible(qgf_window_hwnd)) {
        qgf_tools_win32_closeWindow();
        return 0;
    }
    // Render
    SwapBuffers(qgf_window_hdc);  
    return 1;
}

void
qgf_tools_win32_setupWindow() {
	ShowWindow(qgf_window_hwnd, SW_SHOW);
	SetForegroundWindow(qgf_window_hwnd);
	SetFocus(qgf_window_hwnd);
}

#endif

int
qgf_tools_window_open(long width, long height, char* window_name) {
#ifdef _WIN32
	WNDCLASSEX wc;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT scrrect;
	RECT windowRect;
	HINSTANCE hInstance;
	int posx; 
	int posy;
	int centerx;
	int centery;
	int edge;
	int capt;
	HDC hdc;
	HWND hwnd;
	HGLRC hrc;
	unsigned int pixel_format;
	PIXELFORMATDESCRIPTOR pfd;

	hInstance = GetModuleHandle(NULL);
	qgf_window_hInstance = hInstance;

	if (!qgf_tools_win32_registerWindow(&wc)) {
		qgf_tools_win32_closeWindow();
		return 0;
	}

	if (SystemParametersInfo(SPI_GETWORKAREA, 0, &scrrect, SPIF_UPDATEINIFILE)) {
		centerx = scrrect.right / 2;
		centery = scrrect.bottom / 2;

		edge = GetSystemMetrics(SM_CXEDGE);
		capt = GetSystemMetrics(SM_CXFIXEDFRAME);

		posx = centerx - width / 2 - edge;
		posy = centery - height / 2 - capt;
	}

	
	windowRect.top = posy;
	windowRect.left = posx;
	windowRect.right = posx + width;
	windowRect.bottom = posy + height;

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	hwnd = CreateWindowEx(dwExStyle, TEXT(QGF_WINDOW_CLASS_NAME), CA2T(window_name, CP_UTF8),
						  dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
						  windowRect.left, windowRect.top, windowRect.right - windowRect.left,
						  windowRect.bottom - windowRect.top,
						  0, 0, hInstance, 0);
	if (!hwnd) {
		qgf_tools_win32_closeWindow();
		return 0;
	}
	qgf_window_hwnd = hwnd;

	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	hdc = GetDC(hwnd);

	if (!hdc) { 
		qgf_tools_win32_closeWindow();
		return 0;
	}
	qgf_window_hdc   = hdc;

	pixel_format = ChoosePixelFormat(hdc, &pfd);
	if (!pixel_format) {
		qgf_tools_win32_closeWindow();
		return 0;
	}

	if (!SetPixelFormat(hdc, pixel_format, &pfd)) {
		qgf_tools_win32_closeWindow();
		return 0;
	}

	hrc = wglCreateContext(hdc);
	if (!hrc) {
		qgf_tools_win32_closeWindow();
		return 0;
	}

	qgf_window_hglrc  = hrc;

	if (!wglMakeCurrent(hdc, hrc)) {
		qgf_tools_win32_closeWindow();
		return 0;
	}
	qgf_tools_win32_setupWindow();
	qgf_tools_openGL_initWindow(width, height);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		qgf_tools_win32_closeWindow();
		return 0;
	}

#endif
	if (!qgf_tools_openGL_initShaders()) {
		qgf_tools_window_close();
		return 0;
	}
	return 1;
}

void
qgf_tools_window_close() {
#ifdef _WIN32
	qgf_tools_win32_closeWindow();
#endif
}

void
qgf_tools_window_rename(char* window_name) {
#ifdef _WIN32
	if (qgf_window_hwnd) {
		SetWindowText(qgf_window_hwnd, CA2T(window_name, CP_UTF8));
	}
#endif
}

int
qgf_tools_window_update() {
#ifdef _WIN32
	return qgf_tools_win32_updateWindow();
#endif
}

void
qgf_tools_window_show_message(char* message) {
#ifdef _WIN32
	MessageBox(qgf_window_hwnd, CA2T(message, CP_UTF8), TEXT("Message"), MB_OK);
#endif
}

void
qgf_tools_window_show_warning(char* message) {
#ifdef _WIN32
	MessageBox(qgf_window_hwnd, CA2T(message, CP_UTF8), TEXT(""), MB_OK|MB_ICONEXCLAMATION);
#endif
}

void
qgf_tools_window_show_error(char* message) {
#ifdef _WIN32
	MessageBox(qgf_window_hwnd, CA2T(message, CP_UTF8), TEXT(""), MB_OK|MB_ICONERROR);
#endif
}

void
qgf_tools_window_show_info(char* message) {
#ifdef _WIN32
	MessageBox(qgf_window_hwnd, CA2T(message, CP_UTF8), TEXT(""), MB_OK|MB_ICONINFORMATION);
#endif
}

#ifdef _WIN32
HWND qgf_tools_windowHWND() {
	return qgf_window_hwnd;
}
#endif