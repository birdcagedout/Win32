// Project1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project1.h"
#include <sstream>
#include <iostream>
#include <windows.h>
#include <windowsx.h>

int g_isClicked = 0;	// 마우스 버튼 클릭되면 1, 클릭 안 된 상태면 0
int g_x0 = 0;			// 마우스 클릭시 최초 x좌표
int g_y0 = 0;			// 마우스 클릭시 최초 y좌표


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// WM_DESTROY 처리가 없으면 ==> 창은 닫혀도 좀비 프로세스로 살아있다.
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		/* 실험1. 메시지박스 띄우기
		//MessageBox(hWnd, L"왼쪽버튼 눌렸지롱", L"알림", MB_OK);
		*/
		
		/* 실험2. 마우스 왼쪽버튼 눌린 위치에 사각형 (컨트롤키 추가로 누르면 원 그리기)
		HDC h_dc = GetDC(hWnd);
		//int x = lParam & 0x0000FFFF;			// 하위 2바이트 = 클릭한 위치의 x값
		//int y = (lParam >> 16) & 0x0000FFFF;	// 상위 2바이트 = 클릭한 위치의 y값
		int x = GET_X_LPARAM(lParam);		// windowsx.h
		int y = GET_Y_LPARAM(lParam);		// windowsx.h

		// Ctrl키를 누른 상태이면 타원 그리기
		if (MK_CONTROL & wParam) {
			Ellipse(h_dc, x, y, x + 200, y + 200);
		}
		else {
			Rectangle(h_dc, x, y, x + 200, y + 200);	// 시작x, 시작y, 끝x, 끝y
		}

		ReleaseDC(hWnd, h_dc);
		*/

		// 실험3. 시스템 메트릭으로 스크린 크기 등 정보 얻어오기 ==> 구현 실패 
		// ==> 9강 앞부분 블로그 WM_MOVING 내용 보고 구현 성공 : https://blog.naver.com/tipsware/221233728191
		int scr_x = ::GetSystemMetrics(SM_CXSCREEN);
		int scr_y = ::GetSystemMetrics(SM_CYSCREEN);
		int scr_full_x = ::GetSystemMetrics(SM_CXFULLSCREEN);
		int scr_full_y = ::GetSystemMetrics(SM_CYFULLSCREEN);
		int title_bar = ::GetSystemMetrics(SM_CYCAPTION);
		int num_monitors = ::GetSystemMetrics(SM_CMONITORS);

		wchar_t str[500];
		swprintf_s(str, 500, L"[Screen: %d, %d] [ScreenFull: %d, %d] [titlebar: %d] [num_monitor: %d]", scr_x, scr_y, scr_full_x, scr_full_y, title_bar, num_monitors);
		SetWindowText(hWnd, str);

		// 4K에서 배율 100% : [Screen: 3840, 2160] [ScreenFull: 3840, 2097] [titlebar: 23] [num_monitor: 1]
		// 4K에서 배율 125% : [Screen: 3072, 1728] [ScreenFull: 3072, 1665] [titlebar: 23] [num_monitor: 1]
		// 4K에서 배율 150% : [Screen: 2560, 1440] [ScreenFull: 2560, 1377] [titlebar: 23] [num_monitor: 1]
		

		/* 실험5. 선그리기*/
		g_isClicked = 1;

		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		g_x0 = x;
		g_y0 = y;
	}
	else if (uMsg == WM_LBUTTONUP) {
		g_isClicked = 0;

	}
	else if (uMsg == WM_MOUSEMOVE) {
		/* 실험4. 마우스 이동중에 + L버튼 누른상태 ==> 계속 그리기 
		if (wParam & MK_LBUTTON) {
			HDC h_dc = GetDC(hWnd);
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			Ellipse(h_dc, x - 15, y - 15, x + 15, y + 15);
			ReleaseDC(hWnd, h_dc);
		}
		*/

		/* 실험5. 선그리기*/
		HDC h_dc = GetDC(hWnd);
		MoveToEx(h_dc, g_x0, g_y0, NULL);

		if (g_isClicked) {
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);

			LineTo(h_dc, x, y);
			g_x0 = x;
			g_y0 = y;
		}
		ReleaseDC(hWnd, h_dc);
	}
	

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 최소화, 최대화 등 기본적인 동작 처리
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 윈도 클래스 등록: 3가지가 중요
	WNDCLASS wc;

	wchar_t my_class_name[] = L"tipssoft";
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
	wc.hInstance = hInstance;			// 중요
	wc.lpfnWndProc = WndProc;			// 중요
	wc.lpszClassName = my_class_name;	// 중요
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	// 윈도 만들기 + 보이기
	HWND hWnd = CreateWindow(my_class_name, L"www.tipssoft.com",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 메시지 루프
	// GetMessage()는 보통은 0 아닌 값을 반환. WM_QUIT 메시지 들어오면 0 반환
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}