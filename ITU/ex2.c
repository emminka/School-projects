#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


// Global variable
HINSTANCE hInst;
UINT  MessageCount = 0;
UINT  Count = 0;
int posX = 0;
int posY = 0;
float posun = 0;
float posun2 = 0;
float posun_ruk = 0;
int zmena_farby_kolesa = 182;
int zmena_farby2_kolesa = 183;
int zmena_farby3_kolesa = 176;

int zmena_farby_vagon3 = 255;
int zmena_farby2_vagon3 = 71;
int zmena_farby3_vagon3 = 68;

int zmena_farby_vagon2 = 255;
int zmena_farby2_vagon2 = 163;
int zmena_farby3_vagon2 = 68;

int zmena_farby_vagon1 = 255;
int zmena_farby2_vagon1 = 228;
int zmena_farby3_vagon1 = 68;

int zmena_farby_lokomotiva = 130;
int zmena_farby2_lokomotiva = 255;
int zmena_farby3_lokomotiva = 89;

int zmena_farby_kabina = 96;
int zmena_farby2_kabina = 215;
int zmena_farby3_kabina = 255;

int zmena_farby_okno = 214;
int zmena_farby2_okno = 244;
int zmena_farby3_okno = 255;

int zmena_farby_strecha = 133;
int zmena_farby2_strecha = 127;
int zmena_farby3_strecha = 255;

int zmena_farby_dym = 182;
int zmena_farby2_dym = 183;
int zmena_farby3_dym = 176;

int zmena_farby_telova = 255;
int zmena_farby2_telova = 219;
int zmena_farby3_telova = 172;

int zmena_farby_tmavsia = 224;
int zmena_farby2_tmavsia = 172;
int zmena_farby3_tmavsia = 105;

int zmena_farby_najtmavsia = 141;
int zmena_farby2_najtmavsia = 85;
int zmena_farby3_najtmavsia = 36;






// Function prototypes.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition);
void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps, POINT cursorPosition);

// Application entry point. This is the same as main() in standart C.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	WNDCLASS wcx;          // register class
	HWND hWnd;

	hInst = hInstance;     // Save the application-instance handle.
		// Fill in the window class structure with parameters that describe the main window.

	wcx.style = CS_HREDRAW | CS_VREDRAW;              // redraw if size changes
	wcx.lpfnWndProc = (WNDPROC)MainWndProc;          // points to window procedure
	wcx.cbClsExtra = 0;                               // no extra class memory
	wcx.cbWndExtra = 0;                               // no extra window memory
	wcx.hInstance = hInstance;                        // handle to instance
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);      // predefined app. icon
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);        // predefined arrow
	wcx.hbrBackground = GetStockObject(WHITE_BRUSH);  // white background brush
	wcx.lpszMenuName = (LPCSTR)"MainMenu";          // name of menu resource
	wcx.lpszClassName = (LPCSTR)"MainWClass";        // name of window class

	// Register the window class.

	if (!RegisterClass(&wcx)) return FALSE;

	// create window of registered class

	hWnd = CreateWindow(
		"MainWClass",        // name of window class
		"ITU",               // title-bar string
		WS_OVERLAPPEDWINDOW, // top-level window
		200,                  // default horizontal position
		25,                 // default vertical position
		1000,                // default width
		700,                 // default height
		(HWND)NULL,         // no owner window
		(HMENU)NULL,        // use class menu
		hInstance,           // handle to application instance
		(LPVOID)NULL);      // no window-creation data
	if (!hWnd) return FALSE;

	// Show the window and send a WM_PAINT message to the window procedure.
	// Record the current cursor position.

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// loop of message processing
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}


LRESULT CALLBACK MainWndProc(
	HWND hWnd,        // handle to window
	UINT uMsg,        // message identifier
	WPARAM wParam,    // first message parameter
	LPARAM lParam)    // second message parameter
{
	HDC         hDC;
	PAINTSTRUCT ps;
	POINT cursorPosition;

	// init cursor position 
	GetCursorPos(&cursorPosition);
	ScreenToClient(hWnd, &cursorPosition);

	switch (uMsg)
	{
	case WM_CREATE:
		break;

	// character input 
	case WM_CHAR:
		switch (wParam) {
		case 0x08:  // backspace
		case 0x0A:  // linefeed
		case 0x1B:  // escape
			break;

		case 0x09:  // tab
			break;

		default:
			break;
		}
		break;

	// key input
	case WM_KEYDOWN:
		switch (wParam) {
	        // update posX and posY in order to move object
		case VK_LEFT: // left arrow
			//posX -= 1;
			
			InvalidateRect(hWnd, NULL, TRUE);
			posun -= 1;
			posun_ruk -= 1;
			break;

			
		case VK_RIGHT: // right arrow
		//	posX += 1;
			InvalidateRect(hWnd, NULL, TRUE);
			posun += 1;
			posun_ruk += 1;
			
			break;
		case VK_UP: // up arrow
		InvalidateRect(hWnd, NULL, TRUE);
			posun2 -= 1;
			posun_ruk -= 1;
			break;
		case VK_DOWN: // down arrow
			InvalidateRect(hWnd, NULL, TRUE);
			posun2 += 1;
			posun_ruk += 1;
			break;

		// react on the other pressed keys 
		case VK_SPACE: // space //nove farby
			
			zmena_farby_kolesa = 50;
			zmena_farby2_kolesa = 51;
			zmena_farby3_kolesa = 49;

			zmena_farby_vagon3 = 196;
			zmena_farby2_vagon3 = 3;
			zmena_farby3_vagon3 = 7;

			zmena_farby_vagon2 = 255;
			 zmena_farby2_vagon2 = 124;
			 zmena_farby3_vagon2 = 0;

			zmena_farby_vagon1 = 255;
			 zmena_farby2_vagon1 = 240;
			 zmena_farby3_vagon1 = 0;

			 zmena_farby_lokomotiva = 60;
			 zmena_farby2_lokomotiva = 242;
			 zmena_farby3_lokomotiva = 0;

			 zmena_farby_kabina = 0;
			 zmena_farby2_kabina = 172;
			 zmena_farby3_kabina = 221;

			 zmena_farby_okno = 170;
			 zmena_farby2_okno = 240;
			 zmena_farby3_okno = 255;

			 zmena_farby_strecha = 132;
			 zmena_farby2_strecha = 0;
			 zmena_farby3_strecha = 255;

			 zmena_farby_dym = 134;
			 zmena_farby2_dym = 135;
			 zmena_farby3_dym = 127;

			  zmena_farby_telova = 141;
			 zmena_farby2_telova = 85;
			 zmena_farby3_telova = 36;

			  zmena_farby_tmavsia = 255;
			 zmena_farby2_tmavsia = 219;
			  zmena_farby3_tmavsia = 172;

			  zmena_farby_najtmavsia = 224;
			  zmena_farby2_najtmavsia = 172;
			 zmena_farby3_najtmavsia = 105;

			

			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_BACK: // backspace //spat do povodnych farieb
			InvalidateRect(hWnd, NULL, TRUE);
			zmena_farby_kolesa = 182;
			 zmena_farby2_kolesa = 183;
			 zmena_farby3_kolesa = 176;

			 zmena_farby_vagon3 = 255;
			 zmena_farby2_vagon3 = 71;
			 zmena_farby3_vagon3 = 68;

			  zmena_farby_vagon2 = 255;
			  zmena_farby2_vagon2 = 163;
			  zmena_farby3_vagon2 = 68;

			  zmena_farby_vagon1 = 255;
			  zmena_farby2_vagon1 = 228;
			  zmena_farby3_vagon1 = 68;

			  zmena_farby_lokomotiva = 130;
			  zmena_farby2_lokomotiva = 255;
			   zmena_farby3_lokomotiva = 89;

			   zmena_farby_kabina = 96;
			   zmena_farby2_kabina = 215;
			   zmena_farby3_kabina = 255;

			   zmena_farby_okno = 214;
			   zmena_farby2_okno = 244;
			   zmena_farby3_okno = 255;

			   zmena_farby_strecha = 133;
			   zmena_farby2_strecha = 127;
			   zmena_farby3_strecha = 255;

			   zmena_farby_dym = 182;
			   zmena_farby2_dym = 183;
			   zmena_farby3_dym = 176;

			   zmena_farby_telova = 255;
			   zmena_farby2_telova = 219;
			   zmena_farby3_telova = 172;

			   zmena_farby_tmavsia = 224;
			    zmena_farby2_tmavsia = 172;
			   zmena_farby3_tmavsia = 105;

			   zmena_farby_najtmavsia = 141;
			   zmena_farby2_najtmavsia = 85;
			    zmena_farby3_najtmavsia = 36;

			break;
		case VK_TAB: // tab
			break;
	        // more virtual codes can be found here: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		}
		break;

	// get cursor position 
	case WM_MOUSEMOVE:
		GetCursorPos(&cursorPosition);
		ScreenToClient(hWnd, &cursorPosition);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	// react on mouse clicks
	case WM_LBUTTONDOWN:
		
		break;
	case WM_LBUTTONUP:
		break;

	// paint objects
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		paintObject(hWnd, hDC, ps, posX, posY, cursorPosition);
		paintPosition(hWnd, hDC, ps, cursorPosition);
		// paint other objects
		// paintObject2(hWnd, hDC, ps, posX, posY, cursorPosition);
		// paintObject3(hWnd, hDC, ps, posX, posY, cursorPosition);
		EndPaint(hWnd, &ps);
		DeleteDC(hDC);
		break;

		//
		// Process other messages.
		//

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}


void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition)
{
	float angle = fmod(posun_ruk / 10.0f, (3.14f / 4)) - 3.14f / 4;
	// Paint rectangles, ellipses, polygons, lines etc.
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	SelectObject(hDC, pen);
	SelectObject(hDC, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hDC, RGB(zmena_farby_telova, zmena_farby2_telova, zmena_farby3_telova)); //telova
	Ellipse(hDC, 103 + posun, 69 + posun2, 117 + posun, 83 + posun2); //panaèik vo vagone 3
	MoveToEx(hDC,110 + posun, 83 + posun2, NULL); //telo panacika vo vagone3
	LineTo(hDC, 110 + posun, 100 + posun2);
	MoveToEx(hDC, 110 + posun, 96 + posun2, NULL); //lava ruka panacika vo vagone3
	LineTo(hDC, 110 + posun -cos(angle) * 15, 96 +posun2 + sin(angle) * 15);
	MoveToEx(hDC, 110 + posun,96 + posun2, NULL); //prava ruka panacika vo vagone3
	LineTo(hDC, 110 +posun + cos(angle) * 15, 96 + posun2+ sin(angle) * 15);
	SetDCBrushColor(hDC, RGB(zmena_farby_tmavsia, zmena_farby2_tmavsia, zmena_farby3_tmavsia)); //tmavšia
	Ellipse(hDC, 183 + posun, 69 + posun2, 197 + posun, 83 + posun2); //panaèik vo vagone2
	MoveToEx(hDC, 190 + posun, 83 + posun2, NULL); //telo panacika vo vagone2
	LineTo(hDC, 190 + posun, 100 + posun2);
	MoveToEx(hDC, 190 + posun, 96 + posun2, NULL); //lava ruka panacika vo vagone2
	LineTo(hDC, 190 + posun - cos(angle) * 15, 96 + posun2 + sin(angle) * 15);
	MoveToEx(hDC, 190 + posun, 96 + posun2, NULL); //prava ruka panacika vo vagone2
	LineTo(hDC, 190 + posun + cos(angle) * 15, 96 + posun2 + sin(angle) * 15);
	SetDCBrushColor(hDC, RGB(zmena_farby_najtmavsia, zmena_farby2_najtmavsia, zmena_farby3_najtmavsia)); //najtmanšia
	Ellipse(hDC, 263 + posun, 69 + posun2, 277 + posun, 83 + posun2); //panaèik vo vagone1
	MoveToEx(hDC, 270 + posun, 83 + posun2, NULL); //telo panacika vo vagone1
	LineTo(hDC, 270 + posun, 100 + posun2);
	MoveToEx(hDC, 270 + posun, 96 + posun2, NULL); //lava ruka panacika vo vagone1
	LineTo(hDC, 270 + posun - cos(angle) * 15, 96 + posun2 + sin(angle) * 15);
	MoveToEx(hDC, 270 + posun, 96 + posun2, NULL); //prava ruka panacika vo vagone1
	LineTo(hDC, 270 + posun + cos(angle) * 15, 96 + posun2 + sin(angle) * 15);
	SetDCBrushColor(hDC, RGB(zmena_farby_vagon3, zmena_farby2_vagon3, zmena_farby3_vagon3)); //cervena
	Rectangle(hDC, 80+posun, 100+posun2, 140+posun, 150+posun2); //vagon3
	SetDCBrushColor(hDC, RGB(zmena_farby_vagon2, zmena_farby2_vagon2, zmena_farby3_vagon2)); //oranzova
	Rectangle(hDC, 160 + posun, 100 + posun2, 220 + posun, 150 + posun2); //vagon2
	SetDCBrushColor(hDC, RGB(zmena_farby_vagon1, zmena_farby2_vagon1, zmena_farby3_vagon1)); //zlta
	Rectangle(hDC, 240 + posun, 100 + posun2, 300 + posun, 150 + posun2); //vagon1
	SetDCBrushColor(hDC, RGB(zmena_farby_lokomotiva, zmena_farby2_lokomotiva, zmena_farby3_lokomotiva)); //zelena
	Rectangle(hDC, 320 + posun, 80 + posun2, 430 + posun, 150 + posun2); //lokomotiva
	SetDCBrushColor(hDC, RGB(zmena_farby_kabina, zmena_farby2_kabina, zmena_farby3_kabina)); //modra
	Rectangle(hDC, 320 + posun, 35 + posun2, 371 + posun, 81 + posun2); //kabina
	SetDCBrushColor(hDC, RGB(zmena_farby_okno, zmena_farby2_okno, zmena_farby3_okno)); //svetlmodra
	Rectangle(hDC, 326 + posun, 45 + posun2, 343 + posun, 71 + posun2); //okno v kabine nalavo
	Rectangle(hDC, 348 + posun, 45 + posun2, 365 + posun, 71 + posun2); //okno v kabine napravo
	SetDCBrushColor(hDC, RGB(zmena_farby_strecha, zmena_farby2_strecha, zmena_farby3_strecha)); //fialova
	POINT vertices[] = { {429 + posun, 149 + posun2}, {429 + posun, 125 + posun2}, {445 + posun, 149 + posun2} };
	Polygon(hDC, vertices, 3);
	Rectangle(hDC, 310 + posun, 21 + posun2, 381 + posun, 36 + posun2); //strecha
	Rectangle(hDC, 400 + posun, 51 + posun2, 419 + posun, 81 + posun2); //komin
	Rectangle(hDC, 139 + posun, 130 + posun2, 161 + posun, 140 + posun2); //spojnie vagon3 a vagon2
	Rectangle(hDC, 219 + posun, 130 + posun2, 241 + posun, 140 + posun2); //spojnie vagon2 a vagon1
	Rectangle(hDC, 299 + posun, 130 + posun2, 321 + posun, 140 + posun2); //spojnie vagon1 a lokomotiva
	SetDCBrushColor(hDC, RGB(zmena_farby_dym, zmena_farby2_dym, zmena_farby3_dym)); //siva
	Ellipse(hDC, 408 + posun, 37 + posun2, 420 + posun, 49 + posun2); //najnizsi dym
	Ellipse(hDC, 400 + posun, 28 + posun2, 410 + posun, 38 + posun2); //stredny dym
	Ellipse(hDC, 412 + posun, 23 + posun2, 420 + posun, 31 + posun2); //najvyssi dym
	SetDCBrushColor(hDC, RGB(zmena_farby_kolesa, zmena_farby2_kolesa, zmena_farby3_kolesa)); //siva
	Ellipse(hDC,84 + posun, 137 + posun2, 109 + posun,162 + posun2); //koleso vagon3 nalavo
	Ellipse(hDC, 111 + posun, 137 + posun2, 136 + posun, 162 + posun2); //koleso vagon3 napravo
	Ellipse(hDC, 164 + posun, 137 + posun2, 189 + posun, 162 + posun2); //koleso vagon2 nalavo
	Ellipse(hDC, 191 + posun, 137 + posun2, 216 + posun, 162 + posun2); //koleso vagon2 napravo
	Ellipse(hDC, 244 + posun, 137+posun2, 269 + posun, 162 + posun2); //koleso vagon1 nalavo
	Ellipse(hDC, 271 + posun, 137 + posun2, 296 + posun, 162 + posun2); //koleso vagon1 napravo
	Ellipse(hDC, 324 + posun, 117 + posun2, 369 + posun, 162 + posun2); //koleso lokomotiva napravo
	Ellipse(hDC, 370 + posun, 137 + posun2, 395 + posun, 162 + posun2); //koleso lokomotiva stred
	Ellipse(hDC, 399 + posun, 137 + posun2,424 + posun, 162 + posun2); //koleso lokomotiva nalavo
	

	

	SelectObject(hDC, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hDC, RGB(255, 245, 63));
	

	return;
}

void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps, POINT cursorPosition)
{
	char        text[40];          // buffer to store an output text
	HFONT       font;              // new large font
	HFONT       oldFont;           // saves the previous font

	font = CreateFont(25, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 0);
	oldFont = (HFONT)SelectObject(hDC, font);
	sprintf(text, "Position -- x:%d, y:%d", cursorPosition.x, cursorPosition.y);
	TextOut(hDC, 50, 600, text, (int)strlen(text));
	SelectObject(hDC, oldFont);
	DeleteObject(font);
}
