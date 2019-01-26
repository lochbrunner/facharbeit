
#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <map>
#include <string>
#include "LogBook.h"


using namespace std;


class CResourceManager {

protected:
	CLogBook* m_lpLogBook;

public:
    CResourceManager();
    virtual ~CResourceManager();
	
	void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }
	

    // Initialisierung des Ressourcenmanagers
    BOOL Init(LPDIRECT3DDEVICE9 lpD3DDevice);

    // mit dieser Methode werden Texturen hinzugefuegt
    void AddTexture(LPCWSTR lpFileName);

    // diese Methode gibt einen Zeiger auf die Textur lpTextureName zurueck
    LPDIRECT3DTEXTURE9 GetTexture(LPCWSTR lpTextureName);

    // Framerate zurueckgeben
    int GetFramerate(void);

	void OnResetDevice(void);

    void Update(void);

protected:

    // Zeiger auf das Direct3D-Device
    LPDIRECT3DDEVICE9 m_lpD3DDevice;

    // diese Map speichert den Namen und den Zeiger auf die Textur
    map<wstring, LPDIRECT3DTEXTURE9> m_MapTextures;

    // Frequenz des Counters
    LONGLONG m_Frequency;

    // Zaehlerstand zu Beginn der letzten Sekunde
    LONGLONG m_LastCount;

    // aktueller Zaehlerstand
    LONGLONG m_CurCount;

    // die Framerate
    unsigned int m_Framerate;

    // aktueller Frame
    int m_Frames;

};

