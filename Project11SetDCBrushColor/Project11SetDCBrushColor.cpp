// Project11SetDCBrushColor.cpp : 애플리케이션에 대한 진입점을 정의합니다.
// GetStockObject로 사용할 수 있는 GDI객체 DC_BRUSH를 사용하면
// 1) 코드가 단순해지고 (오류 낮춤)
// 2) 메모리 적게 사용 (OS제공 객체이므로)
// 3) 실행속도 빨라짐? (객체 생성 필요X) ==> 실험해본 결과 이건 아니다.

#include "pch.h"
#include "framework.h"
#include "Project11SetDCBrushColor.h"
#include <cstdlib>		// 표준헤더 for srand(), rand():0~32767
#include <ctime>		// 표준헤더 for time()

#define MAX_LOADSTRING	100
#define MAX_BRUSH		30000

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 더블클릭 메시지를 받으려면 윈도 클래스의 스타일에 CS_DBLCLKS 추가해야함!!

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		HDC h_dc = GetDC(hWnd);
		SelectObject(h_dc, GetStockObject(DC_BRUSH));		// DC_BRUSH를 얻어서 현재 brush로 설정
		SetDCBrushColor(h_dc, RGB(255, 100, 100));			// DC_BRUSH로 얻어온 brush의 색상 변경
		Rectangle(h_dc, 100, 100, 200, 200);
		ReleaseDC(hWnd, h_dc);

		// 주의: GetDC() 다시 호출할 때(또는 ReleaseDC() 호출 후) DC_BRUSH는 초기화된다.(==> 주로 흰색)
		//       즉, SetDCBrushColor()로 설정한 brush가 유지되지 않는다.
		h_dc = GetDC(hWnd);
		Rectangle(h_dc, 300, 100, 400, 200);

		// ReleaseDC() 호출 안 한 상태에서 GetDC()만 다시 호출한 경우 ==> 유지될까? ==> 안 된다!
		SetDCBrushColor(h_dc, RGB(0, 0, 255));				// DC_BRUSH로 얻어온 brush의 색상 변경
		h_dc = GetDC(hWnd);
		Rectangle(h_dc, 500, 100, 600, 200);
		ReleaseDC(hWnd, h_dc);
	}
	// DC_BRUSH 설정값이 유지되는 경우 ==> GetDC ~ ReleaseDC 사이에 다른 brush를 설정/사용하더라도 DC_BRUSH 유지
	else if (uMsg == WM_RBUTTONDOWN) {
		HDC h_dc = GetDC(hWnd);
		SelectObject(h_dc, GetStockObject(DC_BRUSH));		// DC_BRUSH 불러와서 설정
		SetDCBrushColor(h_dc, RGB(0, 255, 0));				// DC_BRUSH 색깔 = 녹색
		Rectangle(h_dc, 100, 300, 200, 400);				// 녹색 brush 사용

		SelectObject(h_dc, GetStockObject(GRAY_BRUSH));		// 회색 brush 설정
		Rectangle(h_dc, 300, 300, 400, 400);				// 회색 brush 사용

		SelectObject(h_dc, GetStockObject(DC_BRUSH));		// 다시 DC_BRUSH 불러옴
		Rectangle(h_dc, 500, 300, 600, 400);				// 어떤 색이 나올까? ==> 녹색(DC_BRUSH는 유지된다)

		ReleaseDC(hWnd, h_dc);
	}

	// CreateSolidBrush로 객체를 생성하는 경우 vs. GetStockObject(DC_BRUSH)를 사용하는 경우
	// 어느쪽의 성능이 더 좋을까 ==> 의외로 차이가 없다.
	// 어느쪽이 코드가 간결할까 ==> DC_BRUSH 윈!
	
	// 왼쪽버튼 더블클릭 : CreateSolidBrush로 객체를 생성
	else if (uMsg == WM_LBUTTONDBLCLK) {
		srand((unsigned int)time(NULL));

		HDC h_dc = GetDC(hWnd);
		HBRUSH h_brush[MAX_BRUSH];

		for (int i = 0; i < MAX_BRUSH; i++) {
			h_brush[i] = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
			SelectObject(h_dc, h_brush[i]);
			Rectangle(h_dc, rand() % 100, rand() % 100, 500 + rand() % 500, 300 + rand() % 300);
			DeleteObject(h_brush[i]);
		}
		ReleaseDC(hWnd, h_dc);
	}
	// 오른버튼 더블클릭 : GetStockObject(DC_BRUSH)를 사용
	else if (uMsg == WM_RBUTTONDBLCLK) {
		HDC h_dc = GetDC(hWnd);

		for (int i = 0; i < MAX_BRUSH; i++) {
			SelectObject(h_dc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(h_dc, RGB(rand() % 256, rand() % 256, rand() % 256));
			Rectangle(h_dc, rand() % 100, rand() % 100, 500 + rand() % 500, 300 + rand() % 300);
		}
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
    LoadStringW(hInstance, IDC_PROJECT11SETDCBRUSHCOLOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT11SETDCBRUSHCOLOR));

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

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT11SETDCBRUSHCOLOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT11SETDCBRUSHCOLOR);
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
