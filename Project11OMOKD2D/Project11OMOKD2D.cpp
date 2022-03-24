// Project11OMOKD2D.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project11OMOKD2D.h"
#include <windowsx.h>

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


// Direct2D 사용
#include <d2d1.h>
#pragma comment(lib, "D2D1.lib")
using namespace D2D1;				// Direct2D에서 자주 쓰는 네임 스페이스 생략할 수 있도록 설정

ID2D1Factory *gp_factory;			// D2D를 구성하는 각종 객체를 생성하는 객체
ID2D1HwndRenderTarget *gp_renderTarget;	// D2D에서 클라이언트 영역에 그림을 그릴 객체


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
	// D2D의 Render Target을 사용하여 그림그리기 시작
	gp_renderTarget->BeginDraw();
	gp_renderTarget->Clear(ColorF(0.95686f, 0.69f, 0.3019f));	// BeginDraw() 후 전체영역이 검을색으로 변경되기 때문

	// brush 생성
	ID2D1SolidColorBrush* p_lineBrush, * p_blackBrush, * p_whiteBrush;
	gp_renderTarget->CreateSolidColorBrush(ColorF(0.095686f, 0.069f, 0.03019f), &p_lineBrush);
	gp_renderTarget->CreateSolidColorBrush(ColorF(0.0f, 0.0f, 0.0f), &p_blackBrush);
	gp_renderTarget->CreateSolidColorBrush(ColorF(1.0f, 1.0f, 1.0f), &p_whiteBrush);

	
	D2D1_POINT_2F start_pos, end_pos;
	// 세로19줄 그리기
	for (int x = 0; x < X_COUNT; x++) {
		start_pos.x = XPOS(x);
		start_pos.y = YPOS(0);
		end_pos.x = XPOS(x);
		end_pos.y = YPOS(Y_COUNT - 1);
		gp_renderTarget->DrawLine(start_pos, end_pos, p_lineBrush, (float)1.0f);
	}

	// 가로19줄 그리기
	for (int y = 0; y < Y_COUNT; y++) {
		start_pos.x = XPOS(0);
		start_pos.y = YPOS(y);
		end_pos.x = XPOS(X_COUNT - 1);
		end_pos.y = YPOS(y);
		gp_renderTarget->DrawLine(start_pos, end_pos, p_lineBrush, (float)1.0f);
	}

	// 돌(원) 객체
	D2D1_ELLIPSE dol_region;
	dol_region.radiusX = (float)HALF_INTERVAL;		// x축 반지름
	dol_region.radiusY = (float)HALF_INTERVAL;		// y축 반지름

	// 돌 위치 체크하면서 돌 있으면 그린다.
	for (int y = 0; y < Y_COUNT; y++) {
		for (int x = 0; x < X_COUNT; x++) {

			// 돌이 있으면 ==> 돌 그리기
			if (g_dol[y][x] > 0) {

				dol_region.point.x = XPOS(x);
				dol_region.point.y = YPOS(y);

				// 검은 돌 그리기
				if (g_dol[y][x] == 1) {
					gp_renderTarget->FillEllipse(dol_region, p_blackBrush);
				}
				// 흰색 돌 그리기
				else {
					gp_renderTarget->FillEllipse(dol_region, p_whiteBrush);
				}
			}
		}
	}

	// 자원해제
	p_lineBrush->Release(); p_lineBrush = NULL;
	p_blackBrush->Release(); p_blackBrush = NULL;
	p_whiteBrush->Release(); p_whiteBrush = NULL;

	// 그리기 종료
	gp_renderTarget->EndDraw();
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
	if (uMsg == WM_CREATE) {
		// 클라이언트 영역 얻기
		RECT r;
		GetClientRect(hWnd, &r);

		// Render Target 생성
		gp_factory->CreateHwndRenderTarget(RenderTargetProperties(), HwndRenderTargetProperties(hWnd, SizeU(r.right, r.bottom)), &gp_renderTarget);
		return 0;
	}
	else if (uMsg == WM_DESTROY) {
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

	// COM 라이브러리 초기화
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	// D2D Factory 객체 생성
	if (S_OK != D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gp_factory)) {
		return 0;
	}


	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PROJECT11OMOKD2D, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT11OMOKD2D));

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

	// 사용하던 Factory 객체 제거
	gp_factory->Release();

	// COM 사용 해제
	CoUninitialize();

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
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT11OMOKD2D));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)NULL;
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT11OMOKD2D);
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
