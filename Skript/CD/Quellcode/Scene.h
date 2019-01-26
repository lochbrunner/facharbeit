#pragma once

#include "windows.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include "ResourceManager.h"
#include "3DObject.h"
#include "Physics.h"
#include "math.h"
#include "3D_Elements.h"
#include <list>
#include <commctrl.h>
#include <D3dtypes.h>
#include "LogBook.h"

#pragma warning (disable : 4996 )
#pragma warning (disable : 4995 )

typedef void    (CALLBACK *LPCALLBACKPICKED)(LPDEFAULTELEMENT lpSelected);
typedef void    (CALLBACK *LPCALLEACKARROWMOVED)(void);

// Definition des Vektorformats, ohne Vergabe der Farbwerte
#define D3D_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

#define STEP_TRANSLATE 0.00115f
#define STEP_ROTATE 0.01f
#define STEP_SCALE 0.002f

#define SHOW_POSITION_ARROW 0
#define SHOW_SPEED_ARROW 1
#define SHOW_SPEEDUP_ARROW 2

#define SELECTED_ELEMENT 0
#define SELECTED_SPEED 1
#define SELECTED_SPEEDUP 2
#define SELECTED_FORCE 3
#define SELECTED_DIRECTION 4

#define FACTOR_SPEED_ARROW 20
#define FACTOR_SPEEDUP_ARROW 50

#define MAX_DISTAMCE 500.0f

#define TOOL_TRANSFORM 0
#define TOOL_SCALE 1



class CScene {

protected:


	WCHAR m_text[64];

	HWND hWnd;
	HWND m_hwndStatus;
	HINSTANCE m_hInstance;

	CPhysics* m_lpPhysics;

	// Farbe fuer den Backbuffer
    D3DCOLOR          m_ClearColor;

    // Direct3D Variablen
    LPDIRECT3D9       m_lpD3D;
    LPDIRECT3DDEVICE9 m_lpD3DDevice;
    LPD3DXFONT        m_lpD3DFont;

    LPDIRECT3DSURFACE9 m_lpBackBuffer;
	D3DPRESENT_PARAMETERS PParams;

	D3DXVECTOR2 size;

	CDefaultCuboid DefaultCuboidMesh; 
	CDefaultSphere DefaultSphereMesh; 
	CEmitter EmitterObject;
	CGrid Grid;
	CTranslateTool TranslateTool;
	CScaleTool ScaleTool;
	CVectorArrow VectorArrow;
	CFieldVector FieldVector;
	C3DObject* lpD3Object;
	CResourceManager* lpResmanager;
	CLogBook* m_lpLogBook;

	D3DXMATRIX m_TransMatrix;
    D3DXMATRIX m_ViewMatrix;
	D3DLIGHT9 m_Light;
	D3DXMATRIX ProjMatrix;

	D3DXVECTOR3 CameraPosition;
	D3DXVECTOR3 CameraViewAt;
	D3DXVECTOR3 CameraRotation;
	float Distance;

	BOOL IsButtonDown[3];
	POINT LastMousePosition;

	list<Protone*>* m_lpListProtone;
	list<Electrone*>* m_lpListElectrone;
	list<DefaultSphere*>* m_lpListDefaultSphere;
	list<DefaultCuboid*>* m_lpListDefaultCuboid;
	list<ElectricalField*>* m_lpListElectricalField;
	list<MagneticalField*>* m_lpListMagneticalField;
	list<GravityField*>* m_lpListGravityField;
	list<Emitter*>* m_lpListEmitter;

	map<DefaultCuboid*, CDefaultCuboid*> m_CuboidMeshMap;
	map<DefaultSphere*, CDefaultSphere*> m_SphereMeshMap;

	HCURSOR CursorTrans;
	HCURSOR CursorScale;
	HCURSOR CursorRotate;

	BOOL RenderGrid;

	LPCALLBACKPICKED m_OnPicked;
	LPCALLEACKARROWMOVED m_OnArrowMoved;

	BOOL ShowArrow;
	BOOL ArrowSelected[3];

	LPDEFAULTELEMENT Selected;
	UINT SelectedPart;
	UINT KindOfSelectedElement;

	UINT ArrowMode;
	BOOL bArrowActivated;

	map<LPDEFAULTELEMENT, CTrace*> TraceMap;
	list<CTrace*> TraceList;

public:
	CScene();
	virtual ~CScene();

	void SetClearColor(D3DCOLOR Color) { m_ClearColor = Color; }
	LPDIRECT3DSURFACE9 GetBackbuffer(void) {return m_lpBackBuffer; }
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_lpD3DDevice; }
	D3DPRESENT_PARAMETERS* GetD3DPresentParametersPtr(void ) { return &PParams; }
	void SetStatusBar(HWND hwndStatus) { m_hwndStatus = hwndStatus; }
	void SetCPhysicsPtr(CPhysics* lpPhysics);
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetRenderGrid(BOOL b) {RenderGrid = b; }
	void SetCallbackOnPicked(LPCALLBACKPICKED OnPicked) { m_OnPicked = OnPicked; }
	void SetCallbackOnArrowMoved(LPCALLEACKARROWMOVED OnArrowMoved) { m_OnArrowMoved = OnArrowMoved; }
	void SetArrowMode(UINT mode);
	void SetSphereSize(DefaultSphere* lpDefaultSphere);
	LPDEFAULTELEMENT GetSelected(void) { return Selected; }
	//void NothingSelected() {ArrowMode[0] = ArrowMode[1] = ArrowMode[2] = ShowArrow = false; }

	void ToggleRenderGrid(void) {RenderGrid = !RenderGrid; }
	void ToggleArrowActivated(int Mode);
	
	DefaultCuboid* AddCuboid(void);
	DefaultSphere* AddSphere(void);
	void AddCuboid(DefaultCuboid* lpDefaultCuboid);
	void AddSphere(DefaultSphere* lpDefaultSphere);
	void AddTraceToSelected(void);
	void AddTraceToSelected(LPDEFAULTELEMENT lpElement);
	void StopTraceToSelected(void);
	void StopTraceToSelected(LPDEFAULTELEMENT lpElement);
	void UpdateTrace(void);
	void UpdateCuboidSize(void);
	void SortOut(void);

	BOOL Init(HINSTANCE hInstance, HWND parentHWnd, UINT x, UINT y, UINT width, UINT height, BOOL bWindowed = TRUE);
	void DrawText(LPCWSTR Text, int x, int y, D3DCOLOR TextColor);
	HWND GetHWnd(void) { return hWnd; }
	
	void BeginScene(void);
    void EndScene(void);
	void Render(void);

	void SetCamera(void);

	void Resize(UINT width, UINT height);
	void InitD3D(UINT width, UINT height);
	void DeInitD3D(void);
	void MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam);
	void CreateScreenshot(void);
	
protected:
	void CreateFont(void);
	void DeleteFont(void);
	BOOL IsPicked(C3DObject* lpMesh);
	BOOL Pick(BOOL bAddSelection = false);
	void RenderArrow(void);
};