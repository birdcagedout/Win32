// Project6ClickNMove.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project6ClickNMove.h"
#include <Windowsx.h>

int g_isClicked = 0;					// 사각형 내부클릭시에만 1
RECT g_rect = { 10, 10, 50, 50 };		// (left, top, right, bottom)
POINT g_prevPos = { 10, 10 };				// 처음 클릭한 이전 위치


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		// 클릭한 위치가 사각형 내부인지 확인
		/*int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		if (x >= g_rect.left && x <= g_rect.right && y >= g_rect.top && y <= g_rect.bottom) {
			g_isClicked = 1;
		}*/

		// 클릭한 위치가 사각형 내부인지 확인하는 간편한 방법
		g_prevPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		if (PtInRect(&g_rect, g_prevPos)) {
			g_isClicked = 1;
			SetCapture(hWnd);		// 마우스가 클라이언트영역을 벗어나도 계속 메시지 받겠다
		}

	}
	else if (uMsg == WM_LBUTTONUP) {
		g_isClicked = 0;
		ReleaseCapture();			// 마우스가 계속 메시지 받는 것을 그만둠
	}
	else if (uMsg == WM_MOUSEMOVE) {
		if (g_isClicked == 1) {
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);

			// 이동거리 구해서 g_rect 4변에 더해줌
			int x_distance = x - g_prevPos.x;
			int y_distance = y - g_prevPos.y;
			/*
			g_rect.left += x_distance;
			g_rect.top += y_distance;
			g_rect.right += x_distance;
			g_rect.bottom += y_distance;*/

			// 이동거리 구해서 g_rect 4변에 더해주는 API함수
			OffsetRect(&g_rect, x_distance, y_distance);
			InvalidateRect(hWnd, NULL, TRUE);	// WM_PAINT 강제 발생 : 잔상 제거를 위해 여기서 Rectangle호출X

			g_prevPos.x = x;
			g_prevPos.y = y;
		}
	}
	else if (uMsg == WM_PAINT) {
		// 정상적인 경우 : BeginPaint ~ EndPaint ~ return 0		==> 우선순위 낮아 나중에 처리(자원소모 낮아 처리가 빠름)
		PAINTSTRUCT ps;
		HDC h_dc = BeginPaint(hWnd, &ps);
		Rectangle(h_dc, g_rect.left, g_rect.top, g_rect.right, g_rect.bottom);

		EndPaint(hWnd, &ps);
		return 0;		// DefWindowProc에서 또다시 BeginPaint ~ EndPaint 작업을 한다(그렇다고 자동으로 그려주지는 않음)
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;

	wchar_t my_class_name[] = L"tipssoft";
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT6CLICKNMOVE));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = my_class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(my_class_name, L"www.tipssoft.com", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}