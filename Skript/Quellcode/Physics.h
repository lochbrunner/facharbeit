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
#define TYPE_DEFAULT_CUBOID 7
#define TYPE_DEFAULT_CHARGE 8

#define PHSYSICS_FLAG_IGNOR_ENVIREMENT 0x00000000
#define PHSYSICS_FLAG_INCLUDE_ENVIREMENT 0x00000001
#define PHSYSICS_FLAG_ONLY_CHARGES 0x00000002


#define PI 3.14159265358979323846f

//#define PI_F static_cast<float>(PI);
#define PI_F 3.14159265f
#define PI_2_F static_cast<float>(PI * 0.5)


struct Protone{
	static const int Type = TYPE_PROTONE;
	int tempType;
	BOOL bSelected;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Speed;
	D3DXVECTOR3 SpeedingUp;
	BOOL bDriver;
	BOOL bDriven;
	BOOL bShowSpeed;
	BOOL bShowSpeedup;
	BOOL bKill;
	BOOL bTrace;
	WCHAR Name[64];
};

typedef Protone* LPDEFAULTELEMENT ;

struct Electrone{
	static const int Type = TYPE_ELECTRONE;
	int tempType;
	BOOL bSelected;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Speed;
	D3DXVECTOR3 SpeedingUp;
	BOOL bDriver;
	BOOL bDriven;
	BOOL bShowSpeed;
	BOOL bShowSpeedup;
	BOOL bKill;
	BOOL bTrace;
	WCHAR Name[64];
};

struct DefaultSphere{
	static const int Type = TYPE_DEFAULT_SPHERE;
	int tempType;
	BOOL bSelected;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Speed;
	D3DXVECTOR3 SpeedingUp;
	float Radius;
	BOOL bDriver;
	BOOL bDriven;
	BOOL bShowSpeed;
	BOOL bShowSpeedup;
	BOOL bKill;
	float Mass;
	float Coulomb;
	BOOL bTrace;
	WCHAR Name[64];
};

struct DefaultCharge{
	static const int Type = TYPE_PROTONE;
	int tempType;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Speed;
	D3DXVECTOR3 SpeedingUp;
	float Coulomb;
	BOOL bKill;
};

struct DefaultCuboid{
	static const int Type = TYPE_DEFAULT_CUBOID;
	int tempType;
	BOOL bSelected;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Speed;
	D3DXVECTOR3 SpeedingUp;
	D3DXVECTOR3 rPosition;
	D3DXVECTOR3 rSpeed;
	D3DXVECTOR3 rSpeedingUp;
	D3DXVECTOR3 Size;
	BOOL bDriver;
	BOOL bDriven;
	BOOL bKill;
	BOOL bShowCharges;
	float Mass;
	float Coulomb;
	UINT NumParticies;
	BOOL bTrace;
	WCHAR Name[64];
	DefaultCharge* lpDefaultCharge;

};


struct ElectricalField{
	static const int Type = TYPE_FIELD_ELECTRICAL;
	int tempType;
	BOOL bSelected;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Vector;
	D3DXVECTOR3 ViewAt;
	BOOL bKill;
	BOOL bShowVector;
	WCHAR Name[64];
};

struct MagneticalField{
	static const int Type = TYPE_FIELD_MAGNETICAL;
	int tempType;
	BOOL bSelected;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Vector;
	D3DXVECTOR3 ViewAt;
	BOOL bKill;
	BOOL bShowVector;
	WCHAR Name[64];
};

struct GravityField{
	int tempType;
	static const int Type = TYPE_FIELD_GRAVITY;
	BOOL bSelected;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Vector;
	D3DXVECTOR3 ViewAt;
	BOOL bKill;
	BOOL bShowVector;
	WCHAR Name[64];
};

struct Emitter{
	int tempType;
	static const int Type = TYPE_OBJECT_EMITTER;
	BOOL bSelected;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Vector;
	D3DXVECTOR3 ViewAt;
	D3DXVECTOR3 Random;
	BOOL bKill;
	BOOL bShowVector;
	UINT elementType;
	UINT waitTime;
	WCHAR Name[64];

};

typedef void    (CALLBACK *LPCALLBACKONSTARTTRACE)(LPDEFAULTELEMENT lpElement);
typedef void    (CALLBACK *LPCALLBACKRENDER)(void);								// used by time intensive prozesses

#define MAGNETICAL_HOLD 0.10118f


class CPhysics {

protected:
	list<Protone*>ListProtone;
	list<Electrone*>ListElectrone;
	list<DefaultSphere*>ListDefaultSphere;
	list<DefaultCuboid*>ListDefaultCuboid;
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
	UINT CountDefaultCuboid;
	
	CLogBook* m_lpLogBook;

	LONG count;
	
	LPCALLBACKONSTARTTRACE m_OnStartRace;
	LPCALLBACKRENDER m_Render;

public:
	CPhysics();
	virtual ~CPhysics();

	void SetResourceManagerPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetCallbackOnStartTrace(LPCALLBACKONSTARTTRACE OnStartRace) { m_OnStartRace = OnStartRace; }
	void SetCallbackRender(LPCALLBACKRENDER Render) { m_Render = Render;}

	Protone* AddProtone(void);
	Electrone* AddElectrone(void);
	DefaultSphere* AddDefaultSphere(void);
	DefaultCuboid* AddDefaultCuboid(void);
	ElectricalField* AddElectricalField(void);
	MagneticalField* AddMagneticalField(void);
	GravityField* AddGravityField(void);
	Emitter* AddEmitter(void);

	void AddProtone(Protone* lpProtone);
	void AddElectrone(Electrone* lpElectrone);
	void AddDefaultSphere(DefaultSphere* lpDefaultSphere);
	void AddDefaultCuboid(DefaultCuboid* lpDefaultCuboid);
	void AddElectricalField(ElectricalField* lpElectricalField);
	void AddMagneticalField(MagneticalField* lpMagneticalField);
	void AddGravityField(GravityField* lpGravityField);
	void AddEmitter(Emitter* lpEmitter);

	void SetDefaultCuboidCharges(DefaultCuboid* lpDefaultCuboid, DWORD Flag);

	void DeleteSelected(void);
	void DeleteSpacial(void);

	void OnRender(float elapsedTime);

	list<Protone*>* GetListProtonePtr(void) {return &ListProtone; }
	list<Electrone*>* GetListElectronePtr(void) {return &ListElectrone; }
	list<DefaultSphere*>* GetListDefaultSpherePtr(void) {return &ListDefaultSphere; }
	list<DefaultCuboid*>* GetListDefaultCuboidPtr(void) {return &ListDefaultCuboid; }
	list<ElectricalField*>* GetListElectricalFieldPtr(void) {return &ListElectricalField; }
	list<MagneticalField*>* GetListMagneticalFieldPtr(void) {return &ListMagneticalField; }
	list<GravityField*>* GetListGravityFieldPtr(void) {return &ListGravityField; }
	list<Emitter*>* GetListEmitterPtr(void) { return &ListEmitter; }

	void ClearLists(void);

protected:
	D3DXVECTOR3 ForceElectrical(D3DXVECTOR3 DrivenPosition, D3DXVECTOR3 DriverPosition);
	D3DXVECTOR3 ForceMagnetical(D3DXVECTOR3 DrivenSpeed, D3DXVECTOR3 DriverForce);
};