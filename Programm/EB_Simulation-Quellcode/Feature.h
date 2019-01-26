#pragma warning (disable : 4996 )

#include "windows.h"
#include "Physics.h"
#include <strsafe.h>
#include <math.h>



#pragma once

#define EDIT_TITLE 0
#define EDIT_POSITION_X 1
#define EDIT_POSITION_Y 2
#define EDIT_POSITION_Z 3
#define EDIT_SPEED_X 4
#define EDIT_SPEED_Y 5
#define EDIT_SPEED_Z 6
#define EDIT_SPEEDING_UP_X 7
#define EDIT_SPEEDING_UP_Y 8
#define EDIT_SPEEDING_UP_Z 9
#define EDIT_DRIVER 10
#define EDIT_DRIVEN 11
#define EDIT_MASS 12
#define EDIT_CHARGE 13

#define BUTTON_OK 11

typedef void    (CALLBACK *LPCALLBACKRENDER)(void);


class CFeatureWindow {

protected:
	HWND m_hWnd;
	HWND hWndOK;
	HWND hWmdEdit[32];
	HINSTANCE m_hInstance;
	CPhysics* m_lpPhysics;
	CLogBook* m_lpLogBook;

	LPCALLBACKRENDER m_UpToRender;

	list<Protone*>* m_lpListProtone;
	list<Electrone*>* m_lpListElectrone;

	DWORD Selected;

public:
	CFeatureWindow();
	virtual ~CFeatureWindow();

	

	BOOL Create(HINSTANCE hInstance, HWND parentHWnd, UINT x, UINT y, UINT width, UINT height);
	void Resize(UINT x, UINT y, UINT width, UINT height);
	void SetCallbackUpToRender(LPCALLBACKRENDER UpToRender) { m_UpToRender = UpToRender; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetCPhysicsPtr(CPhysics* lpPhysics);
	
	void OnPicked(void);
	void UpdateData(void);
	void ResetEdit(void);
	BOOL IsHwndMemberOfThis(HWND hwnd);

	void MessageHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	void ConvertStringToFloat(float* pDest, WCHAR* pSource, UINT lenght);
	BOOL IsStringTrue(WCHAR* pSource, UINT lenght);
};