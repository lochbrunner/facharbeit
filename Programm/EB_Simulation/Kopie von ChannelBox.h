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
#define EDIT_SIZE_X 30
#define EDIT_SIZE_Y 31
#define EDIT_SIZE_Z 32
#define EDIT_NUM_PARTS 33
#define EDIT_SHOW_CHARGES 34
#define EDIT_RADIUS 35

#define UPDATE_CUBOID_PARTS 0
#define UPDATE_CUBOID_CHARGES 1
#define UPDATE_SPHERE_SIZE 2


#define BUTTON_RESET 60
#define BUTTON_SET_CUBOID 61
#define STATIC_NOSELECTION 62

#define WS_STYLE (WS_OVERLAPPED  | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX    | \
							 WS_VISIBLE)


typedef void    (CALLBACK *LPCALLBACKRENDER)(void);
typedef void	(CALLBACK *LPCALLBACKUPDATE) (DWORD ID, DWORD Pointer);

LRESULT CALLBACK ChannelBoxMessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



class CChannelBox{

protected:

	UINT m_SizeX;
	UINT m_SizeY;

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
	LPCALLBACKUPDATE m_UpToDate;

	list<Protone*>* m_lpListProtone;
	list<Electrone*>* m_lpListElectrone;
	list<DefaultSphere*>* m_lpListDefaultSphere;
	list<ElectricalField*>* m_lpListElectricalField;
	list<MagneticalField*>* m_lpListMagneticalField;
	list<GravityField*>* m_lpListGravityField;
	list<DefaultCuboid*>* m_lpListDefaultCuboid;

public:
	CChannelBox();
	virtual ~CChannelBox();

	void SetCallbackUpToRender(LPCALLBACKRENDER UpToRender) { m_UpToRender = UpToRender; }
	void SetCallbackUpToDate(LPCALLBACKUPDATE UpToDate) { m_UpToDate = UpToDate; }
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

