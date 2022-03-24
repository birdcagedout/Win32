// Project10CreateXXXXXBrush.cpp : 애플리케이션에 대한 진입점을 정의합니다.
// 1. CreateSolidBrush()	: Brush객체(단색 채움) 만들기
// 2. CreateHatchBrush()	: Brush객체(빗금 무늬) 만들기

#include "pch.h"
#include "framework.h"
#include "Project10CreateXXXXXBrush.h"

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




LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	// CreateSolidBrush()
	else if (uMsg == WM_LBUTTONDOWN) {
		HDC h_dc = GetDC(hWnd);

		HBRUSH h_brush = CreateSolidBrush(RGB(255, 0, 0));		// 빨간 브러시 생성
		RECT r = { 100, 100, 200, 200 };						// FillRect할 영역(RECT)
		FillRect(h_dc, &r, h_brush);							// FillRect

		DeleteObject(h_brush);
		ReleaseDC(hWnd, h_dc);
	}
	// CreateHatchBrush()
	// HBRUSH CreateHatchBrush(int fnStyle, COLORREF clrref);
	// fnStyle: HS_BDIAGONAL(/), HS_CROSS(+), HS_DIAGCROSS(X), HS_FDIAGONAL(\), HS_HORIZONTAL(-), HS_VERTICAL(I)
	else if (uMsg == WM_RBUTTONDOWN) {
		HDC h_dc = GetDC(hWnd);

		HBRUSH h_brush = CreateHatchBrush(HS_BDIAGONAL, RGB(255, 0, 0));	// 빨간 hatch브러시 생성

		RECT r = { 300, 100, 400, 200 };									// FillRect할 영역(RECT)
		SetBkMode(h_dc, TRANSPARENT);										// TRANSPARENT 또는 OPAQUE
		FillRect(h_dc, &r, h_brush);										// FillRect

		r = { 500, 100, 600, 200 };											// FillRect할 영역(RECT)
		SetBkColor(h_dc, RGB(0, 0, 255));
		FillRect(h_dc, &r, h_brush);										// FillRect

		DeleteObject(h_brush);
		ReleaseDC(hWnd, h_dc);
	}
	// 중간버튼 = 모두 지우기
	else if (uMsg == WM_MBUTTONDOWN) {
		InvalidateRect(hWnd, NULL, TRUE);		// 두번째=NULL이면 전체 client area, 세번째=TRUE이면 배경지움
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
	LoadStringW(hInstance, IDC_PROJECT10CREATEXXXXXBRUSH, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT10CREATEXXXXXBRUSH));

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

	return (int) msg.wParam;
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

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT10CREATEXXXXXBRUSH));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT10CREATEXXXXXBRUSH);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
