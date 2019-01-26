#include "windows.h"
#include "Physics.h"
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
#define STRING_NAME					L"name "
#define STRING_POSITION				L"position "
#define STRING_SPEED				L"speed "
#define STRING_SPEEDING_UP			L"speedingUp "
#define STRING_VIEW_AT				L"viewAt "
#define STRING_VECTOR				L"vector"
#define STRING_DRIVEN				L"bDriven "
#define STRING_DRIVER				L"bDriver "
#define STRING_SELECTED				L"bSelected "
#define STRING_SHOW_SPEED			L"bShowSpeed "
#define STRING_SHOW_SPEEDING_UP		L"bShowSpeedingUp "
#define STRING_SHOW_VECTOR			L"bShowVector "
#define STRING_BRECKET_OPEN			L"{ "
#define STRING_BRECKET_CLOSE		L"} "

//#define VERSION 1

const UINT VERSION = 1;




class CSaveLoad {

protected:
	CPhysics* m_lpPhysics;
	CLogBook* m_lpLogBook;

	list<Protone*>* m_lpListProtone;
	list<Electrone*>* m_lpListElectrone;
	list<ElectricalField*>* m_lpListElectricalField;
	list<MagneticalField*>* m_lpListMagneticalField;
	list<GravityField*>* m_lpListGravityField;

	list<Protone*>::iterator itP;
	list<Electrone*>::iterator itE;
	list<ElectricalField*>::iterator itEF;
	list<MagneticalField*>::iterator itMF;
	list<GravityField*>::iterator itGF;

	OPENFILENAMEW Filename; 
	FILE* Datei;

	WCHAR Title[256];

public:
	CSaveLoad();
	virtual ~CSaveLoad();

	void SetCPhysicsPtr(CPhysics* lpPhysics);
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }

	BOOL Load(LPWSTR lpFilename);
	void Save(LPWSTR lpFilename);
	void SaveAs(void);
	LPCWSTR GetFileName(void) { return Filename.lpstrFile; }
	LPCWSTR GetFileNameEx(void);

protected:
	BOOL IsString1in2(LPWSTR str1, LPWSTR str2, UINT length);
	BOOL IsString1in2(LPWSTR str1, LPWSTR str2, UINT length, int AtPosition);

	void GetString(LPWSTR source, LPWSTR dest, UINT start, UINT length);
	void GetVector(LPWSTR source, D3DXVECTOR3* vec, UINT start, UINT length);
	void GetBool(LPWSTR source, BOOL* b, UINT start, UINT length);
	void GetUInt(LPWSTR source, UINT* ui, UINT start, UINT length);
	UINT GetUInt(LPWSTR source, UINT start, UINT length);

	void AddProtone();
	void AddElectrone();
	void AddElectricalField();
	void AddMagneticalField();
	void AddGravityField();
	

};