#pragma warning (disable : 4996 )

#include "windows.h"
#include "Physics.h"
#include "Scene.h"
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
#define EDIT_SHOW_SPEED 14
#define EDIT_SHOW_SPEEDING_UP 15
#define EDIT_VIEWAT_X 16
#define EDIT_VIEWAT_Y 17
#define EDIT_VIEWAT_Z 18
#define EDIT_SHOW_VECTOR 19
#define EDIT_SHOW_TRACE 20
#define EDIT_TYPE_EMIT 21
#define EDIT_WAIT_TIME 22
#define EDIT_DIRECTION_X 23
#define EDIT_DIRECTION_Y 24
#define EDIT_DIRECTION_Z 25
#define EDIT_RANDOM_X 26
#define EDIT_RANDOM_Y 27
#define EDIT_RANDOM_Z 28
#define EDIT_SHOW_DIRECTION 29


#define BUTTON_RESET 60
#define STATIC_NOSELECTION 61

#define WS_STYLE (WS_OVERLAPPED  | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX    | \
                             WS_VISIBLE)


typedef void    (CALLBACK *LPCALLBACKRENDER)(void);

LRESULT CALLBACK ChannelBoxMessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



class CChannelBox{

protected:
	HWND m_hWnd;
	HWND hWndEdit[64];
	HWND hWndStatic[64];

	HBRUSH hBrush;

	HINSTANCE m_hInstance;
	CPhysics* m_lpPhysics;
	CLogBook* m_lpLogBook;
	CScene* m_lpScene;
	
	UINT ElementType;
	LPDEFAULTELEMENT m_Selected;

	LPCALLBACKRENDER m_UpToRender;

	list<Protone*>* m_lpListProtone;
	list<Electrone*>* m_lpListElectrone;
	list<DefaultSphere*>* m_lpListDefaultSphere;
	list<ElectricalField*>* m_lpListElectricalField;
	list<MagneticalField*>* m_lpListMagneticalField;
	list<GravityField*>* m_lpListGravityField;


public:
	CChannelBox();
	virtual ~CChannelBox();

	void SetCallbackUpToRender(LPCALLBACKRENDER UpToRender) { m_UpToRender = UpToRender; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetCPhysicsPtr(CPhysics* lpPhysics);
	void SetCScenePtr(CScene* lpScene) { m_lpScene = lpScene; }

	BOOL RegistrateWindowClass(HINSTANCE hInstance);
	BOOL CreateChannelBox(HWND parentHWnd, UINT x, UINT y, UINT width, UINT height);

	BOOL FillChannelBox(LPDEFAULTELEMENT Selected);
	BOOL CleanChannelBox(void);
	void Update(void);
	void Reset(void);

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	float ConvertStringToFloat(float* pDest, WCHAR* pSource, int lenght);

};

