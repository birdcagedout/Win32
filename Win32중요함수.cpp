1. 폰트 설정

HGDIOBJ h_newFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
										CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"맑은 고딕");
HGDIOBJ h_oldFont = SelectObject(h_dc, h_newFont);


SelectObject(h_dc, h_oldFont);
DeleteObject(h_newFont);




2. 배경색, 전경색

SetBkMode(h_dc, TRANSPARENT);
SetTextColor(h_dc, RGB(0, 125, 255));



3. TextOut
wchar_t str_buff[100];
wsprintf(str_buff, L"GetSystemTime: %d, %d, %d (%s)  %02d:%02d:%02d", g_st.wYear, g_st.wMonth, g_st.wDay, DAY_OF_WEEK[g_st.wDayOfWeek], g_st.wHour, g_st.wMinute, g_st.wSecond);
TextOut(h_dc, 100, 100, str_buff, wcslen(str_buff));



4. DrawText로 문자열 정렬
(WM_PAINT 앞부분에)
RECT r;
GetClientRect(hWnd, &r);

wchar_t str_buff[100];
wsprintf(str_buff, L"GetSystemTime: %d, %d, %d (%s)  %02d:%02d:%02d", g_st.wYear, g_st.wMonth, g_st.wDay, DAY_OF_WEEK[g_st.wDayOfWeek], g_st.wHour, g_st.wMinute, g_st.wSecond);
DrawText(h_dc, str_buff, wcslen(str_buff), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);



5. Minimize/Maximize/Close 버튼 Enable/Disable
1) Minimize 버튼
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);				// Minimize 버튼 disable
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & WS_MINIMIZEBOX);				// Minimize 버튼 enable

2) Maximize 버튼
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);				// Maximize 버튼 disable
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & WS_MAXIMIZEBOX);				// Maximize 버튼 enable
	
3) Close 버튼
	EnableMenuItem(GetSystemMenu(hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);				// Close 버튼 enable
	EnableMenuItem(GetSystemMenu(hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);	// Close 버튼 disable
	
	
6. 문자열 함수
	1) 포맷: swprintf_s(버퍼, 개수, "%d ... ", a);			// sprintf는 char형, swprintf는 유니코드형, swprintf_s는 유니코드 + 버퍼오버플로시 exception 발생(secure버전)
	2) 복사: wcscpy_s(stringBuffer, 길이, L"Hello world");
	3) 길이: wcslen(버퍼);


7. GetSystemMetrics(식별자)
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



8. 윈도 크기 조정

	RECT clientRect, winRect;
	GetClientRect(a_hWnd, &clientRect);
	GetWindowRect(a_hWnd, &winRect);

	// 윈도 크기 재조정
	SetWindowPos(a_hWnd, NULL, 0, 0,
		winRect.right - winRect.left - clientRect.right + (int)(g_imageRect.right),		// 새로운 width (앞3개term=윈도좌우프레임폭+그림자좌우폭, 4번째=그림좌우크기)
		winRect.bottom - winRect.top - clientRect.bottom + (int)(g_imageRect.bottom),	// 새로운 height (앞3개term=윈도상하타이틀바와프레임폭+그림자좌우폭, 4번째=그림상하크기)
		SWP_NOMOVE);		// 현재 윈도 위치 그대로(크기만 바뀜)
		

