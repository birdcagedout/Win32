// AlphaImageTest.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "AlphaImageTest.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


#include <d2d1.h>					// Direct2D ���
#include <wincodec.h>				// IWICImagingFactory ���

#pragma comment(lib, "D2D1.lib")	// Direct2D�� ���ӽ����̽� ����
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
	//	D2D Factory �� �����Ѵ�.
	//-----------------------------------------------------------------------
	hr = ::D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, 
							  &::g_ipD2DFactory );
	assert( hr == S_OK );

	//-----------------------------------------------------------------------
	//	Windows Imaging Component Factory �� �����Ѵ�.
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
	//	���ڴ��� �����Ѵ�.
	//----------------------------------------------------------------
	IWICBitmapDecoder* ipDecoderPtr = nullptr;
	hr = ::g_ipWICFactory->CreateDecoderFromFilename( pImageFullPath, nullptr,
													  GENERIC_READ,
													  WICDecodeMetadataCacheOnDemand,
													  &ipDecoderPtr );
	assert( hr == S_OK );

	//----------------------------------------------------------------
	//	���ڴ����� �������� ��´�.
	//----------------------------------------------------------------
	IWICBitmapFrameDecode* ipFramePtr = nullptr;
	hr = ipDecoderPtr->GetFrame( 0, &ipFramePtr );
	assert( hr == S_OK );
	
	
	//----------------------------------------------------------------
	//	�������� ������� �ؼ� ���� �����͸� �����.
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
	//	����Ʈ�� �����͸� ������� �ؼ� ���� ��Ʈ���� �����.
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

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ALPHAIMAGETEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ALPHAIMAGETEST));

	// �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
		// �޴� ������ ���� �м��մϴ�.
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
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
