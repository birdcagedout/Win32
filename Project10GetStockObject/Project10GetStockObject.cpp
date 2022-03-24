// Project10GetStockObject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
// 1. GetStockObject()		: OS가 미리 갖고있는 GDI객체 얻기
// 2. FillRect()			: 테두리 없이 Brush로 채우기

#include "pch.h"
#include "framework.h"
#include "Project10GetStockObject.h"

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
	// OS가 미리 갖고있는 GDI객체 갖고오기: GetStockObject()
	// 1. 객체 백업 저장
	// 2. SetObject로 객체 사용
	// 3. SetObejct로 백업 객체 복구
	else if (uMsg == WM_LBUTTONDOWN) {
		HDC h_dc = GetDC(hWnd);
		HBRUSH h_oldBrush = (HBRUSH)GetCurrentObject(h_dc, OBJ_BRUSH);		// 습관 배양
		
		// 회색 브러시
		SelectObject(h_dc, GetStockObject(GRAY_BRUSH));
		Rectangle(h_dc, 100, 100, 200, 200);

		// 비어있는(색깔없는) 브러시
		SelectObject(h_dc, GetStockObject(NULL_BRUSH));
		Rectangle(h_dc, 150, 150, 300, 300);

		// 주의 : GetStockObject로 선택한 브러시는 OS에서 제공하는 자원이므로 
		//        DeleteObject로 제거하면 안 된다.(제거하면 오류)
		SelectObject(h_dc, h_oldBrush);										// 습관 배양
		ReleaseDC(hWnd, h_dc);
	}
	// Paint객체의 테두리는 Pen객체의 영향
	// Pen객체를 흰색으로 바꾸면 어떻게 될까
	else if (uMsg == WM_RBUTTONDOWN) {
		HDC h_dc = GetDC(hWnd);
		HBRUSH h_oldBrush = (HBRUSH)GetCurrentObject(h_dc, OBJ_BRUSH);		// 습관 배양
		HPEN h_oldPen = (HPEN)GetCurrentObject(h_dc, OBJ_PEN);				// 습관 배양
		SelectObject(h_dc, GetStockObject(WHITE_PEN));						// 흰색 펜

		// 회색 브러시
		SelectObject(h_dc, GetStockObject(GRAY_BRUSH));
		Rectangle(h_dc, 500, 100, 600, 200);

		// 비어있는(색깔없는) 브러시
		SelectObject(h_dc, GetStockObject(NULL_BRUSH));
		Rectangle(h_dc, 550, 150, 700, 300);

		// 주의 : GetStockObject로 선택한 브러시는 OS에서 제공하는 자원이므로 
		//        DeleteObject로 제거하면 안 된다.(제거하면 오류)
		SelectObject(h_dc, h_oldPen);                               // 습관 배양
		SelectObject(h_dc, h_oldBrush);                             // 습관 배양
		ReleaseDC(hWnd, h_dc);
	}
	// FillRect()로 테두리 없이 칠하기
	else if (uMsg == WM_MBUTTONDOWN) {
		HDC h_dc = GetDC(hWnd);

		HBRUSH h_brush = CreateSolidBrush(RGB(255, 0, 0));		// 빨간 브러시 생성
		RECT r = { 100, 500, 200, 600 };						// FillRect할 영역(RECT)
		FillRect(h_dc, &r, h_brush);							// FillRect

		DeleteObject(h_brush);
		ReleaseDC(hWnd, h_dc);
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
	LoadStringW(hInstance, IDC_PROJECT10GETSTOCKOBJECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT10GETSTOCKOBJECT));

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
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT10GETSTOCKOBJECT));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT10GETSTOCKOBJECT);
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
