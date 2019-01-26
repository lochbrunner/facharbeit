#pragma once

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <iostream>
#include <strsafe.h>
#include <cstdio>
#include <time.h>


#define LOGBOOK_FILE L"../logfile/logbook"

class __declspec(dllexport) CLogBook 
{ 
public:
	

	CLogBook(LPCWSTR name, LPCWSTR version = L"");
	virtual ~CLogBook();

	void Succed(LPCWSTR location);
	void Succed(LPCWSTR location, LPWSTR describtion);
	void Succed(LPCWSTR location, LPSTR describtion);
	void Error(LPCWSTR location, LPCWSTR reason, const int ErrorCode);
	void Error(LPCWSTR location, HRESULT Error, const int ErrorCode);
	void Warning(LPCWSTR location, LPCWSTR reason, const int ErrorCode);
	void Title(LPCWSTR text);
	void SubTitle(LPCWSTR text);
private: 
	
	FILE *Datei;
	WCHAR FileName[128];
};
