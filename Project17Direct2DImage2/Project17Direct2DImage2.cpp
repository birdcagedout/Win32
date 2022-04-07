// Project17Direct2DImage2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project17Direct2DImage2.h"

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


//====================================================================================
#include <d2d1.h>					// Direct2D 사용
#include <wincodec.h>				// IWICImagingFactory 사용

#pragma comment(lib, "D2D1.lib")	// Direct2D용 네임스페이스 생략
using namespace D2D1;

ID2D1Factory* g_pFactory;				// Factory객체
ID2D1HwndRenderTarget* g_pTarget;		// 클라이언트에 그림 그릴 객체

ID2D1Bitmap* g_pBitmap;					// 기본 bitmap
D2D1_RECT_F g_imageRect;				// 이미지 출력 영역
D2D1_POINT_2F g_centerPos;				// 회전시 중심점
float g_degree = 0.0f;					// 회전 각도(0~360도)
int g_imageIndex = 0;					// smile.gif 프레임 인덱스(0~25)
//const wchar_t *g_pImagePath = L"test_672_504.png";
const wchar_t* g_pImagePath = L"smile.gif";


// 이미지 크기에 맞게 Render Target 크기를 (재)조정
void ResizeTarget(IWICBitmapFrameDecode* a_pImageFrame)
{
	// 이미지 크기 저장
	UINT width, height;
	a_pImageFrame->GetSize(&width, &height);

	// 설정된 크기와 현재 이미지 크기가 맞지 않으면 크기 조정
	if (g_imageRect.right != width || g_imageRect.bottom != height) {

		// Render Target 재조정
		g_pTarget->Resize(D2D1::SizeU(width, height));

		// 이미지 출력 영역을 실제 이미지 크기로 변경
		g_imageRect.right = (FLOAT)width;
		g_imageRect.bottom = (FLOAT)height;

		// 이미지 회전시 중심점 설정
		g_centerPos.x = g_imageRect.right / 2;
		g_centerPos.y = g_imageRect.bottom / 2;
	}
}


// IWICBitmap으로부터 D2D1Bitmap 만들기
int MakeD2D1Bitmap(IWICImagingFactory* a_pWicFactory, IWICBitmapFrameDecode* a_pImageFrame)
{
	// 컨버터 객체
	IWICFormatConverter* p_converter;

	// 반환값 (성공=1, 실패=0)
	int result = 0;

	// IWICBitmap ==> ID2D1Bitmap 변환
	if (a_pWicFactory->CreateFormatConverter(&p_converter) == S_OK) {

		// 비트맵 포맷 설정(초기화)
		if (p_converter->Initialize(a_pImageFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom) == S_OK) {

			// IWICBitmap ==> ID2D1Bitmap
			if (g_pTarget->CreateBitmapFromWicBitmap(p_converter, NULL, &g_pBitmap) == S_OK) {
				result = 1;
			}
		}
		// 변환객체 해제
		p_converter->Release();
	}
	return result;
}


// 이미지 불러오기
int LoadMyImage()
{
	// 기존 이미지 제거
	if (g_pBitmap != NULL) {
		g_pBitmap->Release();
		g_pBitmap = NULL;
	}

	// 1. Factory 생성
	IWICImagingFactory* p_wicFactory;
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&p_wicFactory));

	// 이미지 decoder, 이미지 frame
	IWICBitmapDecoder* p_decoder;
	IWICBitmapFrameDecode* p_frame;

	// 리턴값 (1=성공, 0=실패)
	int result = 0;

	// 2. decoder 생성
	if (p_wicFactory->CreateDecoderFromFilename(g_pImagePath, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &p_decoder) == S_OK) {

		// 3. decoder의 GetFrame()으로 파일 중 g_imageIndex번째의 이미지 얻기
		if (p_decoder->GetFrame(g_imageIndex, &p_frame) == S_OK) {

			// 그림 크기에 맞게 Target 크기 조정
			ResizeTarget(p_frame);

			// IWICBitmap ==> D2D1Bitmap
			result = MakeD2D1Bitmap(p_wicFactory, p_frame);
			p_frame->Release();
		}
		p_decoder->Release();
	}
	p_wicFactory->Release();

	return result;
}


//============ WM 메시지별 처리함수 =================
void OnCreate(HWND a_hWnd)
{
	RECT clientRect, winRect;
	GetClientRect(a_hWnd, &clientRect);

	// Render Target 생성
	g_pFactory->CreateHwndRenderTarget(RenderTargetProperties(),
										HwndRenderTargetProperties(a_hWnd, SizeU(clientRect.right, clientRect.bottom)), 
										&g_pTarget);
	// 이미지 생성
	if (LoadMyImage() == 1) {
		// 윈도 크기 얻기
		GetWindowRect(a_hWnd, &winRect);

		// 윈도 크기 재조정
		SetWindowPos(a_hWnd, NULL, 0, 0,
			winRect.right - winRect.left + (int)(g_imageRect.right) - clientRect.right,		// 새로운 width
			winRect.bottom - winRect.top + (int)(g_imageRect.bottom) - clientRect.bottom,	// 새로운 height
			SWP_NOMOVE);		// 현재 윈도 위치 그대로(크기만 바뀜)

		// 0.05초 타이머
		SetTimer(a_hWnd, 1, 50, NULL);
	}
}


void OnTimer(HWND a_hWnd, UINT a_timerID)
{
	if (a_timerID == 1) {
		/*g_degree = g_degree + 0.5f;

		if (g_degree >= 360) {
			g_degree = 0.0f;
		}*/

		g_imageIndex++;

		if (g_imageIndex >= 25) {
			g_imageIndex = 0;
		}

		LoadMyImage();

		InvalidateRect(a_hWnd, NULL, FALSE);		// WM_PAINT 발생시킴 (배경지우기 X)
	}

}


void OnPaint()
{
	// Direct2D의 Render Target으로 그리기 시작
	g_pTarget->BeginDraw();

	// 이미지 읽기 실패시 검은색 ==> 윈도 전체를 하늘색으로
	g_pTarget->Clear(ColorF(0.0f, 0.8f, 1.0f));

	// 이미지 읽었다면 DrawBitmap()으로 이미지 출력
	if (g_pBitmap != NULL) {
		
		// 회전해서 출력 ==> 출력 전에 한번만 적용되므로 DrawBitmap() 먼저하든 후에하든 결과는 동일
		//g_pTarget->SetTransform(Matrix3x2F::Rotation(g_degree, g_centerPos));
		g_pTarget->DrawBitmap(g_pBitmap, &g_imageRect);
	}

	// 그리기 중지
	g_pTarget->EndDraw();
}




void OnDestroy(HWND a_hWnd)
{
	KillTimer(a_hWnd, 1);

	if (g_pTarget != NULL) {
		g_pTarget->Release();
		g_pTarget = NULL;
	}

	PostQuitMessage(0);
}



// 윈도 메시지 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) {
		OnCreate(hWnd);
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);			// Minimize 버튼 disable
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);			// Maximize 버튼 disable
		EnableMenuItem(GetSystemMenu(hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);			// Close 버튼 enable
		return 0;
		INT_MAX;
	}
	else if (uMsg == WM_LBUTTONDOWN) {

	}
	else if (uMsg == WM_RBUTTONDOWN) {

	}
	else if (uMsg == WM_TIMER) {
		OnTimer(hWnd, wParam);			// wParam=timerID
		return 0;
	}
	else if (uMsg == WM_PAINT) {
		ValidateRect(hWnd, NULL);
		OnPaint();
		return 0;
	}
	else if (uMsg == WM_DESTROY) {
		OnDestroy(hWnd);
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

	// Direct2D Factory객체 생성
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pFactory);

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PROJECT17DIRECT2DIMAGE2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT17DIRECT2DIMAGE2));

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

	// 이미지 해제
	if (g_pBitmap != NULL) {
		g_pBitmap->Release();
	}

	// Factory 해제
	g_pFactory->Release();

	// COM 라이브러리 해제
	CoUninitialize();

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT17DIRECT2DIMAGE2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hbrBackground = (HBRUSH)NULL;
	//wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROJECT17DIRECT2DIMAGE2);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
		CW_USEDEFAULT, 0, 300, 200, nullptr, nullptr, hInstance, nullptr);

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
