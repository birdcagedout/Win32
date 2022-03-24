// Project1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project2DC.h"
#include <WinUser.h>
#include <windowsx.h>

int g_isClicked = 0;	// 마우스 버튼 클릭되면 1, 클릭 안 된 상태면 0
int g_x0 = 0;			// 마우스 클릭시 기준점 x좌표
int g_y0 = 0;			// 마우스 클릭시 기준점 y좌표


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// WM_DESTROY 처리가 없으면 ==> 창은 닫혀도 좀비 프로세스로 살아있다.
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (uMsg == WM_LBUTTONDOWN) {
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
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT2DC));
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