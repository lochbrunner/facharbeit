#include "windows.h"
#include "LogBook.h"

#pragma warning (disable : 4996 )

#pragma once

#define WS_NOMAXIMIZEBOX (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX)

#define WS_NOMINIMIZEBOX (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME)


#ifndef LPCALLBACKWINDOWCLOSED

typedef void    (CALLBACK *LPCALLBACKWINDOWCLOSED)( UINT WindowId);

#endif


class CInfo{

protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCALLBACKWINDOWCLOSED m_OnWindowClosed;
	UINT m_ID;
	
	CLogBook* m_lpLogBook;
	
public:
	CInfo();
	virtual ~CInfo();
	
	void SetCallbackOnWindowClosed(LPCALLBACKWINDOWCLOSED OnWindowClosed) { m_OnWindowClosed = OnWindowClosed; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetID(UINT ID) { m_ID = ID; }
	HWND GetHWnd(void) { return m_hWnd; }

	BOOL Create(HINSTANCE hInstance, HWND parentHWnd, UINT x, UINT y, UINT width, UINT height);
	void MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam);

};