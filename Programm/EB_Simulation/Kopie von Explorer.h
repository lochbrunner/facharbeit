#pragma warning (disable : 4996 )

#include "windows.h"
#include <commctrl.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include "Physics.h"
#include "LogBook.h"
#include <list>


#pragma warning (disable : 4996 )

#pragma once

#define WS_NOMAXIMIZEBOX (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX)

typedef void    (CALLBACK *LPCALLBACKWINDOWCLOSED)( UINT WindowId);

class CExplorerWindow {

protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCALLBACKWINDOWCLOSED m_OnWindowClosed;

	UINT m_ID;
	HWND hwndTree;
	HWND hwndList;
	HWND hwndTab;

    HTREEITEM ProtoneList;
	HTREEITEM ElectroneList;
	
	CPhysics* m_lpPhysics;
	CLogBook* m_lpLogBook;

	list<Protone*>* m_lpListProtone;
	list<Electrone*>* m_lpListElectrone;

public:
	CExplorerWindow();
	virtual ~CExplorerWindow();

	BOOL Create(HINSTANCE hInstance, HWND parentHWnd, UINT x, UINT y, UINT width, UINT height);
	void SetCallbackOnWindowClosed(LPCALLBACKWINDOWCLOSED OnWindowClosed) { m_OnWindowClosed = OnWindowClosed; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetID(UINT ID) { m_ID = ID; }
	void SetCPhysicsPtr(CPhysics* lpPhysics);
	HWND GetHWnd(void) { return m_hWnd; }

	void MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam);
	void UpdateList(void);

	BOOL IsHwndMemberOfThis(HWND hwnd);


};