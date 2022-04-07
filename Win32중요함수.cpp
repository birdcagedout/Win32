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
	