#include "Help.h"

CInfo::CInfo(){

}

CInfo::~CInfo(){

}

BOOL CInfo::Create(HINSTANCE hInstance, HWND parentHWnd, UINT x, UINT y, UINT width, UINT height) {
	m_hWnd = CreateWindowEx(NULL,                   // Keine erweiterten Stile nutzen
                          L"WindowClass",           // Klassenname
                          L"Über...",				// Fenstertitel
                          WS_NOMINIMIZEBOX |		// Fenster
						  WS_VISIBLE,				// Eigenschaften
                          x, y,						// Anfangsposition
                          width, height,			// und Groesse des Fensters
                          parentHWnd,               // Handle des Elternfensters
                          NULL,						// Handle des Menues
                          hInstance,				// Anwendungsinstanz
                          NULL);					// wird nicht benoetigt

	if(!m_hWnd) return FALSE;

	CreateWindow(L"static", L"\n\n\n    Name :  EB-Simulation \n\n    Autor :   Matthias Lochbrunner",WS_CHILD | WS_VISIBLE | ES_LEFT,
	 00, 00, 320, 240, m_hWnd, NULL,	 hInstance, NULL);

	return TRUE;
}

void CInfo::MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam){
	switch(msg) {
		case WM_CLOSE:
				m_OnWindowClosed(m_ID);
			break;
	}
}