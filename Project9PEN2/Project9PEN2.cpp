// Project9PEN2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project9PEN2.h"

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



// 펜 스타일 : PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL

/***************************************************************************
* 유니코드 사용
* 1. char ==> wchar_t (wchar_t가 표준, WCHAR는 윈도only짭)
* 2. 문자열 리터럴에 L붙임
* 3. API함수 중 A로 끝나는 함수 ==> A없는 함수/W붙은 함수로 고쳐줌
* 4. C함수 중 str로 시작하는 함수 ==> wcs로 시작하는 함수로 고쳐줌
****************************************************************************/


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    // 왼쪽버튼 클릭시 펜 스타일 보여줌
    // https://blog.naver.com/tipsware/221120557339
    else if (message == WM_LBUTTONDOWN) {
        HDC h_dc = GetDC(hWnd);
        HPEN h_pen;
        //const char* p_style_names[6]    = { "PS_SOLID", "PS_DASH", "PS_DOT", "PS_DASHDOT", "PS_DASHDOTDOT", "PS_NULL" };    // 펜 스타일 이름
        const wchar_t* p_style_names[6]  = {L"PS_SOLID", L"PS_DASH", L"PS_DOT", L"PS_DASHDOT", L"PS_DASHDOTDOT", L"PS_NULL"};    // 펜 스타일 이름
        int pen_style[6]                = { PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL };                // 스타일 인자
        //SetBkMode(h_dc, TRANSPARENT);                               // 점선 사이 배경색(기본=흰색)을 지우기 위해
        HGDIOBJ h_oldPen = GetCurrentObject(h_dc, OBJ_PEN);         // 미리 기본펜 백업

        for (int i = 0; i < 6; i++) {
            //TextOutA(h_dc, 10, 10 + i * 20, p_style_names[i], strlen(p_style_names[i]));       // 펜 스타일 이름 쓰기(아스키 버전)
            TextOut(h_dc, 10, 10 + i * 20, p_style_names[i], wcslen(p_style_names[i]));       // 펜 스타일 이름 쓰기(유니코드 버전)
            h_pen = CreatePen(pen_style[i], 1, RGB(0, 0, 255));
            SelectObject(h_dc, h_pen);
            MoveToEx(h_dc, 200, 15 + i * 20, NULL);
            LineTo(h_dc, 400, 15 + i * 20);
            DeleteObject(h_pen);
        }

        SelectObject(h_dc, h_oldPen);
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
    LoadStringW(hInstance, IDC_PROJECT9PEN2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT9PEN2));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT9PEN2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT9PEN2);
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
