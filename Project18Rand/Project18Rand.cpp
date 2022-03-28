// Project18Rand.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project18Rand.h"

#include <time.h>				// time()
#include <stdlib.h>				// srand(), rand()

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


#define MAX_COUNT	100

typedef struct _Circle {
	int x;
	int y;
	int r;
	COLORREF c;
} Circle;

Circle g_circles[MAX_COUNT];
RECT g_clientRect;


void MakeCircle(Circle* ap_circle, RECT* ap_rect)
{
	ap_circle->x = rand() % ap_rect->right;
	ap_circle->y = rand() % ap_rect->bottom;
	ap_circle->c = RGB(rand() % 256, rand() % 256, rand() % 256);
	ap_circle->r = rand() % 50 + 10;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) {
		// 난수 시드 = 현재 시간(초)
		srand((unsigned int)time(NULL));

		// Timer 생성
		SetTimer(hWnd, 1, 50, NULL);

		// 클라이언트 영역 얻기
		GetClientRect(hWnd, &g_clientRect);

		// 원 만들기
		Circle* p = g_circles;
		for (int i = 0; i < MAX_COUNT; i++) {
			MakeCircle(p, &g_clientRect);
			p++;
		}
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN) {

	}
	else if (uMsg == WM_RBUTTONDOWN) {

	}
	// WM_TIMER는 WM_PAINT와 같이 flag성 메시지
	// 다른 메시지큐의 메시지를 모두 처리 후에 처리되므로 호출시점이 정확하지 않다
	else if (uMsg == WM_TIMER) {
		if (wParam == 1) {
			RECT rect;
			GetClientRect(hWnd, &rect);

			// Timer호출시마다 반지름이 1씩 작아짐
			for (int i = 0; i < MAX_COUNT; i++) {
				g_circles[i].r--;
				if (g_circles[i].r <= 0) {
					MakeCircle(&g_circles[i], &g_clientRect);		// &g_cicles[i] == g_circles + i
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}
	// SIZE, MOVE, Z-축 깊이가 바뀔 때 호출됨
	else if (uMsg == WM_WINDOWPOSCHANGED) {
		GetClientRect(hWnd, &g_clientRect);
	}
	else if (uMsg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC h_dc = BeginPaint(hWnd, &ps);

		// 원 그리기
		Circle* p = g_circles;
		HGDIOBJ h_oldBrush = GetCurrentObject(h_dc, OBJ_BRUSH);
		SelectObject(h_dc, GetStockObject(DC_BRUSH));

		for (int i = 0; i < MAX_COUNT; i++) {
			SetDCBrushColor(h_dc, p->c);
			Ellipse(h_dc, p->x - p->r, p->y - p->r, p->x + p->r, p->y + p->r);
			p = p + 1;		// Circle 배열의 1개 요소만큼 전진
		}

		SelectObject(h_dc, h_oldBrush);
		EndPaint(hWnd, &ps);
	}
	else if (uMsg == WM_DESTROY) {
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PROJECT18RAND, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT18RAND));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT18RAND));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROJECT18RAND);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
