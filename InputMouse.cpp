#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <sstream>

using std::stringstream;

int windowWidth = 800;
int windowHeight = 600;
bool fullScreen = false;

const unsigned MaxInputLenght = 64;
char msg[MaxInputLenght] = { 0 };

int mouseX = 0, mouseY = 0;
bool mouseLB = false;			
bool mouseMB = false;
bool mouseRB = false;
short mouseWheel = 0;

stringstream text;

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	HWND		hwnd;
	MSG			msg;
	WNDCLASS	wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WinProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "GameWindow";

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, "Erro na criação da janela!", "Game", MB_ICONERROR);
		return 0;
	}

	int windowStyle = WS_OVERLAPPED | WS_SYSMENU;

	if (fullScreen) {
		windowWidth = GetSystemMetrics(SM_CXSCREEN);
		windowHeight = GetSystemMetrics(SM_CYSCREEN);
		windowStyle = WS_POPUP;
	}

	hwnd = CreateWindowEx(
		NULL,
		"GameWindow",
		"Mouse Input",
		windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowWidth,
		windowHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!fullScreen) {
		RECT winRect{ 0, 0, windowWidth, windowHeight };

		AdjustWindowRectEx(&winRect,
			GetWindowStyle(hwnd),
			GetMenu(hwnd) != NULL,
			GetWindowExStyle(hwnd));

		int windowPostX = GetSystemMetrics(SM_CXSCREEN) / 2 - ((winRect.right - winRect.left) / 2);
		int windowPostY = GetSystemMetrics(SM_CYSCREEN) / 2 - ((winRect.bottom - winRect.top) / 2);

		MoveWindow(
			hwnd,
			windowPostX,
			windowPostY,
			winRect.right - winRect.left,
			winRect.bottom - winRect.top,
			TRUE
		);

		ShowWindow(hwnd, nCmdShow);
		UpdateWindow(hwnd);
	}
	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return int(msg.wParam);

	// Fim do Programa
}
// Procedimentos da janela

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC					hdc;
	PAINTSTRUCT			ps;
	RECT				rect;

	switch (message) {

		case WM_MOUSEMOVE:
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);
			return 0;

		case WM_MOUSEWHEEL:
			mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		case WM_LBUTTONDOWN:
			mouseLB = true;
			//InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		case WM_LBUTTONUP:
			mouseLB = false;
			return 0;

		case WM_MBUTTONDOWN:
			mouseMB = true;
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		case WM_MBUTTONUP:
			mouseMB = false;
			return 0;

		case WM_RBUTTONDOWN:
			mouseRB = true;
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		case WM_RBUTTONUP:
			mouseRB = false;
			return 0;

			// Pintura da janela

		case WM_PAINT:
			hdc = GetDC(hwnd);

			if (mouseLB)
				SetPixel(hdc, mouseX +1, mouseY +1, RGB(0, 0, 0));

				ReleaseDC(hwnd, hdc);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}