// Dieses Programm zeigt, wie eine Oberflaeche mit Farben gefuellt  und angezeigt wird.

#pragma warning (disable : 4996 )

// Headerdateien
#include "windows.h"
#include "Scene.h"
#include "Physics.h"
#include "Explorer.h"
#include "Help.h"
#include "SaveLoad.h"
#include "ChannelBox.h"

#define FILE_INI L"../settings/settings.ini"
#define FILE_TEMP L"../settings/temp.ini"


#define BUTTON_EXIT 0
#define BUTTON_ADD_PROTONE 1
#define BUTTON_ADD_ELECTRONE 2
#define BUTTON_SHOW_EXPLORER_WINDOW 3
#define BUTTON_SHOW_INFO_WINDOW 4
#define BUTTON_ANIMATION_START 5
#define BUTTON_ANIMATION_STOP 6
#define BUTTON_ANIMATION_STEP 7
#define BUTTON_ANIMATION_RESET 8
#define BUTTON_NEW_SCENE 9
#define BUTTON_DELETE_ALL 10
#define BUTTON_SHOW_GRID 11
#define BUTTON_HELP_OFFLINE 12
#define BUTTON_ADD_FIELD_ELECTRICAL 13
#define BUTTON_ADD_FIELD_MAGNETICAL 14
#define BUTTON_ADD_FIELD_GRAVITY 15
#define BUTTON_OPEN_FILE 16
#define BUTTON_SAVE_FILE 17
#define BUTTON_SAVE_FILE_AS 18
#define BUTTON_ADD_SCENE 19
#define BUTTON_HELP_ONLINE 20
#define BUTTON_SCREENSHOT 21
#define BUTTON_LAST_FILE_1  22
#define BUTTON_LAST_FILE_2  23
#define BUTTON_LAST_FILE_3  24
#define BUTTON_LAST_FILE_4  25
#define BUTTON_SHOW_CHANNEL_BOX 26
#define BUTTON_ADD_EMITTER 27
#define BUTTON_ADD_DEFAULT_CUBOID 28
#define BUTTON_ADD_DEFAULT_SPHERE 29
#define BUTTON_SELECT_EVERTHING 30
#define BUTTON_ENABLE_HEUN 31
#define BUTTON_DISABLE_HEUN 32



#define TOOLBAR_BUTTON_SHOW_POSITION 50
#define TOOLBAR_BUTTON_SHOW_SPEED 51
#define TOOLBAR_BUTTON_SHOW_SPEEDUP 52
#define TOOLBAR_BUTTON_CEBTER_SCENE 53
#define TOOLBAR_BUTTON_DELTE_ELEMENT 54


#define WI_EXPLORER 0
#define WI_INFO 1

#define UPDATE_RATE_TRACE 3

#ifdef _DEBUG

#define TITLE L"EB-Simulation (debug)"

#else

#define TITLE L"EB-Simulation (release)"

#endif


// Anwendungsfenster erzeugen
HWND CreateMainWindow(HINSTANCE hInstance);

// Menu erzeugen
void CreateMenu(HWND hWnd);

void CreateStatusBar(HWND hWnd);

void CreateToolBar(HWND hWnd);

void GetIniFile();
void SetIniFile();

void SelectEverything();

// Callback Funktion zur Nachrichtenbehandlung
LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

void CALLBACK OnWindowClose(UINT WindowId);

void CALLBACK OnPicked(LPDEFAULTELEMENT lpSelected);

void CALLBACK OnArrowMoved(void);

void CALLBACK OnUpToRender(void);

void CALLBACK OnRender(void);

void CALLBACK OnStartTrace(LPDEFAULTELEMENT lpElement);

void CALLBACK OnUpdate(DWORD ID, DWORD Pointer);

WCHAR lastFile[4][2][256];


// Das Fensterhandle
HWND hWnd = 0;
HINSTANCE m_hInstance;

HWND hwndStatus;
HWND hwndToolbar;

static HBITMAP hBitmap ;
static BITMAP  bitmap ;

CLogBook LogBook(L"EB-Simulation", L"0.4");

CSaveLoad SaveLoad;

CScene Scene;

CPhysics Physics;

CChannelBox ChannelBox;


CExplorerWindow* lpExplorerWindow = NULL;
CInfo* lpInfoWindow = NULL;

BOOL runAnimation = false;
BOOL running = false;
BOOL IsSceneNamed = false;
BOOL saved = false;

LONG count = 0;

DWORD m_PhsicsRenderMode = PHSYSICS_RENDERMODE_NORMAL;


// Windows main-Funktion
int WINAPI WinMain(HINSTANCE hInstance,      // Handle der Programminstanz
                   HINSTANCE hPrevInstance,  // Handle der letzten Instanz
                   LPSTR lpCmdLine,          // Kommandozeile
                   int nCmdShow)             // Art wie das Fenster angezeigt werden soll
{	

	CHAR TextA[256];
	CHAR TextB[256];
	
	strcpy(TextA, lpCmdLine);


	//MessageBoxA(NULL, lpCmdLine, "Info", MB_OK);
	//MessageBoxA(NULL, TextB, "Info", MB_OK);

	LogBook.Title(L"Das Programm wird initialisiert");


	GetIniFile();

    // Fenster erzeugen und Handle speichern
    hWnd = CreateMainWindow(hInstance);


	// Instance speichern
	m_hInstance = hInstance;
	
    // Wenn der Rueckgabewert 0 ist, ist ein Fehler aufgetreten
    if(0 == hWnd)
    {	
        MessageBox(0, L"Fenster konnte nicht erzeugt werden", L"Fehler", MB_OK);
        return -1;
    }
	else {
		LogBook.Succed(L"CreateMainWindow");
	}
	
	CreateMenu(hWnd);
	LogBook.Succed(L"CreateMenu");

	CreateStatusBar(hWnd);
	LogBook.Succed(L"CreateStatusBar");

	CreateToolBar(hWnd);
	LogBook.Succed(L"CreateToolBar");

	RECT rcCurrentClient;
	GetClientRect( hWnd, &rcCurrentClient );

	LogBook.SubTitle(L"Initialiesierung der Scene Klasse");

	Scene.SetLogBookPtr(&LogBook);

	if(!Scene.Init(hInstance, hWnd, 0, 28, rcCurrentClient.right - 0, rcCurrentClient.bottom - 26 - 28)) {
		MessageBoxW(0, L"Child window konnte nicht erstellt werden", L"Fehler", MB_OK);
        return -1;
	}

	LogBook.Succed(L"Scene.Init");

	ChannelBox.SetLogBookPtr(&LogBook);
	ChannelBox.SetCallbackUpToRender(&OnUpToRender);
	ChannelBox.SetCallbackUpToDate(OnUpdate);
	ChannelBox.SetCPhysicsPtr(&Physics);
	ChannelBox.SetCScenePtr(&Scene);
	ChannelBox.RegistrateWindowClass(hInstance);
	ChannelBox.CreateChannelBox(hWnd, GetSystemMetrics(SM_CXSCREEN) - 210, 90, 200, 450); 

	LogBook.Succed(L"ChannelBox.CreateChannelBox");

	Physics.SetCallbackOnStartTrace(&OnStartTrace);
	Physics.SetCallbackRender(&OnRender);
		
	Scene.SetCPhysicsPtr(&Physics);
	Scene.SetStatusBar(hwndStatus);
	Scene.SetCallbackOnPicked(OnPicked);
	Scene.SetCallbackOnArrowMoved(OnArrowMoved);
	Scene.SetClearColor(0xFFA0A0A0);
	//Scene.SetClearColor(0xFF000000);
	//Scene.SetClearColor(0xFFFFFFFF);

	SaveLoad.SetCPhysicsPtr(&Physics);
	SaveLoad.SetLogBookPtr(&LogBook);
	SaveLoad.SetCScenePtr(&Scene);
	    
	// Struktur, in der Informationen zur Nachricht gespeichert werden
    MSG msg = { 0 };

    // Diese Schleife laeuft bis die Nachricht WM_QUIT empfangen wird
	
	running = true;

	bool b = true;

	LogBook.Succed(L"Initialisierungen abgeschlossen");
	LogBook.Title(L"Das Programm laeuft ...");
		

	//if(lpCmdLine[0] == '\"') {
		
		//if(IDYES == MessageBoxA(NULL, TextA, "Info", MB_ICONWARNING | MB_YESNO)) {
		//	SaveLoad.LoadA(TextB);
		//}

	//}

	//int i = 0;

	//while(TextA[i +1] != '\"' && TextA[i +1] != '\0') {
	//	TextB[i]= TextA[i +1];
	//	i++;
	//}
	//TextB[i] = '\0';

	//if(i > 3)SaveLoad.LoadA(TextB);

	
    while(msg.message != WM_QUIT)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {	// Text ausgeben
			if(runAnimation) {
				count++;
				count = count % UPDATE_RATE_TRACE;
				Physics.OnRender(0.0f, m_PhsicsRenderMode);
				if(count == 0)Scene.UpdateTrace();
				Scene.SortOut();
			    Scene.Render();
				ChannelBox.Update();
			} else {
				Sleep(5);
			}
			if(b) {
				Scene.Render();
				LogBook.Succed(L"Erster Rendervorgang");
				b = false;
			}
		}
		
    }

	SetIniFile();
	LogBook.Title(L"Das Programm wird beendet ...");

    // Rueckgabewert an Windows
    return 0;
}


HWND CreateMainWindow(HINSTANCE hInstance)
{	

	WNDCLASSEXW wndClass =
    {
        sizeof(WNDCLASSEX),                                 // Groesse angeben
        CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW,    // Standardstile
        MessageHandler,                                     // Callback-Funktion
        0,                                                  // Zusaetzliche Angaben
        0,                                                  // nicht benoetigt
        hInstance,                                          // Anwendungsinstanz
        LoadIcon(NULL, IDI_WINLOGO),                        // Windows-Logo
        LoadCursor(NULL, IDC_ARROW),                        // Normaler Cursor
        (HBRUSH)GetStockObject(LTGRAY_BRUSH),               // Weisser Pinsel
        NULL,                                               // kein Menue
        L"WindowClass",                                     // Der Name der Klasse
        LoadIcon(hInstance, L"ICON")
    };

    // Klasse registrieren
    RegisterClassEx(&wndClass);



    //return CreateWindowEx(NULL,								// Keine erweiterten Stile nutzen
    //                      L"WindowClass",					// Klassenname
    //                      L"EB-Simulation",					// Fenstertitel
    //                      WS_OVERLAPPEDWINDOW |				// Fenster
				//		  WS_VISIBLE /*| WS_MAXIMIZE*/,         // Eigenschaften
    //                      GetSystemMetrics(SM_CXSCREEN) / 2 - 512,
				//		  GetSystemMetrics(SM_CYSCREEN) / 2 - 384,       // Anfangsposition
    //                      1024, //GetSystemMetrics(SM_CXSCREEN),
				//		  768, //GetSystemMetrics(SM_CYSCREEN),	// und Groesse des Fensters
    //                      NULL,								// Handle des Elternfensters
    //                      NULL,								// Handle des Menues
    //                      hInstance,						// Anwendungsinstanz
    //                      NULL);							// wird nicht benoetigt


	 return CreateWindowEx(NULL,								// Keine erweiterten Stile nutzen
                          L"WindowClass",					// Klassenname
                          TITLE,					// Fenstertitel
                          WS_OVERLAPPEDWINDOW |				// Fenster
						  WS_VISIBLE /*| WS_MAXIMIZE*/,         // Eigenschaften
                          0,
						  0,       // Anfangsposition
                          GetSystemMetrics(SM_CXSCREEN),
						  GetSystemMetrics(SM_CYSCREEN),	// und Groesse des Fensters
                          NULL,								// Handle des Elternfensters
                          NULL,								// Handle des Menues
                          hInstance,						// Anwendungsinstanz
                          NULL);



}


// Diese Funktion erstellt ein Menu
void CreateMenu(HWND hWnd){
	HMENU menu = CreateMenu(); 
	HMENU datei = CreateMenu();
	HMENU ansicht = CreateMenu();
	HMENU hilfe = CreateMenu();
	HMENU bearbeiten = CreateMenu();
	HMENU hinzufügen = CreateMenu();
	HMENU teilchen = CreateMenu();
	HMENU felder = CreateMenu();
	HMENU geraete = CreateMenu();
	HMENU fenster = CreateMenu();
	HMENU animation = CreateMenu();
	HMENU letzteDatei = CreateMenu();

	for(int i = 0; i < 4; i++) {

		if(lastFile[i][0][0] != L'\0') {
			AppendMenu(letzteDatei, 0, BUTTON_LAST_FILE_1 + i, lastFile[i][0]);
		}
	}

	AppendMenu(datei, 0, BUTTON_NEW_SCENE, TEXT("Neu"));
	AppendMenu(datei, MF_SEPARATOR, 0, NULL);
	AppendMenu(datei, 0, BUTTON_OPEN_FILE, TEXT("Öffnen"));
	AppendMenu(datei, 0, BUTTON_SAVE_FILE, TEXT("Speichern"));
	AppendMenu(datei, 0, BUTTON_SAVE_FILE_AS, TEXT("Speichern unter ..."));
	AppendMenu(datei, MF_SEPARATOR, 0, NULL);
	AppendMenu(datei, MF_POPUP, reinterpret_cast<UINT_PTR>(letzteDatei), TEXT("Zuletzt geöffnete Datei"));
	AppendMenu(datei, MF_SEPARATOR, 0, NULL);
	AppendMenu(datei, 0, BUTTON_EXIT, TEXT("Beenden"));
	//AppendMenu(bearbeiten, 0, 99, TEXT("Rückgängig"));
	//AppendMenu(bearbeiten, 0, 99, TEXT("Wiederherstellen"));
	//AppendMenu(bearbeiten, MF_SEPARATOR, 0, NULL);

	AppendMenu(teilchen, 0, BUTTON_ADD_ELECTRONE, TEXT("Elektron"));
	AppendMenu(teilchen, 0, BUTTON_ADD_PROTONE, TEXT("Proton"));
	AppendMenu(hinzufügen, MF_POPUP, reinterpret_cast<UINT_PTR>(teilchen), TEXT("Mikroteilchen"));

	AppendMenu(felder, 0, BUTTON_ADD_FIELD_ELECTRICAL, TEXT("Elektrisches Feld"));
	AppendMenu(felder, 0, BUTTON_ADD_FIELD_MAGNETICAL, TEXT("Magnetisches Feld"));
	AppendMenu(felder, 0, BUTTON_ADD_FIELD_GRAVITY, TEXT("Gravitationsfeld"));
	AppendMenu(hinzufügen, MF_POPUP, reinterpret_cast<UINT_PTR>(felder), TEXT("Homogene Felder"));
	
	AppendMenu(geraete, 0, BUTTON_ADD_DEFAULT_SPHERE, TEXT("Metallkugel"));
	AppendMenu(geraete, 0, BUTTON_ADD_DEFAULT_CUBOID, TEXT("Kondensatorplatte"));
	AppendMenu(geraete, MF_SEPARATOR, 0, NULL);
	AppendMenu(geraete, 0, BUTTON_ADD_EMITTER, TEXT("Emitter"));
	AppendMenu(hinzufügen, MF_POPUP, reinterpret_cast<UINT_PTR>(geraete), TEXT("Makroteilchen"));

	AppendMenu(hinzufügen, MF_SEPARATOR, 0, NULL);
	AppendMenu(hinzufügen, 0, BUTTON_ADD_SCENE, TEXT("Szene aus Datei"));

	AppendMenu(bearbeiten, 0, BUTTON_SELECT_EVERTHING, TEXT("Alles markieren"));
	AppendMenu(bearbeiten, 0, BUTTON_DELETE_ALL, TEXT("Alles löschen"));
	AppendMenu(animation, 0, BUTTON_ANIMATION_START, TEXT("Animation starten"));
	AppendMenu(animation, 0, BUTTON_ANIMATION_STOP, TEXT("Animation stoppen"));
	AppendMenu(animation, MF_SEPARATOR, 0, NULL);
	AppendMenu(animation, 0, BUTTON_ANIMATION_RESET, TEXT("Animation zurücksetzen"));
	AppendMenu(animation, MF_SEPARATOR, 0, NULL);
	AppendMenu(animation, 0, BUTTON_ENABLE_HEUN, TEXT("Verfahren von Heun verwenden"));
	AppendMenu(animation, 0, BUTTON_DISABLE_HEUN, TEXT("Verfahren von Heun ausschalten"));


	AppendMenu(fenster, MF_UNCHECKED, BUTTON_SHOW_EXPLORER_WINDOW, TEXT("Explorer"));
	AppendMenu(fenster, MF_UNCHECKED, BUTTON_SHOW_CHANNEL_BOX, TEXT("Kontroll Fenster"));

	AppendMenu(ansicht, MF_POPUP, reinterpret_cast<UINT_PTR>(fenster), TEXT("Weitere Fenster ..."));
	AppendMenuW(ansicht, MF_CHECKED, BUTTON_SHOW_GRID, TEXT("Raster anzeigen"));
	AppendMenu(ansicht, MF_SEPARATOR, 0, NULL);
	AppendMenu(ansicht, 0, BUTTON_SCREENSHOT, TEXT("Szene als Bild speichern"));

	AppendMenu(hilfe, 0, BUTTON_HELP_ONLINE, TEXT("Online Hilfe aufrufen"));
	AppendMenu(hilfe, 0, BUTTON_HELP_OFFLINE, TEXT("Offline Hilfe aufrufen"));
	AppendMenu(hilfe, MF_SEPARATOR, 0, NULL);
	AppendMenu(hilfe, 0, BUTTON_SHOW_INFO_WINDOW, TEXT("Über ..."));
	
	AppendMenu(menu, MF_POPUP, reinterpret_cast<UINT_PTR>(datei), TEXT("Datei"));
	
	AppendMenu(menu, MF_POPUP, reinterpret_cast<UINT_PTR>(bearbeiten), TEXT("Bearbeiten"));
	AppendMenu(menu, MF_POPUP, reinterpret_cast<UINT_PTR>(hinzufügen), TEXT("Hinzufügen"));
	AppendMenu(menu, MF_POPUP, reinterpret_cast<UINT_PTR>(animation), TEXT("Animation"));
	AppendMenu(menu, MF_POPUP, reinterpret_cast<UINT_PTR>(ansicht), TEXT("Ansicht"));
	AppendMenu(menu, MF_POPUP, reinterpret_cast<UINT_PTR>(hilfe), TEXT("Hilfe"));
	SetMenu(hWnd, menu);
}


void CreateStatusBar(HWND hWnd){
	
	INITCOMMONCONTROLSEX icex;

    icex.dwSize 	= sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC   = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);
	hwndStatus = CreateWindowEx (0, STATUSCLASSNAME, L"",WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP,
			0, 0, 0, 0, hWnd, (HMENU)1, m_hInstance, NULL);

	int Partsize[2];

	Partsize[0] = 250;
	Partsize[1] = 700;
	SendMessage (hwndStatus, SB_SETPARTS, 2, (LPARAM)Partsize);
	SendMessage (hwndStatus, SB_SETTEXT , 0, (LPARAM)L"   Bereit");
	SendMessage (hwndStatus, SB_SETTEXT , 1, (LPARAM)L"   Animation angehalten");
	

}

void CreateToolBar(HWND hWnd){
	
	TBBUTTON buttons[]={
	{1,	BUTTON_SHOW_GRID,TBSTATE_ENABLED,TBSTYLE_BUTTON  , 0, 0},
	{0,          0,TBSTATE_ENABLED,TBSTYLE_SEP     , 0, 0},
	{2,TOOLBAR_BUTTON_SHOW_POSITION,TBSTATE_ENABLED,TBSTYLE_BUTTON  , 0, 0},
	{0,          0,TBSTATE_ENABLED,TBSTYLE_SEP     , 0, 0},
	{3,TOOLBAR_BUTTON_SHOW_SPEEDUP,TBSTATE_ENABLED,TBSTYLE_BUTTON  , 0, 0},
	{4,TOOLBAR_BUTTON_SHOW_SPEED,TBSTATE_ENABLED,TBSTYLE_BUTTON  , 0, 0},
	{0,          0,TBSTATE_ENABLED,TBSTYLE_SEP     , 0, 0},
	{5,TOOLBAR_BUTTON_CEBTER_SCENE,TBSTATE_ENABLED,TBSTYLE_BUTTON  , 0, 0},
	{0,          0,TBSTATE_ENABLED,TBSTYLE_SEP     , 0, 0},
	{6,BUTTON_ANIMATION_START,TBSTATE_ENABLED,TBSTYLE_BUTTON  , 0, 0},
	{7,BUTTON_ANIMATION_STOP,TBSTATE_ENABLED,TBSTYLE_BUTTON  , 0, 0},
	{8,BUTTON_ANIMATION_STEP,TBSTATE_ENABLED,TBSTYLE_BUTTON  , 0, 0},
	{0,          0,TBSTATE_ENABLED,TBSTYLE_SEP     , 0, 0},
	{9,TOOLBAR_BUTTON_DELTE_ELEMENT,TBSTATE_ENABLED,TBSTYLE_BUTTON  , 0, 0}
	
	};

	HBITMAP hBmp = LoadBitmapW(m_hInstance, L"BITMAP");

	hwndToolbar =CreateToolbarEx(hWnd, WS_CHILD| WS_VISIBLE| WS_CLIPSIBLINGS| 
                        CCS_TOP|TBSTYLE_FLAT, 0, 13, 0, *reinterpret_cast<UINT*>(&hBmp),
                        buttons, 14, 24, 24, 0, 0, sizeof(TBBUTTON));

}

void GetIniFile() {
	FILE *Datei = _wfopen(FILE_INI, L"r");
	fread(lastFile[0][0], sizeof(lastFile[0][0]), 1, Datei);
	fread(lastFile[1][0], sizeof(lastFile[0][0]), 1, Datei);
	fread(lastFile[2][0], sizeof(lastFile[0][0]), 1, Datei);
	fread(lastFile[3][0], sizeof(lastFile[0][0]), 1, Datei);

	fread(lastFile[0][1], sizeof(lastFile[0][0]), 1, Datei);
	fread(lastFile[1][1], sizeof(lastFile[0][0]), 1, Datei);
	fread(lastFile[2][1], sizeof(lastFile[0][0]), 1, Datei);
	fread(lastFile[3][1], sizeof(lastFile[0][0]), 1, Datei);

	fclose(Datei);

}

void SetIniFile() {
	FILE *Datei = _wfopen(FILE_INI, L"wb");
	fwrite(lastFile[0][0], sizeof(lastFile[0][0]), 1, Datei);
	fwrite(lastFile[1][0], sizeof(lastFile[0][0]), 1, Datei);
	fwrite(lastFile[2][0], sizeof(lastFile[0][0]), 1, Datei);
	fwrite(lastFile[3][0], sizeof(lastFile[0][0]), 1, Datei);

	fwrite(lastFile[0][1], sizeof(lastFile[0][0]), 1, Datei);
	fwrite(lastFile[1][1], sizeof(lastFile[0][0]), 1, Datei);
	fwrite(lastFile[2][1], sizeof(lastFile[0][0]), 1, Datei);
	fwrite(lastFile[3][1], sizeof(lastFile[0][0]), 1, Datei);

  	fclose(Datei);
}

// Diese Funktion wird von Fenstern mit den Windowsstyls WS_OVERLAPPED und nicht WS_CHILD aufgerufen 
void CALLBACK OnWindowClose(UINT WindowId) {
	switch(WindowId) {
		case WI_EXPLORER:
				delete lpExplorerWindow;
				lpExplorerWindow = NULL;
				
			break;
	}
	switch(WindowId) {
		case WI_INFO:
				delete lpInfoWindow;
				lpInfoWindow = NULL;
				
			break;
	}

}

void CALLBACK OnPicked(LPDEFAULTELEMENT lpSelected) {
	//ChannelBox.CleanChannelBox();
	ChannelBox.FillChannelBox(lpSelected);
}

void CALLBACK OnArrowMoved(void){
	ChannelBox.Update();
}

void CALLBACK OnUpToRender(void){
	Scene.UpdateCuboidSize();
	Scene.Render();
}

void CALLBACK OnRender(void) {
	Scene.Render();
}

void CALLBACK OnStartTrace(LPDEFAULTELEMENT lpElement) {
	Scene.AddTraceToSelected(lpElement);
}

void CALLBACK OnUpdate(DWORD ID, DWORD Pointer){
	switch(ID) {
		case UPDATE_CUBOID_PARTS:
				Physics.SetDefaultCuboidCharges((DefaultCuboid*) Pointer, NULL);
			break;
		case UPDATE_CUBOID_CHARGES:
				Physics.SetDefaultCuboidCharges((DefaultCuboid*) Pointer, PHSYSICS_FLAG_ONLY_CHARGES);
			break;
		case UPDATE_SPHERE_SIZE:
				Scene.SetSphereSize((DefaultSphere*) Pointer);
			break;
	}
}

void SelectEverything() {
	list<Protone*>* lpListProtone = Physics.GetListProtonePtr();
	list<Electrone*>* lpListElectrone = Physics.GetListElectronePtr();
	list<DefaultSphere*>* lpListDefaultSphere = Physics.GetListDefaultSpherePtr();
	list<ElectricalField*>* lpListElectricalField = Physics.GetListElectricalFieldPtr();
	list<MagneticalField*>* lpListMagneticalField = Physics.GetListMagneticalFieldPtr();
	list<GravityField*>* lpListGravityField = Physics.GetListGravityFieldPtr();
	list<DefaultCuboid*>* lpListDefaultCuboid = Physics.GetListDefaultCuboidPtr();

	list<Protone*>::iterator itP;
	list<Electrone*>::iterator itE;
	list<DefaultSphere*>::iterator itD;
	list<ElectricalField*>::iterator itEF;
	list<MagneticalField*>::iterator itMF;
	list<GravityField*>::iterator itGF;
	list<DefaultCuboid*>::iterator itDC;

	for(itP =  lpListProtone->begin(); itP !=  lpListProtone->end(); itP++) {
		(*itP)->bSelected = true;
	}
	for(itE =  lpListElectrone->begin(); itE !=  lpListElectrone->end(); itE++) {
		(*itE)->bSelected = true;
	}
	for(itD =  lpListDefaultSphere->begin(); itD !=  lpListDefaultSphere->end(); itD++) {
		(*itD)->bSelected = true;
	}
	for(itEF =  lpListElectricalField->begin(); itEF !=  lpListElectricalField->end(); itEF++) {
		(*itEF)->bSelected = true;
	}
	for(itMF =  lpListMagneticalField->begin(); itMF !=  lpListMagneticalField->end(); itMF++) {
		(*itMF)->bSelected = true;
	}
	for(itGF =  lpListGravityField->begin(); itGF !=  lpListGravityField->end(); itGF++) {
		(*itGF)->bSelected = true;
	}
	for(itDC =  lpListDefaultCuboid->begin(); itDC !=  lpListDefaultCuboid->end(); itDC++) {
		(*itDC)->bSelected = true;
	}

}


// Diese Funktion wird von Windows aufgrufen, wenn
// eine Nachricht fuer Ihr Programm vorliegt
LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	RECT rcCurrentClient = {0,0,0,0};
	WCHAR Text[512];
    
	if(hwnd == hWnd)
	{

		// testen, um welche Nachricht es sich handelt
		switch(msg)
		{
			// Programm beenden, wenn das Fenster
			// geschlossen wird
			case WM_DESTROY:
					PostQuitMessage(0);
					return 0;
				 break;

			case WM_KEYDOWN:
				   switch(wParam)
				   {
					 // Wenn ESC gedrueckt, Anwendung beenden
					case VK_ESCAPE:
							DestroyWindow(hWnd);
						break;

					case VK_BACK:
							Scene.SetCamera();
						break;

					case 70:
							Scene.SetCamera();
						break;
					
					case 65:
							if(!GetAsyncKeyState(VK_CONTROL)) break;
							SelectEverything();
							Scene.Render();
						break;

					case 87:											// w-key
							Scene.ToggleArrowActivated(TOOL_TRANSFORM);
							Scene.Render();
						break;
					case 69:											// e-key
							Scene.ToggleArrowActivated(TOOL_SCALE);
							Scene.Render();
						break;

					case 46:
							Physics.DeleteSelected();
//							Scene.NothingSelected();
							Scene.Render();
							LogBook.Succed(L"Elemente gelöscht");
						break;
					case 88:											// X-key

							Scene.AddTraceToSelected();
						break;

					case 82:											// R-Key
						if(GetAsyncKeyState(VK_CONTROL)) {
							if(IsSceneNamed) {
								Physics.ClearLists();
								SaveLoad.Load(lastFile[0][1]);
								Scene.Render();
							}
						}
						break;

					case 90:											// Z-Key
						if(GetAsyncKeyState(VK_CONTROL)) {
							if(saved)
								SaveLoad.Load(FILE_TEMP);
						}
						break;

					case 83:											// S-key

						if(GetAsyncKeyState(VK_CONTROL)) {
							if(IsSceneNamed) {
								SaveLoad.Save(lastFile[0][1]);
							} else {
								SaveLoad.SaveAs();

								memcpy(lastFile[3][1], lastFile[2][1], sizeof(lastFile[0][0]));
								memcpy(lastFile[3][0], lastFile[2][0], sizeof(lastFile[0][0]));
								memcpy(lastFile[2][1], lastFile[1][1], sizeof(lastFile[0][0]));
								memcpy(lastFile[2][0], lastFile[1][0], sizeof(lastFile[0][0]));
								memcpy(lastFile[1][1], lastFile[0][1], sizeof(lastFile[0][0]));
								memcpy(lastFile[1][0], lastFile[0][0], sizeof(lastFile[0][0]));
								
								wcscpy(lastFile[0][1], SaveLoad.GetFileName());
								wcscpy(lastFile[0][0], SaveLoad.GetFileNameEx());
								
								StringCchPrintf(Text, 512, L"%s  -  %s", lastFile[0][0], TITLE);
								SendMessage(hWnd, WM_SETTEXT,0,(long)Text);
							}
							IsSceneNamed = true;
						}
						break;

					case VK_TAB:
							SendMessage (hwndStatus, SB_SETTEXT , 0, (LPARAM)L"   Render");
								
							Physics.OnRender(0.0f, m_PhsicsRenderMode);
							Scene.UpdateTrace();
							Scene.Render();
							ChannelBox.Update();
							SendMessage (hwndStatus, SB_SETTEXT , 0, (LPARAM)L"   Bereit");
						 break;

					case VK_F2:
						runAnimation = !runAnimation;
						if(runAnimation) SendMessage (hwndStatus, SB_SETTEXT , 1, (LPARAM)L"   Animation läuft ...");
						else SendMessage (hwndStatus, SB_SETTEXT , 1, (LPARAM)L"   Animation angehalten");
						
						break;
					default:
							WCHAR Text[64];
							StringCchPrintf( Text, 256, L"Taste: %d wurde gedrückt", wParam);

							SendMessage (hwndStatus, SB_SETTEXT , 0, (LPARAM)Text);
						break;

				   }
				break;

			case WM_PAINT:

				Scene.Render();
					                
				break;

			//case WM_ENTERSIZEMOVE:
			//		running = false;
			//		
			//	break;

			case WM_SIZE:
					if(running) {					
						GetClientRect( hWnd, &rcCurrentClient );
						Scene.Resize(rcCurrentClient.right - 0, rcCurrentClient.bottom - 26 - 28);
						//FeatureWindow.Resize(rcCurrentClient.right - 200, 28, 200, rcCurrentClient.bottom - 26 - 28);
						SetWindowPos(hwndStatus, NULL, 0, rcCurrentClient.bottom - 26, rcCurrentClient.right, 26, SWP_NOOWNERZORDER);
						Scene.Render();
					}
				break;

			//case WM_EXITSIZEMOVE:
			//		running = true;
			//		
			//		//RECT rcCurrentClient;
			//		GetClientRect( hWnd, &rcCurrentClient );
			//		Scene.Resize(rcCurrentClient.right - 200, rcCurrentClient.bottom - 26 - 28);
			//		FeatureWindow.Resize(rcCurrentClient.right - 200, 28, 200, rcCurrentClient.bottom - 26 - 28);
			//		SetWindowPos(hwndStatus, NULL, 0, rcCurrentClient.bottom - 26, rcCurrentClient.right, 26, SWP_NOOWNERZORDER);
			//		
			//		Scene.Render();
			//	break;

			//case WM_MENUSELECT:

			//	break;

			case WM_COMMAND:
					switch (HIWORD(wParam)) {
						case BN_CLICKED: // BUTTONCLICKED
								switch(LOWORD(wParam))//Welcher ?
									{
										case BUTTON_EXIT:
												PostQuitMessage(0);
											break;

										case BUTTON_OPEN_FILE:
												Physics.ClearLists();
												if(!SaveLoad.Load(NULL)) break;

												memcpy(lastFile[3][1], lastFile[2][1], sizeof(lastFile[0][0]));
												memcpy(lastFile[3][0], lastFile[2][0], sizeof(lastFile[0][0]));
												memcpy(lastFile[2][1], lastFile[1][1], sizeof(lastFile[0][0]));
												memcpy(lastFile[2][0], lastFile[1][0], sizeof(lastFile[0][0]));
												memcpy(lastFile[1][1], lastFile[0][1], sizeof(lastFile[0][0]));
												memcpy(lastFile[1][0], lastFile[0][0], sizeof(lastFile[0][0]));
												
												wcscpy(lastFile[0][1], SaveLoad.GetFileName());
												wcscpy(lastFile[0][0], SaveLoad.GetFileNameEx());
												
												StringCchPrintf(Text, 512, L"%s  -  %s", lastFile[0][0], TITLE);

												SendMessage(hWnd, WM_SETTEXT,0,reinterpret_cast<long>(Text));
												IsSceneNamed = true;
												Scene.Render();
											break;
										
										case BUTTON_ADD_SCENE:
												if(!SaveLoad.Load(NULL)) break;
												//StringCchPrintf(Text, 512, L"%s  -  %s", SaveLoad.GetFileNameEx(), TITLE);
												//SendMessage(hWnd, WM_SETTEXT,0,(long)Text);
											break;

										case BUTTON_SAVE_FILE:
												if(IsSceneNamed) {
													SaveLoad.Save(lastFile[0][1]);
												} else {
													SaveLoad.SaveAs();

													memcpy(lastFile[3][1], lastFile[2][1], sizeof(lastFile[0][0]));
													memcpy(lastFile[3][0], lastFile[2][0], sizeof(lastFile[0][0]));
													memcpy(lastFile[2][1], lastFile[1][1], sizeof(lastFile[0][0]));
													memcpy(lastFile[2][0], lastFile[1][0], sizeof(lastFile[0][0]));
													memcpy(lastFile[1][1], lastFile[0][1], sizeof(lastFile[0][0]));
													memcpy(lastFile[1][0], lastFile[0][0], sizeof(lastFile[0][0]));
													
													wcscpy(lastFile[0][1], SaveLoad.GetFileName());
													wcscpy(lastFile[0][0], SaveLoad.GetFileNameEx());
													
													StringCchPrintf(Text, 512, L"%s  -  %s", lastFile[0][0], TITLE);
													SendMessage(hWnd, WM_SETTEXT,0,(long)Text);
												}
												IsSceneNamed = true;
											break;

										case BUTTON_SAVE_FILE_AS:
												SaveLoad.SaveAs();

												memcpy(lastFile[3][1], lastFile[2][1], sizeof(lastFile[0][0]));
												memcpy(lastFile[3][0], lastFile[2][0], sizeof(lastFile[0][0]));
												memcpy(lastFile[2][1], lastFile[1][1], sizeof(lastFile[0][0]));
												memcpy(lastFile[2][0], lastFile[1][0], sizeof(lastFile[0][0]));
												memcpy(lastFile[1][1], lastFile[0][1], sizeof(lastFile[0][0]));
												memcpy(lastFile[1][0], lastFile[0][0], sizeof(lastFile[0][0]));
												
												wcscpy(lastFile[0][1], SaveLoad.GetFileName());
												wcscpy(lastFile[0][0], SaveLoad.GetFileNameEx());
												
												StringCchPrintf(Text, 512, L"%s  -  %s", lastFile[0][0], TITLE);
												SendMessage(hWnd, WM_SETTEXT,0,(long)Text);

												IsSceneNamed = true;
											break;

										case BUTTON_LAST_FILE_1:
												Physics.ClearLists();
												if(!SaveLoad.Load(lastFile[0][1])) break;
												StringCchPrintf(Text, 512, L"%s  -  %s", lastFile[0][0], TITLE);
												SendMessage(hWnd, WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));
												IsSceneNamed = true;
												Scene.Render();

											break;

										case BUTTON_LAST_FILE_2:
												Physics.ClearLists();
												if(!SaveLoad.Load(lastFile[1][1])) break;
												StringCchPrintf(Text, 512, L"%s  -  %s", lastFile[1][0], TITLE);
												SendMessage(hWnd, WM_SETTEXT,0,reinterpret_cast<LPARAM>(Text));
												IsSceneNamed = true;
												Scene.Render();

											break;
										case BUTTON_LAST_FILE_3:
												Physics.ClearLists();
												if(!SaveLoad.Load(lastFile[2][1])) break;
												StringCchPrintf(Text, 512, L"%s  -  %s", lastFile[2][0], TITLE);
												SendMessage(hWnd, WM_SETTEXT,0,reinterpret_cast<LPARAM>(Text));
												IsSceneNamed = true;
												Scene.Render();

											break;
										case BUTTON_LAST_FILE_4:
												Physics.ClearLists();
												if(!SaveLoad.Load(lastFile[3][1])) break;
												StringCchPrintf(Text, 512, L"%s  -  %s", lastFile[3][0], TITLE);
												SendMessage(hWnd, WM_SETTEXT,0,reinterpret_cast<LPARAM>(Text));
												IsSceneNamed = true;
												Scene.Render();

											break;

										case BUTTON_ADD_PROTONE:
												Physics.AddProtone();
												if(lpExplorerWindow) lpExplorerWindow->UpdateList();
												Scene.Render();
												LogBook.Succed(L"Proton hinzugefuegt");
											break;

										case BUTTON_ADD_ELECTRONE:
												Physics.AddElectrone();
												if(lpExplorerWindow) lpExplorerWindow->UpdateList();
												Scene.Render();
												LogBook.Succed(L"Elektron hinzugefuegt");
											break;

										case BUTTON_ADD_FIELD_ELECTRICAL:
												Physics.AddElectricalField();
												if(lpExplorerWindow) lpExplorerWindow->UpdateList();
												Scene.Render();
												LogBook.Succed(L"Elektrisches Feld hinzugefuegt");
											break;

										case BUTTON_ADD_FIELD_MAGNETICAL:
												Physics.AddMagneticalField();
												if(lpExplorerWindow) lpExplorerWindow->UpdateList();
												Scene.Render();
												LogBook.Succed(L"Magnetisches Feld hinzugefuegt");
											break;

										case BUTTON_ADD_FIELD_GRAVITY:
												Physics.AddGravityField();
												if(lpExplorerWindow) lpExplorerWindow->UpdateList();
												Scene.Render();
												LogBook.Succed(L"Gravitationsfeld hinzugefuegt");
											break;

										case BUTTON_ADD_EMITTER:
												Physics.AddEmitter();
												Scene.Render();
												LogBook.Succed(L"Emitter hinzugefuegt");
											break;

										case BUTTON_ADD_DEFAULT_CUBOID:
												Scene.AddCuboid();			
												Scene.Render();
												LogBook.Succed(L"Cuboid hinzugefuegt");
											break;

										case BUTTON_ADD_DEFAULT_SPHERE:
												Scene.AddSphere();			
												Scene.Render();
												LogBook.Succed(L"Sphere hinzugefuegt");
											break;
												
										case BUTTON_SHOW_EXPLORER_WINDOW:
												if(!lpExplorerWindow) {
													lpExplorerWindow = new CExplorerWindow();
													lpExplorerWindow->SetCPhysicsPtr(&Physics);
													lpExplorerWindow->Create(m_hInstance, hWnd, 100, 100, 200, 450);
													lpExplorerWindow->SetID(WI_EXPLORER);
													lpExplorerWindow->SetCallbackOnWindowClosed(&OnWindowClose);
													LogBook.Succed(L"CExplorerWindow-Instance erstellt");
												}
											break;

										case BUTTON_SHOW_CHANNEL_BOX:
												ChannelBox.CreateChannelBox(hWnd, GetSystemMetrics(SM_CXSCREEN) - 210, 90, 200, 360); 
											break;

										case BUTTON_SHOW_INFO_WINDOW:
												if(!lpInfoWindow) {
													lpInfoWindow = new CInfo();
													lpInfoWindow->Create(m_hInstance, hWnd, GetSystemMetrics(SM_CXSCREEN) / 2 - 160,
																GetSystemMetrics(SM_CYSCREEN)/ 2 - 120, 320, 240);
													lpInfoWindow->SetID(WI_INFO);
													lpInfoWindow->SetCallbackOnWindowClosed(&OnWindowClose);
													LogBook.Succed(L"CInfo-Instance erstellt");
												}
											break;

										case BUTTON_SCREENSHOT:
												Scene.CreateScreenshot();
											break;

										case BUTTON_HELP_OFFLINE:
												ShellExecute(NULL,NULL, L"../help/index.html",NULL,NULL, SW_SHOWNORMAL);
												LogBook.Succed(L"Folgende Datei wurde aufgerufen:", L"../help/index.html");

											break;

										case BUTTON_HELP_ONLINE:
												ShellExecute(NULL,NULL, L"http://vatix.fkrauthan.de/projects/EB_Simulator/Support/",NULL,NULL, SW_SHOWNORMAL);
												LogBook.Succed(L"Folgende Datei wurde aufgerufen:", L"http://vatix.fkrauthan.de/projects/EB_Simulator/Support/");

											break;

										case BUTTON_ANIMATION_START:
												if(!runAnimation) {
													runAnimation = true;
													SendMessage (hwndStatus, SB_SETTEXT , 1, (LPARAM)L"   Animation läuft ...");
												}												
											break;

										case BUTTON_ANIMATION_STOP:
												if(runAnimation){
													runAnimation = false;
													SendMessage (hwndStatus, SB_SETTEXT , 1, (LPARAM)L"   Animation angehalten");
												}
											break;

										case BUTTON_DELETE_ALL:
												Physics.ClearLists();
												Scene.Render();
												LogBook.Succed(L"Alle Elemente wurden gelöscht");
											break;

										case BUTTON_SELECT_EVERTHING:
												SelectEverything();
											break;

										case BUTTON_NEW_SCENE:
												Physics.ClearLists();
												Scene.Render();
											break;

										case BUTTON_SHOW_GRID:
												Scene.ToggleRenderGrid();
												Scene.Render();
											break;

										case BUTTON_ANIMATION_STEP:
												Physics.OnRender(0.0f, m_PhsicsRenderMode);
												Scene.UpdateTrace();
												Scene.Render();
												ChannelBox.Update();	
											break;

										case BUTTON_ENABLE_HEUN:
												m_PhsicsRenderMode = PHSYSICS_RENDERMODE_HEUN;
											break;

										case BUTTON_DISABLE_HEUN:
												m_PhsicsRenderMode = PHSYSICS_RENDERMODE_NORMAL;
											break;

										case TOOLBAR_BUTTON_SHOW_POSITION:
												Scene.SetArrowMode(SHOW_POSITION_ARROW);
												Scene.ToggleArrowActivated(TOOL_TRANSFORM);
												Scene.Render();
											break;

										case TOOLBAR_BUTTON_SHOW_SPEED:
												Scene.SetArrowMode(SHOW_SPEED_ARROW);
												Scene.Render();
												ChannelBox.Update();
											break;

										case TOOLBAR_BUTTON_SHOW_SPEEDUP:
												Scene.SetArrowMode(SHOW_SPEEDUP_ARROW);
												Scene.Render();
												ChannelBox.Update();
											break;

										case TOOLBAR_BUTTON_CEBTER_SCENE:
												Scene.SetCamera();
											break;

										case TOOLBAR_BUTTON_DELTE_ELEMENT:

											break;
			
									}
							break;
					}
			break;
		}
	}else if(hwnd == Scene.GetHWnd()) {
		Scene.MessageHandler(msg, wParam, lParam);
	} else if(lpExplorerWindow && (lpExplorerWindow->IsHwndMemberOfThis(hwnd))) {
		lpExplorerWindow->MessageHandler(msg, wParam, lParam);
	} else if(lpInfoWindow && (lpInfoWindow->GetHWnd() == hwnd)) {
		lpInfoWindow->MessageHandler(msg, wParam, lParam);
	} 

    // Standardnachrichtenverarbeitung von Windows
    return DefWindowProc(hwnd, msg, wParam, lParam);
}



