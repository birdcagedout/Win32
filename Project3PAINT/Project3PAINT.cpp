// Project3PAINT.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project3PAINT.h"
#include <WinUser.h>
#include <Windowsx.h>

#define MAX_COUNT 10000
int g_isClicked = 0;					// 클릭시 1, 클릭 떼면 0
int g_count = 0;						// 좌표가 저장된 개수
int g_x[MAX_COUNT], g_y[MAX_COUNT];		// 클릭된 좌표들을 저장할 배열

int g_isDisconnected[MAX_COUNT];		// 비연결점을 저장할 배열: LBUTTONUP(끊어짐)이 될 때마다 그때의 g_counter를 저장
int g_disconnCount = 0;					// 비연결점의 개수를 저장


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		g_isClicked = 1;

		if (g_count < MAX_COUNT) {
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);

			g_x[g_count] = x;
			g_y[g_count] = y;
			g_count++;
		}
	}
	else if (uMsg == WM_LBUTTONUP) {
		g_isClicked = 0;
		g_isDisconnected[g_disconnCount] = g_count;		// 이때 g_count값이 k이면 좌표[k-1] ~ 좌표[k] 사이는 연결되지 않음
		g_disconnCount++;
	}
	else if (uMsg == WM_MOUSEMOVE) {
		if (g_isClicked && g_count < MAX_COUNT) {
			HDC h_dc = GetDC(hWnd);
			MoveToEx(h_dc, g_x[g_count-1], g_y[g_count-1], NULL);

			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);

			LineTo(h_dc, x, y);

			g_x[g_count] = x;
			g_y[g_count] = y;
			g_count++;
		}
		
	}
	// 윈도를 다시 그려야 할 때 WM_PAINT 메시지 발생
	// 무효화된 영역을 다시 그릴 책임 ==> 프로그램에 있음(OS가 자동으로 그려주지 않음)
	// 반드시 BeginPaint ~ EndPaint 사용할 것
	else if (uMsg == WM_PAINT) {

		// 정상적인 경우 : BeginPaint ~ EndPaint ~ return 0		==> 우선순위 낮아 나중에 처리(자원소모 낮아 처리가 빠름)
		PAINTSTRUCT ps;
		HDC h_dc = BeginPaint(hWnd, &ps);

		if (g_count > 0) {
			MoveToEx(h_dc, g_x[0], g_y[0], NULL);
			
			int disconnCountIndex = 0;				// 비연결점 배열 돌릴 더미 index
			int disconnCount = g_disconnCount;		// 비연결점 처리블록 종료용 더미변수
			for (int i = 1; i < g_count; i++) {
				if ((disconnCount > 0) && (i == g_isDisconnected[disconnCountIndex])) {	// 비연결점 처리 블록
					MoveToEx(h_dc, g_x[i], g_y[i], NULL);
					disconnCountIndex++;
					if (disconnCountIndex == disconnCount) {								// 비연결점 마지막
						disconnCount = 0;													// 비연결점 처리 블록 종료
					}
				}
				else {
					LineTo(h_dc, g_x[i], g_y[i]);
				}
			}
		}

		EndPaint(hWnd, &ps);
		return 0;		// DefWindowProc에서 또다시 BeginPaint ~ EndPaint 작업을 한다(그렇다고 자동으로 그려주지는 않음)

		// DC로 처리하는 경우 : GetDC ~ ReleaseDC				==> 같은 우선순위(자원소모 커서 처리가 느림)	==> 금지
		/*HDC h_dc = GetDC(hWnd);
		Rectangle(h_dc, 10, 10, 100, 100);
		ReleaseDC(hWnd, h_dc);
		return 0;*/

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
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT3PAINT));
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