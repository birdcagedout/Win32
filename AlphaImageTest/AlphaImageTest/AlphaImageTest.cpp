// AlphaImageTest.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "AlphaImageTest.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


#include <d2d1.h>					// Direct2D 사용
#include <wincodec.h>				// IWICImagingFactory 사용

#pragma comment(lib, "D2D1.lib")	// Direct2D용 네임스페이스 생략
using namespace D2D1;

ID2D1Factory*				g_ipD2DFactory = nullptr;
ID2D1HwndRenderTarget*		g_ipRT = nullptr;
IWICImagingFactory*			g_ipWICFactory = nullptr;
IWICFormatConverter*		g_ipConvertedSrcBmp = nullptr;
ID2D1Bitmap*				g_ipTransparentSquirrelBitmap = nullptr;
ID2D1Bitmap*				g_ipOpacitySquirrelBitmap = nullptr;



void InitializeD2D( void )
{

	HRESULT hr = E_FAIL;

	//-----------------------------------------------------------------------
	//	D2D Factory 를 생성한다.
	//-----------------------------------------------------------------------
	hr = ::D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, 
							  &::g_ipD2DFactory );
	assert( hr == S_OK );

	//-----------------------------------------------------------------------
	//	Windows Imaging Component Factory 를 생성한다.
	//-----------------------------------------------------------------------
	hr = ::CoCreateInstance( CLSID_WICImagingFactory,
							 NULL, CLSCTX_INPROC_SERVER,
							 IID_PPV_ARGS( &::g_ipWICFactory ) );
	assert( hr == S_OK );

}

void InitializeRT( HWND hWnd )
{	

	assert( ::g_ipRT == nullptr );
	assert( hWnd != 0 );

	HRESULT hr = E_FAIL;
	RECT rc;
	::GetClientRect( hWnd, &rc );
	

	D2D1_RENDER_TARGET_PROPERTIES dxRTProperties = ::D2D1::RenderTargetProperties();	
	D2D1_SIZE_U dxSize = ::D2D1::SizeU( rc.right - rc.left,
										rc.bottom - rc.top );
	
	hr = ::g_ipD2DFactory->CreateHwndRenderTarget( dxRTProperties,
												   ::D2D1::HwndRenderTargetProperties( hWnd, dxSize ),
													&::g_ipRT );
	assert( hr == S_OK );
	
}


void OnResizeRT( UINT uWidth, UINT uHeight )
{

	if( ::g_ipRT != nullptr )
	{
		::g_ipRT->Resize( ::D2D1::SizeU( uWidth, uHeight ) );
	}

}


ID2D1Bitmap* CreateD2DBitmapFromFile( HWND hWnd, TCHAR* pImageFullPath, BOOL bUseAlpha = TRUE )
{

	assert( pImageFullPath != nullptr );
	assert( ::g_ipWICFactory != nullptr );
	assert( ::g_ipRT != nullptr );
	

	HRESULT hr = E_FAIL;


	//----------------------------------------------------------------
	//	디코더를 생성한다.
	//----------------------------------------------------------------
	IWICBitmapDecoder* ipDecoderPtr = nullptr;
	hr = ::g_ipWICFactory->CreateDecoderFromFilename( pImageFullPath, nullptr,
													  GENERIC_READ,
													  WICDecodeMetadataCacheOnDemand,
													  &ipDecoderPtr );
	assert( hr == S_OK );

	//----------------------------------------------------------------
	//	디코더에서 프레임을 얻는다.
	//----------------------------------------------------------------
	IWICBitmapFrameDecode* ipFramePtr = nullptr;
	hr = ipDecoderPtr->GetFrame( 0, &ipFramePtr );
	assert( hr == S_OK );
	
	
	//----------------------------------------------------------------
	//	프레임을 기반으로 해서 포맷 컨버터를 만든다.
	//----------------------------------------------------------------
	SafeRelease( ::g_ipConvertedSrcBmp );
	hr = ::g_ipWICFactory->CreateFormatConverter( &::g_ipConvertedSrcBmp );
	assert( hr == S_OK );

	if( bUseAlpha == TRUE )
	{	
		hr = ::g_ipConvertedSrcBmp->Initialize( ipFramePtr,
												GUID_WICPixelFormat32bppPBGRA,											
												WICBitmapDitherTypeNone,
												nullptr,
												0.0f,
												WICBitmapPaletteTypeCustom );
	}
	else
	{
		hr = ::g_ipConvertedSrcBmp->Initialize( ipFramePtr,
												GUID_WICPixelFormat32bppBGR,											
												WICBitmapDitherTypeNone,
												nullptr,
												0.0f,
												WICBitmapPaletteTypeCustom );
	}
	assert( hr == S_OK );

	//----------------------------------------------------------------
	//	컨버트된 데이터를 기반으로 해서 실제 비트맵을 만든다.
	//----------------------------------------------------------------
	//SafeRelease( ::g_ipBunnyBitmap );

	ID2D1Bitmap* ipResult = nullptr;	
	hr = ::g_ipRT->CreateBitmapFromWicBitmap( ::g_ipConvertedSrcBmp,												
											  nullptr,											  
											  &ipResult );
	
	
	assert( hr == S_OK );

	

	SafeRelease( ipDecoderPtr );
	SafeRelease( ipFramePtr );

	return ipResult;
	
}




int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ALPHAIMAGETEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ALPHAIMAGETEST));

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, NULL, 0, 0))
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
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ALPHAIMAGETEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ALPHAIMAGETEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }


   ::InitializeD2D();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_SIZE :		
		{
			if( ::g_ipRT == nullptr )
			{
				::InitializeRT( hWnd );
				//::AutoLoadImages( hWnd );
				RECT rc = { 0, 0, 10, 10 };
				::g_ipTransparentSquirrelBitmap = ::CreateD2DBitmapFromFile( hWnd, _T( "Images/aaa.png" ), TRUE );				
				::g_ipOpacitySquirrelBitmap = ::CreateD2DBitmapFromFile( hWnd, _T( "Images/aaa.png" ), FALSE );
			}
			else
			{
				UINT uWidth = LOWORD( lParam );
				UINT uHeight = HIWORD( lParam );
				::OnResizeRT( uWidth, uHeight );
			}
			
		}
		break;

	case WM_DISPLAYCHANGE :
		{
			::InvalidateRect( hWnd, NULL, FALSE );
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
		{
			HRESULT hr = E_FAIL;
			::D2D1_RECT_F dxArea = ::D2D1::RectF( 0.0f, 0.0f, 200.0f, 200.0f );

			::g_ipRT->BeginDraw();
			
			::g_ipRT->Clear( ::D2D1::ColorF( ::D2D1::ColorF::Pink ) );

			D2D1::Matrix3x2F matTrans;
			matTrans = ::D2D1::Matrix3x2F::Translation( 20.0f, 20.0f );
			::g_ipRT->SetTransform( matTrans );	
			::g_ipRT->DrawBitmap( ::g_ipTransparentSquirrelBitmap, dxArea );


			matTrans = ::D2D1::Matrix3x2F::Translation( 20.0f, 270.0f );
			::g_ipRT->SetTransform( matTrans );	
			::g_ipRT->DrawBitmap( ::g_ipOpacitySquirrelBitmap, dxArea );

			::g_ipRT->EndDraw();

		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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
