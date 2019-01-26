#include "windows.h"
#include "Physics.h"
#include "Scene.h"
#include <iostream>
#include <strsafe.h>
#include <cstdio>

#define STRING_TITLE				L"This_is_a_EB-Simulation_file "
#define STRING_VERSION				L"version "
#define STRING_PROTONE				L"Protone "
#define STRING_ELECTRONE			L"Electrone "
#define STRING_FIELD_ELECTRICAL		L"ElectricalField "
#define STRING_FIELD_MAGNETICAL		L"MagneticalField "
#define STRING_FIELD_GRAVITY		L"GravityField "
#define STRING_OBJECT_EMITTER		L"Emitter "
#define STRING_OBJECT_CUBOID		L"Cuboid "
#define STRING_VIRTUAL_CHARGE		L"virtualCharge "
#define STRING_OBJECT_SPHERE		L"Sphere "
#define STRING_NAME					L"name "
#define STRING_POSITION				L"position "
#define STRING_SPEED				L"speed "
#define STRING_SPEEDING_UP			L"speedingUp "
#define STRING_ROTATION				L"rotation "
#define STRING_ROTATION_SPEED		L"rotationSpeed "
#define STRING_ROTATION_SPEEDING_UP	L"rotationSpeedingUp "
#define STRING_SIZE					L"size "
#define STRING_RADIUS				L"radius "
#define STRING_VIEW_AT				L"viewAt "
#define STRING_VECTOR				L"vector"
#define STRING_RANDOM_VECTOR		L"randomVector"
#define STRING_MASS					L"mass "
#define STRING_CHARGES				L"charges "
#define STRING_NUM_PARTS			L"numParts "
#define STRING_WAIT_TIME			L"timeToWait "
#define STRING_ELEMENT_TYPE			L"elementType "
#define STRING_DRIVEN				L"bDriven "
#define STRING_DRIVER				L"bDriver "
#define STRING_SELECTED				L"bSelected "
#define STRING_SHOW_SPEED			L"bShowSpeed "
#define STRING_SHOW_SPEEDING_UP		L"bShowSpeedingUp "
#define STRING_SHOW_VECTOR			L"bShowVector "
#define STRING_SHOW_TRACE			L"bShowTrace "
#define STRING_SHOW_CHARGES			L"bShowCharges "
#define STRING_ID					L"id "
#define STRING_BRECKET_OPEN			L"{ "
#define STRING_BRECKET_CLOSE		L"} "


//----------------------------------------------------------------------------------------------------
// Global functions
//----------------------------------------------------------------------------------------------------


void GetString(LPWSTR source, LPWSTR dest, UINT start, UINT length);
void GetVector(LPWSTR source, D3DXVECTOR3* vec, UINT start, UINT length);
void GetBool(LPWSTR source, BOOL* b, UINT start, UINT length);
void GetUInt(LPWSTR source, UINT* ui, UINT start, UINT length);
void GetFloat(LPWSTR source, float* lpF, UINT start, UINT length);
UINT GetUInt(LPWSTR source, UINT start, UINT length);
BOOL IsString1in2(LPWSTR str1, LPWSTR str2, UINT length);
BOOL IsString1in2(LPWSTR str1, LPWSTR str2, UINT length, int AtPosition);

const UINT VERSION = 2;


//----------------------------------------------------------------------------------------------------
// CSaveLoad class
//----------------------------------------------------------------------------------------------------

class CSaveLoad {

protected:
	CPhysics* m_lpPhysics;
	CLogBook* m_lpLogBook;
	CScene* m_lpScene;

	list<Protone*>* m_lpListProtone;
	list<Electrone*>* m_lpListElectrone;
	list<ElectricalField*>* m_lpListElectricalField;
	list<MagneticalField*>* m_lpListMagneticalField;
	list<GravityField*>* m_lpListGravityField;
	list<Emitter*>* m_lpListEmitter;
	list<DefaultCuboid*>* m_lpListDefaultCuboid;
	list<DefaultSphere*>* m_lpListDefaultSphere;

	list<Protone*>::iterator itP;
	list<Electrone*>::iterator itE;
	list<ElectricalField*>::iterator itEF;
	list<MagneticalField*>::iterator itMF;
	list<GravityField*>::iterator itGF;
	list<Emitter*>::iterator itOE;
	list<DefaultCuboid*>::iterator itDC;
	list<DefaultSphere*>::iterator itDS;

	OPENFILENAMEW Filename; 
	FILE* Datei;

	WCHAR Title[256];

public:
	CSaveLoad();
	virtual ~CSaveLoad();

	void SetCPhysicsPtr(CPhysics* lpPhysics);
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	void SetCScenePtr(CScene* lpScene) {m_lpScene = lpScene; }

	void SaveInMemmory();
	void LoadFromMemmory();

	BOOL Load(LPWSTR lpFilename);
	BOOL LoadA(LPSTR lpFilename);
	void Save(LPWSTR lpFilename);
	void SaveAs(void);
	LPCWSTR GetFileName(void) { return Filename.lpstrFile; }
	LPCWSTR GetFileNameEx(void);

protected:

	void AddProtone();
	void AddElectrone();
	void AddElectricalField();
	void AddMagneticalField();
	void AddGravityField();
	void AddEmitter();
	void AddDefaultCuboid();
	void AddDefaultSphere();	

};

//----------------------------------------------------------------------------------------------------
// CSettingsLoader class
//----------------------------------------------------------------------------------------------------

typedef struct _VECTOR3{
	float x,y,z;
	}  VECTOR3;

class CSettingsLoader {

protected:
	map<WCHAR[64], LPCWSTR>m_MapString;
	map<BOOL, LPCWSTR>m_MapBool;
	map<int, LPCWSTR>m_MapInt;
	map<UINT, LPCWSTR>m_MapUInt;
	map<float, LPCWSTR>m_MapFloat;
	map<VECTOR3, LPCWSTR>m_MapVector;

public:
	CSettingsLoader();
	virtual ~CSettingsLoader();

	DWORD GetPointer(LPCWSTR Pointer);

protected:

	void LoadSettings();
	void SaveSettings(LPCWSTR Name, LPCWSTR Pointer, UINT type);

};