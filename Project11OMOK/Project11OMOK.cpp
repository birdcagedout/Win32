// Project11OMOK.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project11OMOK.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance, HBRUSH h_brush);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------
#define X_COUNT			19			// 가로 19줄
#define Y_COUNT			19			// 세로 19줄

#define START_X			50			// 왼쪽 여백
#define START_Y			50			// 위쪽 여백

#define INTERVAL		26			// 줄 간격
#define HALF_INTERVAL	INTERVAL/2	// 바둑돌 반지름

#define XPOS(x)			(START_X + (x) * INTERVAL)	// x위치 계산
#define YPOS(y)			(START_Y + (y) * INTERVAL)	// y위치 계산

unsigned char g_dol[Y_COUNT][X_COUNT];		// 돌 놓인 위치 + 돌종류(0=없음, 1=검돌, 2=흰돌)
unsigned char g_step;						// 돌 놓을 차례(0=검돌, 1=흰돌)



// WM_PAINT를 처리하는 프로시저
void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC h_dc = BeginPaint(hWnd, &ps);

	// 세로19줄 그리기
	for (int x = 0; x < X_COUNT; x++) {
		MoveToEx(h_dc, XPOS(x), YPOS(0), NULL);
		LineTo(h_dc, XPOS(x), YPOS(Y_COUNT - 1));
	}

	// 가로19줄 그리기
	for (int y = 0; y < Y_COUNT; y++) {
		MoveToEx(h_dc, XPOS(0), YPOS(y), NULL);
		LineTo(h_dc, XPOS(X_COUNT - 1), YPOS(y));
	}

	// 돌 위치 체크하면서 돌 있으면 그린다.
	for (int y = 0; y < Y_COUNT; y++) {
		for (int x = 0; x < X_COUNT; x++) {

			// 돌이 있으면 ==> 돌 그리기
			if (g_dol[y][x] > 0) {

				// 검은 돌
				if (g_dol[y][x] == 1) {
					SelectObject(h_dc, GetStockObject(BLACK_BRUSH));
				}
				// 흰색 돌
				else {
					SelectObject(h_dc, GetStockObject(WHITE_BRUSH));
				}

				// 테두리 없는 원 그리기
				SelectObject(h_dc, GetStockObject(NULL_PEN));
				Ellipse(h_dc, XPOS(x) - HALF_INTERVAL, YPOS(y) - HALF_INTERVAL,
					XPOS(x) + HALF_INTERVAL, YPOS(y) + HALF_INTERVAL);
			}
		}
	}
	EndPaint(hWnd, &ps);
}


// WM_LBUTTONDOWN을 처리하는 프로시저
void OnLButtonDown(HWND hWnd, int a_x, int a_y)
{
	// 클릭한 좌표가 바둑판 안쪽인지 체크
	if (a_x > (XPOS(0) - HALF_INTERVAL) && a_y > (YPOS(0) - HALF_INTERVAL)
		&& a_x < (XPOS(X_COUNT - 1) + HALF_INTERVAL) && a_y < (YPOS(Y_COUNT - 1) + HALF_INTERVAL)) {

		// 클릭 좌표를 유효한 착점 위치로 바꿈
		int x = (a_x - START_X + HALF_INTERVAL) / INTERVAL;
		int y = (a_y - START_Y + HALF_INTERVAL) / INTERVAL;

		// 해당 위치에 돌이 있는지 체크 ==> 없으면 돌 놓기
		if (g_dol[y][x] == 0) {

			// 검은돌=1, 흰돌=2
			g_dol[y][x] = g_step + 1;

			// 차례 변경
			g_step = !g_step;

			// 화면 갱신
			InvalidateRect(hWnd, NULL, TRUE);
		}

	}
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	// 새로 그리기
	else if (uMsg == WM_PAINT) {
		OnPaint(hWnd);
		return 0;
	}
	// 좌클릭 ==> 돌 놓기
	else if (uMsg == WM_LBUTTONDOWN) {
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		OnLButtonDown(hWnd, x, y);
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
	LoadStringW(hInstance, IDC_PROJECT11OMOK, szWindowClass, MAX_LOADSTRING);
	
	
	// 윈도 클래스 등록 + 배경색깔=바둑판노란색
	HBRUSH h_bkBrush = CreateSolidBrush(RGB(244, 176, 77));
	MyRegisterClass(hInstance, h_bkBrush);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT11OMOK));

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

	// 배경색깔 HBRUSH 제거
	DeleteObject(h_bkBrush);
	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance, HBRUSH h_bkBrush)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT11OMOK));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = h_bkBrush;
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT11OMOK);
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
	  CW_USEDEFAULT, 0, 600, 600, nullptr, nullptr, hInstance, nullptr);

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
