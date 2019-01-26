#include "SaveLoad.h"

//----------------------------------------------------------------------------------------------------
// CSaveLoad class
//----------------------------------------------------------------------------------------------------

CSaveLoad::CSaveLoad(){
	Filename.lpstrFile = NULL;
	m_lpScene = NULL;
}

CSaveLoad::~CSaveLoad(){

}

void CSaveLoad::SetCPhysicsPtr(CPhysics* lpPhysics) {
	m_lpPhysics = lpPhysics; 

	m_lpListProtone = m_lpPhysics->GetListProtonePtr();
	m_lpListElectrone = m_lpPhysics->GetListElectronePtr();
	m_lpListElectricalField = m_lpPhysics->GetListElectricalFieldPtr();
	m_lpListMagneticalField = m_lpPhysics->GetListMagneticalFieldPtr();
	m_lpListGravityField = m_lpPhysics->GetListGravityFieldPtr();
	m_lpListEmitter = m_lpPhysics->GetListEmitterPtr();
	m_lpListDefaultCuboid = m_lpPhysics->GetListDefaultCuboidPtr();
	m_lpListDefaultSphere = m_lpPhysics->GetListDefaultSpherePtr();

}

LPCWSTR CSaveLoad::GetFileNameEx(void){
	
	//Filename.lpstrFile;
	int i = 0;
	int j = 0;

	ZeroMemory(Title, sizeof(Title));

	//while(Filename.lpstrFile[i] != L'\0') {
	for(int i = 0; i < 256; i++) {
		Title[j] = Filename.lpstrFile[i];
		if(Filename.lpstrFile[i] == L'\0') break;
		j++;
		if(Filename.lpstrFile[i] == L'\\') {
			j = 0;
			ZeroMemory(Title, sizeof(Title));
		}
	}
	return Title;
}

void CSaveLoad::Save(LPWSTR lpFilename){

	if(lpFilename == NULL) {
		return;
	}

	//MessageBox(NULL, L"Datei wird geschrieben...", L"Info", MB_OK);


	_wfopen_s(&Datei, lpFilename, L"w");
	fwprintf(Datei, L"%s\n", STRING_TITLE);
	fwprintf(Datei, L"%s: %d;\n\n", STRING_VERSION, VERSION);
	for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {

		fwprintf(Datei, L"%s\n%s\n", STRING_PROTONE, STRING_BRECKET_OPEN);
		fwprintf(Datei, L"   %s: %s ;\n",STRING_NAME, (*itP)->Name);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_POSITION, (*itP)->Position.x, (*itP)->Position.y, (*itP)->Position.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_SPEED, (*itP)->Speed.x, (*itP)->Speed.y, (*itP)->Speed.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_SPEEDING_UP, (*itP)->SpeedingUp.x, (*itP)->SpeedingUp.y, (*itP)->SpeedingUp.z);
		fwprintf(Datei, L"   %s: %d;\n", STRING_DRIVEN, (*itP)->bDriven);
		fwprintf(Datei, L"   %s: %d;\n", STRING_DRIVER, (*itP)->bDriver);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SELECTED, (*itP)->bSelected);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_SPEED, (*itP)->bShowSpeed);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_SPEEDING_UP, (*itP)->bShowSpeedup);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_TRACE, (*itP)->bTrace);     // Trace can not be exported
		fwprintf(Datei, L"%s\n\n", STRING_BRECKET_CLOSE);
	}

	for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {

		fwprintf(Datei, L"%s\n%s\n", STRING_ELECTRONE, STRING_BRECKET_OPEN);
		fwprintf(Datei, L"   %s: %s ;\n", STRING_NAME, (*itE)->Name);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_POSITION, (*itE)->Position.x, (*itE)->Position.y, (*itE)->Position.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_SPEED, (*itE)->Speed.x, (*itE)->Speed.y, (*itE)->Speed.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_SPEEDING_UP, (*itE)->SpeedingUp.x, (*itE)->SpeedingUp.y, (*itE)->SpeedingUp.z);
		fwprintf(Datei, L"   %s: %d;\n", STRING_DRIVEN, (*itE)->bDriven);
		fwprintf(Datei, L"   %s: %d;\n", STRING_DRIVER, (*itE)->bDriver);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SELECTED, (*itE)->bSelected);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_SPEED, (*itE)->bShowSpeed);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_SPEEDING_UP, (*itE)->bShowSpeedup);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_TRACE, (*itE)->bTrace);     // Trace can not be exported
		fwprintf(Datei, L"%s\n\n", STRING_BRECKET_CLOSE);
	}

	for(itEF = m_lpListElectricalField->begin(); itEF != m_lpListElectricalField->end(); itEF++) {
		fwprintf(Datei, L"%s\n%s\n", STRING_FIELD_ELECTRICAL, STRING_BRECKET_OPEN);
		fwprintf(Datei, L"   %s: %s ;\n", STRING_NAME, (*itEF)->Name);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_POSITION, (*itEF)->Position.x, (*itEF)->Position.y, (*itEF)->Position.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_VIEW_AT, (*itEF)->ViewAt.x, (*itEF)->ViewAt.y, (*itEF)->ViewAt.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_VECTOR, (*itEF)->Vector.x, (*itEF)->Vector.y, (*itEF)->Vector.z);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SELECTED, (*itEF)->bSelected);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_VECTOR, (*itEF)->bShowVector);
		fwprintf(Datei, L"%s\n\n", STRING_BRECKET_CLOSE);
	}

	for(itMF = m_lpListMagneticalField->begin(); itMF != m_lpListMagneticalField->end(); itMF++) {
		fwprintf(Datei, L"%s\n%s\n", STRING_FIELD_MAGNETICAL, STRING_BRECKET_OPEN);
		fwprintf(Datei, L"   %s: %s ;\n", STRING_NAME, (*itMF)->Name);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_POSITION, (*itMF)->Position.x, (*itMF)->Position.y, (*itMF)->Position.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_VIEW_AT, (*itMF)->ViewAt.x, (*itMF)->ViewAt.y, (*itMF)->ViewAt.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_VECTOR, (*itMF)->Vector.x, (*itMF)->Vector.y, (*itMF)->Vector.z);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SELECTED, (*itMF)->bSelected);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_VECTOR, (*itMF)->bShowVector);
		fwprintf(Datei, L"%s\n\n", STRING_BRECKET_CLOSE);
	}

	for(itGF = m_lpListGravityField->begin(); itGF != m_lpListGravityField->end(); itGF++) {
		fwprintf(Datei, L"%s\n%s\n", STRING_FIELD_GRAVITY, STRING_BRECKET_OPEN);
		fwprintf(Datei, L"   %s: %s;\n", STRING_NAME, (*itGF)->Name);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_POSITION, (*itGF)->Position.x, (*itGF)->Position.y, (*itGF)->Position.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_VIEW_AT, (*itGF)->ViewAt.x, (*itGF)->ViewAt.y, (*itGF)->ViewAt.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_VECTOR, (*itGF)->Vector.x, (*itGF)->Vector.y, (*itGF)->Vector.z);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SELECTED, (*itGF)->bSelected);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_VECTOR, (*itGF)->bShowVector);
		fwprintf(Datei, L"%s\n\n", STRING_BRECKET_CLOSE);
	}

	for(itOE = m_lpListEmitter->begin(); itOE != m_lpListEmitter->end(); itOE++) {
		fwprintf(Datei, L"%s\n%s\n", STRING_OBJECT_EMITTER, STRING_BRECKET_OPEN);
		fwprintf(Datei, L"   %s: %s;\n", STRING_NAME, (*itOE)->Name);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_POSITION, (*itOE)->Position.x, (*itOE)->Position.y, (*itOE)->Position.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_VIEW_AT, (*itOE)->ViewAt.x, (*itOE)->ViewAt.y, (*itOE)->ViewAt.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_VECTOR, (*itOE)->Vector.x, (*itOE)->Vector.y, (*itOE)->Vector.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_RANDOM_VECTOR, (*itOE)->Random.x, (*itOE)->Random.y, (*itOE)->Random.z);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_VECTOR, (*itOE)->bShowVector);
		fwprintf(Datei, L"   %s: %d;\n", STRING_WAIT_TIME, (*itOE)->waitTime);    	
		fwprintf(Datei, L"   %s: %d;\n", STRING_ELEMENT_TYPE, (*itOE)->elementType);
		fwprintf(Datei, L"%s\n\n", STRING_BRECKET_CLOSE);
	}

	for(itDC = m_lpListDefaultCuboid->begin(); itDC != m_lpListDefaultCuboid->end(); itDC++) {
		fwprintf(Datei, L"%s\n%s\n", STRING_OBJECT_CUBOID, STRING_BRECKET_OPEN);
		fwprintf(Datei, L"   %s: %s;\n", STRING_NAME, (*itDC)->Name);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_POSITION, (*itDC)->Position.x, (*itDC)->Position.y, (*itDC)->Position.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_SPEED, (*itDC)->Speed.x, (*itDC)->Speed.y, (*itDC)->Speed.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_SPEEDING_UP, (*itDC)->SpeedingUp.x, (*itDC)->SpeedingUp.y, (*itDC)->SpeedingUp.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_ROTATION, (*itDC)->rPosition.x, (*itDC)->rPosition.y, (*itDC)->rPosition.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_ROTATION_SPEED, (*itDC)->rSpeed.x, (*itDC)->rSpeed.y, (*itDC)->rSpeed.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_ROTATION_SPEEDING_UP, (*itDC)->rSpeedingUp.x, (*itDC)->rSpeedingUp.y, (*itDC)->rSpeedingUp.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_SIZE, (*itDC)->Size.x, (*itDC)->Size.y, (*itDC)->Size.z);
		fwprintf(Datei, L"   %s: %d;\n", STRING_DRIVEN, (*itDC)->bDriven);
		fwprintf(Datei, L"   %s: %d;\n", STRING_DRIVER, (*itDC)->bDriver);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SELECTED, (*itDC)->bSelected);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_CHARGES, (*itDC)->bShowCharges);
		fwprintf(Datei, L"   %s: %f;\n", STRING_MASS, (*itDC)->Mass);
		fwprintf(Datei, L"   %s: %f;\n", STRING_CHARGES, (*itDC)->Coulomb);
		fwprintf(Datei, L"   %s: %d;\n", STRING_NUM_PARTS, (*itDC)->NumParticies);
		for(int i = 0; i < (*itDC)->NumParticies; i++) {
			fwprintf(Datei, L"   %s %s\n", STRING_VIRTUAL_CHARGE, STRING_BRECKET_OPEN);
			fwprintf(Datei, L"      %s: %d;\n", STRING_ID, i);
			fwprintf(Datei, L"      %s: %f, %f, %f;\n", STRING_POSITION, (*itDC)->lpDefaultCharge[i].Position.x, (*itDC)->lpDefaultCharge[i].Position.y, (*itDC)->lpDefaultCharge[i].Position.z);
			fwprintf(Datei, L"      %s: %f, %f, %f;\n", STRING_SPEED, (*itDC)->lpDefaultCharge[i].Speed.x, (*itDC)->lpDefaultCharge[i].Speed.y, (*itDC)->lpDefaultCharge[i].Speed.z);
			fwprintf(Datei, L"      %s: %f, %f, %f;\n", STRING_SPEEDING_UP, (*itDC)->lpDefaultCharge[i].SpeedingUp.x, (*itDC)->lpDefaultCharge[i].SpeedingUp.y, (*itDC)->lpDefaultCharge[i].SpeedingUp.z);
			fwprintf(Datei, L"      %s: %f;\n", STRING_CHARGES, (*itDC)->lpDefaultCharge[i].Coulomb);
			fwprintf(Datei, L"   %s\n", STRING_BRECKET_CLOSE);
		}
		fwprintf(Datei, L"%s\n\n", STRING_BRECKET_CLOSE);
	}

	for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++) {
		fwprintf(Datei, L"%s\n%s\n", STRING_OBJECT_SPHERE, STRING_BRECKET_OPEN);
		fwprintf(Datei, L"   %s: %s ;\n", STRING_NAME, (*itDS)->Name);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_POSITION, (*itDS)->Position.x, (*itDS)->Position.y, (*itDS)->Position.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_SPEED, (*itDS)->Speed.x, (*itDS)->Speed.y, (*itDS)->Speed.z);
		fwprintf(Datei, L"   %s: %f, %f, %f;\n", STRING_SPEEDING_UP, (*itDS)->SpeedingUp.x, (*itDS)->SpeedingUp.y, (*itDS)->SpeedingUp.z);
		fwprintf(Datei, L"   %s: %d;\n", STRING_DRIVEN, (*itDS)->bDriven);
		fwprintf(Datei, L"   %s: %d;\n", STRING_DRIVER, (*itDS)->bDriver);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SELECTED, (*itDS)->bSelected);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_SPEED, (*itDS)->bShowSpeed);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_SPEEDING_UP, (*itDS)->bShowSpeedup);
		fwprintf(Datei, L"   %s: %d;\n", STRING_SHOW_TRACE, (*itDS)->bTrace);     // Trace can not be exported
		fwprintf(Datei, L"   %s: %f;\n", STRING_RADIUS, (*itDS)->Radius);
		fwprintf(Datei, L"   %s: %f;\n", STRING_MASS, (*itDS)->Mass);
		fwprintf(Datei, L"   %s: %f;\n", STRING_CHARGES, (*itDS)->Coulomb);
		fwprintf(Datei, L"%s\n\n", STRING_BRECKET_CLOSE);
	}

	fclose(Datei);
}
	
void CSaveLoad::SaveAs(){


	WCHAR Dir[128], Name[128]; 
	ZeroMemory(Name, sizeof(Name)); 
	GetModuleFileName(NULL, Dir, 128); 

	ZeroMemory(&Filename, sizeof(OPENFILENAME)); 
	Filename.lStructSize=sizeof(OPENFILENAME); 
	Filename.lpstrInitialDir=Dir; 
	Filename.lpstrFile=Name; 
	Filename.nMaxFile=128;
	Filename.lpstrDefExt = L"sim";
	Filename.lpstrFilter=L"EB-Simulation files\0*.sim\0";
	//Filename.lpstrFilter=L"Text-Dateien\0*.txt\0Alle Dateien\0*.*\0EB-Simulation files\0*.sim\0";
	if(!GetSaveFileName(&Filename)) return;

	Save(Filename.lpstrFile);
	
}
BOOL CSaveLoad::LoadA(LPSTR lpFilename){
	
	if(lpFilename == NULL) {
		return false;
	} 
	else {
		fopen_s(&Datei, lpFilename, "r");
	}
	
	WCHAR buffer[256];

	// the header

	//MessageBox(NULL, L"Datei wird gelesen...", L"Info", MB_OK);

	fgetws(buffer, 255, Datei);

	//MessageBox(NULL, buffer, L"Info", MB_OK);


	if(!IsString1in2(STRING_TITLE, buffer,255)) {
		if( IDNO == MessageBoxW(NULL, L"Dies ist keine gültige SIM Datei ! \n\nSoll dennoch versucht werden diese Datei zu öffnen ?", L"Warnung", MB_ICONWARNING | MB_YESNO)) return false;

	}

	
    while(!feof(Datei))   {
		fgetws(buffer, 255, Datei);

		if(IsString1in2(STRING_VERSION, buffer,255)) {
			if(GetUInt(buffer, 6, 255) < VERSION) {
				if(IDNO == MessageBox(NULL, L"Diese Dateiversion ist nicht mehr aktuell !\n\nSoll dennoch versucht werden diese Datei zu öffnen ? ", L"Versionskonflikt", MB_ICONWARNING |MB_YESNO)) {
					fclose(Datei);
					return false;
				}
			} 
			if(GetUInt(buffer, 6, 255) > VERSION) {
				if(IDNO == MessageBox(NULL, L"Diese Dateiversion ist aktueller als ihre Programmversion !\n\nSoll dennoch versucht werden diese Datei zu öffnen ?", L"Versionskonflikt", MB_ICONWARNING |MB_YESNO)) {
					fclose(Datei);
					return false;
				}	
			}
			
		}

		if(IsString1in2(STRING_PROTONE, buffer,255)) {
			AddProtone();
		}

		else if(IsString1in2(STRING_ELECTRONE, buffer,255)) {
			AddElectrone();
		}

		else if(IsString1in2(STRING_FIELD_ELECTRICAL, buffer,255)) {
			AddElectricalField();
		}

		else if(IsString1in2(STRING_FIELD_MAGNETICAL, buffer,255)) {
			AddMagneticalField();
		}

		else if(IsString1in2(STRING_FIELD_GRAVITY, buffer,255)) {
			AddGravityField();
		}    

		else if(IsString1in2(STRING_OBJECT_EMITTER, buffer,255)) {
			AddEmitter();
		}

		else if(IsString1in2(STRING_OBJECT_CUBOID, buffer,255)) {
			AddDefaultCuboid();
		}

		else if(IsString1in2(STRING_OBJECT_SPHERE, buffer,255)) {
			AddDefaultSphere();
		}
    }

    fclose(Datei);
	return true;
	
}

BOOL CSaveLoad::Load(LPWSTR lpFilename){
	
	if(lpFilename == NULL) {
		WCHAR Dir[128], Name[128]; 
		ZeroMemory(Name, sizeof(Name)); 
		GetModuleFileName(NULL, Dir, 128); 

		ZeroMemory(&Filename, sizeof(OPENFILENAME)); 
		Filename.lStructSize=sizeof(OPENFILENAME); 
		Filename.lpstrInitialDir=Dir; 
		Filename.lpstrFile=Name;
		Filename.lpstrFilter=L"EB-Simulation files\0*.sim\0";
		Filename.nMaxFile=128;
		if(!GetOpenFileName(&Filename)) return false;
		_wfopen_s(&Datei, Filename.lpstrFile, L"r");
	} 
	else {
		_wfopen_s(&Datei, lpFilename, L"r");
	}
	
	WCHAR buffer[256];

	// the header

	//MessageBox(NULL, L"Datei wird gelesen...", L"Info", MB_OK);

	fgetws(buffer, 255, Datei);

	//MessageBox(NULL, buffer, L"Info", MB_OK);


	if(!IsString1in2(STRING_TITLE, buffer,255)) {
		if( IDNO == MessageBoxW(NULL, L"Dies ist keine gültige SIM Datei ! \n\nSoll dennoch versucht werden diese Datei zu öffnen ?", L"Warnung", MB_ICONWARNING | MB_YESNO)) return false;

	}

	
    while(!feof(Datei))   {
		fgetws(buffer, 255, Datei);

		if(IsString1in2(STRING_VERSION, buffer,255)) {
			if(GetUInt(buffer, 6, 255) < VERSION) {
				if(IDNO == MessageBox(NULL, L"Diese Dateiversion ist nicht mehr aktuell !\n\nSoll dennoch versucht werden diese Datei zu öffnen ? ", L"Versionskonflikt", MB_ICONWARNING |MB_YESNO)) {
					fclose(Datei);
					return false;
				}
			} 
			if(GetUInt(buffer, 6, 255) > VERSION) {
				if(IDNO == MessageBox(NULL, L"Diese Dateiversion ist aktueller als ihre Programmversion !\n\nSoll dennoch versucht werden diese Datei zu öffnen ?", L"Versionskonflikt", MB_ICONWARNING |MB_YESNO)) {
					fclose(Datei);
					return false;
				}	
			}
			
		}

		if(IsString1in2(STRING_PROTONE, buffer,255)) {
			AddProtone();
		}

		else if(IsString1in2(STRING_ELECTRONE, buffer,255)) {
			AddElectrone();
		}

		else if(IsString1in2(STRING_FIELD_ELECTRICAL, buffer,255)) {
			AddElectricalField();
		}

		else if(IsString1in2(STRING_FIELD_MAGNETICAL, buffer,255)) {
			AddMagneticalField();
		}

		else if(IsString1in2(STRING_FIELD_GRAVITY, buffer,255)) {
			AddGravityField();
		}    

		else if(IsString1in2(STRING_OBJECT_EMITTER, buffer,255)) {
			AddEmitter();
		} 
		
		else if(IsString1in2(STRING_OBJECT_CUBOID, buffer,255)) {
			AddDefaultCuboid();
		}

		else if(IsString1in2(STRING_OBJECT_SPHERE, buffer,255)) {
			AddDefaultSphere();
		}
    }

    fclose(Datei);
	return true;
	
}


void CSaveLoad::AddProtone() {
	Protone* lpProtone = new Protone;
	lpProtone->bTrace = false;											// important to open version 1 files correctly
	m_lpPhysics->AddProtone(lpProtone);
	

	// Read the Features 
	WCHAR buffer[256];

	fgetws(buffer, 255, Datei);
	if(!IsString1in2(STRING_BRECKET_OPEN, buffer,255)) return;
		
	while(!feof(Datei))   {
		fgetws(buffer, 255, Datei);
		if(IsString1in2(STRING_NAME, buffer,255)) {
			//MessageBox(NULL, L"Name gefunden", L"Info", MB_OK);
			WCHAR text[256];
			GetString(buffer, text, sizeof(STRING_NAME) - 5 , 255);
			memcpy(lpProtone->Name, text, sizeof(lpProtone->Name));
		}
		else if(IsString1in2(STRING_POSITION, buffer,255)) {
			//MessageBox(NULL, L"Position gefunden", L"Info", MB_OK);
			GetVector(buffer, &(lpProtone->Position), 10, 255);
			
		} 
		else if(IsString1in2(STRING_SPEED, buffer,255)) {
			GetVector(buffer, &(lpProtone->Speed), 10, 255);
		} 
		else if(IsString1in2(STRING_SPEEDING_UP, buffer,255)) {
			GetVector(buffer, &(lpProtone->SpeedingUp), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_SPEED, buffer,255)) {
			GetBool(buffer, &(lpProtone->bShowSpeed), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_SPEEDING_UP, buffer,255)) {
			GetBool(buffer, &(lpProtone->bShowSpeedup), 10, 255);
		}
		else if(IsString1in2(STRING_SELECTED, buffer,255)) {
			GetBool(buffer, &(lpProtone->bSelected), 10, 255);
		}
		else if(IsString1in2(STRING_DRIVEN, buffer,255)) {
			GetBool(buffer, &(lpProtone->bDriven), 10, 255);
		}
		else if(IsString1in2(STRING_DRIVER, buffer,255)) {
			GetBool(buffer, &(lpProtone->bDriver), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_TRACE, buffer,255)) {
			GetBool(buffer, &(lpProtone->bTrace), 10, 255);
			if(lpProtone->bTrace && m_lpScene) m_lpScene->AddTraceToSelected(lpProtone);
		}

		if(IsString1in2(STRING_BRECKET_CLOSE, buffer,255)) return;
	}

}

void CSaveLoad::AddElectrone() {
	Electrone* lpElectone = new Electrone;
	lpElectone->bTrace = false;											// important to open version 1 files correctly
	m_lpPhysics->AddElectrone(lpElectone);
	
	// Read the Features 
	WCHAR buffer[256];

	fgetws(buffer, 255, Datei);
	if(!IsString1in2(STRING_BRECKET_OPEN, buffer,255)) return;
		
	while(!feof(Datei))   {
		fgetws(buffer, 255, Datei);
		if(IsString1in2(STRING_NAME, buffer,255)) {
			//MessageBox(NULL, L"Name gefunden", L"Info", MB_OK);
			WCHAR text[256];
			GetString(buffer, text, sizeof(STRING_NAME) - 5 , 255);
			memcpy(lpElectone->Name, text, sizeof(lpElectone->Name));
		}
		else if(IsString1in2(STRING_POSITION, buffer,255)) {
			//MessageBox(NULL, L"Position gefunden", L"Info", MB_OK);
			GetVector(buffer, &(lpElectone->Position), 10, 255);
			
		} else if(IsString1in2(STRING_SPEED, buffer,255)) {
			GetVector(buffer, &(lpElectone->Speed), 10, 255);
		} 
		else if(IsString1in2(STRING_SPEEDING_UP, buffer,255)) {
			GetVector(buffer, &(lpElectone->SpeedingUp), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_SPEED, buffer,255)) {
			GetBool(buffer, &(lpElectone->bShowSpeed), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_SPEEDING_UP, buffer,255)) {
			GetBool(buffer, &(lpElectone->bShowSpeedup), 10, 255);
		}
		else if(IsString1in2(STRING_SELECTED, buffer,255)) {
			GetBool(buffer, &(lpElectone->bSelected), 10, 255);
		}
		else if(IsString1in2(STRING_DRIVEN, buffer,255)) {
			GetBool(buffer, &(lpElectone->bDriven), 10, 255);
		}
		else if(IsString1in2(STRING_DRIVER, buffer,255)) {
			GetBool(buffer, &(lpElectone->bDriver), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_TRACE, buffer,255)) {
			GetBool(buffer, &(lpElectone->bTrace), 10, 255);
			if(lpElectone->bTrace && m_lpScene) m_lpScene->AddTraceToSelected((LPDEFAULTELEMENT)lpElectone);
		}

		if(IsString1in2(STRING_BRECKET_CLOSE, buffer,255)) return;
	}
}

void CSaveLoad::AddElectricalField(){

	ElectricalField* lpField = new ElectricalField;
	m_lpPhysics->AddElectricalField(lpField);


	// Read the Features 
	WCHAR buffer[256];

	fgetws(buffer, 255, Datei);
	if(!IsString1in2(STRING_BRECKET_OPEN, buffer,255)) return;

	while(!feof(Datei))   {
		fgetws(buffer, 255, Datei);
		if(IsString1in2(STRING_NAME, buffer,255)) {
			WCHAR text[256];
			GetString(buffer, text, sizeof(STRING_NAME) - 5 , 255);
			memcpy(lpField->Name, text, sizeof(lpField->Name));
		}
		else if(IsString1in2(STRING_POSITION, buffer,255)) {
			GetVector(buffer, &(lpField->Position), 10, 255);
		}
		else if(IsString1in2(STRING_VIEW_AT, buffer,255)) {
			GetVector(buffer, &(lpField->ViewAt), 10, 255);
		}
		else if(IsString1in2(STRING_VECTOR, buffer,255)) {
			GetVector(buffer, &(lpField->Vector), 10, 255);
		}
		else if(IsString1in2(STRING_SELECTED, buffer,255)) {
			GetBool(buffer, &(lpField->bSelected), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_VECTOR, buffer,255)) {
			GetBool(buffer, &(lpField->bShowVector), 10, 255);
		}
		else if(IsString1in2(STRING_BRECKET_CLOSE, buffer,255)) return;
	}
}

void CSaveLoad::AddMagneticalField() {
	MagneticalField* lpField = new MagneticalField;
	m_lpPhysics->AddMagneticalField(lpField);

	// Read the Features 
	WCHAR buffer[256];

	fgetws(buffer, 255, Datei);
	if(!IsString1in2(STRING_BRECKET_OPEN, buffer,255)) return;

	while(!feof(Datei))   {
		fgetws(buffer, 255, Datei);
		if(IsString1in2(STRING_NAME, buffer,255)) {
			WCHAR text[256];
			GetString(buffer, text, sizeof(STRING_NAME) - 5 , 255);
			memcpy(lpField->Name, text, sizeof(lpField->Name));
		}
		else if(IsString1in2(STRING_POSITION, buffer,255)) {
			GetVector(buffer, &(lpField->Position), 10, 255);
		}
		else if(IsString1in2(STRING_VIEW_AT, buffer,255)) {
			GetVector(buffer, &(lpField->ViewAt), 10, 255);
		}
		else if(IsString1in2(STRING_VECTOR, buffer,255)) {
			GetVector(buffer, &(lpField->Vector), 10, 255);
		}
		else if(IsString1in2(STRING_SELECTED, buffer,255)) {
			GetBool(buffer, &(lpField->bSelected), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_VECTOR, buffer,255)) {
			GetBool(buffer, &(lpField->bShowVector), 10, 255);
		}
		else if(IsString1in2(STRING_BRECKET_CLOSE, buffer,255)) return;
	}
}

void CSaveLoad::AddGravityField(){
	GravityField* lpField = new GravityField;
	m_lpPhysics->AddGravityField(lpField);

	// Read the Features 
	WCHAR buffer[256];

	fgetws(buffer, 255, Datei);
	if(!IsString1in2(STRING_BRECKET_OPEN, buffer,255)) return;

	while(!feof(Datei))   {
		fgetws(buffer, 255, Datei);
		if(IsString1in2(STRING_NAME, buffer,255)) {
			WCHAR text[256];
			GetString(buffer, text, sizeof(STRING_NAME) - 5 , 255);
			memcpy(lpField->Name, text, sizeof(lpField->Name));
		}
		else if(IsString1in2(STRING_POSITION, buffer,255)) {
			GetVector(buffer, &(lpField->Position), 10, 255);
		}
		else if(IsString1in2(STRING_VIEW_AT, buffer,255)) {
			GetVector(buffer, &(lpField->ViewAt), 10, 255);
		}
		else if(IsString1in2(STRING_VECTOR, buffer,255)) {
			GetVector(buffer, &(lpField->Vector), 10, 255);
		}
		else if(IsString1in2(STRING_SELECTED, buffer,255)) {
			GetBool(buffer, &(lpField->bSelected), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_VECTOR, buffer,255)) {
			GetBool(buffer, &(lpField->bShowVector), 10, 255);
		}
		else if(IsString1in2(STRING_BRECKET_CLOSE, buffer,255)) return;
	}
}

void CSaveLoad::AddEmitter() {
	Emitter* lpEmitter = new Emitter;
	m_lpPhysics->AddEmitter(lpEmitter);

	// Read the Features 
	WCHAR buffer[256];

	fgetws(buffer, 255, Datei);
	if(!IsString1in2(STRING_BRECKET_OPEN, buffer,255)) return;

	while(!feof(Datei))   {
		fgetws(buffer, 255, Datei);
		if(IsString1in2(STRING_NAME, buffer,255)) {
			WCHAR text[256];
			GetString(buffer, text, sizeof(STRING_NAME) - 5 , 255);
			memcpy(lpEmitter->Name, text, sizeof(lpEmitter->Name));
		}
		else if(IsString1in2(STRING_POSITION, buffer,255)) {
			GetVector(buffer, &(lpEmitter->Position), 10, 255);
		}
		else if(IsString1in2(STRING_VIEW_AT, buffer,255)) {
			GetVector(buffer, &(lpEmitter->ViewAt), 10, 255);
		}
		else if(IsString1in2(STRING_VECTOR, buffer,255)) {
			GetVector(buffer, &(lpEmitter->Vector), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_VECTOR, buffer,255)) {
			GetBool(buffer, &(lpEmitter->bShowVector), 10, 255);
		}
		else if(IsString1in2(STRING_WAIT_TIME, buffer,255)) {
			GetUInt(buffer, &(lpEmitter->waitTime), 10, 255);
		}
		else if(IsString1in2(STRING_ELEMENT_TYPE, buffer,255)) {
			GetUInt(buffer, &(lpEmitter->elementType), 10, 255);
		}

		if(IsString1in2(STRING_BRECKET_CLOSE, buffer,255)) return;

	}
}

void CSaveLoad::AddDefaultCuboid() {

	DefaultCuboid* lpDefaultCuboid = new DefaultCuboid;
	//DefaultCuboid* lpDefaultCuboid = m_lpScene->AddCuboid();

	
	// Read the Features 
	WCHAR buffer[256];

	fgetws(buffer, 255, Datei);
	if(!IsString1in2(STRING_BRECKET_OPEN, buffer,255)) return;

	UINT id = 0;

	while(!feof(Datei))   {
		fgetws(buffer, 255, Datei);
		if(IsString1in2(STRING_NAME, buffer,255)) {
			WCHAR text[256];
			GetString(buffer, text, sizeof(STRING_NAME) - 5 , 255);
			memcpy(lpDefaultCuboid->Name, text, sizeof(lpDefaultCuboid->Name));
		}
		else if(IsString1in2(STRING_POSITION, buffer,255)) {
			GetVector(buffer, &(lpDefaultCuboid->Position), 10, 255);
		}
		else if(IsString1in2(STRING_SPEED, buffer,255)) {
			GetVector(buffer, &(lpDefaultCuboid->Speed), 10, 255);
		}
		else if(IsString1in2(STRING_SPEEDING_UP, buffer,255)) {
			GetVector(buffer, &(lpDefaultCuboid->SpeedingUp), 10, 255);
		}
		else if(IsString1in2(STRING_ROTATION, buffer,255)) {
			GetVector(buffer, &(lpDefaultCuboid->rPosition), 10, 255);
		}
		else if(IsString1in2(STRING_ROTATION_SPEED, buffer,255)) {
			GetVector(buffer, &(lpDefaultCuboid->rSpeed), 10, 255);
		}
		else if(IsString1in2(STRING_ROTATION_SPEEDING_UP, buffer,255)) {
			GetVector(buffer, &(lpDefaultCuboid->rSpeedingUp), 10, 255);
		}
		else if(IsString1in2(STRING_SIZE, buffer,255)) {
			GetVector(buffer, &(lpDefaultCuboid->Size), 10, 255);
		}
		else if(IsString1in2(STRING_DRIVEN, buffer,255)) {
			GetBool(buffer, &(lpDefaultCuboid->bDriven), 10, 255);
		}
		else if(IsString1in2(STRING_DRIVER, buffer,255)) {
			GetBool(buffer, &(lpDefaultCuboid->bDriver), 10, 255);
		}
		else if(IsString1in2(STRING_SELECTED, buffer,255)) {
			GetBool(buffer, &(lpDefaultCuboid->bSelected), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_CHARGES, buffer,255)) {
			GetBool(buffer, &(lpDefaultCuboid->bShowCharges), 10, 255);
		}
		else if(IsString1in2(STRING_NUM_PARTS, buffer,255)) {
			GetUInt(buffer, &(lpDefaultCuboid->NumParticies), 10, 255);
			lpDefaultCuboid->lpDefaultCharge = new DefaultCharge[lpDefaultCuboid->NumParticies];
		}
		else if(IsString1in2(STRING_MASS, buffer,255)) {
			GetFloat(buffer, &(lpDefaultCuboid->Mass), 10, 255);
		}
		else if(IsString1in2(STRING_CHARGES, buffer,255)) {
			GetFloat(buffer, &(lpDefaultCuboid->Coulomb), 10, 255);
		}

		else if(IsString1in2(STRING_VIRTUAL_CHARGE, buffer,255)) {
			
			while(!feof(Datei)) {
				fgetws(buffer, 255, Datei);

				if(IsString1in2(STRING_ID, buffer,255)) {
					GetUInt(buffer, &id, 10, 255);
				}					
				else if(IsString1in2(STRING_POSITION, buffer,255)) {
					GetVector(buffer, &(lpDefaultCuboid->lpDefaultCharge[id].Position), 10, 255);
				}
				else if(IsString1in2(STRING_SPEED, buffer,255)) {
					GetVector(buffer, &(lpDefaultCuboid->lpDefaultCharge[id].Speed), 10, 255);
				}
				else if(IsString1in2(STRING_SPEEDING_UP, buffer,255)) {
					GetVector(buffer, &(lpDefaultCuboid->lpDefaultCharge[id].SpeedingUp), 10, 255);
				}
				else if(IsString1in2(STRING_CHARGES, buffer,255)) {
					GetFloat(buffer, &(lpDefaultCuboid->lpDefaultCharge[id].Coulomb), 10, 255);
				}
		
				if(IsString1in2(STRING_BRECKET_CLOSE, buffer,255)) {
					break;
				}
			}
			id++;

		}

		else if(IsString1in2(STRING_BRECKET_CLOSE, buffer,255)) {
				m_lpScene->AddCuboid(lpDefaultCuboid);
				//m_lpPhysics->SetDefaultCuboidCharges(lpDefaultCuboid);
			return;
		}
	}


}

void CSaveLoad::AddDefaultSphere() {
	DefaultSphere* lpDefaultSphere = new DefaultSphere;
	//DefaultCuboid* lpDefaultCuboid = m_lpScene->AddCuboid();

	
	// Read the Features 
	WCHAR buffer[256];

	fgetws(buffer, 255, Datei);
	if(!IsString1in2(STRING_BRECKET_OPEN, buffer,255)) return;

	while(!feof(Datei))   {
		fgetws(buffer, 255, Datei);
		if(IsString1in2(STRING_NAME, buffer,255)) {
			WCHAR text[256];
			GetString(buffer, text, sizeof(STRING_NAME) - 5 , 255);
			memcpy(lpDefaultSphere->Name, text, sizeof(lpDefaultSphere->Name));
		}
		else if(IsString1in2(STRING_POSITION, buffer,255)) {
			GetVector(buffer, &(lpDefaultSphere->Position), 10, 255);
		}
		else if(IsString1in2(STRING_SPEED, buffer,255)) {
			GetVector(buffer, &(lpDefaultSphere->Speed), 10, 255);
		}
		else if(IsString1in2(STRING_SPEEDING_UP, buffer,255)) {
			GetVector(buffer, &(lpDefaultSphere->SpeedingUp), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_SPEED, buffer,255)) {
			GetBool(buffer, &(lpDefaultSphere->bShowSpeed), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_SPEEDING_UP, buffer,255)) {
			GetBool(buffer, &(lpDefaultSphere->bShowSpeedup), 10, 255);
		}
		else if(IsString1in2(STRING_SELECTED, buffer,255)) {
			GetBool(buffer, &(lpDefaultSphere->bSelected), 10, 255);
		}
		else if(IsString1in2(STRING_DRIVEN, buffer,255)) {
			GetBool(buffer, &(lpDefaultSphere->bDriven), 10, 255);
		}
		else if(IsString1in2(STRING_DRIVER, buffer,255)) {
			GetBool(buffer, &(lpDefaultSphere->bDriver), 10, 255);
		}
		else if(IsString1in2(STRING_SHOW_TRACE, buffer,255)) {
			GetBool(buffer, &(lpDefaultSphere->bTrace), 10, 255);
			//if(lpDefaultSphere->bTrace && m_lpScene) m_lpScene->AddTraceToSelected(lpProtone);
		}
		else if(IsString1in2(STRING_MASS, buffer,255)) {
			GetFloat(buffer, &(lpDefaultSphere->Mass), 10, 255);
		}
		else if(IsString1in2(STRING_CHARGES, buffer,255)) {
			GetFloat(buffer, &(lpDefaultSphere->Coulomb), 10, 255);
		}
		else if(IsString1in2(STRING_RADIUS, buffer,255)) {
			GetFloat(buffer, &(lpDefaultSphere->Radius), 10, 255);
		}

		else if(IsString1in2(STRING_BRECKET_CLOSE, buffer,255)) {
				m_lpScene->AddSphere(lpDefaultSphere);
			return;
		}
	}
}

void CSaveLoad::SaveInMemmory() {
	
}

void CSaveLoad::LoadFromMemmory(){

}

//----------------------------------------------------------------------------------------------------
// CSettingsLoader class
//----------------------------------------------------------------------------------------------------

CSettingsLoader::CSettingsLoader() {

}

CSettingsLoader::~CSettingsLoader() {

}

//----------------------------------------------------------------------------------------------------
// Global functions
//----------------------------------------------------------------------------------------------------

void /*CSaveLoad::*/GetUInt(LPWSTR source, UINT* ui, UINT start, UINT length){
	UINT temp = 0;

	for(UINT i = start; i < length; i++) {
		switch(source[i]) {
			case L'0':
				temp *= 10;
				temp += 0;
				break;
			case L'1':
				temp *= 10;
				temp += 1;
				break;
			case L'2':
				temp *= 10;
				temp += 2;
				break;
			case L'3':
				temp *= 10;
				temp += 3;
				break;
			case L'4':
				temp *= 10;
				temp += 4;
				break;
			case L'5':
				temp *= 10;
				temp += 5;
				break;
			case L'6':
				temp *= 10;
				temp += 6;
				break;
			case L'7':
				temp *= 10;
				temp += 7;
				break;
			case L'8':
				temp *= 10;
				temp += 8;
				break;
			case L'9':
				temp *= 10;
				temp += 9;
				break;
			case L';':
				*ui = temp;
				return;			
			
		}
	}

}
UINT /*CSaveLoad::*/GetUInt(LPWSTR source, UINT start, UINT length){
	UINT temp = 0;

	for(UINT i = start; i < length; i++) {
		switch(source[i]) {
			case L'0':
				temp *= 10;
				temp += 0;
				break;
			case L'1':
				temp *= 10;
				temp += 1;
				break;
			case L'2':
				temp *= 10;
				temp += 2;
				break;
			case L'3':
				temp *= 10;
				temp += 3;
				break;
			case L'4':
				temp *= 10;
				temp += 4;
				break;
			case L'5':
				temp *= 10;
				temp += 5;
				break;
			case L'6':
				temp *= 10;
				temp += 6;
				break;
			case L'7':
				temp *= 10;
				temp += 7;
				break;
			case L'8':
				temp *= 10;
				temp += 8;
				break;
			case L'9':
				temp *= 10;
				temp += 9;
				break;
			case L';':
				return temp;;				
		}
	}
	return  NULL;
}
void /*CSaveLoad::*/GetBool(LPWSTR source, BOOL* b, UINT start, UINT length){
	
	for(UINT i = start; i < length; i++) {
		if(source[i] == L'1') {
			*b = true;
			return;
		} 
		else if(source[i] == L';') {
			*b = false;
			return;
		}
	}
	*b = false;
}

void /*CSaveLoad::*/GetFloat(LPWSTR source, float* lpF, UINT start, UINT length) {
	float f = 0.0f;

	int section = 0;
	float signe = 1.0f;

	for(UINT i = start; i < length; i++) {
		switch(source[i]) {
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
				case L';':
						*lpF = f * signe;
					return;
			
		}
	}
}

void /*CSaveLoad::*/GetVector(LPWSTR source, D3DXVECTOR3* vec, UINT start, UINT length){

	bool bStringStarted = false;
	UINT i;
	int prog = 0;
	
	float f = 0.0f;
	int section = 0;
	float signe = 1.0f;
	
	// first get the x
	for(i = start; i < length; i++) {
		if(!bStringStarted) {
			if(source[i] != L' ' && source[i] != L':') bStringStarted = true;
		}
		if(bStringStarted) {
			if(source[i] == L',' || source[i] == L';' ) {
				switch(prog) {
					case 0:
						vec->x = f * signe;
						f = 0.0f;
						section = 0;
						signe = 1.0f;
						break;
					case 1:
						vec->y = f * signe;
						f = 0.0f;
						section = 0;
						signe = 1.0f;
						break;
					case 2: 
						vec->z = f * signe;
						f = 0.0f;
						section = 0;
						signe = 1.0f;
						break;
					default: 
						return;
						
				}
				prog++;
			} else {
				switch(source[i]) {
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
					
				}
			}
		}
	}

}


void /*CSaveLoad::*/GetString(LPWSTR source, LPWSTR dest, UINT start, UINT length){

	bool bStringStarted = false;
	int destPos = 0;

	for(UINT i = start; i < length; i++) {
		if(!bStringStarted) {
			if(source[i] != L' ' && source[i] != L':') bStringStarted = true;
		}
		if(bStringStarted) {
			if(source[i] == L';') {
				dest[destPos] = L'\0';
				return;
			} else {
				dest[destPos] = source[i];
				dest++;
			}
		}
		
	}
}


BOOL /*CSaveLoad::*/IsString1in2(LPWSTR str1, LPWSTR str2, UINT length) {

	int pos = 0;
	UINT i;
	
	// search a possible beginnng of the string
	for(i = 0; i < length; i++) {
		if(str2[i] == str1[0]) {
			if(IsString1in2(str1, str2, length, i)) return true;
		}
	}


	return false;
}

BOOL /*CSaveLoad::*/IsString1in2(LPWSTR str1, LPWSTR str2, UINT length, int AtPosition) {
	for(UINT i = 0; i < length; i++) {
		if(str2[i + AtPosition] != str1[i]) return false;
		else if(str1[i] == L' ') return true;
	}

	return false;
}

