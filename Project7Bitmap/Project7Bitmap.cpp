// Project7Bitmap.cpp : 애플리케이션에 대한 진입점을 정의합니다.
// BitBlt : Bitmap Block Transfer : DC간 bitmap 복사

#include "pch.h"
#include "framework.h"
#include "Project7Bitmap.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		HDC h_screendc = GetWindowDC(NULL);		// 화면 전체의 DC 얻어옴
		//HDC h_screendc = GetDC(NULL);
		HDC h_dc = GetDC(hWnd);

		// 굉장히 많이 쓰는 BitBlt함수
		// (사본DC, x사본, y사본, x사본크기, y사본크기,		==> 사본DC와 사본좌표 4개
		//  원본DC, x원본, y원본, 복사옵션)					==  원본DC와 원본좌표 2개 + 옵션
		// 복사옵션: SRCCOPY, SRCPAINT, SRCAND, SRCINVERT, SRCERASE, NOTSRCCOPY, 
		//           NOTSRCERASE, MERGECOPY, MERGEPAINT, PATCOPY, PATPAINT,
		//           PATINVERT, DSTINVERT, BLACKNESS, WHITENESS
		BitBlt(h_dc, 0, 0, 1000, 1000, h_screendc, 0, 0, SRCCOPY);

		
		ReleaseDC(NULL, h_screendc);
		ReleaseDC(hWnd, h_dc);
	}
	else if (uMsg == WM_LBUTTONUP) {
	
	}
	else if (uMsg == WM_MOUSEMOVE) {
	
	}
	else if (uMsg == WM_PAINT) {
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
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT7BITMAP));
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