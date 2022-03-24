// Project9PEN.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project9PEN.h"

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




int g_color = 0;       // RBUTTONDOWN      : 펜색깔 바꾸기
int g_style = 0;        // RBUTTONDBLCLK    : 펜모양 바꾸기

// 펜 스타일 : PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    // 왼쪽버튼 클릭시 브러시 색깔을 바꾼다
    else if (message == WM_LBUTTONDOWN) {
        HDC h_dc = GetDC(hWnd);										// DC생성
        HPEN h_oldPen, h_newPen;
        COLORREF col;

        // 펜 색깔
        if (g_color == 0) {
            col = RGB(255, 0, 0);		// R펜
        }
        else if (g_color == 1) {
            col = RGB(0, 255, 0);		// G펜
        }
        else {
            col = RGB(0, 0, 255);		// B펜
        }

        // 펜 모양
        if (g_style == 0) {
            h_newPen = CreatePen(PS_SOLID, 1, col);
        }
        else if (g_style == 1) {
            h_newPen = CreatePen(PS_DASH, 1, col);
        }
        else if (g_style == 2) {
            h_newPen = CreatePen(PS_DOT, 1, col);
        }
        else if (g_style == 3) {
            h_newPen = CreatePen(PS_DASHDOT, 1, col);
        }
        else if (g_style == 4) {
            h_newPen = CreatePen(PS_DASHDOTDOT, 1, col);
        }
        else {
            h_newPen = CreatePen(PS_NULL, 1, col);
        }

        h_oldPen = (HPEN)SelectObject(h_dc, h_newPen);		        // 새로운 펜으로 교체
        MoveToEx(h_dc, 100, 100, NULL);
        LineTo(h_dc, 300*(g_color+1), 300*(g_style+1));
        h_newPen = (HPEN)SelectObject(h_dc, h_oldPen);		        // 헌 펜로 원래대로 교체
        DeleteObject(h_newPen);
        ReleaseDC(hWnd, h_dc);
    }
    // 우클릭 할 때마다 펜 색깔 바뀜: 0클릭=R, 1클릭=G, 2클릭=B
    else if (message == WM_RBUTTONDOWN) {
        if (g_color == 0) {
            g_color = 1;
        }
        else if (g_color == 1) {
            g_color = 2;
        }
        else {
            g_color = 0;
        }
    }
    // 중간버튼 클릭할 때마다 펜 스타일 바뀜: 0클릭=실선, 1클릭=대쉬, 2클릭=점선, 3클릭=대쉬점선, 4클릭=대쉬점점선, 5클릭=선없음
    else if (message == WM_MBUTTONDOWN) {
        if (g_style == 0) {
            g_style = 1;
        }
        else if (g_style == 1) {
            g_style = 2;
        }
        else if (g_style == 2) {
            g_style = 3;
        }
        else if (g_style == 3) {
            g_style = 4;
        }
        else if (g_style == 4) {
            g_style = 5;
        }
        else {
            g_style = 0;
        }
    }
    else if (message == WM_DESTROY) {
        PostQuitMessage(0);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
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
    LoadStringW(hInstance, IDC_PROJECT9PEN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT9PEN));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT9PEN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT9PEN);
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
