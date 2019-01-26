#include "ChannelBox.h"

CChannelBox* lpChannelBox;

CChannelBox::CChannelBox() {
	lpChannelBox = this;
	m_hWnd = NULL;
	m_Selected = NULL;
}

CChannelBox::~CChannelBox(){
	DeleteObject (hBrush);
}

void CChannelBox::SetCPhysicsPtr(CPhysics* lpPhysics) {
	m_lpPhysics = lpPhysics; 

	m_lpListProtone = m_lpPhysics->GetListProtonePtr();
	m_lpListElectrone = m_lpPhysics->GetListElectronePtr();
	m_lpListDefaultSphere = m_lpPhysics->GetListDefaultSpherePtr();
	m_lpListElectricalField = m_lpPhysics->GetListElectricalFieldPtr();
	m_lpListMagneticalField = m_lpPhysics->GetListMagneticalFieldPtr();
	m_lpListGravityField = m_lpPhysics->GetListGravityFieldPtr();
	m_lpListDefaultCuboid = m_lpPhysics->GetListDefaultCuboidPtr();

}

BOOL CChannelBox::RegistrateWindowClass(HINSTANCE hInstance) {

	m_hInstance = hInstance;

	hBrush = CreateSolidBrush (RGB ( 0xF0, 0xF0, 0xF0));

	WNDCLASSEXW wndClass =
    {
        sizeof(WNDCLASSEX),                                 // Groesse angeben
        CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW,    // Standardstile
        ChannelBoxMessageHandler,                           // Callback-Funktion
        0,                                                  // Zusaetzliche Angaben
        0,                                                  // nicht benoetigt
        hInstance,                                          // Anwendungsinstanz
        LoadIcon(NULL, IDI_WINLOGO),                        // Windows-Logo
        LoadCursor(NULL, IDC_ARROW),                        // Normaler Cursor
        hBrush,               // Grauer Pinsel
        NULL,                                               // kein Menue
        L"ChannelBox",                                      // Der Name der Klasse
        LoadIcon(hInstance, L"ICON")
    };

    // Klasse registrieren
    RegisterClassEx(&wndClass);

	return TRUE;
}

BOOL CChannelBox::CreateChannelBox(HWND parentHWnd, UINT x, UINT y, UINT width, UINT height) {

	m_SizeX = width;
	m_SizeY = height;

	if(m_hWnd) return false;

	m_hWnd = CreateWindowEx(NULL,									// Keine erweiterten Stile nutzen
                          L"ChannelBox",					// Klassenname
                          L"ChannelBox",					// Fenstertitel
                          /*WS_OVERLAPPEDWINDOW |*/				// Fenster
						  WS_STYLE,							 // Eigenschaften
                          x,
						  y,								// Anfangsposition
                          width,
						  height,							// und Groesse des Fensters
                          parentHWnd,								// Handle des Elternfensters
                          NULL,								// Handle des Menues
                          m_hInstance,						// Anwendungsinstanz
                          NULL);

	hWndStatic[STATIC_NOSELECTION] = CreateWindow(L"static", L"Nichts markiert",WS_CHILD | WS_VISIBLE | ES_CENTER ,
		20, 5, 160, 16, m_hWnd, (HMENU) STATIC_NOSELECTION, m_hInstance, NULL) ;

	if(m_Selected != NULL) FillChannelBox(m_Selected); 

	return TRUE;
}

BOOL CChannelBox::CleanChannelBox() {

	DestroyWindow(hWndEdit[EDIT_TITLE]);
	DestroyWindow(hWndEdit[BUTTON_RESET]);

	switch(ElementType) {
		case TYPE_PROTONE:
				DestroyWindow(hWndEdit[EDIT_POSITION_X]);
				DestroyWindow(hWndEdit[EDIT_POSITION_Y]);
				DestroyWindow(hWndEdit[EDIT_POSITION_Z]);

				DestroyWindow(hWndStatic[EDIT_POSITION_X]);
				DestroyWindow(hWndStatic[EDIT_POSITION_Y]);
				DestroyWindow(hWndStatic[EDIT_POSITION_Z]);

				DestroyWindow(hWndEdit[EDIT_SPEED_X]);
				DestroyWindow(hWndEdit[EDIT_SPEED_Y]);
				DestroyWindow(hWndEdit[EDIT_SPEED_Z]);

				DestroyWindow(hWndStatic[EDIT_SPEED_X]);
				DestroyWindow(hWndStatic[EDIT_SPEED_Y]);
				DestroyWindow(hWndStatic[EDIT_SPEED_Z]);

				DestroyWindow(hWndEdit[EDIT_SPEEDING_UP_X]);
				DestroyWindow(hWndEdit[EDIT_SPEEDING_UP_Y]);
				DestroyWindow(hWndEdit[EDIT_SPEEDING_UP_Z]);

				DestroyWindow(hWndStatic[EDIT_SPEEDING_UP_X]);
				DestroyWindow(hWndStatic[EDIT_SPEEDING_UP_Y]);
				DestroyWindow(hWndStatic[EDIT_SPEEDING_UP_Z]);

				DestroyWindow(hWndEdit[EDIT_DRIVEN]);
				DestroyWindow(hWndEdit[EDIT_DRIVER]);
				DestroyWindow(hWndEdit[EDIT_SHOW_SPEED]);
				DestroyWindow(hWndEdit[EDIT_SHOW_SPEEDING_UP]);
				DestroyWindow(hWndEdit[EDIT_SHOW_TRACE]);
	
			break;

		case TYPE_ELECTRONE:
				DestroyWindow(hWndEdit[EDIT_POSITION_X]);
				DestroyWindow(hWndEdit[EDIT_POSITION_Y]);
				DestroyWindow(hWndEdit[EDIT_POSITION_Z]);

				DestroyWindow(hWndStatic[EDIT_POSITION_X]);
				DestroyWindow(hWndStatic[EDIT_POSITION_Y]);
				DestroyWindow(hWndStatic[EDIT_POSITION_Z]);

				DestroyWindow(hWndEdit[EDIT_SPEED_X]);
				DestroyWindow(hWndEdit[EDIT_SPEED_Y]);
				DestroyWindow(hWndEdit[EDIT_SPEED_Z]);

				DestroyWindow(hWndStatic[EDIT_SPEED_X]);
				DestroyWindow(hWndStatic[EDIT_SPEED_Y]);
				DestroyWindow(hWndStatic[EDIT_SPEED_Z]);

				DestroyWindow(hWndEdit[EDIT_SPEEDING_UP_X]);
				DestroyWindow(hWndEdit[EDIT_SPEEDING_UP_Y]);
				DestroyWindow(hWndEdit[EDIT_SPEEDING_UP_Z]);

				DestroyWindow(hWndStatic[EDIT_SPEEDING_UP_X]);
				DestroyWindow(hWndStatic[EDIT_SPEEDING_UP_Y]);
				DestroyWindow(hWndStatic[EDIT_SPEEDING_UP_Z]);

				DestroyWindow(hWndEdit[EDIT_DRIVEN]);
				DestroyWindow(hWndEdit[EDIT_DRIVER]);
				DestroyWindow(hWndEdit[EDIT_SHOW_SPEED]);
				DestroyWindow(hWndEdit[EDIT_SHOW_SPEEDING_UP]);
				DestroyWindow(hWndEdit[EDIT_SHOW_TRACE]);
	
			break;
		case TYPE_FIELD_ELECTRICAL:
		case TYPE_FIELD_MAGNETICAL:
		case TYPE_FIELD_GRAVITY:
				DestroyWindow(hWndEdit[EDIT_POSITION_X]);
				DestroyWindow(hWndEdit[EDIT_POSITION_Y]);
				DestroyWindow(hWndEdit[EDIT_POSITION_Z]);

				DestroyWindow(hWndStatic[EDIT_POSITION_X]);
				DestroyWindow(hWndStatic[EDIT_POSITION_Y]);
				DestroyWindow(hWndStatic[EDIT_POSITION_Z]);

				DestroyWindow(hWndEdit[EDIT_VIEWAT_X]);
				DestroyWindow(hWndEdit[EDIT_VIEWAT_Y]);
				DestroyWindow(hWndEdit[EDIT_VIEWAT_Z]);

				DestroyWindow(hWndStatic[EDIT_VIEWAT_X]);
				DestroyWindow(hWndStatic[EDIT_VIEWAT_Y]);
				DestroyWindow(hWndStatic[EDIT_VIEWAT_Z]);

				DestroyWindow(hWndEdit[EDIT_SHOW_VECTOR]);

			break;

		case TYPE_OBJECT_EMITTER:

				DestroyWindow(hWndEdit[EDIT_POSITION_X]);
				DestroyWindow(hWndEdit[EDIT_POSITION_Y]);
				DestroyWindow(hWndEdit[EDIT_POSITION_Z]);

				DestroyWindow(hWndStatic[EDIT_POSITION_X]);
				DestroyWindow(hWndStatic[EDIT_POSITION_Y]);
				DestroyWindow(hWndStatic[EDIT_POSITION_Z]);

				DestroyWindow(hWndEdit[EDIT_DIRECTION_X]);
				DestroyWindow(hWndEdit[EDIT_DIRECTION_Y]);
				DestroyWindow(hWndEdit[EDIT_DIRECTION_Z]);

				DestroyWindow(hWndStatic[EDIT_DIRECTION_X]);
				DestroyWindow(hWndStatic[EDIT_DIRECTION_Y]);
				DestroyWindow(hWndStatic[EDIT_DIRECTION_Z]);
			
				DestroyWindow(hWndEdit[EDIT_RANDOM_X]);
				DestroyWindow(hWndEdit[EDIT_RANDOM_Y]);
				DestroyWindow(hWndEdit[EDIT_RANDOM_Z]);

				DestroyWindow(hWndStatic[EDIT_RANDOM_X]);
				DestroyWindow(hWndStatic[EDIT_RANDOM_Y]);
				DestroyWindow(hWndStatic[EDIT_RANDOM_Z]);

				DestroyWindow(hWndEdit[EDIT_WAIT_TIME]);
				DestroyWindow(hWndStatic[EDIT_WAIT_TIME]);
			
				DestroyWindow(hWndEdit[EDIT_SHOW_DIRECTION]);
				DestroyWindow(hWndEdit[BUTTON_RESET]);
				
			break;

		case TYPE_DEFAULT_CUBOID:

				DestroyWindow(hWndEdit[EDIT_POSITION_X]);
				DestroyWindow(hWndEdit[EDIT_POSITION_Y]);
				DestroyWindow(hWndEdit[EDIT_POSITION_Z]);
				
				DestroyWindow(hWndStatic[EDIT_POSITION_X]);
				DestroyWindow(hWndStatic[EDIT_POSITION_Y]);
				DestroyWindow(hWndStatic[EDIT_POSITION_Z]);


				DestroyWindow(hWndEdit[EDIT_SIZE_X]);
				DestroyWindow(hWndEdit[EDIT_SIZE_Y]);
				DestroyWindow(hWndEdit[EDIT_SIZE_Z]);
				
				DestroyWindow(hWndStatic[EDIT_SIZE_X]);
				DestroyWindow(hWndStatic[EDIT_SIZE_Y]);
				DestroyWindow(hWndStatic[EDIT_SIZE_Z]);


				DestroyWindow(hWndEdit[EDIT_MASS]);
				DestroyWindow(hWndEdit[EDIT_CHARGE]);
				DestroyWindow(hWndEdit[EDIT_NUM_PARTS]);
				
				DestroyWindow(hWndStatic[EDIT_MASS]);
				DestroyWindow(hWndStatic[EDIT_CHARGE]);
				DestroyWindow(hWndStatic[EDIT_NUM_PARTS]);

				DestroyWindow(hWndEdit[BUTTON_SET_CUBOID]);
				DestroyWindow(hWndEdit[EDIT_SHOW_CHARGES]);

			break;

		case TYPE_DEFAULT_SPHERE:
				
			DestroyWindow(hWndEdit[EDIT_POSITION_X]);
			DestroyWindow(hWndEdit[EDIT_POSITION_Y]);
			DestroyWindow(hWndEdit[EDIT_POSITION_Z]);
				
			DestroyWindow(hWndStatic[EDIT_POSITION_X]);
			DestroyWindow(hWndStatic[EDIT_POSITION_Y]);
			DestroyWindow(hWndStatic[EDIT_POSITION_Z]);

			DestroyWindow(hWndEdit[EDIT_SPEED_X]);
			DestroyWindow(hWndEdit[EDIT_SPEED_Y]);
			DestroyWindow(hWndEdit[EDIT_SPEED_Z]);

			DestroyWindow(hWndStatic[EDIT_SPEED_X]);
			DestroyWindow(hWndStatic[EDIT_SPEED_Y]);
			DestroyWindow(hWndStatic[EDIT_SPEED_Z]);

			DestroyWindow(hWndEdit[EDIT_SPEEDING_UP_X]);
			DestroyWindow(hWndEdit[EDIT_SPEEDING_UP_Y]);
			DestroyWindow(hWndEdit[EDIT_SPEEDING_UP_Z]);

			DestroyWindow(hWndStatic[EDIT_SPEEDING_UP_X]);
			DestroyWindow(hWndStatic[EDIT_SPEEDING_UP_Y]);
			DestroyWindow(hWndStatic[EDIT_SPEEDING_UP_Z]);

			DestroyWindow(hWndEdit[EDIT_RADIUS]);
			DestroyWindow(hWndEdit[EDIT_CHARGE]);
			DestroyWindow(hWndEdit[EDIT_MASS]);

			DestroyWindow(hWndStatic[EDIT_RADIUS]);
			DestroyWindow(hWndStatic[EDIT_CHARGE]);
			DestroyWindow(hWndStatic[EDIT_MASS]);

			DestroyWindow(hWndEdit[EDIT_DRIVEN]);
			DestroyWindow(hWndEdit[EDIT_DRIVER]);
			DestroyWindow(hWndEdit[EDIT_SHOW_SPEED]);
			DestroyWindow(hWndEdit[EDIT_SHOW_SPEEDING_UP]);
			DestroyWindow(hWndEdit[EDIT_SHOW_TRACE]);

			
			break;

	}

	hWndStatic[STATIC_NOSELECTION] = CreateWindow(L"static", L"Nichts markiert",WS_CHILD | WS_VISIBLE | ES_CENTER ,
		05, 05, 160, 16, m_hWnd, (HMENU) STATIC_NOSELECTION, m_hInstance, NULL) ;

	//SetWindowPos(m_hWnd, NULL, 0, 0, m_SizeX, m_SizeY, SWP_NOMOVE | SWP_NOOWNERZORDER);
	
	return TRUE;
}


BOOL CChannelBox::FillChannelBox(LPDEFAULTELEMENT Selected) {

	if(m_Selected == Selected) {
		Update();
		return true;
	}

	CleanChannelBox();
	m_Selected = Selected;

	//SetWindowPos(m_hWnd, NULL, 20, 5, 160, 16, m_hWnd, SWP_NOMOVE | SWP_NOOWNERZORDER);

	Protone* lpProtone;
	Electrone* lpElectrone;
	ElectricalField* lpElectricalField;
	Emitter* lpEmitter;
	DefaultCuboid* lpDefaultCuboid;
	DefaultSphere* lpDefaultSphere;
	//MagneticalField* lpMagneticalField;
	//GravityField* lpGravityField;

	WCHAR Text[256];
	
	if(!Selected) return false;	
	ElementType = Selected->tempType;

	DestroyWindow(hWndStatic[STATIC_NOSELECTION]);


	//hWndEdit[EDIT_TITLE] = CreateWindow(L"edit", Selected->Name,WS_CHILD | WS_VISIBLE | ES_CENTER ,
	//	20, 05, 160, 16, m_hWnd, (HMENU) EDIT_TITLE, m_hInstance, NULL) ;


	switch(ElementType) {
		case TYPE_PROTONE:
				lpProtone = Selected;

					hWndEdit[EDIT_TITLE] = CreateWindow(L"edit", lpProtone->Name,WS_CHILD | WS_VISIBLE | ES_CENTER ,
				20, 05, 160, 16, m_hWnd, (HMENU) EDIT_TITLE, m_hInstance, NULL) ;

				StringCchPrintf( Text, 256, L"%3.3f ",lpProtone->Position.x );
				hWndEdit[EDIT_POSITION_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 40, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpProtone->Position.y );
				hWndEdit[EDIT_POSITION_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 58, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpProtone->Position.z );
				hWndEdit[EDIT_POSITION_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 76, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_POSITION_X] = CreateWindow(L"static", L"Position X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 40, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Y] = CreateWindow(L"static", L"Position Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 58, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Z] = CreateWindow(L"static", L"Position Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 76, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				StringCchPrintf( Text, 256, L"%3.3f ",lpProtone->Speed.x );
				hWndEdit[EDIT_SPEED_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 96, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpProtone->Speed.y );
				hWndEdit[EDIT_SPEED_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 114, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpProtone->Speed.z );
				hWndEdit[EDIT_SPEED_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 132, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_SPEED_X] = CreateWindow(L"static", L"Geschwind. X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 96, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEED_Y] = CreateWindow(L"static", L"Geschwind. Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 114, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEED_Z] = CreateWindow(L"static", L"Geschwind. Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 132, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				StringCchPrintf( Text, 256, L"%3.3f ",lpProtone->SpeedingUp.x );
				hWndEdit[EDIT_SPEEDING_UP_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 152, 60, 16, m_hWnd, (HMENU) EDIT_SPEEDING_UP_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpProtone->SpeedingUp.y );
				hWndEdit[EDIT_SPEEDING_UP_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 170, 60, 16, m_hWnd, (HMENU) EDIT_SPEEDING_UP_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpProtone->SpeedingUp.z );
				hWndEdit[EDIT_SPEEDING_UP_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 188, 60, 16, m_hWnd, (HMENU) EDIT_SPEEDING_UP_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_SPEEDING_UP_X] = CreateWindow(L"static", L"Beschleunigung X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 152, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEEDING_UP_Y] = CreateWindow(L"static", L"Beschleunigung Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 170, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEEDING_UP_Z] = CreateWindow(L"static", L"Beschleunigung Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 188, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				hWndEdit[EDIT_DRIVEN] = CreateWindow(L"button", L"Driven", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 216, 150, 16, m_hWnd, (HMENU) EDIT_DRIVEN, m_hInstance, NULL) ;
				if(lpProtone->bDriven)SendMessage(hWndEdit[EDIT_DRIVEN], BM_SETCHECK, BST_CHECKED, NULL);

				hWndEdit[EDIT_DRIVER] = CreateWindow(L"button", L"Driver", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 234, 150, 16, m_hWnd, (HMENU) EDIT_DRIVER, m_hInstance, NULL) ;
				if(lpProtone->bDriver)SendMessage(hWndEdit[EDIT_DRIVER], BM_SETCHECK, BST_CHECKED, NULL);

				hWndEdit[EDIT_SHOW_SPEED] = CreateWindow(L"button", L"Geschwindigkeit anzeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 260, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_SPEED, m_hInstance, NULL) ;
				if(lpProtone->bShowSpeed)SendMessage(hWndEdit[EDIT_SHOW_SPEED], BM_SETCHECK, BST_CHECKED, NULL);

				hWndEdit[EDIT_SHOW_SPEEDING_UP] = CreateWindow(L"button", L"Beschleunigung anzeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 278, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_SPEEDING_UP, m_hInstance, NULL) ;
				if(lpProtone->bShowSpeedup)SendMessage(hWndEdit[EDIT_SHOW_SPEEDING_UP], BM_SETCHECK, BST_CHECKED, NULL);
				
				hWndEdit[EDIT_SHOW_TRACE] = CreateWindow(L"button", L"Spur anzeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 298, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_TRACE, m_hInstance, NULL) ;
				if(lpProtone->bTrace)SendMessage(hWndEdit[EDIT_SHOW_TRACE], BM_SETCHECK, BST_CHECKED, NULL);
	

					hWndEdit[BUTTON_RESET] = CreateWindow(L"button", L"Alles zurücksetzen", WS_CHILD | WS_VISIBLE, 
					30, 320, 150, 22, m_hWnd, (HMENU) BUTTON_RESET, m_hInstance, NULL) ;
				

				break;
		case TYPE_ELECTRONE:
				lpElectrone = (Electrone*)Selected;

				hWndEdit[EDIT_TITLE] = CreateWindow(L"edit", lpElectrone->Name,WS_CHILD | WS_VISIBLE | ES_CENTER ,
				20, 05, 160, 16, m_hWnd, (HMENU) EDIT_TITLE, m_hInstance, NULL) ;

				StringCchPrintf( Text, 256, L"%3.3f ",lpElectrone->Position.x );
				hWndEdit[EDIT_POSITION_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 40, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpElectrone->Position.y );
				hWndEdit[EDIT_POSITION_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 58, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpElectrone->Position.z );
				hWndEdit[EDIT_POSITION_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 76, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_POSITION_X] = CreateWindow(L"static", L"Position X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 40, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Y] = CreateWindow(L"static", L"Position Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 58, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Z] = CreateWindow(L"static", L"Position Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 76, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				StringCchPrintf( Text, 256, L"%3.3f ",lpElectrone->Speed.x );
				hWndEdit[EDIT_SPEED_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 96, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpElectrone->Speed.y );
				hWndEdit[EDIT_SPEED_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 114, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpElectrone->Speed.z );
				hWndEdit[EDIT_SPEED_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 132, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_SPEED_X] = CreateWindow(L"static", L"Geschwind. X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 96, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEED_Y] = CreateWindow(L"static", L"Geschwind. Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 114, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEED_Z] = CreateWindow(L"static", L"Geschwind. Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 132, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				StringCchPrintf( Text, 256, L"%3.3f ",lpElectrone->SpeedingUp.x );
				hWndEdit[EDIT_SPEEDING_UP_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 152, 60, 16, m_hWnd, (HMENU) EDIT_SPEEDING_UP_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpElectrone->SpeedingUp.y );
				hWndEdit[EDIT_SPEEDING_UP_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 170, 60, 16, m_hWnd, (HMENU) EDIT_SPEEDING_UP_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpElectrone->SpeedingUp.z );
				hWndEdit[EDIT_SPEEDING_UP_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 188, 60, 16, m_hWnd, (HMENU) EDIT_SPEEDING_UP_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_SPEEDING_UP_X] = CreateWindow(L"static", L"Beschleunigung X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 152, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEEDING_UP_Y] = CreateWindow(L"static", L"Beschleunigung Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 170, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEEDING_UP_Z] = CreateWindow(L"static", L"Beschleunigung Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 188, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				hWndEdit[EDIT_DRIVEN] = CreateWindow(L"button", L"Driven", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 216, 150, 16, m_hWnd, (HMENU) EDIT_DRIVEN, m_hInstance, NULL) ;
				if(lpElectrone->bDriven)SendMessage(hWndEdit[EDIT_DRIVEN], BM_SETCHECK, BST_CHECKED, NULL);

				hWndEdit[EDIT_DRIVER] = CreateWindow(L"button", L"Driver", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 234, 150, 16, m_hWnd, (HMENU) EDIT_DRIVER, m_hInstance, NULL) ;
				if(lpElectrone->bDriver)SendMessage(hWndEdit[EDIT_DRIVER], BM_SETCHECK, BST_CHECKED, NULL);

				hWndEdit[EDIT_SHOW_SPEED] = CreateWindow(L"button", L"Geschwindigkeit anzeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 260, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_SPEED, m_hInstance, NULL) ;
				if(lpElectrone->bShowSpeed)SendMessage(hWndEdit[EDIT_SHOW_SPEED], BM_SETCHECK, BST_CHECKED, NULL);

				hWndEdit[EDIT_SHOW_SPEEDING_UP] = CreateWindow(L"button", L"Beschleunigung anzeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 276, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_SPEEDING_UP, m_hInstance, NULL) ;
				if(lpElectrone->bShowSpeedup)SendMessage(hWndEdit[EDIT_SHOW_SPEEDING_UP], BM_SETCHECK, BST_CHECKED, NULL);

				hWndEdit[EDIT_SHOW_TRACE] = CreateWindow(L"button", L"Spur anzeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 298, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_TRACE, m_hInstance, NULL) ;
				if(lpElectrone->bTrace)SendMessage(hWndEdit[EDIT_SHOW_TRACE], BM_SETCHECK, BST_CHECKED, NULL);

					hWndEdit[BUTTON_RESET] = CreateWindow(L"button", L"Alles zurücksetzen", WS_CHILD | WS_VISIBLE, 
					30, 320, 150, 22, m_hWnd, (HMENU) BUTTON_RESET, m_hInstance, NULL) ;

			break;
		case TYPE_FIELD_ELECTRICAL:
		case TYPE_FIELD_MAGNETICAL:
		case TYPE_FIELD_GRAVITY:
				lpElectricalField = (ElectricalField*) Selected;

				hWndEdit[EDIT_TITLE] = CreateWindow(L"edit", lpElectricalField->Name,WS_CHILD | WS_VISIBLE | ES_CENTER ,
				20, 05, 160, 16, m_hWnd, (HMENU) EDIT_TITLE, m_hInstance, NULL) ;

				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->Position.x );
				hWndEdit[EDIT_POSITION_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 40, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->Position.y );
				hWndEdit[EDIT_POSITION_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 58, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->Position.z );
				hWndEdit[EDIT_POSITION_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 76, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_POSITION_X] = CreateWindow(L"static", L"Position X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 40, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Y] = CreateWindow(L"static", L"Position Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 58, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Z] = CreateWindow(L"static", L"Position Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 76, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->ViewAt.x );
				hWndEdit[EDIT_VIEWAT_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 96, 60, 16, m_hWnd, (HMENU) EDIT_VIEWAT_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->ViewAt.y );
				hWndEdit[EDIT_VIEWAT_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 114, 60, 16, m_hWnd, (HMENU) EDIT_VIEWAT_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->ViewAt.z );
				hWndEdit[EDIT_VIEWAT_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 132, 60, 16, m_hWnd, (HMENU) EDIT_VIEWAT_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_VIEWAT_X] = CreateWindow(L"static", L"Feldvektor X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 96, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_VIEWAT_Y] = CreateWindow(L"static", L"Feldvektor Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 114, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_VIEWAT_Z] = CreateWindow(L"static", L"Feldvektor Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 132, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;

				hWndEdit[EDIT_SHOW_VECTOR] = CreateWindow(L"button", L"Vector zeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 160, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_VECTOR, m_hInstance, NULL) ;
				if(lpElectricalField->bShowVector) SendMessage(hWndEdit[EDIT_SHOW_VECTOR], BM_SETCHECK, BST_CHECKED, NULL);

				
					hWndEdit[BUTTON_RESET] = CreateWindow(L"button", L"Alles zurücksetzen", WS_CHILD | WS_VISIBLE, 
					30, 200, 150, 22, m_hWnd, (HMENU) BUTTON_RESET, m_hInstance, NULL) ;

			break;

		case TYPE_OBJECT_EMITTER:
				lpEmitter = (Emitter*) Selected;

				hWndEdit[EDIT_TITLE] = CreateWindow(L"edit", lpEmitter->Name,WS_CHILD | WS_VISIBLE | ES_CENTER ,
				20, 05, 160, 16, m_hWnd, (HMENU) EDIT_TITLE, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->Position.x );
				hWndEdit[EDIT_POSITION_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 40, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->Position.y );
				hWndEdit[EDIT_POSITION_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 58, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->Position.z );
				hWndEdit[EDIT_POSITION_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 76, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_POSITION_X] = CreateWindow(L"static", L"Position X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 40, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Y] = CreateWindow(L"static", L"Position Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 58, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Z] = CreateWindow(L"static", L"Position Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 76, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->ViewAt.x );
				hWndEdit[EDIT_DIRECTION_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 96, 60, 16, m_hWnd, (HMENU) EDIT_DIRECTION_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->ViewAt.y );
				hWndEdit[EDIT_DIRECTION_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 114, 60, 16, m_hWnd, (HMENU) EDIT_DIRECTION_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->ViewAt.z );
				hWndEdit[EDIT_DIRECTION_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 132, 60, 16, m_hWnd, (HMENU) EDIT_DIRECTION_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_DIRECTION_X] = CreateWindow(L"static", L"Richtung X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 96, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_DIRECTION_Y] = CreateWindow(L"static", L"Richtung Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 114, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_DIRECTION_Z] = CreateWindow(L"static", L"Richtung Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 132, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->Random.x );
				hWndEdit[EDIT_RANDOM_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 152, 60, 16, m_hWnd, (HMENU) EDIT_RANDOM_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->Random.y );
				hWndEdit[EDIT_RANDOM_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 170, 60, 16, m_hWnd, (HMENU) EDIT_RANDOM_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->Random.z );
				hWndEdit[EDIT_RANDOM_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 188, 60, 16, m_hWnd, (HMENU) EDIT_RANDOM_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_RANDOM_X] = CreateWindow(L"static", L"Zufall X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 152, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_RANDOM_Y] = CreateWindow(L"static", L"Zufall Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 170, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_RANDOM_Z] = CreateWindow(L"static", L"Zufall Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 188, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				hWndEdit[EDIT_SHOW_DIRECTION] = CreateWindow(L"button", L"Richtung zeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 208, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_DIRECTION, m_hInstance, NULL) ;
				if(lpEmitter->bShowVector) SendMessage(hWndEdit[EDIT_SHOW_DIRECTION], BM_SETCHECK, BST_CHECKED, NULL);


				StringCchPrintf( Text, 256, L"%3.3d ",lpEmitter->waitTime );
				hWndEdit[EDIT_WAIT_TIME] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 228, 60, 16, m_hWnd, (HMENU) EDIT_WAIT_TIME, m_hInstance, NULL) ;

				hWndStatic[EDIT_WAIT_TIME] = CreateWindow(L"static", L"Zeit bis zum Nächsten..",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 228, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


					hWndEdit[BUTTON_RESET] = CreateWindow(L"button", L"Alles zurücksetzen", WS_CHILD | WS_VISIBLE, 
					30, 250, 150, 22, m_hWnd, (HMENU) BUTTON_RESET, m_hInstance, NULL) ;

			break;

		case TYPE_DEFAULT_CUBOID:
				lpDefaultCuboid = (DefaultCuboid*) Selected;

				hWndEdit[EDIT_TITLE] = CreateWindow(L"edit", lpDefaultCuboid->Name,WS_CHILD | WS_VISIBLE | ES_CENTER ,
				20, 05, 160, 16, m_hWnd, (HMENU) EDIT_TITLE, m_hInstance, NULL) ;

								StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Position.x );
				hWndEdit[EDIT_POSITION_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 40, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Position.y );
				hWndEdit[EDIT_POSITION_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 58, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Position.z );
				hWndEdit[EDIT_POSITION_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 76, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_POSITION_X] = CreateWindow(L"static", L"Position X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 40, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Y] = CreateWindow(L"static", L"Position Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 58, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Z] = CreateWindow(L"static", L"Position Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 76, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;

				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Size.x );
				hWndEdit[EDIT_SIZE_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 96, 60, 16, m_hWnd, (HMENU) EDIT_SIZE_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Size.y );
				hWndEdit[EDIT_SIZE_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 114, 60, 16, m_hWnd, (HMENU) EDIT_SIZE_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Size.z );
				hWndEdit[EDIT_SIZE_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 132, 60, 16, m_hWnd, (HMENU) EDIT_SIZE_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_SIZE_X] = CreateWindow(L"static", L"Größe X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 96, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SIZE_Y] = CreateWindow(L"static", L"Größe Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 114, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SIZE_Z] = CreateWindow(L"static", L"Größe Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 132, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Mass );
				hWndEdit[EDIT_MASS] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 152, 60, 16, m_hWnd, (HMENU) EDIT_MASS, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Coulomb);
				hWndEdit[EDIT_CHARGE] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 170, 60, 16, m_hWnd, (HMENU) EDIT_CHARGE, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.0d ",lpDefaultCuboid->NumParticies);
				hWndEdit[EDIT_NUM_PARTS] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 188, 60, 16, m_hWnd, (HMENU) EDIT_NUM_PARTS, m_hInstance, NULL) ;

				hWndStatic[EDIT_MASS] = CreateWindow(L"static", L"Masse ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 152, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_CHARGE] = CreateWindow(L"static", L"Ladung ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 170, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_NUM_PARTS] = CreateWindow(L"static", L"Aufteilung ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 188, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				hWndEdit[EDIT_SHOW_CHARGES] = CreateWindow(L"button", L"Ladung zeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 208, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_CHARGES, m_hInstance, NULL) ;
				if(lpDefaultCuboid->bShowCharges) SendMessage(hWndEdit[EDIT_SHOW_CHARGES], BM_SETCHECK, BST_CHECKED, NULL);
				

				hWndEdit[BUTTON_RESET] = CreateWindow(L"button", L"Alles zurücksetzen", WS_CHILD | WS_VISIBLE, 
					30, 250, 150, 22, m_hWnd, (HMENU) BUTTON_RESET, m_hInstance, NULL) ;

				hWndEdit[BUTTON_SET_CUBOID] = CreateWindow(L"button", L"Ladungen setzen", WS_CHILD | WS_VISIBLE, 
					30, 280, 150, 22, m_hWnd, (HMENU) BUTTON_SET_CUBOID, m_hInstance, NULL) ;

			break;

		case TYPE_DEFAULT_SPHERE:

				//SetWindowPos(m_hWnd, NULL, 0, 0, m_SizeX, m_SizeY + 70, SWP_NOMOVE | SWP_NOOWNERZORDER);

				lpDefaultSphere = (DefaultSphere*) Selected;

				hWndEdit[EDIT_TITLE] = CreateWindow(L"edit", lpDefaultSphere->Name,WS_CHILD | WS_VISIBLE | ES_CENTER ,
				20, 05, 160, 16, m_hWnd, (HMENU) EDIT_TITLE, m_hInstance, NULL) ;

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Position.x );
				hWndEdit[EDIT_POSITION_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 40, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Position.y );
				hWndEdit[EDIT_POSITION_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 58, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Position.z );
				hWndEdit[EDIT_POSITION_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 76, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_POSITION_X] = CreateWindow(L"static", L"Position X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 40, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Y] = CreateWindow(L"static", L"Position Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 58, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_POSITION_Z] = CreateWindow(L"static", L"Position Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 76, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Speed.x );
				hWndEdit[EDIT_SPEED_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 96, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Speed.y );
				hWndEdit[EDIT_SPEED_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 114, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Speed.z );
				hWndEdit[EDIT_SPEED_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 132, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_SPEED_X] = CreateWindow(L"static", L"Geschwind. X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 96, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEED_Y] = CreateWindow(L"static", L"Geschwind. Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 114, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEED_Z] = CreateWindow(L"static", L"Geschwind. Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 132, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->SpeedingUp.x );
				hWndEdit[EDIT_SPEEDING_UP_X] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 152, 60, 16, m_hWnd, (HMENU) EDIT_SPEEDING_UP_X, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->SpeedingUp.y );
				hWndEdit[EDIT_SPEEDING_UP_Y] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 170, 60, 16, m_hWnd, (HMENU) EDIT_SPEEDING_UP_Y, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->SpeedingUp.z );
				hWndEdit[EDIT_SPEEDING_UP_Z] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 188, 60, 16, m_hWnd, (HMENU) EDIT_SPEEDING_UP_Z, m_hInstance, NULL) ;

				hWndStatic[EDIT_SPEEDING_UP_X] = CreateWindow(L"static", L"Beschleunigung X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 152, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEEDING_UP_Y] = CreateWindow(L"static", L"Beschleunigung Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 170, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_SPEEDING_UP_Z] = CreateWindow(L"static", L"Beschleunigung Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 188, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Radius);
				hWndEdit[EDIT_RADIUS] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 208, 60, 16, m_hWnd, (HMENU) EDIT_RADIUS, m_hInstance, NULL) ;
				
				
				hWndStatic[EDIT_RADIUS] = CreateWindow(L"static", L"Radius ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 208, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;


				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Mass );
				hWndEdit[EDIT_MASS] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 226, 60, 16, m_hWnd, (HMENU) EDIT_MASS, m_hInstance, NULL) ;
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Coulomb);
				hWndEdit[EDIT_CHARGE] = CreateWindow(L"edit", Text,  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					120, 244, 60, 16, m_hWnd, (HMENU) EDIT_CHARGE, m_hInstance, NULL) ;
				
				hWndStatic[EDIT_MASS] = CreateWindow(L"static", L"Masse ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 226, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;
				hWndStatic[EDIT_CHARGE] = CreateWindow(L"static", L"Ladung ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
					10, 244, 105, 16, m_hWnd, (HMENU) 99, m_hInstance, NULL) ;



				hWndEdit[EDIT_DRIVEN] = CreateWindow(L"button", L"Driven", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 272, 150, 16, m_hWnd, (HMENU) EDIT_DRIVEN, m_hInstance, NULL) ;
				if(lpDefaultSphere->bDriven)SendMessage(hWndEdit[EDIT_DRIVEN], BM_SETCHECK, BST_CHECKED, NULL);

				hWndEdit[EDIT_DRIVER] = CreateWindow(L"button", L"Driver", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 292, 150, 16, m_hWnd, (HMENU) EDIT_DRIVER, m_hInstance, NULL) ;
				if(lpDefaultSphere->bDriver)SendMessage(hWndEdit[EDIT_DRIVER], BM_SETCHECK, BST_CHECKED, NULL);

				hWndEdit[EDIT_SHOW_SPEED] = CreateWindow(L"button", L"Geschwindigkeit anzeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 316, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_SPEED, m_hInstance, NULL) ;
				if(lpDefaultSphere->bShowSpeed)SendMessage(hWndEdit[EDIT_SHOW_SPEED], BM_SETCHECK, BST_CHECKED, NULL);

				hWndEdit[EDIT_SHOW_SPEEDING_UP] = CreateWindow(L"button", L"Beschleunigung anzeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 336, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_SPEEDING_UP, m_hInstance, NULL) ;
				if(lpDefaultSphere->bShowSpeedup)SendMessage(hWndEdit[EDIT_SHOW_SPEEDING_UP], BM_SETCHECK, BST_CHECKED, NULL);
				
				hWndEdit[EDIT_SHOW_TRACE] = CreateWindow(L"button", L"Spur anzeigen", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 
					30, 356, 150, 16, m_hWnd, (HMENU) EDIT_SHOW_TRACE, m_hInstance, NULL) ;
				if(lpDefaultSphere->bTrace)SendMessage(hWndEdit[EDIT_SHOW_TRACE], BM_SETCHECK, BST_CHECKED, NULL);



		
				

				hWndEdit[BUTTON_RESET] = CreateWindow(L"button", L"Alles zurücksetzen", WS_CHILD | WS_VISIBLE, 
					30, 380, 150, 22, m_hWnd, (HMENU) BUTTON_RESET, m_hInstance, NULL) ;


			break;

	}

	return TRUE;

}

LRESULT CALLBACK ChannelBoxMessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return lpChannelBox->MessageHandler(hwnd, msg, wParam, lParam);
	
}

void CChannelBox::Update() {

	if(!m_Selected) return;

	WCHAR Text[256];
	ElectricalField* lpElectricalField;
	Emitter* lpEmitter;
	DefaultCuboid* lpDefaultCuboid;
	DefaultSphere* lpDefaultSphere;

	switch(m_Selected->tempType) {
		case TYPE_PROTONE:
		case TYPE_ELECTRONE:
		
				StringCchPrintf( Text, 256, L"%3.3f ",m_Selected->Position.x );
				SendMessage(hWndEdit[EDIT_POSITION_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",m_Selected->Position.y );
				SendMessage(hWndEdit[EDIT_POSITION_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",m_Selected->Position.z );
				SendMessage(hWndEdit[EDIT_POSITION_Z],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",m_Selected->Speed.x );
				SendMessage(hWndEdit[EDIT_SPEED_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",m_Selected->Speed.y );
				SendMessage(hWndEdit[EDIT_SPEED_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",m_Selected->Speed.z );
				SendMessage(hWndEdit[EDIT_SPEED_Z],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",m_Selected->SpeedingUp.x );
				SendMessage(hWndEdit[EDIT_SPEEDING_UP_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",m_Selected->SpeedingUp.y );
				SendMessage(hWndEdit[EDIT_SPEEDING_UP_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",m_Selected->SpeedingUp.z );
				SendMessage(hWndEdit[EDIT_SPEEDING_UP_Z],WM_SETTEXT,0,(LPARAM)Text);

				if(m_Selected->bDriven)SendMessage(hWndEdit[EDIT_DRIVEN], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_DRIVEN], BM_SETCHECK, BST_UNCHECKED, NULL);

				if(m_Selected->bDriver)SendMessage(hWndEdit[EDIT_DRIVER], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_DRIVER], BM_SETCHECK, BST_UNCHECKED, NULL);

				if(m_Selected->bShowSpeed)SendMessage(hWndEdit[EDIT_SHOW_SPEED], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_SHOW_SPEED], BM_SETCHECK, BST_UNCHECKED, NULL);

				if(m_Selected->bShowSpeedup)SendMessage(hWndEdit[EDIT_SHOW_SPEEDING_UP], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_SHOW_SPEEDING_UP], BM_SETCHECK, BST_UNCHECKED, NULL);

				if(m_Selected->bTrace)SendMessage(hWndEdit[EDIT_SHOW_TRACE], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_SHOW_TRACE], BM_SETCHECK, BST_UNCHECKED, NULL);

			break;

		case TYPE_FIELD_ELECTRICAL:
		case TYPE_FIELD_GRAVITY:
		case TYPE_FIELD_MAGNETICAL:

				lpElectricalField = (ElectricalField*) m_Selected;

				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->Position.x );
				SendMessage(hWndEdit[EDIT_POSITION_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->Position.y );
				SendMessage(hWndEdit[EDIT_POSITION_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->Position.z );
				SendMessage(hWndEdit[EDIT_POSITION_Z],WM_SETTEXT,0,(LPARAM)Text);
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->ViewAt.x );
				SendMessage(hWndEdit[EDIT_VIEWAT_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->ViewAt.y );
				SendMessage(hWndEdit[EDIT_VIEWAT_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpElectricalField->ViewAt.z );
				SendMessage(hWndEdit[EDIT_VIEWAT_Z],WM_SETTEXT,0,(LPARAM)Text);

				if(lpElectricalField->bShowVector)SendMessage(hWndEdit[EDIT_SHOW_VECTOR], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_SHOW_VECTOR], BM_SETCHECK, BST_UNCHECKED, NULL);

			break;

		case TYPE_OBJECT_EMITTER:
				
				lpEmitter = (Emitter*) m_Selected;

				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->Position.x );
				SendMessage(hWndEdit[EDIT_POSITION_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->Position.y );
				SendMessage(hWndEdit[EDIT_POSITION_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->Position.z );
				SendMessage(hWndEdit[EDIT_POSITION_Z],WM_SETTEXT,0,(LPARAM)Text);
				
				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->ViewAt.x );
				SendMessage(hWndEdit[EDIT_DIRECTION_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->ViewAt.y );
				SendMessage(hWndEdit[EDIT_DIRECTION_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpEmitter->ViewAt.z );
				SendMessage(hWndEdit[EDIT_DIRECTION_Z],WM_SETTEXT,0,(LPARAM)Text);

				if(lpEmitter->bShowVector)SendMessage(hWndEdit[EDIT_SHOW_DIRECTION], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_SHOW_DIRECTION], BM_SETCHECK, BST_UNCHECKED, NULL);

			break;

		case TYPE_DEFAULT_CUBOID:
				lpDefaultCuboid = (DefaultCuboid*) m_Selected;

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Position.x );
				SendMessage(hWndEdit[EDIT_POSITION_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Position.y );
				SendMessage(hWndEdit[EDIT_POSITION_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Position.z );
				SendMessage(hWndEdit[EDIT_POSITION_Z],WM_SETTEXT,0,(LPARAM)Text);

				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Size.x );
				SendMessage(hWndEdit[EDIT_SIZE_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Size.y );
				SendMessage(hWndEdit[EDIT_SIZE_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Size.z );
				SendMessage(hWndEdit[EDIT_SIZE_Z],WM_SETTEXT,0,(LPARAM)Text);

				
				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Mass);
				SendMessage(hWndEdit[EDIT_MASS],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultCuboid->Coulomb);
				SendMessage(hWndEdit[EDIT_CHARGE],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%d ",lpDefaultCuboid->NumParticies);
				SendMessage(hWndEdit[EDIT_NUM_PARTS],WM_SETTEXT,0,(LPARAM)Text);

			break;

		case TYPE_DEFAULT_SPHERE:

				lpDefaultSphere = (DefaultSphere*) m_Selected;

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Position.x );
				SendMessage(hWndEdit[EDIT_POSITION_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Position.y );
				SendMessage(hWndEdit[EDIT_POSITION_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Position.z );
				SendMessage(hWndEdit[EDIT_POSITION_Z],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Speed.x );
				SendMessage(hWndEdit[EDIT_SPEED_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Speed.y );
				SendMessage(hWndEdit[EDIT_SPEED_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Speed.z );
				SendMessage(hWndEdit[EDIT_SPEED_Z],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->SpeedingUp.x );
				SendMessage(hWndEdit[EDIT_SPEEDING_UP_X],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->SpeedingUp.y );
				SendMessage(hWndEdit[EDIT_SPEEDING_UP_Y],WM_SETTEXT,0,(LPARAM)Text);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->SpeedingUp.z );
				SendMessage(hWndEdit[EDIT_SPEEDING_UP_Z],WM_SETTEXT,0,(LPARAM)Text);

				if(lpDefaultSphere->bDriven)SendMessage(hWndEdit[EDIT_DRIVEN], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_DRIVEN], BM_SETCHECK, BST_UNCHECKED, NULL);

				if(lpDefaultSphere->bDriver)SendMessage(hWndEdit[EDIT_DRIVER], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_DRIVER], BM_SETCHECK, BST_UNCHECKED, NULL);

				if(lpDefaultSphere->bShowSpeed)SendMessage(hWndEdit[EDIT_SHOW_SPEED], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_SHOW_SPEED], BM_SETCHECK, BST_UNCHECKED, NULL);

				if(lpDefaultSphere->bShowSpeedup)SendMessage(hWndEdit[EDIT_SHOW_SPEEDING_UP], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_SHOW_SPEEDING_UP], BM_SETCHECK, BST_UNCHECKED, NULL);

				if(lpDefaultSphere->bTrace)SendMessage(hWndEdit[EDIT_SHOW_TRACE], BM_SETCHECK, BST_CHECKED, NULL);
				else SendMessage(hWndEdit[EDIT_SHOW_TRACE], BM_SETCHECK, BST_UNCHECKED, NULL);

				StringCchPrintf( Text, 256, L"%3.3f ",lpDefaultSphere->Radius );
				SendMessage(hWndEdit[EDIT_RADIUS],WM_SETTEXT,0,(LPARAM)Text);


			break;
	}
}

void CChannelBox::Reset(void) {
	if(!m_Selected) return;

	ElectricalField* lpElectricalField;
	Emitter* lpEmitter;
	DefaultCuboid* lpDefaultCuboid;
	DefaultSphere* lpDefaultSphere;

	switch(m_Selected->tempType) {
		case TYPE_PROTONE:
		case TYPE_ELECTRONE:
				m_Selected->bDriven = true;
				m_Selected->bDriver = true;
				m_Selected->bShowSpeed = false;
				m_Selected->bShowSpeedup = false;
				m_Selected->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_Selected->Speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_Selected->SpeedingUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			
			break;
		case TYPE_FIELD_ELECTRICAL:
		case TYPE_FIELD_MAGNETICAL:
		case TYPE_FIELD_GRAVITY:
				lpElectricalField = (ElectricalField*) m_Selected;
				lpElectricalField->bShowVector = false;
				lpElectricalField->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				lpElectricalField->Vector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				lpElectricalField->ViewAt = D3DXVECTOR3(0.001f, 0.0f, 0.0f);
			break;

		case TYPE_OBJECT_EMITTER:
				lpEmitter = (Emitter*) m_Selected;
				lpEmitter->bShowVector = false;
				lpEmitter->waitTime = 100;
				lpEmitter->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				lpEmitter->ViewAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;
		
		case TYPE_DEFAULT_CUBOID:
				lpDefaultCuboid = (DefaultCuboid*) m_Selected;
				lpDefaultCuboid->Coulomb = 0.0f;
				lpDefaultCuboid->Mass = 1.0f;
				lpDefaultCuboid->NumParticies = 4;
				lpDefaultCuboid->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				lpDefaultCuboid->Size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

			break;

		case TYPE_DEFAULT_SPHERE:
				lpDefaultSphere = (DefaultSphere*) m_Selected;

				lpDefaultSphere->bDriven = true;
				lpDefaultSphere->bDriver = true;
				lpDefaultSphere->bShowSpeed = false;
				lpDefaultSphere->bShowSpeedup = false;
				lpDefaultSphere->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				lpDefaultSphere->Speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				lpDefaultSphere->SpeedingUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				lpDefaultSphere->Coulomb = 0.0f;
				lpDefaultSphere->Mass = 1.0f;
				lpDefaultSphere->Radius = 1.0f;

				m_lpScene->SetSphereSize(lpDefaultSphere);
				
			break;
	}
}

LRESULT CALLBACK CChannelBox::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if(hwnd ==  m_hWnd) {
		if(msg == WM_DESTROY)
			m_hWnd = NULL;
	}
	
	list<Protone*>::iterator itP;
	list<Electrone*>::iterator itE;
	list<DefaultCuboid*>::iterator itDC;
	list<DefaultSphere*>::iterator itDS;

	int ilen;
	WCHAR* buf;
	WCHAR Text[256];
	ElectricalField* lpElectricalField = (ElectricalField*) m_Selected;
	Emitter* lpEmitter = (Emitter*) m_Selected;
	DefaultCuboid* lpDefaultCuboid = (DefaultCuboid*) m_Selected;
	DefaultSphere* lpDefaultSphere = (DefaultSphere*) m_Selected;
	float f;

	//MessageBox(NULL, L"CChannelBox::MessageHandler", L"Info", MB_OK);
	switch(msg) {

		case WM_SIZE:

			//m_lpScene->Render();

			break;

		case WM_COMMAND:
			//MessageBox(NULL, L"msg == WM_COMMAND", L"Info", MB_OK);
			switch (HIWORD(wParam))//Aktionsauswahl
			{
				case BN_CLICKED:
						//MessageBox(NULL, L"HIWORD(wParam) == BN_CLICKED", L"Info", MB_OK);
						switch(LOWORD(wParam))//Welcher ?
						{
							case EDIT_DRIVEN:
									m_Selected->bDriven = (SendMessage(hWndEdit[EDIT_DRIVEN], BM_GETCHECK, 0, 0) == BST_CHECKED);
									for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {
										if((*itP)->bSelected)(*itP)->bDriven = m_Selected->bDriven;
									}
									for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {
										if((*itE)->bSelected)(*itE)->bDriven = m_Selected->bDriven;
									}
									for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++) {
										if((*itDS)->bSelected)(*itDS)->bDriven = m_Selected->bDriven;
									}

									m_UpToRender();
								break;

							case EDIT_DRIVER:
									m_Selected->bDriver = (SendMessage(hWndEdit[EDIT_DRIVER], BM_GETCHECK, 0, 0) == BST_CHECKED);
									for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {
										if((*itP)->bSelected)(*itP)->bDriver = m_Selected->bDriver;
									}
									for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {
										if((*itE)->bSelected)(*itE)->bDriver = m_Selected->bDriver;
									}
									for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++) {
										if((*itDS)->bSelected)(*itDS)->bDriver = m_Selected->bDriver;
									}

									m_UpToRender();
								break;

							case EDIT_SHOW_SPEED:

									m_Selected->bShowSpeed = (SendMessage(hWndEdit[EDIT_SHOW_SPEED], BM_GETCHECK, 0, 0) == BST_CHECKED);
									for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {
										if((*itP)->bSelected)(*itP)->bShowSpeed = m_Selected->bShowSpeed;
									}
									for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {
										if((*itE)->bSelected)(*itE)->bShowSpeed = m_Selected->bShowSpeed;
									}
									for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++) {
										if((*itDS)->bSelected)(*itDS)->bShowSpeed = m_Selected->bShowSpeed;
									}
									m_UpToRender();
								break;

							case EDIT_SHOW_SPEEDING_UP:
									m_Selected->bShowSpeedup = (SendMessage(hWndEdit[EDIT_SHOW_SPEEDING_UP], BM_GETCHECK, 0, 0) == BST_CHECKED);
									for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {
										if((*itP)->bSelected)(*itP)->bShowSpeedup = m_Selected->bShowSpeedup;
									}
									for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {
										if((*itE)->bSelected)(*itE)->bShowSpeedup = m_Selected->bShowSpeedup;
									}
									for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++) {
										if((*itDS)->bSelected)(*itDS)->bShowSpeedup = m_Selected->bShowSpeedup;
									}
									m_UpToRender();
								break;
							case EDIT_SHOW_VECTOR:
									((ElectricalField*) m_Selected)->bShowVector = (SendMessage(hWndEdit[EDIT_SHOW_VECTOR], BM_GETCHECK, 0, 0) == BST_CHECKED);
									m_UpToRender();
								break;

							case EDIT_SHOW_CHARGES:
									lpDefaultCuboid = (DefaultCuboid*) m_Selected;
									lpDefaultCuboid->bShowCharges = (SendMessage(hWndEdit[EDIT_SHOW_CHARGES], BM_GETCHECK, 0, 0) == BST_CHECKED);
									for(itDC = m_lpListDefaultCuboid->begin(); itDC != m_lpListDefaultCuboid->end(); itDC++) {
										if((*itDC)->bSelected)(*itDC)->bShowCharges = lpDefaultCuboid->bShowCharges;
									}
									m_UpToRender();
	
								break;

							case EDIT_SHOW_TRACE:
									if(m_Selected->bTrace != (SendMessage(hWndEdit[EDIT_SHOW_TRACE], BM_GETCHECK, 0, 0) == BST_CHECKED)) {
										if(!m_Selected->bTrace) {
											//m_lpScene->AddTraceToSelected();
											for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {
												if(!(*itP)->bTrace && (*itP)->bSelected)m_lpScene->AddTraceToSelected((*itP));
											}
											for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {
												if(!(*itE)->bTrace && (*itE)->bSelected) m_lpScene->AddTraceToSelected((LPDEFAULTELEMENT) (*itE));
											}
										}
										else {
											//m_lpScene->StopTraceToSelected();
											for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {
												if((*itP)->bTrace && (*itP)->bSelected) m_lpScene->StopTraceToSelected((*itP));
											}
											for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {
												if((*itE)->bTrace && (*itE)->bSelected) m_lpScene->StopTraceToSelected((LPDEFAULTELEMENT) (*itE));
											}
										}
									}

									m_UpToRender();
								break;

							case EDIT_SHOW_DIRECTION:
									((Emitter*) m_Selected)->bShowVector = (SendMessage(hWndEdit[EDIT_SHOW_DIRECTION], BM_GETCHECK, 0, 0) == BST_CHECKED);

									m_UpToRender();
								break;

							case BUTTON_RESET:
									Reset();
									Update();
									m_UpToRender();
								break;

							case BUTTON_SET_CUBOID:
									m_UpToDate(UPDATE_CUBOID_PARTS, (DWORD) m_Selected);
									m_UpToRender();
								break;
						}
					break;

				case EN_KILLFOCUS:
						switch(LOWORD(wParam))//Welcher ?
						{	
								case EDIT_TITLE:
									
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_TITLE],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_TITLE],WM_GETTEXT,ilen+1,(LPARAM)buf);
									
									StringCchPrintf(m_Selected->Name, sizeof(m_Selected->Name) / sizeof(WCHAR), buf); 
									
									delete[] buf;

								break;
							case EDIT_POSITION_X:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_POSITION_X],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_POSITION_X],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(m_Selected->Position.x), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", m_Selected->Position.x );
									SendMessage(hWndEdit[EDIT_POSITION_X],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_POSITION_Y:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_POSITION_Y],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_POSITION_Y],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(m_Selected->Position.y), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", m_Selected->Position.y );
									SendMessage(hWndEdit[EDIT_POSITION_Y],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_POSITION_Z:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_POSITION_Z],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_POSITION_Z],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(m_Selected->Position.z), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", m_Selected->Position.z );
									SendMessage(hWndEdit[EDIT_POSITION_Z],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;


							case EDIT_SPEED_X:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_SPEED_X],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_SPEED_X],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(m_Selected->Speed.x), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", m_Selected->Speed.x );
									SendMessage(hWndEdit[EDIT_SPEED_X],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_SPEED_Y:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_SPEED_Y],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_SPEED_Y],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(m_Selected->Speed.y), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", m_Selected->Speed.y );
									SendMessage(hWndEdit[EDIT_SPEED_Y],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_SPEED_Z:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_SPEED_Z],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_SPEED_Z],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(m_Selected->Speed.z), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", m_Selected->Speed.z );
									SendMessage(hWndEdit[EDIT_SPEED_Z],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;


							case EDIT_SPEEDING_UP_X:
								ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_SPEEDING_UP_X],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_SPEEDING_UP_X],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(m_Selected->SpeedingUp.x), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", m_Selected->SpeedingUp.x );
									SendMessage(hWndEdit[EDIT_SPEEDING_UP_X],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_SPEEDING_UP_Y:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_SPEEDING_UP_Y],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_SPEEDING_UP_Y],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(m_Selected->SpeedingUp.y), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", m_Selected->SpeedingUp.y );
									SendMessage(hWndEdit[EDIT_SPEEDING_UP_Y],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_SPEEDING_UP_Z:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_SPEEDING_UP_Z],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_SPEEDING_UP_Z],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(m_Selected->SpeedingUp.z), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", m_Selected->SpeedingUp.z );
									SendMessage(hWndEdit[EDIT_SPEEDING_UP_Z],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;


							case EDIT_VIEWAT_X:
									lpElectricalField = (ElectricalField*) m_Selected;
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_VIEWAT_X],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_VIEWAT_X],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpElectricalField->ViewAt.x), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpElectricalField->ViewAt.x );
									SendMessage(hWndEdit[EDIT_VIEWAT_X],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_VIEWAT_Y:
									lpElectricalField = (ElectricalField*) m_Selected;
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_VIEWAT_Y],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_VIEWAT_Y],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpElectricalField->ViewAt.y), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpElectricalField->ViewAt.y );
									SendMessage(hWndEdit[EDIT_VIEWAT_Y],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_VIEWAT_Z:
									lpElectricalField = (ElectricalField*) m_Selected;
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_VIEWAT_Z],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_VIEWAT_Z],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpElectricalField->ViewAt.z), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpElectricalField->ViewAt.z );
									SendMessage(hWndEdit[EDIT_VIEWAT_Z],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_DIRECTION_X:
									lpEmitter = (Emitter*) m_Selected;
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_DIRECTION_X],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_DIRECTION_X],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpEmitter->ViewAt.x), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpEmitter->ViewAt.x );
									SendMessage(hWndEdit[EDIT_DIRECTION_X],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_DIRECTION_Y:
									lpEmitter = (Emitter*) m_Selected;
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_DIRECTION_Y],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_DIRECTION_Y],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpEmitter->ViewAt.y), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpEmitter->ViewAt.y );
									SendMessage(hWndEdit[EDIT_DIRECTION_Y],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_DIRECTION_Z:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_DIRECTION_Z],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_DIRECTION_Z],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpEmitter->ViewAt.z), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpEmitter->ViewAt.z );
									SendMessage(hWndEdit[EDIT_DIRECTION_Z],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_RANDOM_X:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_RANDOM_X],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_RANDOM_X],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpEmitter->Random.x ), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpEmitter->Random.x );
									SendMessage(hWndEdit[EDIT_RANDOM_X],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_RANDOM_Y:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_RANDOM_Y],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_RANDOM_Y],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpEmitter->Random.y ), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpEmitter->Random.y );
									SendMessage(hWndEdit[EDIT_RANDOM_Y],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_RANDOM_Z:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_RANDOM_Z],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_RANDOM_Z],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpEmitter->Random.z ), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpEmitter->Random.z );
									SendMessage(hWndEdit[EDIT_RANDOM_Z],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_SIZE_X:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_SIZE_X],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_SIZE_X],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpDefaultCuboid->Size.x ), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpDefaultCuboid->Size.x );
									SendMessage(hWndEdit[EDIT_SIZE_X],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToDate(UPDATE_CUBOID_PARTS, (DWORD)lpDefaultCuboid);
									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_SIZE_Y:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_SIZE_Y],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_SIZE_Y],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpDefaultCuboid->Size.y ), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpDefaultCuboid->Size.y );
									SendMessage(hWndEdit[EDIT_SIZE_Y],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToDate(UPDATE_CUBOID_PARTS, (DWORD)lpDefaultCuboid);
									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_SIZE_Z:
									lpDefaultCuboid = (DefaultCuboid*) m_Selected;
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_SIZE_Z],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_SIZE_Z],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpDefaultCuboid->Size.z ), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpDefaultCuboid->Size.z );
									SendMessage(hWndEdit[EDIT_SIZE_Z],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToDate(UPDATE_CUBOID_PARTS, (DWORD)lpDefaultCuboid);
									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_RADIUS:
									//lpDefaultSphere = (DefaultSphere*) m_Selected;
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_RADIUS],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_RADIUS],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&(lpDefaultSphere->Radius), buf, ilen + 1);
									
									StringCchPrintf( Text, 256, L"%3.3f ", lpDefaultSphere->Radius );
									SendMessage(hWndEdit[EDIT_RADIUS],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));
									
									m_lpScene->SetSphereSize(lpDefaultSphere);
									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_MASS:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_MASS],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_MASS],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									switch(m_Selected->tempType) {
										case TYPE_DEFAULT_CUBOID:
												ConvertStringToFloat(&(lpDefaultCuboid->Mass), buf, ilen + 1);
												StringCchPrintf( Text, 256, L"%3.3f ", lpDefaultCuboid->Mass);
											break;
										case TYPE_DEFAULT_SPHERE:
												ConvertStringToFloat(&(lpDefaultSphere->Mass), buf, ilen + 1);
												StringCchPrintf( Text, 256, L"%3.3f ", lpDefaultSphere->Mass);
											break;
									}

									SendMessage(hWndEdit[EDIT_MASS],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_CHARGE:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_CHARGE],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_CHARGE],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									switch(m_Selected->tempType) {
										case TYPE_DEFAULT_CUBOID:
												ConvertStringToFloat(&(lpDefaultCuboid->Coulomb), buf, ilen + 1);
												StringCchPrintf( Text, 256, L"%3.3f ", lpDefaultCuboid->Coulomb);
												m_UpToDate(UPDATE_CUBOID_CHARGES, (DWORD)lpDefaultCuboid);
											break;
										case TYPE_DEFAULT_SPHERE:
												ConvertStringToFloat(&(lpDefaultSphere->Coulomb), buf, ilen + 1);
												StringCchPrintf( Text, 256, L"%3.3f ", lpDefaultSphere->Coulomb);
											break;
									}

									SendMessage(hWndEdit[EDIT_CHARGE],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));
									
									
									m_UpToRender();
									delete[] buf;
								break;

							case EDIT_NUM_PARTS:
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_NUM_PARTS],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_NUM_PARTS],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&f, buf, ilen + 1);
									
									lpDefaultCuboid->NumParticies = static_cast<UINT>(f);
									
									StringCchPrintf( Text, 256, L"%3d ", lpDefaultCuboid->NumParticies);
									SendMessage(hWndEdit[EDIT_NUM_PARTS],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));
					
									m_UpToDate(UPDATE_CUBOID_PARTS, (DWORD)lpDefaultCuboid);
									m_UpToRender();
									delete[] buf;
								break;


							case EDIT_WAIT_TIME:
									lpEmitter = (Emitter*) m_Selected;
									ilen = static_cast<int>(SendMessage(hWndEdit[EDIT_WAIT_TIME],WM_GETTEXTLENGTH,0,0));
									buf = new WCHAR[ilen + 1];
									
									SendMessage(hWndEdit[EDIT_WAIT_TIME],WM_GETTEXT,ilen+1,reinterpret_cast<LPARAM>(buf));

									ConvertStringToFloat(&f, buf, ilen + 1);

									lpEmitter->waitTime = f;
									
									StringCchPrintf( Text, 256, L"%3.3d ", lpEmitter->waitTime );
									SendMessage(hWndEdit[EDIT_WAIT_TIME],WM_SETTEXT,0, reinterpret_cast<LPARAM>(Text));

									m_UpToRender();
									delete[] buf;
								break;
						}
					break;
			}
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

float CChannelBox::ConvertStringToFloat(float* pDest, WCHAR* pSource, int lenght){
	int section = 0;
	float signe = 1.0f;
	float f = 0.0f;
	for(int i = 0; i < lenght; i++) {
		switch(pSource[i]) {
			case L'-':
				if(f == 0.0f) {
					signe = -1.0f;
				}
				break;
			case L'0':
				if(section == 0) {
					f *= 10.0f;
					f += 0.0f;
				} else {
					f += 0.0f / pow(10.0f, section);
					section++;
				}
				break;
			case L'1':
				if(section == 0) {
					f *= 10.0f;
					f += 1.0f;
				} else {
					f += 1.0f / pow(10.0f, section);
					section++;
				}
				break;
			case L'2':
				if(section == 0) {
					f *= 10.0f;
					f += 2.0f;
				} else {
					f += 2.0f / pow(10.0f, section);
					section++;
				}
				break;
			case L'3':
				if(section == 0) {
					f *= 10.0f;
					f += 3.0f;
				} else {
					f += 3.0f / pow(10.0f, section);
					section++;
				}
				break;
			case L'4':
				if(section == 0) {
					f *= 10.0f;
					f += 4.0f;
				} else {
					f += 4.0f / pow(10.0f, section);
					section++;
				}
				break;
			case L'5':
				if(section == 0) {
					f *= 10.0f;
					f += 5.0f;
				} else {
					f += 5.0f / pow(10.0f, section);
					section++;
				}
				break;
			case L'6':
				if(section == 0) {
					f *= 10.0f;
					f += 6.0f;
				} else {
					f += 6.0f / pow(10.0f, section);
					section++;
				}
				break;
			case L'7':
				if(section == 0) {
					f *= 10.0f;
					f += 7.0f;
				} else {
					f += 7.0f / pow(10.0f, section);
					section++;
				}
				break;
			case L'8':
				if(section == 0) {
					f *= 10.0f;
					f += 8.0f;
				} else {
					f += 8.0f / pow(10.0f, section);
					section++;
				}
				break;
			case L'9':
				if(section == 0) {
					f *= 10.0f;
					f += 9.0f;
				} else {
					f += 9.0f / pow(10.0f, section);
					section++;
				}
				break;
			case L'.':
				if(section == NULL) section = 1;
					
				break;
			case L',':
				if(section == NULL) section = 1;
					
				break;
			case L' ':
				*pDest = f * signe;
				break;
				//return;

			case L'\n':
				*pDest = f * signe;
				return f * signe;
					
				break;
			
		}
	}
	*pDest = f * signe;
	return f * signe;
}