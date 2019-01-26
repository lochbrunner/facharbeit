#include "Physics.h"

CPhysics::CPhysics(){
	CountProtone = 0;
	CountElectrone = 0;
	CountDefaultSphere = 0;
	CountDefaultCuboid = 0;
	CountElectricalField = 0;
	CountMagneticalField = 0;
	CountGravityField = 0;
	CountEmitter = 0;
	count = 0;

	m_OnStartRace = NULL;
}

CPhysics::~CPhysics(){
	
	ClearLists();

}

Protone* CPhysics::AddProtone() {

	Protone* lpProtone = new Protone;
	ListProtone.push_back(lpProtone);
	StringCchPrintf(lpProtone->Name, sizeof(lpProtone->Name), L"Proton%d", CountProtone);
	lpProtone->tempType = TYPE_PROTONE;
	lpProtone->bDriven = true;
	lpProtone->bDriver = true;
	lpProtone->bShowSpeed = false;
	lpProtone->bShowSpeedup = false;
	lpProtone->bSelected = false;
	lpProtone->bTrace = false;
	lpProtone->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpProtone->Speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpProtone->SpeedingUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CountProtone++;

	return lpProtone;
}

void CPhysics::AddProtone(Protone* lpProtone) {
	lpProtone->tempType = TYPE_PROTONE;
	ListProtone.push_back(lpProtone);
	CountProtone++;
}


Electrone* CPhysics::AddElectrone() {

	Electrone* lpElectrone = new Electrone;
	ListElectrone.push_back(lpElectrone);
	StringCchPrintf(lpElectrone->Name, sizeof(lpElectrone->Name), L"Elektron%d", CountElectrone);
	lpElectrone->tempType = TYPE_ELECTRONE;
	lpElectrone->bDriven = true;
	lpElectrone->bDriver = true;
	lpElectrone->bShowSpeed = false;
	lpElectrone->bShowSpeedup = false;
	lpElectrone->bSelected = false;
	lpElectrone->bTrace = false;
	lpElectrone->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpElectrone->Speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpElectrone->SpeedingUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CountElectrone++;

	return lpElectrone;
}

void CPhysics::AddElectrone(Electrone* lpElectrone) {
	lpElectrone->tempType = TYPE_ELECTRONE;
	ListElectrone.push_back(lpElectrone);
	CountElectrone++;
}

DefaultSphere* CPhysics::AddDefaultSphere(){

	DefaultSphere* lpDefaultSphere = new DefaultSphere;
	ListDefaultSphere.push_back(lpDefaultSphere);
	StringCchPrintf(lpDefaultSphere->Name, sizeof(lpDefaultSphere->Name), L"DefaultSphere%d", CountDefaultSphere);
	lpDefaultSphere->tempType = TYPE_DEFAULT_SPHERE;
	lpDefaultSphere->bDriven = true;
	lpDefaultSphere->bDriver = true;
	lpDefaultSphere->bShowSpeed = false;
	lpDefaultSphere->bShowSpeedup = false;
	lpDefaultSphere->bSelected = false;
	lpDefaultSphere->bTrace = false;
	lpDefaultSphere->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpDefaultSphere->Speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpDefaultSphere->SpeedingUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpDefaultSphere->Radius = 1.0f;
	lpDefaultSphere->Coulomb = 1.0f;
	lpDefaultSphere->Mass = 1.0f;
	CountDefaultSphere++;

	return lpDefaultSphere;
}

void CPhysics::AddDefaultSphere(DefaultSphere* lpDefaultSphere) {
	lpDefaultSphere->tempType = TYPE_DEFAULT_SPHERE;
	ListDefaultSphere.push_back(lpDefaultSphere);
	CountDefaultSphere++;
}

DefaultCuboid* CPhysics::AddDefaultCuboid() {

	DefaultCuboid* lpDefaultCuboid = new DefaultCuboid;
	ListDefaultCuboid.push_back(lpDefaultCuboid);
	StringCchPrintf(lpDefaultCuboid->Name, sizeof(lpDefaultCuboid->Name), L"DefaultCuboid%d", CountDefaultCuboid);
	lpDefaultCuboid->tempType = TYPE_DEFAULT_CUBOID;
	lpDefaultCuboid->bShowCharges = false;
	lpDefaultCuboid->bDriven = true;
	lpDefaultCuboid->bDriver = true;
	lpDefaultCuboid->bKill = false;
	lpDefaultCuboid->bSelected = false;
	lpDefaultCuboid->bTrace = false;
	lpDefaultCuboid->Coulomb = 1.0f;
	lpDefaultCuboid->Mass = 1.0f;
	lpDefaultCuboid->NumParticies = 2;
	lpDefaultCuboid->Size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	lpDefaultCuboid->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpDefaultCuboid->Speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpDefaultCuboid->SpeedingUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpDefaultCuboid->rPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpDefaultCuboid->rSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpDefaultCuboid->rSpeedingUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CountDefaultCuboid++;

	SetDefaultCuboidCharges(lpDefaultCuboid, 0);

	return lpDefaultCuboid;
}

void CPhysics::SetDefaultCuboidCharges(DefaultCuboid* lpDefaultCuboid, DWORD Flag) {

	if(Flag == PHSYSICS_FLAG_ONLY_CHARGES) {
		for(int i = 0; i < lpDefaultCuboid->NumParticies; i++) {
			lpDefaultCuboid->lpDefaultCharge[i].Coulomb = lpDefaultCuboid->Coulomb / lpDefaultCuboid->NumParticies;
		}
		return;
	}

	delete[] lpDefaultCuboid->lpDefaultCharge;
	
	//			Create the Charges
	lpDefaultCuboid->lpDefaultCharge = new DefaultCharge[lpDefaultCuboid->NumParticies];

	D3DXVECTOR3 v;

	//			Give the Charges a random position
	for(int i = 0; i < lpDefaultCuboid->NumParticies; i++) {
		lpDefaultCuboid->lpDefaultCharge[i].Coulomb = lpDefaultCuboid->Coulomb / lpDefaultCuboid->NumParticies;
		
		v.x = lpDefaultCuboid->Position.x - (lpDefaultCuboid->Size.x / 2.0f ) + ( lpDefaultCuboid->Size.x * (static_cast<float>(rand() % 100) / 100.0f));
		v.y = lpDefaultCuboid->Position.y - (lpDefaultCuboid->Size.y / 2.0f ) + ( lpDefaultCuboid->Size.y * (static_cast<float>(rand() % 100) / 100.0f));
		v.z = lpDefaultCuboid->Position.z - (lpDefaultCuboid->Size.z / 2.0f ) + ( lpDefaultCuboid->Size.z * (static_cast<float>(rand() % 100) / 100.0f));

		lpDefaultCuboid->lpDefaultCharge[i].Position = v;

	}

	//			Balance the Charges

	BOOL ready = false;
	long temp = 0;

	long waitTimeToRender = 100000 / lpDefaultCuboid->NumParticies / lpDefaultCuboid->NumParticies;

	D3DXVECTOR3 maxPos = /*lpDefaultCuboid->Position + */(lpDefaultCuboid->Size * 0.5f);
	D3DXVECTOR3 minPos = /*lpDefaultCuboid->Position - */-(lpDefaultCuboid->Size * 0.5f);

	while(!ready) {
		ready = true;
		temp++;
		
		temp = temp % waitTimeToRender;
		if(temp == 0) m_Render();

		for(int i = 0; i < lpDefaultCuboid->NumParticies; i++) {

			lpDefaultCuboid->lpDefaultCharge[i].SpeedingUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			for(int j = 0; j < lpDefaultCuboid->NumParticies; j++) {
				if(j != i) {
					lpDefaultCuboid->lpDefaultCharge[i].SpeedingUp += ForceElectrical(lpDefaultCuboid->lpDefaultCharge[i].Position,
																				lpDefaultCuboid->lpDefaultCharge[j].Position);
				}
			}

			lpDefaultCuboid->lpDefaultCharge[i].Speed += lpDefaultCuboid->lpDefaultCharge[i].SpeedingUp *= 0.3f;
			lpDefaultCuboid->lpDefaultCharge[i].Speed *= 0.2f;
			lpDefaultCuboid->lpDefaultCharge[i].Position += lpDefaultCuboid->lpDefaultCharge[i].Speed;


			if(lpDefaultCuboid->lpDefaultCharge[i].Position.x < minPos.x){
				lpDefaultCuboid->lpDefaultCharge[i].Position.x = minPos.x;
				lpDefaultCuboid->lpDefaultCharge[i].Speed.x = 0.0f;
			}
			if(lpDefaultCuboid->lpDefaultCharge[i].Position.y < minPos.y){
				lpDefaultCuboid->lpDefaultCharge[i].Position.y = minPos.y;
				lpDefaultCuboid->lpDefaultCharge[i].Speed.y = 0.0f;
			}
			if(lpDefaultCuboid->lpDefaultCharge[i].Position.z < minPos.z){
				lpDefaultCuboid->lpDefaultCharge[i].Position.z = minPos.z;
				lpDefaultCuboid->lpDefaultCharge[i].Speed.z = 0.0f;
			}
			if(lpDefaultCuboid->lpDefaultCharge[i].Position.x > maxPos.x){
				lpDefaultCuboid->lpDefaultCharge[i].Position.x = maxPos.x;
				lpDefaultCuboid->lpDefaultCharge[i].Speed.x = 0.0f;
			}
			if(lpDefaultCuboid->lpDefaultCharge[i].Position.y > maxPos.y){
				lpDefaultCuboid->lpDefaultCharge[i].Position.y = maxPos.y;
				lpDefaultCuboid->lpDefaultCharge[i].Speed.y = 0.0f;
			}
			if(lpDefaultCuboid->lpDefaultCharge[i].Position.z > maxPos.z){
				lpDefaultCuboid->lpDefaultCharge[i].Position.z = maxPos.z;
				lpDefaultCuboid->lpDefaultCharge[i].Speed.z = 0.0f;
			}

			ready = ready && (abs(lpDefaultCuboid->lpDefaultCharge[i].Speed.x) < 0.00004f &&
					abs(lpDefaultCuboid->lpDefaultCharge[i].Speed.y) < 0.00004f &&
					abs(lpDefaultCuboid->lpDefaultCharge[i].Speed.z) < 0.00004f );

		}

	}

}


void CPhysics::AddDefaultCuboid(DefaultCuboid *lpDefaultCuboid) {
	lpDefaultCuboid->tempType = TYPE_DEFAULT_CUBOID;
	ListDefaultCuboid.push_back(lpDefaultCuboid);
	CountDefaultCuboid++;
}

ElectricalField* CPhysics::AddElectricalField() {
	ElectricalField* lpElectricalField = new ElectricalField;
	ListElectricalField.push_back(lpElectricalField);
	StringCchPrintf(lpElectricalField->Name, sizeof(lpElectricalField->Name), L"ElectricalField%d", CountElectricalField);
	lpElectricalField->tempType = TYPE_FIELD_ELECTRICAL;
	lpElectricalField->bSelected = false;
	lpElectricalField->bShowVector = false;
	lpElectricalField->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpElectricalField->Vector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpElectricalField->ViewAt = D3DXVECTOR3(0.00001f, 1.0f, 0.00001f);

	CountElectricalField++;

	return lpElectricalField;
}

void CPhysics::AddElectricalField(ElectricalField *lpElectricalField) {
	lpElectricalField->tempType = TYPE_FIELD_ELECTRICAL;
	ListElectricalField.push_back(lpElectricalField);
	CountElectricalField++;
}

MagneticalField* CPhysics::AddMagneticalField() {
	MagneticalField* lpMagneticalField = new MagneticalField;
	ListMagneticalField.push_back(lpMagneticalField);
	StringCchPrintf(lpMagneticalField->Name, sizeof(lpMagneticalField->Name), L"MagneticalField%d", CountMagneticalField);
	lpMagneticalField->tempType = TYPE_FIELD_MAGNETICAL;
	lpMagneticalField->bSelected = false;
	lpMagneticalField->bShowVector = false;
	lpMagneticalField->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpMagneticalField->Vector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpMagneticalField->ViewAt = D3DXVECTOR3(0.00001f, 1.0f, 0.00001f);

	CountMagneticalField++;

	return lpMagneticalField;
}

void CPhysics::AddMagneticalField(MagneticalField *lpMagneticalField) {
	lpMagneticalField->tempType = TYPE_FIELD_MAGNETICAL;
	ListMagneticalField.push_back(lpMagneticalField);
	CountMagneticalField++;
}

GravityField* CPhysics::AddGravityField() {
	GravityField* lpGravityField = new GravityField;
	ListGravityField.push_back(lpGravityField);
	StringCchPrintf(lpGravityField->Name, sizeof(lpGravityField->Name), L"GravityField%d", CountGravityField);
	lpGravityField->tempType = TYPE_FIELD_GRAVITY;	
	lpGravityField->bSelected = false;
	lpGravityField->bShowVector = false;
	lpGravityField->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpGravityField->Vector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpGravityField->ViewAt = D3DXVECTOR3(0.00001f, 1.0f, 0.000001f);

	CountGravityField++;

	return lpGravityField;
}

void CPhysics::AddGravityField(GravityField *lpGravityField) {
	lpGravityField->tempType = TYPE_FIELD_GRAVITY;
	ListGravityField.push_back(lpGravityField);
	CountGravityField++;
}

Emitter* CPhysics::AddEmitter() {
	Emitter* lpEmitter = new Emitter;
	ListEmitter.push_back(lpEmitter);
	StringCchPrintf(lpEmitter->Name, sizeof(lpEmitter->Name), L"Emitter%d", CountEmitter);
	lpEmitter->tempType = TYPE_OBJECT_EMITTER;
	lpEmitter->bSelected = false;
	lpEmitter->bShowVector = false;
	lpEmitter->waitTime = 100;
	lpEmitter->elementType = TYPE_ELECTRONE;
	lpEmitter->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lpEmitter->Vector = D3DXVECTOR3(1.00001f, 0.0f, 0.0f);
	lpEmitter->ViewAt = D3DXVECTOR3(1.00001f, 0.0f, 0.0f);
	lpEmitter->Random = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CountEmitter++;
	
	return lpEmitter;
}

void CPhysics::AddEmitter(Emitter* lpEmitter) {
	lpEmitter->tempType = TYPE_OBJECT_EMITTER;
	ListEmitter.push_back(lpEmitter);

	CountEmitter++;
}



void CPhysics::ClearLists(void){
	list<Protone*>::iterator itP;

	for(itP = ListProtone.begin(); itP!= ListProtone.end(); itP++ ) {
		delete *itP;
	}

	ListProtone.clear();


	list<Electrone*>::iterator itE;

	for(itE = ListElectrone.begin(); itE!= ListElectrone.end(); itE++ ) {
		delete *itE;
	}

	ListElectrone.clear();


	list<DefaultSphere*>::iterator itDS;

	for(itDS = ListDefaultSphere.begin(); itDS!= ListDefaultSphere.end(); itDS++ ) {
		delete *itDS;
	}

	ListDefaultSphere.clear();


	list<ElectricalField*>::iterator itEF;

	for(itEF = ListElectricalField.begin(); itEF!= ListElectricalField.end(); itEF++ ) {
		delete *itEF;
	}

	ListElectricalField.clear();


	list<MagneticalField*>::iterator itMF;

	for(itMF = ListMagneticalField.begin(); itMF!= ListMagneticalField.end(); itMF++ ) {
		delete *itMF;
	}

	ListMagneticalField.clear();


	list<GravityField*>::iterator itGF;

	for(itGF = ListGravityField.begin(); itGF!= ListGravityField.end(); itGF++ ) {
		delete *itGF;
	}

	ListGravityField.clear();


	list<Emitter*>::iterator itOE;

	for(itOE = ListEmitter.begin(); itOE!= ListEmitter.end(); itOE++ ) {
		delete *itOE;
	}

	ListEmitter.clear();

	
	list<DefaultCuboid*>::iterator itDC;

	for(itDC = ListDefaultCuboid.begin(); itDC != ListDefaultCuboid.end(); itDC++) {
		delete[] (*itDC)->lpDefaultCharge;
		delete *itDC;
	}

	ListDefaultCuboid.clear();

}

void CPhysics::DeleteSelected(void){
	list<Protone*>::iterator itP;
	list<Protone*>tempP;
	
	for(itP = ListProtone.begin(); itP!= ListProtone.end(); itP++ ) {
		if((*itP)->bSelected) {
			delete *itP;
			*itP = NULL;
		} else {
			tempP.push_back(*itP);
		}
	}

	ListProtone = tempP;

	
	list<Electrone*>::iterator itE;
	list<Electrone*>tempE;

	for(itE = ListElectrone.begin(); itE!= ListElectrone.end(); itE++ ) {
		if((*itE)->bSelected) {
			//ListElectrone.erase(itE);
			delete *itE;
			*itE = NULL;
		} else {
			tempE.push_back(*itE);
		}
	}

	ListElectrone = tempE;


	list<DefaultSphere*>::iterator itDS;
	list<DefaultSphere*>tempDS;

	for(itDS = ListDefaultSphere.begin(); itDS!= ListDefaultSphere.end(); itDS++ ) {
		if((*itDS)->bSelected) {
			//ListDefaultSphere.erase(itDS);
			delete *itDS;
			*itDS = NULL;
		} else {
			tempDS.push_back(*itDS);
		}
	}

	ListDefaultSphere = tempDS;


	list<DefaultCuboid*>::iterator itDC;
	list<DefaultCuboid*>tempDC;

	for(itDC = ListDefaultCuboid.begin(); itDC!= ListDefaultCuboid.end(); itDC++ ) {
		if((*itDC)->bSelected) {
			//ListDefaultSphere.erase(itDS);
			delete[] (*itDC)->lpDefaultCharge;
			delete *itDC;
			*itDC = NULL;
		} else {
			tempDC.push_back(*itDC);
		}
	}

	ListDefaultCuboid = tempDC;


	list<ElectricalField*>::iterator itEF;
	list<ElectricalField*>tempEF;

	for(itEF = ListElectricalField.begin(); itEF!= ListElectricalField.end(); itEF++ ) {
		if((*itEF)->bSelected) {
			//ListElectricalField.erase(itEF);
			delete *itEF;
			*itEF = NULL;
		} else {
			tempEF.push_back(*itEF);
		}
	}

	ListElectricalField = tempEF;


	list<MagneticalField*>::iterator itMF;
	list<MagneticalField*>tempMF;

	for(itMF = ListMagneticalField.begin(); itMF!= ListMagneticalField.end(); itMF++ ) {
		if((*itMF)->bSelected) {
			//ListElectricalField.erase(itMF);
			delete *itMF;
			*itMF = NULL;
		} else {
			tempMF.push_back(*itMF);
		}
	}

	ListMagneticalField = tempMF;


	list<GravityField*>::iterator itGF;
	list<GravityField*>tempGF;

	for(itGF = ListGravityField.begin(); itGF!= ListGravityField.end(); itGF++ ) {
		if((*itGF)->bSelected) {
			//ListElectricalField.erase(itGF);
			delete *itGF;
			*itGF = NULL;
		} else {
			tempGF.push_back(*itGF);
		}
	}

	ListGravityField = tempGF;


	list<Emitter*>::iterator itOE;
	list<Emitter*>tempOE;

	for(itOE = ListEmitter.begin(); itOE!= ListEmitter.end(); itOE++ ) {
		if((*itOE)->bSelected) {
			//ListElectricalField.erase(itOE);
			delete *itOE;
			*itOE = NULL;
		} else {
			tempOE.push_back(*itOE);
		}
	}

	ListEmitter = tempOE;

}

void CPhysics::DeleteSpacial(void) {
	list<Protone*>::iterator itP;
	list<Protone*>tempP;
	
	for(itP = ListProtone.begin(); itP!= ListProtone.end(); itP++ ) {
		if((*itP)->bKill) {
			delete *itP;
			*itP = NULL;
		} else {
			tempP.push_back(*itP);
		}
	}

	ListProtone = tempP;

	
	list<Electrone*>::iterator itE;
	list<Electrone*>tempE;

	for(itE = ListElectrone.begin(); itE!= ListElectrone.end(); itE++ ) {
		if((*itE)->bKill) {
			//ListElectrone.erase(itE);
			delete *itE;
			*itE = NULL;
		} else {
			tempE.push_back(*itE);
		}
	}

	ListElectrone = tempE;
}

void CPhysics::OnRender(float elapsedTime, DWORD rendermode) {

	count++;
	
	list<Protone*>::iterator itP;
	list<Electrone*>::iterator itE;

	list<ElectricalField*>::iterator itEF;
	list<MagneticalField*>::iterator itMF;
	list<GravityField*>::iterator itGF;

	list<Emitter*>::iterator itOE;
	list<DefaultCuboid*>::iterator itDC;
	list<DefaultSphere*>::iterator itDS;

	switch(rendermode) {
		case PHSYSICS_RENDERMODE_NORMAL:
			for(itP = ListProtone.begin(); itP!= ListProtone.end(); itP++ ) {
				(*itP)->Speed += (*itP)->SpeedingUp * 0.0002f;
				(*itP)->Position += (*itP)->Speed;
			}
			
			for(itE = ListElectrone.begin(); itE!= ListElectrone.end(); itE++ ) {
				(*itE)->Speed += (*itE)->SpeedingUp * 0.200f;
				(*itE)->Position += (*itE)->Speed;
			}

			for(itDS = ListDefaultSphere.begin(); itDS!= ListDefaultSphere.end(); itDS++ ) {
				(*itDS)->Speed += (*itDS)->SpeedingUp * 0.200f / (*itDS)->Mass;
				(*itDS)->Position += (*itDS)->Speed;
			}
			break;

case PHSYSICS_RENDERMODE_HEUN:
	for(itP = ListProtone.begin(); itP!= ListProtone.end(); itP++ ) {
		(*itP)->Speed += (*itP)->SpeedingUp * 0.0002f * 0.5f;
		(*itP)->Position += (*itP)->Speed * 0.5f;
	}
	for(itE = ListElectrone.begin(); itE!= ListElectrone.end(); itE++ ) {
		(*itE)->Speed += (*itE)->SpeedingUp * 0.200f * 0.5f;
		(*itE)->Position += (*itE)->Speed * 0.5f;
	}
	...
	break;

	// Berechung der aktuellen Beschleunigungen

case PHSYSICS_RENDERMODE_HEUN:
	for(itP = ListProtone.begin(); itP!= ListProtone.end(); itP++ ) {
		(*itP)->Speed += (*itP)->SpeedingUp * 0.0002f * 0.5f;
		(*itP)->Position += (*itP)->Speed * 0.5f;
	}
	for(itE = ListElectrone.begin(); itE!= ListElectrone.end(); itE++ ) {
		(*itE)->Speed += (*itE)->SpeedingUp * 0.200f * 0.5f;
		(*itE)->Position += (*itE)->Speed * 0.5f;
	}
	...
	break;
	}
}


D3DXVECTOR3 CPhysics::ForceElectrical(D3DXVECTOR3 DrivenPosition, D3DXVECTOR3 DriverPosition){
	D3DXVECTOR3 v;
	float Distance = sqrt((DrivenPosition.x - DriverPosition.x) * (DrivenPosition.x - DriverPosition.x) + 
		(DrivenPosition.y - DriverPosition.y) * (DrivenPosition.y - DriverPosition.y) + 
		(DrivenPosition.z - DriverPosition.z) * (DrivenPosition.z - DriverPosition.z));

	v = (DrivenPosition - DriverPosition) / (Distance * Distance * Distance);

	return v;
}


D3DXVECTOR3 CPhysics::ForceMagnetical(D3DXVECTOR3 DrivenSpeed, D3DXVECTOR3 DriverForce){
	D3DXVECTOR3 v = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	v.x += -DrivenSpeed.z * DriverForce.y;
	v.z += +DrivenSpeed.x * DriverForce.y;

	v.y += +DrivenSpeed.z * DriverForce.x;
	v.z += -DrivenSpeed.y * DriverForce.x;

	v.x += +DrivenSpeed.y * DriverForce.z;
	v.y += -DrivenSpeed.x * DriverForce.z;

	v.x += DrivenSpeed.x * DriverForce.y * DriverForce.y * MAGNETICAL_HOLD;
	v.x += DrivenSpeed.x * DriverForce.z * DriverForce.z * MAGNETICAL_HOLD;
	v.x += DrivenSpeed.x * DriverForce.x * DriverForce.x * MAGNETICAL_HOLD;

	v.y += DrivenSpeed.y * DriverForce.x * DriverForce.x * MAGNETICAL_HOLD;
	v.y += DrivenSpeed.y * DriverForce.z * DriverForce.z * MAGNETICAL_HOLD;
	v.y += DrivenSpeed.y * DriverForce.y * DriverForce.y * MAGNETICAL_HOLD;

	v.z += DrivenSpeed.z * DriverForce.y * DriverForce.y * MAGNETICAL_HOLD;
	v.z += DrivenSpeed.z * DriverForce.x * DriverForce.x * MAGNETICAL_HOLD;
	v.z += DrivenSpeed.z * DriverForce.z * DriverForce.z * MAGNETICAL_HOLD;

	return v;
}

	//float rotY = atan(DrivenSpeed.x / DrivenSpeed.z);
	//float rotX = atan(DrivenSpeed.y / (DrivenSpeed.x * DrivenSpeed.x + DrivenSpeed.z * DrivenSpeed.z));

	//if(DrivenSpeed.z < 0.0f) rotY += PI_F;



	
	
	//float speed = sqrt(DrivenSpeed.x * DrivenSpeed.x + DrivenSpeed.y * DrivenSpeed.y + DrivenSpeed.z * DrivenSpeed.z);

	//// 0 is in straight to the Z
	//float rotY = atan(DrivenSpeed.x / DrivenSpeed.z);

	//if(DrivenSpeed.z < 0.0f) rotY += PI_F;
	//

	////rotY += 2.094395f;   //  = 0.666 * PI;
	////rotY += 1.047197f;   //  = 0.333 * PI;
	//rotY -= 0.1f * DriverForce.y;
	//rotY += PI_2_F;

	//v.x = DriverForce.y * speed * sin(rotY);
	//v.y = 0.0f;
	//v.z = DriverForce.y * speed * cos(rotY);


