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

typedef struct _Vertex_XYZ_Nor_Diff {
	FLOAT x, y, z;
	FLOAT nx, ny, nz;
    DWORD color;
	
} Vertex_XYZ_Nor_Diff;

#define VertexNorm Vertex_XYZ_Nor_Diff

typedef struct _VertexTex{
	FLOAT x, y, z;    // die untransformierte Position des Vertex
    FLOAT nx, ny, nz; // die Normale
    DWORD color;      // die Farbe des Vertex
    FLOAT tu, tv;     // Texturkoordinaten
} VertexTex;

#define  LightableVertex Vertex_XYZ_Nor_Diff


//----------------------------------------------------------------------------------------------------
// The global functions
//----------------------------------------------------------------------------------------------------

CustomVertex MakeCustomVertex(FLOAT vx, FLOAT vy, FLOAT vz, DWORD ccolor);
CustomVertex MakeCustomVertex(D3DXVECTOR3 position, DWORD Color);
Vertex_XYZ_Nor_Diff MakeLightVertex(FLOAT vx, FLOAT vy, FLOAT vz, FLOAT nx, FLOAT ny, FLOAT nz, DWORD ccolor);
Vertex_XYZ_Nor_Diff MakeLightVertex(D3DXVECTOR3 position, D3DXVECTOR3 normal, DWORD ccolor);
VertexTex MakeVertexTex(FLOAT cx, FLOAT cy, FLOAT cz, FLOAT nx, FLOAT ny, FLOAT nz, DWORD ccolor);
BOOL IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, 
					   D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, 
                        FLOAT* t, FLOAT* u, FLOAT* v );

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
// The CTranslateTool class
//----------------------------------------------------------------------------------------------------

class CTranslateTool{

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
	CTranslateTool(void);
	virtual ~CTranslateTool(void);

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
// The CScaleTool class
//----------------------------------------------------------------------------------------------------

class CScaleTool {

protected:
	LPDIRECT3DDEVICE9 m_lpD3DDevice;

	LPDIRECT3DVERTEXBUFFER9 VB_Lines;
	CustomVertex* LinesVertices;

	CResourceManager* m_lpResmanager;
	C3DObject* lpD3Elements;
	CLogBook* m_lpLogBook;
	D3DXVECTOR3 m_Position;

	D3DXVECTOR3 m_ViewAt;

	float distance;

public:
	CScaleTool(void);
	virtual ~CScaleTool(void);

	void SetD3Device(LPDIRECT3DDEVICE9 lpD3DDevice) { m_lpD3DDevice = lpD3DDevice; }
	void SetResourceManagerPtr(CResourceManager* lpResmanager) { m_lpResmanager = lpResmanager; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetPosition(D3DXVECTOR3 Position) { m_Position = Position; }
	void AddPosition(D3DXVECTOR3 Position) { m_Position += Position; }
	void AddViewAt(D3DXVECTOR3 deltaViewAt);
	void ZeroViewAt(void);

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
	float m_Radius;
	
	LPDIRECT3DINDEXBUFFER9 IB_Sphere;
	LPDIRECT3DVERTEXBUFFER9 VB_Sphere;
	VertexNorm* m_lpVerticies;
	DWORD* m_lpIndicies;

	D3DMATERIAL9 m_lpMaterial;

	DWORD FVF_FORMAT;

	D3DXMATRIX WorldMatrix;
	
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vRotation;

	int NumFaces;
	int NumVerticies;

public:
	CDefaultSphere();
	virtual ~CDefaultSphere();

	void SetD3Device(LPDIRECT3DDEVICE9 lpD3DDevice) { m_lpD3DDevice = lpD3DDevice; }
	void SetResourceManagerPtr(CResourceManager* lpResmanager) { m_lpResmanager = lpResmanager; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetPosition(D3DXVECTOR3 position) { m_vPosition = position; }
	void SetRotation(D3DXVECTOR3 rotation) { m_vRotation = rotation; }
	void SetSize(float Radius);

	float GetSize(void) { return m_Radius; }

	BOOL CreateSphere(float Radius, UINT resolutionVertical = 14, UINT resolutionHorizontal = 24);

	void Render(void);
	void SetWorldMatrix(void);

	BOOL IsPicked(D3DXVECTOR3 v, D3DXMATRIX ViewMatrix, FLOAT *pU, FLOAT *pV, FLOAT *pDist);

};


//----------------------------------------------------------------------------------------------------
// The CDefaultCuboid class
//----------------------------------------------------------------------------------------------------

class CDefaultCuboid {

protected:
	LPDIRECT3DDEVICE9 m_lpD3DDevice;
	CResourceManager* m_lpResmanager;
	CLogBook* m_lpLogBook;
	D3DXVECTOR3 m_Size;
	
	LPDIRECT3DINDEXBUFFER9 IB_Cuboid;
	LPDIRECT3DVERTEXBUFFER9 VB_Cuboid;
	VertexNorm* m_lpVerticies;
	DWORD* m_lpIndicies;

	D3DMATERIAL9 m_lpMaterial;

	DWORD FVF_FORMAT;

	D3DXMATRIX WorldMatrix;
	
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vRotation;

	int VertexCount;
	int FaceCount;

public:
	CDefaultCuboid();
	virtual ~CDefaultCuboid();

	void SetD3Device(LPDIRECT3DDEVICE9 lpD3DDevice) { m_lpD3DDevice = lpD3DDevice; }
	void SetResourceManagerPtr(CResourceManager* lpResmanager) { m_lpResmanager = lpResmanager; }
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetPosition(D3DXVECTOR3 position) { m_vPosition = position; }
	void SetRotation(D3DXVECTOR3 rotation) { m_vRotation = rotation; }
	void SetSize(D3DXVECTOR3 Size);

	D3DXVECTOR3 GetSize(void) { return m_Size; }

	BOOL CreateCuboid(float SizeX, float SizeY, float SizeZ);

	BOOL CreateCuboid(D3DXVECTOR3 Size);
	void Render(void);
	void SetWorldMatrix(void);

	BOOL IsPicked(D3DXVECTOR3 v, D3DXMATRIX ViewMatrix, FLOAT *pU, FLOAT *pV, FLOAT *pDist);
	

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