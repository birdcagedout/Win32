// Project17Direct2DImage1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Project17Direct2DImage1.h"

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
// IWICImagingFactory 추가
#include <wincodec.h>

#include <windows.h>		// Sleep(밀리세컨드)

#pragma comment(lib, "D2D1.lib")		// Direct2D에서 자주 사용하는 namespace 생략하도록 설정
using namespace D2D1;


// Direct2D를 구성하는 각종 객체 생성하는 Factory
ID2D1Factory* gp_factory;

// Direct2D에서 클라이언트 영역에 그림 그릴 객체
ID2D1HwndRenderTarget* gp_renderTarget;

// 기본 renderTarget에서 사용할 비트맵 객체
ID2D1Bitmap* gp_bitmap;

// PNG 이미지를 출력할 좌표
D2D1_RECT_F g_imageRect;

// 이미지 파일 이름
const wchar_t g_imageFileName[100] = L"smile.gif";



// 이미지 읽어오는 함수
int LoadMyImage(HWND ah_Wnd, ID2D1RenderTarget* ap_target, const wchar_t* ap_path)
{
	// 기존 읽어온 이미지가 있으면 해제
	if (gp_bitmap != NULL) {
		gp_bitmap->Release();
		gp_bitmap = NULL;
	}

	// WIC객체 생성을 위한 factory 객체
	IWICImagingFactory* p_wicFactory;
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&p_wicFactory));

	IWICBitmapDecoder* p_decoder = NULL;		// 이미지 디코더
	IWICBitmapFrameDecode* p_frame = NULL;		// 특정 이미지
	IWICFormatConverter* p_converter = NULL;	// 이미지 컨버터

	int result = 0;						// 그림 파일을 읽은 결과 값 (0이면 그림 읽기 실패, 1이면 그림 읽기 성공)
	
	// WIC용 Factory 객체를 사용하여 이미지 디코더 객체를 생성
	if (p_wicFactory->CreateDecoderFromFilename(ap_path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &p_decoder) == S_OK) {
		
		// 파일을 구성하는 이미지 중에서 첫번째 이미지를 선택한다.
		for (int nImageIndex = 0; nImageIndex <= 25; nImageIndex++) {
			
			if (p_decoder->GetFrame(nImageIndex, &p_frame) == S_OK) {

				// IWICBitmap형식의 비트맵을 ID2D1Bitmap형식으로 변환하기 위한 객체 생성
				if (p_wicFactory->CreateFormatConverter(&p_converter) == S_OK) {

					// 선택된 그림을 어떤 형식의 비트맵으로 변환할 것인지 설정한다.
					if (p_converter->Initialize(p_frame,
												GUID_WICPixelFormat32bppPBGRA,
												WICBitmapDitherTypeNone,
												NULL, 0.0f, WICBitmapPaletteTypeCustom) == S_OK) {

						// IWICBitmap 형식의 비트맵으로 ID2D1Bitmap 객체를 생성한다.
						if (ap_target->CreateBitmapFromWicBitmap(p_converter, NULL, &gp_bitmap) == S_OK) {
							result = 1;		// 성공적으로 생성한 경우
						}
					}
				}
			}
			Sleep(100);
			InvalidateRect(ah_Wnd, NULL, TRUE);
		}
	}

	if(p_converter != NULL) p_converter->Release();				// 이미지 변환 객체 제거
	if(p_frame != NULL) p_frame->Release();					// 그림파일에 있는 이미지를 선택하기 위해 사용한 객체 제거
	p_decoder->Release();				// 압축을 해제하기 위해 생성한 객체 제거
	p_wicFactory->Release();			// WIC를 사용하기 위해 만들었던 Factory 객체 제거

	return result;  // PNG 파일을 읽은 결과를 반환한다.
}



// 윈도 메시지 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) {
		// 클라이언트 영역 얻기
		RECT r;
		GetClientRect(hWnd, &r);

		// Render Target 생성
		gp_factory->CreateHwndRenderTarget(
			RenderTargetProperties(), 
			HwndRenderTargetProperties(hWnd, SizeU(r.right, r.bottom)), 
			&gp_renderTarget);

		// 그림 그릴 영역 저장
		g_imageRect.right = (float)r.right;
		g_imageRect.bottom = (float)r.bottom;

		// bitmap객체에 이미지 생성
		LoadMyImage(hWnd, gp_renderTarget, g_imageFileName);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN) {

	}
	else if (uMsg == WM_RBUTTONDOWN) {

	}
	else if (uMsg == WM_PAINT) {
		// WM_PAINT 메시지가 발생하지 않게 막는다
		//ValidateRect(hWnd, NULL);

		// Direct2D의 Render Target을 이용해 그림 그리기 시작.
		gp_renderTarget->BeginDraw();

		// 정상적으로 이미지를 읽어왔으면 DrawBitmap()로 화면에 출력
		if (gp_bitmap != NULL) {
			gp_renderTarget->DrawBitmap(gp_bitmap, &g_imageRect);
		}
		// 이미지를 읽어오지 못했다면 검은색이 출력되므로 ==> 윈도 영역을 하늘색으로 채움
		else {
			gp_renderTarget->Clear(ColorF(0.0f, 0.8f, 1.0f));
		}
		// 그림 그리기 끝
		gp_renderTarget->EndDraw();

		return 0;
	}
	else if (uMsg == WM_DESTROY) {
		//Render Target을 사용했다면 해제
		if (gp_renderTarget != NULL) {
			gp_renderTarget->Release();
			gp_renderTarget = NULL;
		}
		PostQuitMessage(0);
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


	// COM 초기화
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	// Direct2D Factory 객체
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gp_factory);


	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PROJECT17DIRECT2DIMAGE1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT17DIRECT2DIMAGE1));

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
	if (gp_bitmap != NULL) {
		gp_bitmap->Release();
	}
	// Factory 해제
	gp_factory->Release();
	// COM 해제
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT17DIRECT2DIMAGE1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hbrBackground = (HBRUSH)NULL;					// Direct2D로 클라이언트영역 전체를 그리므로 BRUSH 설정 안함

	//wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROJECT17DIRECT2DIMAGE1);
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
		100, 100, 200, 230, nullptr, nullptr, hInstance, nullptr);

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
