#include "Feature.h"


CFeatureWindow::CFeatureWindow(){
	m_hWnd = NULL;
	Selected = NULL;
}

CFeatureWindow::~CFeatureWindow() {
	m_lpPhysics = NULL;
	m_lpListProtone = NULL;
	m_lpListElectrone = NULL;
}

void CFeatureWindow::SetCPhysicsPtr(CPhysics* lpPhysics) {
	m_lpPhysics = lpPhysics; 

	m_lpListProtone = m_lpPhysics->GetListProtonePtr();
	m_lpListElectrone = m_lpPhysics->GetListElectronePtr();

}

BOOL CFeatureWindow::Create(HINSTANCE hInstance, HWND parentHWnd, UINT x, UINT y, UINT width, UINT height) {

	m_hWnd = CreateWindowEx(NULL,                   // Keine erweiterten Stile nutzen
                          L"WindowClass",           // Klassenname
                          L"",						// Fenstertitel
                          /*WS_OVERLAPPEDWINDOW |*/     // Fenster
						  //WS_THICKFRAME | 
						  WS_VISIBLE |WS_CHILD,     // Eigenschaften
                          x, y,						// Anfangsposition
                          width, height,			// und Groesse des Fensters
                          parentHWnd,               // Handle des Elternfensters
                          NULL,						// Handle des Menues
                          hInstance,				// Anwendungsinstanz
                          NULL);					// wird nicht benoetigt

	if(!m_hWnd) return FALSE;


	hWmdEdit[EDIT_TITLE] = CreateWindow(L"edit", L"Name",WS_CHILD | WS_VISIBLE | ES_CENTER ,
		20, 05, 160, 16, m_hWnd, (HMENU) EDIT_TITLE, hInstance, NULL) ;

	hWmdEdit[EDIT_POSITION_X] = CreateWindow(L"edit", L"- . - - - ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		120, 40, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_X, hInstance, NULL) ;
	hWmdEdit[EDIT_POSITION_Y] = CreateWindow(L"edit", L"- . - - - ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		120, 58, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Y, hInstance, NULL) ;
	hWmdEdit[EDIT_POSITION_Z] = CreateWindow(L"edit", L"- . - - - ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		120, 76, 60, 16, m_hWnd, (HMENU) EDIT_POSITION_Z, hInstance, NULL) ;
	
	hWmdEdit[EDIT_SPEED_X] = CreateWindow(L"edit", L"- . - - - ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		120, 96, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_X, hInstance, NULL) ;
	hWmdEdit[EDIT_SPEED_Y] = CreateWindow(L"edit", L"- . - - - ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		120, 114, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_Y, hInstance, NULL) ;
	hWmdEdit[EDIT_SPEED_Z] = CreateWindow(L"edit", L"- . - - - ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		120, 132, 60, 16, m_hWnd, (HMENU) EDIT_SPEED_Z, hInstance, NULL) ;

	hWmdEdit[EDIT_DRIVER] = CreateWindow(L"edit", L"- - - - - ",  WS_CHILD | WS_VISIBLE | ES_RIGHT ,
		120, 152, 60, 16, m_hWnd, (HMENU) EDIT_DRIVER, hInstance, NULL) ;
	hWmdEdit[EDIT_DRIVEN] = CreateWindow(L"edit", L"- - - - - ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		120, 170, 60, 16, m_hWnd, (HMENU) EDIT_DRIVEN, hInstance, NULL) ;

	hWmdEdit[EDIT_MASS] = CreateWindow(L"edit", L"- . - - - ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		120, 190, 60, 16, m_hWnd, (HMENU) EDIT_MASS, hInstance, NULL) ;
	hWmdEdit[EDIT_CHARGE] = CreateWindow(L"edit", L"- . - - - ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		120, 208, 60, 16, m_hWnd, (HMENU) EDIT_CHARGE, hInstance, NULL) ;



	CreateWindow(L"static", L"Position X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		10, 40, 105, 16, m_hWnd, (HMENU) 99, hInstance, NULL) ;
	CreateWindow(L"static", L"Position Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		10, 58, 105, 16, m_hWnd, (HMENU) 99, hInstance, NULL) ;
	CreateWindow(L"static", L"Position Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		10, 76, 105, 16, m_hWnd, (HMENU) 99, hInstance, NULL) ;

	CreateWindow(L"static", L"Geschwind. X ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		10, 96, 105, 16, m_hWnd, (HMENU) 99, hInstance, NULL) ;
	CreateWindow(L"static", L"Geschwind. Y ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		10, 114, 105, 16, m_hWnd, (HMENU) 99, hInstance, NULL) ;
	CreateWindow(L"static", L"Geschwind. Z ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		10, 132, 105, 16, m_hWnd, (HMENU) 99, hInstance, NULL) ;

	CreateWindow(L"static", L"Driver ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		10, 152, 105, 16, m_hWnd, (HMENU) 99, hInstance, NULL) ;
	CreateWindow(L"static", L"Driven ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		10, 170, 105, 16, m_hWnd, (HMENU) 99, hInstance, NULL) ;

	CreateWindow(L"static", L"Masse ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		10, 190, 105, 16, m_hWnd, (HMENU) 99, hInstance, NULL) ;
	CreateWindow(L"static", L"Ladung ",  WS_CHILD | WS_VISIBLE | ES_RIGHT,
		10, 208, 105, 16, m_hWnd, (HMENU) 99, hInstance, NULL) ;


	hWndOK = CreateWindowW(L"button", L"OK",  WS_CHILD | WS_VISIBLE,
		10, 240, 180, 20, m_hWnd, (HMENU) BUTTON_OK, hInstance, NULL) ;

	return TRUE;
}

void CFeatureWindow::Resize(UINT x, UINT y, UINT width, UINT height){
	SetWindowPos(m_hWnd, NULL, x, y, width, height, SWP_NOOWNERZORDER);
}

void CFeatureWindow::OnPicked(){
	list<Protone*>::iterator itP;

	for(itP = m_lpListProtone->begin(); itP!= m_lpListProtone->end(); itP++ ) {
		if((*itP)->bSelected){
			if(Selected ==(DWORD)(*itP)) return;
			Selected = (DWORD)(*itP);

			WCHAR Text[256];

			SendMessage(hWmdEdit[EDIT_TITLE],WM_SETTEXT,0,(long)(*itP)->Name);

			StringCchPrintf( Text, 256, L"%3.3f ",(*itP)->Position.x );
			SendMessage(hWmdEdit[EDIT_POSITION_X],WM_SETTEXT,0,(long)Text);

			StringCchPrintf( Text, 256, L"%3.3f ",(*itP)->Position.y );
			SendMessage(hWmdEdit[EDIT_POSITION_Y],WM_SETTEXT,0,(long)Text);

			StringCchPrintf( Text, 256, L"%3.3f ",(*itP)->Position.z );
			SendMessage(hWmdEdit[EDIT_POSITION_Z],WM_SETTEXT,0,(long)Text);


			StringCchPrintf( Text, 256, L"%3.3f ",(*itP)->Speed.x );
			SendMessage(hWmdEdit[EDIT_SPEED_X],WM_SETTEXT,0,(long)Text);

			StringCchPrintf( Text, 256, L"%3.3f ",(*itP)->Speed.y );
			SendMessage(hWmdEdit[EDIT_SPEED_Y],WM_SETTEXT,0,(long)Text);

			StringCchPrintf( Text, 256, L"%3.3f ",(*itP)->Speed.z );
			SendMessage(hWmdEdit[EDIT_SPEED_Z],WM_SETTEXT,0,(long)Text);

			if((*itP)->bDriver) StringCchPrintf( Text, 256, L"Ein  ");
			else StringCchPrintf( Text, 256, L"Aus  ");
			SendMessage(hWmdEdit[EDIT_DRIVER],WM_SETTEXT,0,(long)Text);

			if((*itP)->bDriven) StringCchPrintf( Text, 256, L"Ein  ");
			else StringCchPrintf( Text, 256, L"Aus  ");
			SendMessage(hWmdEdit[EDIT_DRIVEN],WM_SETTEXT,0,(long)Text);

			return;

		}
	}

	list<Electrone*>::iterator itE;

	for(itE = m_lpListElectrone->begin(); itE!= m_lpListElectrone->end(); itE++ ) {
		if((*itE)->bSelected) {
			if(Selected ==(DWORD)(*itE)) return;
			Selected = (DWORD)(*itE);

			WCHAR Text[256];

			SendMessage(hWmdEdit[EDIT_TITLE],WM_SETTEXT,0,(long)(*itE)->Name);

			StringCchPrintf( Text, 256, L"%3.3f ",(*itE)->Position.x );
			SendMessage(hWmdEdit[EDIT_POSITION_X],WM_SETTEXT,0,(long)Text);

			StringCchPrintf( Text, 256, L"%3.3f ",(*itE)->Position.y );
			SendMessage(hWmdEdit[EDIT_POSITION_Y],WM_SETTEXT,0,(long)Text);

			StringCchPrintf( Text, 256, L"%3.3f ",(*itE)->Position.z );
			SendMessage(hWmdEdit[EDIT_POSITION_Z],WM_SETTEXT,0,(long)Text);


			StringCchPrintf( Text, 256, L"%3.3f ",(*itE)->Speed.x );
			SendMessage(hWmdEdit[EDIT_SPEED_X],WM_SETTEXT,0,(long)Text);

			StringCchPrintf( Text, 256, L"%3.3f ",(*itE)->Speed.y );
			SendMessage(hWmdEdit[EDIT_SPEED_Y],WM_SETTEXT,0,(long)Text);

			StringCchPrintf( Text, 256, L"%3.3f ",(*itE)->Speed.z );
			SendMessage(hWmdEdit[EDIT_SPEED_Z],WM_SETTEXT,0,(long)Text);

			if((*itE)->bDriver) StringCchPrintf( Text, 256, L"Ein  ");
			else StringCchPrintf( Text, 256, L"Aus  ");
			SendMessage(hWmdEdit[EDIT_DRIVER],WM_SETTEXT,0,(long)Text);

			if((*itE)->bDriven) StringCchPrintf( Text, 256, L"Ein  ");
			else StringCchPrintf( Text, 256, L"Aus  ");
			SendMessage(hWmdEdit[EDIT_DRIVEN],WM_SETTEXT,0,(long)Text);

			return;
		}
	}

	Selected = NULL;
	ResetEdit();
}

void CFeatureWindow::UpdateData(void){

	if(!Selected) {
		ResetEdit();
		return;
	}

	WCHAR Text[256];
	
	StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Position.x );
	SendMessage(hWmdEdit[EDIT_POSITION_X],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Position.y );
	SendMessage(hWmdEdit[EDIT_POSITION_Y],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Position.z );
	SendMessage(hWmdEdit[EDIT_POSITION_Z],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Speed.x );
	SendMessage(hWmdEdit[EDIT_SPEED_X],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Speed.y );
	SendMessage(hWmdEdit[EDIT_SPEED_Y],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Speed.z );
	SendMessage(hWmdEdit[EDIT_SPEED_Z],WM_SETTEXT,0,(long)Text);

}

void CFeatureWindow::ResetEdit(void) {
	WCHAR Text[256];

	StringCchPrintf( Text, 256, L"- - - - - ");
	SendMessage(hWmdEdit[EDIT_TITLE],WM_SETTEXT,0,(long)Text);
	
	StringCchPrintf( Text, 256, L"- . - - - ");
	SendMessage(hWmdEdit[EDIT_POSITION_X],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"- . - - - ");
	SendMessage(hWmdEdit[EDIT_POSITION_Y],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"- . - - - ");
	SendMessage(hWmdEdit[EDIT_POSITION_Z],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"- . - - - ");
	SendMessage(hWmdEdit[EDIT_SPEED_X],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"- . - - - ");
	SendMessage(hWmdEdit[EDIT_SPEED_Y],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"- . - - - ");
	SendMessage(hWmdEdit[EDIT_SPEED_Z],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"- - - - - ");
	SendMessage(hWmdEdit[EDIT_DRIVER],WM_SETTEXT,0,(long)Text);

	StringCchPrintf( Text, 256, L"- - - - - ");
	SendMessage(hWmdEdit[EDIT_DRIVEN],WM_SETTEXT,0,(long)Text);

}

BOOL CFeatureWindow::IsHwndMemberOfThis(HWND hwnd){
	if(hwnd == m_hWnd) return true;
	/*for(int i = 0; i < (sizeof(hWmdEdit) / sizeof(hWmdEdit[0])); i++) {
		if(hwnd == hWmdEdit[i]) return true;
	}*/
	//return true;
	if(hwnd == hWndOK) return true;
	return true;
	return false;
}

void CFeatureWindow::MessageHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam){
	switch(msg) {
		case WM_COMMAND:
			switch (HIWORD(wparam))//Aktionsauswahl
			{
			case BN_CLICKED://Button gedrückt ?
				switch(LOWORD(wparam))//Welcher ?
				{
					case BUTTON_OK:
						//PostQuitMessage(0);
					break;
				}
			case EN_KILLFOCUS:
	

				if(!Selected) return;

				int ilen;
				WCHAR* buf;
				WCHAR Text[256];
				switch(LOWORD(wparam)) {
					case EDIT_TITLE:
							
							ilen = SendMessage(hWmdEdit[EDIT_TITLE],WM_GETTEXTLENGTH,0,0);
							buf = new WCHAR[ilen + 1];
							
							SendMessage(hWmdEdit[EDIT_TITLE],WM_GETTEXT,ilen+1,(long)buf);
							
							StringCchPrintf(((Electrone*)Selected)->Name, sizeof(((Electrone*)Selected)->Name) / sizeof(WCHAR), buf); 
							
							delete[] buf;
						break;

					case EDIT_POSITION_X:

							ilen = SendMessage(hWmdEdit[EDIT_POSITION_X],WM_GETTEXTLENGTH,0,0);
							buf = new WCHAR[ilen + 1];
							
							SendMessage(hWmdEdit[EDIT_POSITION_X],WM_GETTEXT,ilen+1,(long)buf);

							ConvertStringToFloat(&(((Electrone*)Selected)->Position.x), buf, ilen + 1);
							
							StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Position.x );
							SendMessage(hWmdEdit[EDIT_POSITION_X],WM_SETTEXT,0,(long)Text);

							m_UpToRender();


							delete[] buf;
						break;
					case EDIT_POSITION_Y:

							ilen = SendMessage(hWmdEdit[EDIT_POSITION_Y],WM_GETTEXTLENGTH,0,0);
							buf = new WCHAR[ilen + 1];
							
							SendMessage(hWmdEdit[EDIT_POSITION_Y],WM_GETTEXT,ilen+1,(long)buf);

							ConvertStringToFloat(&(((Electrone*)Selected)->Position.y), buf, ilen + 1);
							
							StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Position.y );
							SendMessage(hWmdEdit[EDIT_POSITION_Y],WM_SETTEXT,0,(long)Text);

							m_UpToRender();


							delete[] buf;
						break;

					case EDIT_POSITION_Z:

							ilen = SendMessage(hWmdEdit[EDIT_POSITION_Z],WM_GETTEXTLENGTH,0,0);
							buf = new WCHAR[ilen + 1];
							
							SendMessage(hWmdEdit[EDIT_POSITION_Z],WM_GETTEXT,ilen+1,(long)buf);

							ConvertStringToFloat(&(((Electrone*)Selected)->Position.z), buf, ilen + 1);
							
							StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Position.z );
							SendMessage(hWmdEdit[EDIT_POSITION_Z],WM_SETTEXT,0,(long)Text);

							m_UpToRender();


							delete[] buf;							
						break;

					case EDIT_SPEED_X:

							ilen = SendMessage(hWmdEdit[EDIT_SPEED_X],WM_GETTEXTLENGTH,0,0);
							buf = new WCHAR[ilen + 1];
							
							SendMessage(hWmdEdit[EDIT_SPEED_X],WM_GETTEXT,ilen+1,(long)buf);

							ConvertStringToFloat(&(((Electrone*)Selected)->Speed.x), buf, ilen + 1);
							
							StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Speed.x);
							SendMessage(hWmdEdit[EDIT_SPEED_X],WM_SETTEXT,0,(long)Text);

							m_UpToRender();


							delete[] buf;					
						break;

					case EDIT_SPEED_Y:

							ilen = SendMessage(hWmdEdit[EDIT_SPEED_Y],WM_GETTEXTLENGTH,0,0);
							buf = new WCHAR[ilen + 1];
							
							SendMessage(hWmdEdit[EDIT_SPEED_Y],WM_GETTEXT,ilen+1,(long)buf);

							ConvertStringToFloat(&(((Electrone*)Selected)->Speed.y), buf, ilen + 1);
							
							StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Speed.y);
							SendMessage(hWmdEdit[EDIT_SPEED_Y],WM_SETTEXT,0,(long)Text);

							m_UpToRender();


							delete[] buf;							
						break;

					case EDIT_SPEED_Z:

							ilen = SendMessage(hWmdEdit[EDIT_SPEED_Z],WM_GETTEXTLENGTH,0,0);
							buf = new WCHAR[ilen + 1];
							
							SendMessage(hWmdEdit[EDIT_SPEED_Z],WM_GETTEXT,ilen+1,(long)buf);

							ConvertStringToFloat(&(((Electrone*)Selected)->Speed.z), buf, ilen + 1);
							
							StringCchPrintf( Text, 256, L"%3.3f ",((Electrone*)Selected)->Speed.z);
							SendMessage(hWmdEdit[EDIT_SPEED_Z],WM_SETTEXT,0,(long)Text);

							m_UpToRender();

							delete[] buf;						
						break;

					case EDIT_DRIVER:
							ilen = SendMessage(hWmdEdit[EDIT_DRIVER],WM_GETTEXTLENGTH,0,0);
							buf = new WCHAR[ilen + 1];
							
							SendMessage(hWmdEdit[EDIT_DRIVER],WM_GETTEXT,ilen+1,(long)buf);

							((Electrone*)Selected)->bDriver = IsStringTrue(buf, ilen + 1);

							if(((Electrone*)Selected)->bDriver) StringCchPrintf( Text, 256, L"EIN");
							else StringCchPrintf( Text, 256, L"AUS");
							SendMessage(hWmdEdit[EDIT_DRIVER],WM_SETTEXT,0,(long)Text);

							m_UpToRender();

							delete[] buf;							
						break;

					case EDIT_DRIVEN:
							ilen = SendMessage(hWmdEdit[EDIT_DRIVEN],WM_GETTEXTLENGTH,0,0);
							buf = new WCHAR[ilen + 1];
							
							SendMessage(hWmdEdit[EDIT_DRIVEN],WM_GETTEXT,ilen+1,(long)buf);

							((Electrone*)Selected)->bDriven = IsStringTrue(buf, ilen + 1);

							if(((Electrone*)Selected)->bDriven) StringCchPrintf( Text, 256, L"EIN");
							else StringCchPrintf( Text, 256, L"AUS");
							SendMessage(hWmdEdit[EDIT_DRIVEN],WM_SETTEXT,0,(long)Text);

							m_UpToRender();

							delete[] buf;							
						break;

				}
			break;
		}
	}
}

void CFeatureWindow::ConvertStringToFloat(float* pDest, WCHAR* pSource, UINT lenght){
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
				return;
					
				break;
			
		}
	}
	*pDest = f * signe;
}

BOOL CFeatureWindow::IsStringTrue(WCHAR* pSource, UINT lenght){

	int pos = 0;

	for(int i = 0; i < lenght; i++) {
		switch(pos) {
			case 0:
					if(pSource[i] == L'E') pos++;
					if(pSource[i] == L'e') pos++;
				 break;
			case 1:
					if(pSource[i] == L'I') pos++;
					if(pSource[i] == L'i') pos++;
				 break;
			 case 2:
					if(pSource[i] == L'N') pos++;
					if(pSource[i] == L'n') pos++;
				 break;
			 case 3:
					return true;
				 break;

		}
		
	}
	return false;
}