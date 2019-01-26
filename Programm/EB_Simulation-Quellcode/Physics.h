#pragma warning (disable : 4996 )
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <list>
#include "math.h"
#include <string>
//#include <strsafe.h>
#include "LogBook.h"

//#pragma warning (disable : 4996 )

#pragma once

using namespace std;

#define TYPE_PROTONE 0
#define TYPE_ELECTRONE 1
#define TYPE_DEFAULT_SPHERE 2
#define TYPE_FIELD_ELECTRICAL 3
#define TYPE_FIELD_MAGNETICAL 4
#define TYPE_FIELD_GRAVITY 5
#define TYPE_OBJECT_EMITTER 6

#define PI 3.14159265358979323846f

//#define PI_F static_cast<float>(PI);
#define PI_F 3.14159265f
#define PI_2_F static_cast<float>(PI * 0.5)


struct Protone{
	static const int Type = TYPE_PROTONE;
	int tempType;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Speed;
	D3DXVECTOR3 SpeedingUp;
	BOOL bDriver;
	BOOL bDriven;
	BOOL bShowSpeed;
	BOOL bShowSpeedup;
	BOOL bSelected;
	BOOL bKill;
	BOOL bTrace;
	WCHAR Name[64];
};

typedef Protone* LPDEFAULTELEMENT ;

struct Electrone{
	static const int Type = TYPE_ELECTRONE;
	int tempType;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Speed;
	D3DXVECTOR3 SpeedingUp;
	BOOL bDriver;
	BOOL bDriven;
	BOOL bShowSpeed;
	BOOL bShowSpeedup;
	BOOL bSelected;
	BOOL bKill;
	BOOL bTrace;
	WCHAR Name[64];
};

struct DefaultSphere{
	static const int Type = TYPE_DEFAULT_SPHERE;
	int tempType;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Speed;
	D3DXVECTOR3 SpeedingUp;
	BOOL bDriver;
	BOOL bDriven;
	BOOL bShowSpeed;
	BOOL bShowSpeedup;
	BOOL bSelected;
	BOOL bKill;
	float Mass;
	float Coulomb;
	BOOL bTrace;
	WCHAR Name[64];
};

struct ElectricalField{
	static const int Type = TYPE_FIELD_ELECTRICAL;
	int tempType;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Vector;
	D3DXVECTOR3 ViewAt;
	BOOL bSelected;
	BOOL bKill;
	BOOL bShowVector;
	WCHAR Name[64];
};

struct MagneticalField{
	static const int Type = TYPE_FIELD_MAGNETICAL;
	int tempType;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Vector;
	D3DXVECTOR3 ViewAt;
	BOOL bSelected;
	BOOL bKill;
	BOOL bShowVector;
	WCHAR Name[64];
};

struct GravityField{
	int tempType;
	static const int Type = TYPE_FIELD_GRAVITY;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Vector;
	D3DXVECTOR3 ViewAt;
	BOOL bSelected;
	BOOL bKill;
	BOOL bShowVector;
	WCHAR Name[64];
};

struct Emitter{
	int tempType;
	static const int Type = TYPE_OBJECT_EMITTER;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Vector;
	D3DXVECTOR3 ViewAt;
	D3DXVECTOR3 Random;
	BOOL bSelected;
	BOOL bKill;
	BOOL bShowVector;
	UINT elementType;
	UINT waitTime;
	WCHAR Name[64];

};

typedef void    (CALLBACK *LPCALLBACKONSTARTTRACE)(LPDEFAULTELEMENT lpElement);

class CPhysics {

protected:
	list<Protone*>ListProtone;
	list<Electrone*>ListElectrone;
	list<DefaultSphere*>ListDefaultSphere;
	list<ElectricalField*>ListElectricalField;
	list<MagneticalField*>ListMagneticalField;
	list<GravityField*>ListGravityField;
	list<Emitter*>ListEmitter;

	UINT CountProtone;
	UINT CountElectrone;
	UINT CountDefaultSphere;
	UINT CountElectricalField;
	UINT CountMagneticalField;
	UINT CountGravityField;
	UINT CountEmitter;
	
	CLogBook* m_lpLogBook;

	LONG count;
	
	LPCALLBACKONSTARTTRACE m_OnStartRace;

public:
	CPhysics();
	virtual ~CPhysics();

	void SetResourceManagerPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetCallbackOnStartTrace(LPCALLBACKONSTARTTRACE OnStartRace) { m_OnStartRace = OnStartRace; }

	void AddProtone(void);
	void AddElectrone(void);
	void AddDefault(void);
	void AddElectricalField(void);
	void AddMagneticalField(void);
	void AddGravityField(void);
	void AddEmitter(void);

	void AddProtone(Protone* lpProtone);
	void AddElectrone(Electrone* lpElectrone);
	void AddDefault(DefaultSphere* lpDefaultSphere);
	void AddElectricalField(ElectricalField* lpElectricalField);
	void AddMagneticalField(MagneticalField* lpMagneticalField);
	void AddGravityField(GravityField* lpGravityField);
	void AddEmitter(Emitter* lpEmitter);

	void DeleteSelected(void);
	void DeleteSpacial(void);

	void OnRender(float elapsedTime);

	list<Protone*>* GetListProtonePtr(void) {return &ListProtone; }
	list<Electrone*>* GetListElectronePtr(void) {return &ListElectrone; }
	list<DefaultSphere*>* GetListDefaultSpherePtr(void) {return &ListDefaultSphere; }
	list<ElectricalField*>* GetListElectricalFieldPtr(void) {return &ListElectricalField; }
	list<MagneticalField*>* GetListMagneticalFieldPtr(void) {return &ListMagneticalField; }
	list<GravityField*>* GetListGravityFieldPtr(void) {return &ListGravityField; }
	list<Emitter*>* GetListEmitterPtr(void) { return &ListEmitter; }

	void ClearLists(void);

protected:
	D3DXVECTOR3 ForceElectrical(D3DXVECTOR3 DrivenPosition, D3DXVECTOR3 DriverPosition);
	D3DXVECTOR3 ForceMagnetical(D3DXVECTOR3 DrivenSpeed, D3DXVECTOR3 DriverForce);
};