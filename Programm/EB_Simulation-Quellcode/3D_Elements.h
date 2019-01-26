#include "windows.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include "3DObject.h"
#include "ResourceManager.h"
#include "LogBook.h"
#include "Physics.h"
//#include "math.h"
//#include <list>
//#include <commctrl.h>

#pragma once

//#define PI 3.14159265358979323846f

#ifndef PI_F

#define PI_F 3.14159265f

#endif

typedef struct _CustomVertex{
    FLOAT x, y, z;    // die untransformierte Position des Vertex
    DWORD color;      // die Farbe des Vertex
		
} CustomVertex;



CustomVertex MakeCustomVertex(FLOAT cx, FLOAT cy, FLOAT cz, DWORD ccolor);
CustomVertex MakeCustomVertex(D3DXVECTOR3 position, DWORD Color);


//----------------------------------------------------------------------------------------------------
// The CGrid class
//----------------------------------------------------------------------------------------------------

class CGrid{

protected:
	LPDIRECT3DDEVICE9 m_lpD3DDevice;

	LPDIRECT3DVERTEXBUFFER9 VB_Grid;

	CustomVertex* GridVertices;

	UINT count;
	FLOAT distance;

	CLogBook* m_lpLogBook;
	

public:
	CGrid(void);
	virtual ~CGrid(void);
	void SetD3Device(LPDIRECT3DDEVICE9 lpD3DDevice) { m_lpD3DDevice = lpD3DDevice; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }

	void Init(void);
	void Render(void);
	void DeInit(void);
	
};


//----------------------------------------------------------------------------------------------------
// The CArrow class
//----------------------------------------------------------------------------------------------------

class CArrow{

protected:
	LPDIRECT3DDEVICE9 m_lpD3DDevice;

	LPDIRECT3DVERTEXBUFFER9 VB_Lines;
	CustomVertex* LinesVertices;

	CResourceManager* m_lpResmanager;
	C3DObject* lpD3Elements;
	CLogBook* m_lpLogBook;
	D3DXVECTOR3 m_Position;

	float distance;

public:
	CArrow(void);
	virtual ~CArrow(void);

	void SetD3Device(LPDIRECT3DDEVICE9 lpD3DDevice) { m_lpD3DDevice = lpD3DDevice; }
	void SetResourceManagerPtr(CResourceManager* lpResmanager) { m_lpResmanager = lpResmanager; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetPosition(D3DXVECTOR3 Position) { m_Position = Position; }
	void AddPosition(D3DXVECTOR3 Position) { m_Position += Position; }

	C3DObject* Get3DObject(UINT ID) { return (lpD3Elements + ID); }
	float GetDistance(void) { return distance; }
	D3DXVECTOR3 GetPosition(void) { return m_Position; }

	void Init(void);
	void DeInit(void);

	void Render(D3DXVECTOR3 PositionCamera);

};

//----------------------------------------------------------------------------------------------------
// The CVectorArrow class
//----------------------------------------------------------------------------------------------------

#define ARROW_SPEED 0
#define ARROW_SPEEDUP 1

class CVectorArrow{

protected:
	LPDIRECT3DDEVICE9 m_lpD3DDevice;
	CResourceManager* m_lpResmanager;

	LPDIRECT3DVERTEXBUFFER9 VB_Line[2];
	CustomVertex* LineVertices;

	D3DXVECTOR3 m_Position[2];
	D3DXVECTOR3 m_PositionViewAt[2];

	C3DObject* lpD3Elements;
	CLogBook* m_lpLogBook;

public:
	CVectorArrow();
	virtual ~CVectorArrow();

	void SetD3Device(LPDIRECT3DDEVICE9 lpD3DDevice) { m_lpD3DDevice = lpD3DDevice; }
	void SetResourceManagerPtr(CResourceManager* lpResmanager) { m_lpResmanager = lpResmanager; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetPosition(UINT ID, D3DXVECTOR3 Position) { m_Position[ID] = Position; }
	void AddPosition(UINT ID, D3DXVECTOR3 Position) { m_Position[ID] += Position; }
	void SetPositionViewAt(UINT ID, D3DXVECTOR3 ViewAt);
	void SetMatrix(UINT ID);
	C3DObject* Get3DObject(UINT ID) { return (lpD3Elements + ID); }

	void Init(void);
	void DeInit(void);

	void Render(UINT ID);

};

//----------------------------------------------------------------------------------------------------
// The CFieldVector class
//----------------------------------------------------------------------------------------------------

#define OBJECT_FIELD_ELECTRICAL 0
#define OBJECT_FIELD_MAGNETICAL 2
#define OBJECT_FIELD_GRAVITY 1

class CFieldVector{

protected:
	LPDIRECT3DDEVICE9 m_lpD3DDevice;
	CResourceManager* m_lpResmanager;

	LPDIRECT3DVERTEXBUFFER9 VB_Line[6];
	CustomVertex* LineVertices;

	D3DXVECTOR3 m_Position[6];
	D3DXVECTOR3 m_PositionViewAt[6];

	C3DObject* lpD3Elements;
	CLogBook* m_lpLogBook;

	float RotateX, RotateY;

public:
	CFieldVector();
	virtual ~CFieldVector();

	void SetD3Device(LPDIRECT3DDEVICE9 lpD3DDevice) { m_lpD3DDevice = lpD3DDevice; }
	void SetResourceManagerPtr(CResourceManager* lpResmanager) { m_lpResmanager = lpResmanager; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetPosition(UINT ID, D3DXVECTOR3 Position) { m_Position[ID] = Position; }
	void AddPosition(UINT ID, D3DXVECTOR3 Position) { m_Position[ID] += Position; }
	void SetPositionViewAt(UINT ID, D3DXVECTOR3 ViewAt);

	void SetMatrix(D3DXVECTOR3 Position);
	C3DObject* Get3DObject() { return (lpD3Elements); }

	void Init(void);
	void DeInit(void);

	void Render(UINT ID);

};

//----------------------------------------------------------------------------------------------------
// The CDefaultSphere class
//----------------------------------------------------------------------------------------------------

class CDefaultSphere {

protected:
	LPDIRECT3DDEVICE9 m_lpD3DDevice;
	CResourceManager* m_lpResmanager;
	CLogBook* m_lpLogBook;

public:
	CDefaultSphere();
	virtual ~CDefaultSphere();

	void SetD3Device(LPDIRECT3DDEVICE9 lpD3DDevice) { m_lpD3DDevice = lpD3DDevice; }
	void SetResourceManagerPtr(CResourceManager* lpResmanager) { m_lpResmanager = lpResmanager; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }

};


//----------------------------------------------------------------------------------------------------
// The CTrace class
//----------------------------------------------------------------------------------------------------

class CTrace {

protected:
	LPDIRECT3DDEVICE9 m_lpD3DDevice;
	CResourceManager* m_lpResmanager;
	CLogBook* m_lpLogBook;

	LPDIRECT3DVERTEXBUFFER9 VB_Trace;
	CustomVertex* TraceVertices;

	UINT numPoints;

public:
	CTrace();
	virtual ~CTrace();

	void SetD3Device(LPDIRECT3DDEVICE9 lpD3DDevice) { m_lpD3DDevice = lpD3DDevice; }
	void SetResourceManagerPtr(CResourceManager* lpResmanager) { m_lpResmanager = lpResmanager; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }

	void AddPoint(D3DXVECTOR3 position, DWORD Color);
	void Render(void);

};

//----------------------------------------------------------------------------------------------------
// The CEmitter class
//----------------------------------------------------------------------------------------------------

class CEmitter {

protected:
	LPDIRECT3DDEVICE9 m_lpD3DDevice;
	CResourceManager* m_lpResmanager;
	CLogBook* m_lpLogBook;
	
	C3DObject EmitterMesh;

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;

	int maxFaces;

public:
	CEmitter();
	virtual ~CEmitter();

	void SetD3Device(LPDIRECT3DDEVICE9 lpD3DDevice) { m_lpD3DDevice = lpD3DDevice; }
	void SetResourceManagerPtr(CResourceManager* lpResmanager) { m_lpResmanager = lpResmanager; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }

	void SetPosition(D3DXVECTOR3 Position) { m_Position = Position; }
	void SetRotation(D3DXVECTOR3 Rotation) { m_Rotation = Rotation; }
	void SetViewAt(D3DXVECTOR3 ViewAt);
	void SetMatrix(void);

	C3DObject* GetMeshPtr(void) { return &EmitterMesh; }

	void Init(void);
	void Render(void);

};