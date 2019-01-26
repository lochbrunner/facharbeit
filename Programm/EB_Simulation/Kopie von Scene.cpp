#include "Scene.h"

CScene::CScene() {
	m_lpD3D			= NULL;
    m_lpD3DFont		= NULL;
    m_lpD3DDevice	= NULL;
    m_lpBackBuffer	= NULL;
	hWnd			= NULL;

	bArrowActivated = true;

	CameraPosition = D3DXVECTOR3(20.0f, 10.0f, 20.0f);
	CameraViewAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CameraRotation.x = -atan(10 / sqrt(20.0f * 20.0f + 20.0f * 20.0f));
	CameraRotation.y = PI_F / 4.0f * 5.0f;

	RenderGrid = true;

	Distance = sqrt(20.0f * 20.0f + 20.0f * 20.0f + 10.0f * 10.0f);
}

CScene::~CScene() {
	DeInitD3D();
	map<LPDEFAULTELEMENT, CTrace*>::iterator it;

	for(it = TraceMap.begin(); it != TraceMap.end();it++)
	{
        delete (it->second);
	}
	TraceMap.clear();


	list<CTrace*>::iterator itL;

	for(itL = TraceList.begin(); itL != TraceList.end(); itL++)
	{
        delete (*itL);
	}
	TraceList.clear();

	
	map<DefaultCuboid*, CDefaultCuboid*>::iterator itDC;

	for(itDC = m_CuboidMeshMap.begin(); itDC != m_CuboidMeshMap.end(); itDC++) {
		delete (itDC->second);
	}
	m_CuboidMeshMap.clear();

}

void CScene::SetCPhysicsPtr(CPhysics* lpPhysics) {
	m_lpPhysics = lpPhysics; 

	m_lpListProtone = m_lpPhysics->GetListProtonePtr();
	m_lpListElectrone = m_lpPhysics->GetListElectronePtr();
	m_lpListDefaultSphere = m_lpPhysics->GetListDefaultSpherePtr();
	m_lpListDefaultCuboid = m_lpPhysics->GetListDefaultCuboidPtr();
	m_lpListElectricalField = m_lpPhysics->GetListElectricalFieldPtr();
	m_lpListMagneticalField = m_lpPhysics->GetListMagneticalFieldPtr();
	m_lpListGravityField = m_lpPhysics->GetListGravityFieldPtr();
	m_lpListEmitter = m_lpPhysics->GetListEmitterPtr();

}

void CScene::SetCamera(){
	 

	D3DXVECTOR3 v;

	if(Selected) v = (reinterpret_cast<Electrone*>(Selected))->Position - CameraViewAt;
	else v = -CameraViewAt;

	CameraViewAt += v * 0.125f;

	if((v.x * v.x + v.y * v.y + v.z * v.z) < 0.01f) {

		if(Selected) CameraViewAt = ((Electrone*)Selected)->Position;
		else CameraViewAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		D3DXVECTOR3 v2 = CameraViewAt- CameraPosition;



		CameraRotation.x = atan(v2.y / sqrt(v2.x * v2.x + v2.z * v2.z));


		if(v2.z < 0.0f) {
			CameraRotation.y = atan(v2.x / v2.z) + PI_F;
		}

		else CameraRotation.y = atan(v2.x / v2.z);

		Distance = sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);

		return;
	}

	D3DXMatrixLookAtLH(&m_ViewMatrix, 
                       &CameraPosition, 
                       &CameraViewAt, 
                       &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	m_lpD3DDevice->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_lpD3DDevice->GetLight(0, &m_Light);
	
	m_Light.Position = CameraPosition;
	m_Light.Position.x -= Distance * cos(CameraRotation.y) * 0.25f;
	m_Light.Position.z += Distance * sin(CameraRotation.y) * 0.25f;
	
	
	m_lpD3DDevice->SetLight(0, &m_Light);

	Render();
	
	SetCamera();

}

BOOL CScene::Init(HINSTANCE hInstance, HWND parentHWnd, UINT x, UINT y, UINT width, UINT height, BOOL bWindowed) {

	m_hInstance = hInstance;

	hWnd = CreateWindowEx(NULL,                   // Keine erweiterten Stile nutzen
                          L"WindowClass",           // Klassenname
                          L"Scene",               // Fenstertitel
                          /*WS_OVERLAPPEDWINDOW |*/     // Fenster
						  /*WS_THICKFRAME | */
						  WS_VISIBLE |WS_CHILD,     // Eigenschaften
                          x, y,                   // Anfangsposition
                          width, height,					// und Groesse des Fensters
                          parentHWnd,               // Handle des Elternfensters
                          NULL,						// Handle des Menues
                          hInstance,				// Anwendungsinstanz
                          NULL);					// wird nicht benoetigt

	m_lpLogBook->Succed(L"CreateWindowEx");
	
	//Cursor laden
	

	CursorScale = LoadCursor(m_hInstance, L"CURSOR_8");
	CursorTrans = LoadCursor(m_hInstance, L"CURSOR_12");
	CursorRotate = LoadCursor(m_hInstance, L"CURSOR_ROTATE");

	m_lpLogBook->Succed(L"Cursor geladen");

	// Direct3D-Objekt erzeugen
    m_lpD3D = Direct3DCreate9(D3D_SDK_VERSION);

    if(NULL == m_lpD3D)
    {
        // Fehler, D3D-Objekt wurde nicht erzeugt
		MessageBox(0, L"D3D-Objekt konnte nicht erzeugt werden", L"Fehler", MB_OK);
        return FALSE;
    }

	m_lpLogBook->Succed(L"D3D-Objekt erzeugt");
	
	ZeroMemory(&PParams, sizeof(PParams));

    PParams.SwapEffect       = D3DSWAPEFFECT_DISCARD;
    PParams.hDeviceWindow    = hWnd;
    PParams.Windowed         = bWindowed;

    PParams.BackBufferWidth  = width;
    PParams.BackBufferHeight = height;
    PParams.BackBufferFormat = D3DFMT_A8R8G8B8;

    // Z-Buffer erzeugen
    PParams.AutoDepthStencilFormat = D3DFMT_D16;
    PParams.EnableAutoDepthStencil = TRUE;

	size.x = static_cast<float>(width);
	size.y = static_cast<float>(height);
	
	InitD3D(width , height);
	return TRUE;

    
}

void CScene::InitD3D(UINT width, UINT height) {

	PParams.BackBufferWidth = width;
	PParams.BackBufferHeight = height;

	size.x = static_cast<float>(width);
	size.y = static_cast<float>(height);
	
	HRESULT hr;

    // Direct3D-Geraet anlegen
    if(FAILED(hr = m_lpD3D->CreateDevice(
                                 D3DADAPTER_DEFAULT, 
                                 D3DDEVTYPE_HAL, 
                                 hWnd, 
								 D3DCREATE_HARDWARE_VERTEXPROCESSING, 
                                 &PParams, 
                                 &m_lpD3DDevice)))
    {	
		if(FAILED(hr = m_lpD3D->CreateDevice(
                                 D3DADAPTER_DEFAULT, 
                                 D3DDEVTYPE_HAL, 
                                 hWnd, 
                                 D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
                                 &PParams, 
                                 &m_lpD3DDevice)))
		{
			// Fehler, Geraet kann nicht angelegt werden
			const WCHAR* Err = DXGetErrorDescription9(hr);

			DXTRACE_MSG(Err);

			m_lpLogBook->Error(L"CreateDevice", hr, 0);

			MessageBox(0, L"DirectX Device konnte nicht erstellt werden", L"Fehler", MB_OK);

			return ;
		}
		m_lpLogBook->Succed(L"DirectX Device erzeugt", L"SOFTWARE_VERTEXPROCESSING");
    }
	
	m_lpLogBook->Succed(L"DirectX Device erzeugt");

	// Schrift erzeugen
    CreateFont();

	m_lpLogBook->Succed(L"Font erzeugt");

    // Zeiger auf dem Hintergrundpuffer holen
    m_lpD3DDevice->GetBackBuffer(0, 0, 
                                 D3DBACKBUFFER_TYPE_MONO, 
                                 &m_lpBackBuffer);

	m_lpLogBook->Succed(L"Zeiger auf den Backbuffer geholt");


    // Aktivieren des Z-Buffers
    m_lpD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// Das Vertex-Format festlegen
    m_lpD3DDevice->SetFVF(D3D_CUSTOMVERTEX);

    //CameraPosition
	
    D3DXMatrixLookAtLH(&m_ViewMatrix, 
                       &CameraPosition, 
                       &CameraViewAt, 
                       &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

    m_lpD3DDevice->SetTransform(D3DTS_VIEW, &m_ViewMatrix);

     //Sichtkegel definieren

	if(true) { 
		D3DXMatrixPerspectiveFovLH(&ProjMatrix, 
                               D3DX_PI/4, 
							   static_cast<float>(width) / static_cast<float>(height), 
                               1.0f, 
                               1000.0f );
	} else {

		D3DXMatrixOrthoLH(
			&ProjMatrix,
			100,
			70,
			1.0f,
			2000.0f
		);
	}

    m_lpD3DDevice->SetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_lpLogBook->Succed(L"D3D-Device eingerichtet");

    ZeroMemory(&m_Light, sizeof(D3DLIGHT9));

    // bei der Lichtquelle soll es sich um eine
    // Punktlichtquelle handeln
    m_Light.Type = D3DLIGHT_POINT;

    // die Lichtfarbe wird auf Weiss gesetzt
    m_Light.Diffuse.r = 1.5f;
    m_Light.Diffuse.g = 1.5f;
    m_Light.Diffuse.b = 1.5f;

    // das Licht soll eine Reichweite von 1000 haben
    m_Light.Range = 1000.0f;

    // das Licht wird an der gleichen Stelle positio-
    // niert, an der sich auch unsere Kamera befindet
    m_Light.Position = CameraPosition;

	m_Light.Position.x -= Distance * cos(CameraRotation.y) * 0.25f;
	m_Light.Position.z += Distance * sin(CameraRotation.y) * 0.25f;

    // das Element Attenuation0 bestimmt, wie stark die
    // Lichtstaerke mit der Entfernung abfaellt
    // 1.0 bedeutet dabei keinen Abfall der Lichtstaerke
    m_Light.Attenuation0 = 1.0f;

    // das definierte Licht bekommt die Nummer 0
    m_lpD3DDevice->SetLight(0, &m_Light);

    // Lichtquelle anschalten
    m_lpD3DDevice->LightEnable(0, TRUE);

	m_lpLogBook->Succed(L"Licht hinzugefügt");

	lpResmanager = new CResourceManager;
	lpResmanager->Init(m_lpD3DDevice);
	
	Grid.SetLogBookPtr(m_lpLogBook);
	Grid.SetD3Device(m_lpD3DDevice);
	Grid.Init();

	m_lpLogBook->Succed(L"Grid-Klasse initialisiert");
	
	TranslateTool.SetLogBookPtr(m_lpLogBook);
	TranslateTool.SetD3Device(m_lpD3DDevice);
	TranslateTool.SetResourceManagerPtr(lpResmanager);
	TranslateTool.Init();

	m_lpLogBook->Succed(L"TranslateTool-Klasse initialisiert");

	ScaleTool.SetLogBookPtr(m_lpLogBook);
	ScaleTool.SetD3Device(m_lpD3DDevice);
	ScaleTool.SetResourceManagerPtr(lpResmanager);
	ScaleTool.Init();

	m_lpLogBook->Succed(L"ScaleTool-Klasse initialisiert");
	
	VectorArrow.SetLogBookPtr(m_lpLogBook);
	VectorArrow.SetD3Device(m_lpD3DDevice);
	VectorArrow.SetResourceManagerPtr(lpResmanager);
	VectorArrow.Init();

	m_lpLogBook->Succed(L"VectorArrow-Klasse initialisiert");
	
	FieldVector.SetLogBookPtr(m_lpLogBook);
	FieldVector.SetD3Device(m_lpD3DDevice);
	FieldVector.SetResourceManagerPtr(lpResmanager);
	FieldVector.Init();

	m_lpLogBook->Succed(L"FieldVector-Klasse initialisiert");

	EmitterObject.SetLogBookPtr(m_lpLogBook);
	EmitterObject.SetD3Device(m_lpD3DDevice);
	EmitterObject.SetResourceManagerPtr(lpResmanager);
	EmitterObject.SetPosition(D3DXVECTOR3(0.0f, 0.0f ,0.0f));
	EmitterObject.SetRotation(D3DXVECTOR3(0.0f, 0.0f ,0.0f));
	EmitterObject.Init();

	m_lpLogBook->Succed(L"Emitter-Klasse initialisiert");

	lpD3Object = new C3DObject;
	lpD3Object->SetLogBookPtr(m_lpLogBook);
	lpD3Object->Init(m_lpD3DDevice, lpResmanager);
	lpD3Object->Load(L"../data/sphere_high.x");
	lpD3Object->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -4.0f));
	lpD3Object->SetRotation(0.0f, 0.0f, 0.0f);

	m_lpLogBook->Succed(L"Eine C3DObject-Instance erzeugt und initialisiert", L"../data/sphere_high.x");

}

void CScene::DeInitD3D() {
	// testen, ob die Objekt angelegt
    // wurden und ggf. freigeben

	m_lpLogBook->SubTitle(L"Scene-Klasse wird beendet");

	DeleteFont();

	m_lpLogBook->Succed(L"Font wurde freigegeben");
	
	if(NULL != lpD3Object)
    {	
		delete lpD3Object;
        lpD3Object = NULL;
    }

	m_lpLogBook->Succed(L"D3DObjekt wurde freigegeben");

	if(NULL != lpResmanager)
    {
        delete lpResmanager;
		m_lpLogBook->Succed(L"ResoucenManager wurde beendet");
        lpResmanager = NULL;
    }

	
	TranslateTool.DeInit();
	m_lpLogBook->Succed(L"TranslateTool-Klasse wurde beendet");

	VectorArrow.DeInit();
	m_lpLogBook->Succed(L"VectorArrow-Klasse wurde beendet");

	Grid.DeInit();
	m_lpLogBook->Succed(L"Grid-Klasse wurde beendet");

	FieldVector.DeInit();
	m_lpLogBook->Succed(L"FieldVector-Klasse wurde beendet");

	
	if(NULL != m_lpBackBuffer)
    {
        m_lpBackBuffer->Release();
        m_lpBackBuffer = NULL;
    }

	m_lpLogBook->Succed(L"Backbuffer wurde freigegeben");

    if(NULL != m_lpD3DDevice)
    {
        m_lpD3DDevice->Release();
        m_lpD3DDevice = NULL;
    }

	m_lpLogBook->Succed(L"D3D.Device wurde freigegeben");

	if(NULL != m_lpD3D)
    {
        m_lpD3D->Release();
        m_lpD3D = NULL;
    }

	m_lpLogBook->Succed(L"D3D-Obekt wurde freigeben");
	
}

void CScene::CreateFont(void){
	// Struktur fuer die Beschreibung der Schriftart
    // anlegen und Elemente mit 0 initialisieren
    D3DXFONT_DESC    Font = { 0 };

    // das Element lfFaceName muss den Namen der
    // gewuenschten Schriftart enthalten
    wcscpy(Font.FaceName, L"Arial");

    // nach erfolgreichem Aufruf zeigt lpD3DFont
    // auf ein Objekt vom Typ D3DXFont
    D3DXCreateFontIndirect(m_lpD3DDevice, 
                           &Font, 
                           &m_lpD3DFont);
}

void CScene::DeleteFont(void){
	if(NULL != m_lpD3DFont)
    {
        m_lpD3DFont->Release();
        m_lpD3DFont = NULL;
    }
}

void CScene::BeginScene(void)
{
    // Z-Buffer löschen
    m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, m_ClearColor, 1.0f, 0);

    m_lpD3DDevice->BeginScene();
}

void CScene::Render(void) {

	BOOL bBackbufferIsCleaned = false;

	list<Protone*>::iterator itP;
	list<Electrone*>::iterator itE;
	list<ElectricalField*>::iterator itEF;
	list<MagneticalField*>::iterator itMF;
	list<GravityField*>::iterator itGF;
	list<Emitter*>::iterator itOE;
	list<DefaultCuboid*>::iterator itDC;
	list<DefaultSphere*>::iterator itDS;

	D3DXVECTOR3 v = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float count = 0.0f;
	if(SelectedPart == SELECTED_ELEMENT) {
		
		for(itP = m_lpListProtone->begin(); itP!= m_lpListProtone->end(); itP++ ) {
			if((*itP)->bSelected) {
				v += (*itP)->Position;
				count++;
			}
		}
		for(itE = m_lpListElectrone->begin(); itE!= m_lpListElectrone->end(); itE++ ) {
			if((*itE)->bSelected){
				v += (*itE)->Position;
				count++;
			}
		}
		for(itEF = m_lpListElectricalField->begin(); itEF!= m_lpListElectricalField->end(); itEF++ ) {
			if((*itEF)->bSelected){
				v += (*itEF)->Position;
				count++;
			}
		}
		for(itMF = m_lpListMagneticalField->begin(); itMF!= m_lpListMagneticalField->end(); itMF++ ) {
			if((*itMF)->bSelected){
				v += (*itMF)->Position;
				count++;
			}
		}
		for(itGF = m_lpListGravityField->begin(); itGF!= m_lpListGravityField->end(); itGF++ ) {
			if((*itGF)->bSelected){
				v += (*itGF)->Position;
				count++;
			}
		}
		for(itOE = m_lpListEmitter->begin(); itOE!= m_lpListEmitter->end(); itOE++ ) {
			if((*itOE)->bSelected){
				v += (*itOE)->Position;
				count++;
			}
		}

		for(itDC = m_lpListDefaultCuboid->begin(); itDC!= m_lpListDefaultCuboid->end(); itDC++ ) {
			if((*itDC)->bSelected){
				v += (*itDC)->Position;
				count++;
			}
		}

		for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++ ) {
			if((*itDS)->bSelected){
				v += (*itDS)->Position;
				count++;
			}
		}

		if(count != 0){
			v /= count;
			TranslateTool.SetPosition(v);
			ScaleTool.SetPosition(v);
		}
		else {
			ShowArrow = false;
		}
	}
	else if(SelectedPart == SELECTED_SPEED) {
		for(itP = m_lpListProtone->begin(); itP!= m_lpListProtone->end(); itP++ ) {
			if((*itP)->bSelected) {
				v += (*itP)->Position + (*itP)->Speed * FACTOR_SPEED_ARROW;
				count++;
			}
		}
		for(itE = m_lpListElectrone->begin(); itE!= m_lpListElectrone->end(); itE++ ) {
			if((*itE)->bSelected){
				v += (*itE)->Position + (*itE)->Speed * FACTOR_SPEED_ARROW;
				count++;
			}
		}
		if(count != 0){
			v /= count;
			TranslateTool.SetPosition(v);
			ScaleTool.SetPosition(v);
		}
		else {
			ShowArrow = false;
		}
	}

	m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, m_ClearColor, 1.0f, 0);

    m_lpD3DDevice->BeginScene();

	m_Light.Diffuse.r = 1.0f;
	m_Light.Diffuse.g = 1.0f;
	m_Light.Diffuse.b = 1.0f;		
	m_lpD3DDevice->SetLight(0, &m_Light);
	
	//DefaultCuboidMesh.Render();
	//DefaultSphereMesh.Render();

	for(itDC = m_lpListDefaultCuboid->begin(); itDC!= m_lpListDefaultCuboid->end(); itDC++ ) {
		m_CuboidMeshMap[(*itDC)]->SetPosition((*itDC)->Position);
		m_CuboidMeshMap[(*itDC)]->SetWorldMatrix();
		m_CuboidMeshMap[(*itDC)]->Render();

	}

	for(itDS = m_lpListDefaultSphere->begin(); itDS!= m_lpListDefaultSphere->end(); itDS++ ) {
		m_SphereMeshMap[(*itDS)]->SetPosition((*itDS)->Position);
		m_SphereMeshMap[(*itDS)]->SetWorldMatrix();
		m_SphereMeshMap[(*itDS)]->Render();

	}
	
	
	m_Light.Diffuse.r = 2.0f;
	m_Light.Diffuse.g = 2.0f;
	m_Light.Diffuse.b = 2.0f;		
	m_lpD3DDevice->SetLight(0, &m_Light);

	m_lpD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for(itDC = m_lpListDefaultCuboid->begin(); itDC!= m_lpListDefaultCuboid->end(); itDC++ ) {
		
		if((*itDC)->bSelected) {
			m_CuboidMeshMap[(*itDC)]->SetPosition((*itDC)->Position);
			m_CuboidMeshMap[(*itDC)]->SetWorldMatrix();
			m_CuboidMeshMap[(*itDC)]->Render();
		}
	}

	for(itDS = m_lpListDefaultSphere->begin(); itDS!= m_lpListDefaultSphere->end(); itDS++ ) {
		
		if((*itDS)->bSelected) {
			m_SphereMeshMap[(*itDS)]->SetPosition((*itDS)->Position);
			m_SphereMeshMap[(*itDS)]->SetWorldMatrix();
			m_SphereMeshMap[(*itDS)]->Render();
		}
	}

	m_lpD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_Light.Diffuse.r = 1.0f;
	m_Light.Diffuse.g = 1.0f;
	m_Light.Diffuse.b = 1.0f;		
	m_lpD3DDevice->SetLight(0, &m_Light);


	for(itEF = m_lpListElectricalField->begin(); itEF != m_lpListElectricalField->end(); itEF++) {
		if(!(*itEF)->bSelected) {
			FieldVector.SetPosition(OBJECT_FIELD_ELECTRICAL, (*itEF)->Position);
			FieldVector.SetPositionViewAt(OBJECT_FIELD_ELECTRICAL, (*itEF)->ViewAt);
			FieldVector.Render(OBJECT_FIELD_ELECTRICAL);
		} else {
			FieldVector.SetPosition(OBJECT_FIELD_ELECTRICAL + 3, (*itEF)->Position);
			FieldVector.SetPositionViewAt(OBJECT_FIELD_ELECTRICAL + 3, (*itEF)->ViewAt);
			FieldVector.Render(OBJECT_FIELD_ELECTRICAL + 3);
		}
	}

	for(itMF = m_lpListMagneticalField->begin(); itMF != m_lpListMagneticalField->end(); itMF++) {
		if(!(*itMF)->bSelected) {
			FieldVector.SetPosition(OBJECT_FIELD_MAGNETICAL, (*itMF)->Position);
			FieldVector.SetPositionViewAt(OBJECT_FIELD_MAGNETICAL, (*itMF)->ViewAt);
			FieldVector.Render(OBJECT_FIELD_MAGNETICAL);
		} else {
			FieldVector.SetPosition(OBJECT_FIELD_MAGNETICAL + 3, (*itMF)->Position);
			FieldVector.SetPositionViewAt(OBJECT_FIELD_MAGNETICAL + 3, (*itMF)->ViewAt);
			FieldVector.Render(OBJECT_FIELD_MAGNETICAL + 3);
		}
	}

	for(itGF = m_lpListGravityField->begin(); itGF != m_lpListGravityField->end(); itGF++) {
		if(!(*itGF)->bSelected) {
			FieldVector.SetPosition(OBJECT_FIELD_GRAVITY, (*itGF)->Position);
			FieldVector.SetPositionViewAt(OBJECT_FIELD_GRAVITY, (*itGF)->ViewAt);
			FieldVector.Render(OBJECT_FIELD_GRAVITY);
		} else {
			FieldVector.SetPosition(OBJECT_FIELD_GRAVITY + 3, (*itGF)->Position);
			FieldVector.SetPositionViewAt(OBJECT_FIELD_GRAVITY + 3, (*itGF)->ViewAt);
			FieldVector.Render(OBJECT_FIELD_GRAVITY + 3);
		}
	}

	if(RenderGrid)Grid.Render();

	for(itOE = m_lpListEmitter->begin(); itOE != m_lpListEmitter->end(); itOE++) {
		
		EmitterObject.SetViewAt((*itOE)->ViewAt);
		EmitterObject.SetPosition((*itOE)->Position);
		EmitterObject.SetMatrix();
		EmitterObject.Render();

		if((*itOE)->bSelected) {
			m_Light.Diffuse.r = 2.0f;
			m_Light.Diffuse.g = 2.0f;
			m_Light.Diffuse.b = 2.0f;		
			m_lpD3DDevice->SetLight(0, &m_Light);
			m_lpD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			EmitterObject.Render();

			m_lpD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			m_Light.Diffuse.r = 1.0f;
			m_Light.Diffuse.g = 1.0f;
			m_Light.Diffuse.b = 1.0f;		
			m_lpD3DDevice->SetLight(0, &m_Light);
		}
	}
	

	m_Light.Diffuse.r = 0.5f;
	m_Light.Diffuse.g = 0.5f;
	m_Light.Diffuse.b = 1.5f;		
	m_lpD3DDevice->SetLight(0, &m_Light);

	
	for(itP = m_lpListProtone->begin(); itP!= m_lpListProtone->end(); itP++ ) {

		lpD3Object->SetPosition((*itP)->Position);
		lpD3Object->SetRotation(0.0f, 0.0f, 0.0f);
		lpD3Object->Render(lpD3Object->GetMaxFaces());

		if(Selected == (LPDEFAULTELEMENT)(*itP)) {
			if(SelectedPart == SELECTED_SPEED) TranslateTool.SetPosition(((*itP)->Position + (*itP)->Speed * FACTOR_SPEED_ARROW));
			else if(SelectedPart == SELECTED_SPEEDUP) TranslateTool.SetPosition(((*itP)->Position + (*itP)->SpeedingUp * FACTOR_SPEEDUP_ARROW));

		}
	
		if((*itP)->bSelected) {

			m_Light.Diffuse.r = 1.5f;
			m_Light.Diffuse.g = 1.5f;
			m_Light.Diffuse.b = 4.5f;		
			m_lpD3DDevice->SetLight(0, &m_Light);

			m_lpD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			lpD3Object->SetPosition((*itP)->Position);
			lpD3Object->SetRotation(0.0f, 0.0f, 0.0f);
			lpD3Object->Render(lpD3Object->GetMaxFaces());

			m_lpD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			m_Light.Diffuse.r = 0.5f;
			m_Light.Diffuse.g = 0.5f;
			m_Light.Diffuse.b = 1.5f;		
			m_lpD3DDevice->SetLight(0, &m_Light);
		}

	}
	
	m_Light.Diffuse.r = 1.5f;
	m_Light.Diffuse.g = 0.5f;
	m_Light.Diffuse.b = 0.5f;		
	m_lpD3DDevice->SetLight(0, &m_Light);


	for(itE = m_lpListElectrone->begin(); itE!= m_lpListElectrone->end(); itE++ ) {

		if(Selected == (LPDEFAULTELEMENT)(*itE)) {
			if(SelectedPart == SELECTED_SPEED) TranslateTool.SetPosition(((*itE)->Position + (*itE)->Speed * FACTOR_SPEED_ARROW));
			else if(SelectedPart == SELECTED_SPEEDUP) TranslateTool.SetPosition(((*itE)->Position + (*itE)->SpeedingUp * FACTOR_SPEEDUP_ARROW));

		}
		
		lpD3Object->SetPosition((*itE)->Position);
		lpD3Object->SetRotation(0.0f, 0.0f, 0.0f);
		lpD3Object->Render(lpD3Object->GetMaxFaces());
		
		if((*itE)->bSelected) {

			//Arrow.SetPosition((*itE)->Position);

			m_Light.Diffuse.r = 3.5f;
			m_Light.Diffuse.g = 0.5f;
			m_Light.Diffuse.b = 0.5f;		
			m_lpD3DDevice->SetLight(0, &m_Light);

			m_lpD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			lpD3Object->SetPosition((*itE)->Position);
			lpD3Object->SetRotation(0.0f, 0.0f, 0.0f);
			lpD3Object->Render(lpD3Object->GetMaxFaces());

			m_lpD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			m_Light.Diffuse.r = 1.5f;
			m_Light.Diffuse.g = 0.5f;
			m_Light.Diffuse.b = 0.5f;		
			m_lpD3DDevice->SetLight(0, &m_Light);
		}

	}

	
	// Render the current traces
	
	map<LPDEFAULTELEMENT, CTrace*>::iterator it;

	for(it = TraceMap.begin(); it != TraceMap.end();it++)
	{
		(it->second)->Render();
	}

	// Render the closed traces
	
	list<CTrace*>::iterator itL;

	for(itL = TraceList.begin(); itL != TraceList.end();itL++)
	{
		(*itL)->Render();
	}
	
	// the Speedarrows

	for(itP = m_lpListProtone->begin(); itP!= m_lpListProtone->end(); itP++ ) {
		if((*itP)->bShowSpeed) {
			if(!bBackbufferIsCleaned) {
				m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
				bBackbufferIsCleaned = true;
			}
			VectorArrow.SetPosition(ARROW_SPEED, (*itP)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEED, (*itP)->Speed * 20.0f);
			VectorArrow.Render(ARROW_SPEED);
		}
		if((*itP)->bShowSpeedup) {
			if(!bBackbufferIsCleaned) {
				m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
				bBackbufferIsCleaned = true;
			}
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itP)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itP)->SpeedingUp * 50.0f);
			VectorArrow.Render(ARROW_SPEEDUP);
		}

	}

	for(itE = m_lpListElectrone->begin(); itE!= m_lpListElectrone->end(); itE++ ) {
		if((*itE)->bShowSpeed) {
			if(!bBackbufferIsCleaned) {
				m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
				bBackbufferIsCleaned = true;
			}
			VectorArrow.SetPosition(ARROW_SPEED, (*itE)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEED, (*itE)->Speed * 20.0f);
			VectorArrow.Render(ARROW_SPEED);
		}
		if((*itE)->bShowSpeedup) {
			if(!bBackbufferIsCleaned) {
				m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
				bBackbufferIsCleaned = true;
			}
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itE)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itE)->SpeedingUp * 50.0f);
			VectorArrow.Render(ARROW_SPEEDUP);
		}

	}

	for(itDS = m_lpListDefaultSphere->begin(); itDS!= m_lpListDefaultSphere->end(); itDS++ ) {
		if((*itDS)->bShowSpeed) {
			if(!bBackbufferIsCleaned) {
				m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
				bBackbufferIsCleaned = true;
			}
			VectorArrow.SetPosition(ARROW_SPEED, (*itDS)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEED, (*itDS)->Speed * 20.0f);
			VectorArrow.Render(ARROW_SPEED);
		}
		if((*itDS)->bShowSpeedup) {
			if(!bBackbufferIsCleaned) {
				m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
				bBackbufferIsCleaned = true;
			}
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itDS)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itDS)->SpeedingUp * 50.0f);
			VectorArrow.Render(ARROW_SPEEDUP);
		}
	}

	for(itEF = m_lpListElectricalField->begin(); itEF!= m_lpListElectricalField->end(); itEF++ ) {
		if((*itEF)->bShowVector){
			if(!bBackbufferIsCleaned) {
				m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
				bBackbufferIsCleaned = true;
			}
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itEF)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itEF)->ViewAt * 1.0f);
			VectorArrow.Render(ARROW_SPEEDUP);
		}
	}

	for(itMF = m_lpListMagneticalField->begin(); itMF!= m_lpListMagneticalField->end(); itMF++ ) {
		if((*itMF)->bShowVector){
			if(!bBackbufferIsCleaned) {
				m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
				bBackbufferIsCleaned = true;
			}
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itMF)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itMF)->ViewAt * 1.0f);
			VectorArrow.Render(ARROW_SPEEDUP);
		}
	}

	for(itGF = m_lpListGravityField->begin(); itGF!= m_lpListGravityField->end(); itGF++ ) {
		if((*itGF)->bShowVector){
			if(!bBackbufferIsCleaned) {
				m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
				bBackbufferIsCleaned = true;
			}
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itGF)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itGF)->ViewAt * 1.0f);
			VectorArrow.Render(ARROW_SPEEDUP);
		}
	}

	for(itOE = m_lpListEmitter->begin(); itOE!= m_lpListEmitter->end(); itOE++ ) {
		if((*itOE)->bShowVector){
			if(!bBackbufferIsCleaned) {
				m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
				bBackbufferIsCleaned = true;
			}
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itOE)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itOE)->ViewAt * 1.0f);
			VectorArrow.Render(ARROW_SPEEDUP);
		}
	}

	/*-------------------------------------------------*/
	//   Use only to show the charges of the cubiod
	/*-------------------------------------------------*/

	m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);

	for(itDC = m_lpListDefaultCuboid->begin(); itDC!= m_lpListDefaultCuboid->end(); itDC++ ) {
		if((*itDC)->bShowCharges) {
			for(int i = 0; i < (*itDC)->NumParticies; i++) {
				lpD3Object->SetPosition((*itDC)->lpDefaultCharge[i].Position + (*itDC)->Position);
				lpD3Object->SetRotation(0.0f, 0.0f, 0.0f);
				lpD3Object->Render(lpD3Object->GetMaxFaces());
			}
		}
	}


	if(ShowArrow && bArrowActivated && ArrowMode == TOOL_TRANSFORM){
		m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);

		TranslateTool.Render(CameraPosition);
	}

	if(ShowArrow && bArrowActivated && ArrowMode == TOOL_SCALE){
		m_lpD3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);

		ScaleTool.Render(CameraPosition);
	}

	DrawText(m_text, 100, 100, 0xFFFF0000);

		
	m_lpD3DDevice->EndScene();

    m_lpD3DDevice->Present(0, 0, 0, 0);
	
}
void CScene::SortOut() {
	list<Protone*>::iterator itP;
	list<Electrone*>::iterator itE;

	for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end();itP++) {
		(*itP)->bKill = (((*itP)->Position.x > MAX_DISTAMCE) || ((*itP)->Position.y > MAX_DISTAMCE) || ((*itP)->Position.z > MAX_DISTAMCE));
		if((*itP)->bKill) StopTraceToSelected(*itP);
	}
	
	for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end();itE++) {
		(*itE)->bKill = (((*itE)->Position.x > MAX_DISTAMCE) || ((*itE)->Position.y > MAX_DISTAMCE) || ((*itE)->Position.z > MAX_DISTAMCE));
		if((*itE)->bKill)StopTraceToSelected((LPDEFAULTELEMENT) *itE);
	}

	m_lpPhysics->DeleteSpacial();

}
void CScene::EndScene(void)
{
    m_lpD3DDevice->EndScene();

    m_lpD3DDevice->Present(0, 0, 0, 0);
}

void CScene::AddTraceToSelected(void) {

	if(!Selected) return;

	if(Selected->tempType == TYPE_PROTONE) {
		
		if(Selected->bTrace) {
			StopTraceToSelected();
			return;
		}

		CTrace* lpTrace = new CTrace; 

		m_lpLogBook->Succed(L"CTrace* lpTrace = new CTrace;");

		//lpTrace->SetLogBookPtr(m_lpLogBook);
		lpTrace->SetResourceManagerPtr(lpResmanager);
		lpTrace->SetD3Device(m_lpD3DDevice);

		m_lpLogBook->Succed(L"Alle Zeiger gesetzt");

		lpTrace->AddPoint(Selected->Position, 0xFF0000FF);

		TraceMap[Selected] = lpTrace;
		Selected->bTrace = true;
	}

	else if(Selected->tempType == TYPE_ELECTRONE) {

		if(Selected->bTrace) {
			StopTraceToSelected();
			return;
		}

		CTrace* lpTrace = new CTrace; 

		m_lpLogBook->Succed(L"CTrace* lpTrace = new CTrace;");

		//lpTrace->SetLogBookPtr(m_lpLogBook);
		lpTrace->SetResourceManagerPtr(lpResmanager);
		lpTrace->SetD3Device(m_lpD3DDevice);

		m_lpLogBook->Succed(L"Alle Zeiger gesetzt");

		lpTrace->AddPoint(Selected->Position, 0xFFFF0000);

		TraceMap[Selected] = lpTrace;
		Selected->bTrace = true;
	}

}

void CScene::AddTraceToSelected(LPDEFAULTELEMENT lpElement) {
	if(!lpElement) return;

	if(lpElement->tempType == TYPE_PROTONE) {
		
		//if(lpElement->bTrace) {
		//	StopTraceToSelected(lpElement);
		//	return;
		//}

		CTrace* lpTrace = new CTrace; 

		m_lpLogBook->Succed(L"CTrace* lpTrace = new CTrace;");

		//lpTrace->SetLogBookPtr(m_lpLogBook);
		lpTrace->SetResourceManagerPtr(lpResmanager);
		lpTrace->SetD3Device(m_lpD3DDevice);

		m_lpLogBook->Succed(L"Alle Zeiger gesetzt");

		lpTrace->AddPoint(lpElement->Position, 0xFF0000FF);

		TraceMap[lpElement] = lpTrace;
		lpElement->bTrace = true;
	}

	else if(lpElement->tempType == TYPE_ELECTRONE) {

		//if(lpElement->bTrace) {
		//	StopTraceToSelected(lpElement);
		//	return;
		//}

		CTrace* lpTrace = new CTrace; 

		m_lpLogBook->Succed(L"CTrace* lpTrace = new CTrace;");

		//lpTrace->SetLogBookPtr(m_lpLogBook);
		lpTrace->SetResourceManagerPtr(lpResmanager);
		lpTrace->SetD3Device(m_lpD3DDevice);

		m_lpLogBook->Succed(L"Alle Zeiger gesetzt");

		lpTrace->AddPoint(lpElement->Position, 0xFFFF0000);

		TraceMap[lpElement] = lpTrace;
		lpElement->bTrace = true;
	}
}

void CScene::StopTraceToSelected(LPDEFAULTELEMENT lpElement){
	if(!lpElement) return;

	if(lpElement->tempType == TYPE_PROTONE) {

		if(!lpElement->bTrace) return;

	    // Iterator fuer die Suche in der Map
		map<LPDEFAULTELEMENT, CTrace*>::iterator result;

		// Nach dem Namen der Textur suchen
		result = TraceMap.find(lpElement);
		
		TraceList.push_back(result->second);

		TraceMap.erase(result);

		lpElement->bTrace = false;

	}
	else if(lpElement->tempType == TYPE_ELECTRONE) {

		if(!lpElement->bTrace) return;

	    // Iterator fuer die Suche in der Map
		map<LPDEFAULTELEMENT, CTrace*>::iterator result;

		// Nach dem Namen der Textur suchen
		result = TraceMap.find(lpElement);
		
		TraceList.push_back(result->second);

		TraceMap.erase(result);

		lpElement->bTrace = false;
	}
}

void CScene::StopTraceToSelected() {
	if(!Selected) return;

	if(Selected->tempType == TYPE_PROTONE) {

		if(!Selected->bTrace) return;

	    // Iterator fuer die Suche in der Map
		map<LPDEFAULTELEMENT, CTrace*>::iterator result;

		// Nach dem Namen der Textur suchen
		result = TraceMap.find(Selected);
		
		TraceList.push_back(result->second);

		TraceMap.erase(result);

		Selected->bTrace = false;

	}
	else if(Selected->tempType == TYPE_ELECTRONE) {

		if(!Selected->bTrace) return;

	    // Iterator fuer die Suche in der Map
		map<LPDEFAULTELEMENT, CTrace*>::iterator result;

		// Nach dem Namen der Textur suchen
		result = TraceMap.find(Selected);
		
		TraceList.push_back(result->second);

		TraceMap.erase(result);

		Selected->bTrace = false;
	}
}

void CScene::UpdateTrace() {
	map<LPDEFAULTELEMENT, CTrace*>::iterator it;

	for(it = TraceMap.begin(); it != TraceMap.end();it++)
	{	
		if((it->first)->tempType == TYPE_PROTONE) {
			(it->second)->AddPoint((it->first)->Position, 0xFF0000FF);
		}
		else {
			(it->second)->AddPoint((it->first)->Position, 0xFFFF0000);

			//float f = (sqrt( (it->first)->Speed.x * (it->first)->Speed.x + (it->first)->Speed.y * (it->first)->Speed.y + (it->first)->Speed.z * (it->first)->Speed.z ));
			//f *= 1000.0f;
			//if(f < 0.0f) f = 0.0f;
			//if(f > 255.0f) f = 255.0f;
			//BYTE speed = static_cast<BYTE>(f);
			//DWORD Color = D3DCOLOR_ARGB(0xFF, speed , 0xFF - speed, 0x00);

			//(it->second)->AddPoint((it->first)->Position, Color);
		}
	}
}

void CScene::UpdateCuboidSize(void) {

	list<DefaultCuboid*>::iterator itDC;
	D3DXVECTOR3 v;
	
	for(itDC = m_lpListDefaultCuboid->begin(); itDC != m_lpListDefaultCuboid->end(); itDC++) {
		v = m_CuboidMeshMap[*itDC]->GetSize();
		if(v.x != (*itDC)->Size.x || v.y != (*itDC)->Size.y || v.z != (*itDC)->Size.z){
			m_CuboidMeshMap[*itDC]->SetSize((*itDC)->Size);
		}
	}

}

// Text an Position x, y ausgeben
void CScene::DrawText(LPCWSTR Text, int x, int y, D3DCOLOR TextColor)
{
    RECT r = { x, y, 0, 0 };

    // Grösse des Rechtecks berechnen
    m_lpD3DFont->DrawText(NULL, Text, -1, &r, DT_CALCRECT, TextColor);

    // Text ausgeben
    m_lpD3DFont->DrawText(NULL, Text, -1, &r, DT_CENTER, TextColor);
}

void CScene::Resize(UINT width, UINT height){
	if(height < 2 || width < 2) return;
	if(PParams.BackBufferWidth == width && 	PParams.BackBufferHeight == height) return;


	SetWindowPos(hWnd, NULL, NULL, NULL, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER);


	return;

//	/*while(D3DERR_DEVICELOST == m_lpD3DDevice->TestCooperativeLevel()) {
//		Sleep(5);
//	}*/
//	//if(D3DERR_DEVICENOTRESET == m_lpD3DDevice->TestCooperativeLevel() || true) {
//
//	//m_lpD3DFont->OnLostDevice();
//
////	HRESULT hr;
//
//	PParams.BackBufferWidth = width;
//	PParams.BackBufferHeight = height;
//	//hr  = m_lpD3DDevice->Reset(&PParams);
//	//
//	//if(hr != D3D_OK){
//	//	MessageBox(0, DXGetErrorDescription9(hr),DXGetErrorString9(hr), MB_OK);
//	//	MessageBox(0, L"Leider muss das Programm aufgrund eines ungültigen Befehls beendet werden. \nDieses Problem solte in späteren Versionen behoben sein.",L"Warnung !!!", MB_OK);
//	//} else {
//	//	m_lpLogBook->Succed(L"Device wurde resetet");
//	//}
//
//	//m_lpD3DFont->OnResetDevice();
//		
//			
//	//return;
//	
//	DeInitD3D();
//	InitD3D(width, height);
//	
//	return;
//
//	//MessageBox(0, L"Die Fenstergröße wurde geändert", L"Achtung", MB_OK);
//
//	DeleteFont();
//
//	//MessageBox(0, L"Das Font wurde freigegeben", L"Achtung", MB_OK);
//
//	if(NULL != lpResmanager) {
//		//delete lpResmanager;
//		lpResmanager = NULL;
//	}
//
//	MessageBox(0, L"Der Resoucenmanager wurde beendet", L"Achtung", MB_OK);
//		
//	if(NULL != lpD3Object) {
//		//delete lpD3Object;
//		lpD3Object = NULL;
//	}
//
//	MessageBox(0, L"Das Mesh für die Kugel wurde aufgelöst", L"Achtung", MB_OK);
//
//	
//	if(NULL != m_lpBackBuffer)
//    {
//        m_lpBackBuffer->Release();
//        m_lpBackBuffer = NULL;
//
//	}
//
//	MessageBox(0, L"Der Backbuffer wurde freigegeben", L"Achtung", MB_OK);
//
//	Arrow.DeInit();
//
//	MessageBox(0, L"Arrow wurde beendet", L"Achtung", MB_OK);
//
//	VectorArrow.DeInit();
//
//	MessageBox(0, L"VectorArrow wurde beendet", L"Achtung", MB_OK);
//
//    //Grid.DeInit();
//	
//	MessageBox(0, L"Grid wurde alles beendet", L"Achtung", MB_OK);
//	
//
//	if(NULL != m_lpD3DDevice)
//    {
//        m_lpD3DDevice->Release();
//        m_lpD3DDevice = NULL;
//    }
//
//	MessageBox(0, L"Das Device wurde freigegeben", L"Achtung", MB_OK);
//
//	
//
//		
//	InitD3D(width, height);

	//Grid.SetD3Device(m_lpD3DDevice);
	//Grid.Init();

	//Arrow.SetD3Device(m_lpD3DDevice);
	//Arrow.SetResourceManagerPtr(lpResmanager);
	//Arrow.Init();

	return ;

	
}

void CScene::SetArrowMode(UINT mode) {
	
	if(!Selected) return;
	if(((Electrone*)Selected)->tempType == TYPE_PROTONE || ((Electrone*)Selected)->tempType == TYPE_ELECTRONE) {
		if(mode == SHOW_SPEED_ARROW) ((Electrone*)Selected)->bShowSpeed = !((Electrone*)Selected)->bShowSpeed;
		if(mode == SHOW_SPEEDUP_ARROW) ((Electrone*)Selected)->bShowSpeedup = !((Electrone*)Selected)->bShowSpeedup;
	} 
	else if(((Electrone*)Selected)->tempType == TYPE_FIELD_ELECTRICAL ||
		((Electrone*)Selected)->tempType == TYPE_FIELD_MAGNETICAL ||
		((Electrone*)Selected)->tempType == TYPE_FIELD_GRAVITY || true) {
		if(mode == SHOW_SPEEDUP_ARROW) ((ElectricalField*)Selected)->bShowVector = !((ElectricalField*)Selected)->bShowVector;
	}
	else if(Selected->tempType == TYPE_OBJECT_EMITTER) {
		if(mode == SHOW_SPEEDUP_ARROW) ((Emitter*)Selected)->bShowVector = !((Emitter*)Selected)->bShowVector;
	}

	Render();

}

void CScene::ToggleArrowActivated(int Mode) {
	if(ArrowMode == Mode) bArrowActivated = !bArrowActivated;
	else bArrowActivated = true;
	ArrowMode = Mode;
	
}

DefaultCuboid* CScene::AddCuboid() {
	DefaultCuboid* lpDefaultCuboid = m_lpPhysics->AddDefaultCuboid();
	CDefaultCuboid* lpDefaultCuboidMesh = new CDefaultCuboid();

	lpDefaultCuboidMesh->SetLogBookPtr(m_lpLogBook);
	lpDefaultCuboidMesh->SetD3Device(m_lpD3DDevice);
	lpDefaultCuboidMesh->SetResourceManagerPtr(lpResmanager);
	lpDefaultCuboidMesh->SetPosition(lpDefaultCuboid->Position);
	lpDefaultCuboidMesh->SetRotation(lpDefaultCuboid->rPosition);
	lpDefaultCuboidMesh->CreateCuboid(lpDefaultCuboid->Size);
	lpDefaultCuboidMesh->SetWorldMatrix();

	m_CuboidMeshMap[lpDefaultCuboid] = lpDefaultCuboidMesh;
	return lpDefaultCuboid;
}

DefaultSphere* CScene::AddSphere() {
	DefaultSphere* lpDefaultSphere = m_lpPhysics->AddDefaultSphere();
	CDefaultSphere* lpDefaultSphereMesh = new CDefaultSphere();

	lpDefaultSphereMesh->SetLogBookPtr(m_lpLogBook);
	lpDefaultSphereMesh->SetD3Device(m_lpD3DDevice);
	lpDefaultSphereMesh->SetResourceManagerPtr(lpResmanager);
	lpDefaultSphereMesh->SetPosition(lpDefaultSphere->Position);
	lpDefaultSphereMesh->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	lpDefaultSphereMesh->CreateSphere(lpDefaultSphere->Radius);
	lpDefaultSphereMesh->SetWorldMatrix();

	m_SphereMeshMap[lpDefaultSphere] = lpDefaultSphereMesh;
	return lpDefaultSphere;
}

void CScene::AddCuboid(DefaultCuboid* lpDefaultCuboid) {
	m_lpPhysics->AddDefaultCuboid(lpDefaultCuboid);
	CDefaultCuboid* lpDefaultCuboidMesh = new CDefaultCuboid();

	lpDefaultCuboidMesh->SetLogBookPtr(m_lpLogBook);
	lpDefaultCuboidMesh->SetD3Device(m_lpD3DDevice);
	lpDefaultCuboidMesh->SetResourceManagerPtr(lpResmanager);
	lpDefaultCuboidMesh->SetPosition(lpDefaultCuboid->Position);
	lpDefaultCuboidMesh->SetRotation(lpDefaultCuboid->rPosition);
	lpDefaultCuboidMesh->CreateCuboid(lpDefaultCuboid->Size);
	lpDefaultCuboidMesh->SetWorldMatrix();

	m_CuboidMeshMap[lpDefaultCuboid] = lpDefaultCuboidMesh;

}

void CScene::AddSphere(DefaultSphere* lpDefaultSphere) {
	m_lpPhysics->AddDefaultSphere(lpDefaultSphere);
	CDefaultSphere* lpDefaultSphereMesh = new CDefaultSphere();

	lpDefaultSphereMesh->SetLogBookPtr(m_lpLogBook);
	lpDefaultSphereMesh->SetD3Device(m_lpD3DDevice);
	lpDefaultSphereMesh->SetResourceManagerPtr(lpResmanager);
	lpDefaultSphereMesh->SetPosition(lpDefaultSphere->Position);
	lpDefaultSphereMesh->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	lpDefaultSphereMesh->CreateSphere(lpDefaultSphere->Radius);
	lpDefaultSphereMesh->SetWorldMatrix();

	m_SphereMeshMap[lpDefaultSphere] = lpDefaultSphereMesh;

}

void CScene::SetSphereSize(DefaultSphere* lpDefaultSphere) {
	m_SphereMeshMap[lpDefaultSphere]->SetSize(lpDefaultSphere->Radius);
}

BOOL CScene::Pick(BOOL bAddSelection){
	

	// Get the pick ray from the mouse position
    const D3DXMATRIX *pmatProj = &ProjMatrix;;

    POINT ptCursor;
    GetCursorPos( &ptCursor );
    ScreenToClient( hWnd, &ptCursor );


    D3DXVECTOR3 v;
	v.x =  ( ( ( 2.0f * ptCursor.x ) / PParams.BackBufferWidth  ) - 1 ) / pmatProj->_11;
    v.y = -( ( ( 2.0f * ptCursor.y ) / PParams.BackBufferHeight ) - 1 ) / pmatProj->_22;
    v.z =  1.0f;

    // Get the inverse view matrix
    const D3DXMATRIX matView = m_ViewMatrix;


	if(ShowArrow && bArrowActivated && ArrowMode == TOOL_TRANSFORM) {
		if(IsPicked(TranslateTool.Get3DObject(0))) {
			ArrowSelected[0] = true;
			return true;
		} else if(IsPicked(TranslateTool.Get3DObject(1))) {
			ArrowSelected[1] = true;
			return true;
		} else if(IsPicked(TranslateTool.Get3DObject(2))) {
			ArrowSelected[2] = true;
			return true;
		}
	}
	else if(ShowArrow && bArrowActivated && ArrowMode == TOOL_SCALE) {
		if(IsPicked(ScaleTool.Get3DObject(0))) {
			ArrowSelected[0] = true;
			return true;
		} else if(IsPicked(ScaleTool.Get3DObject(1))) {
			ArrowSelected[1] = true;
			return true;
		} else if(IsPicked(ScaleTool.Get3DObject(2))) {
			ArrowSelected[2] = true;
			return true;
		}
	}

	Selected = NULL;

	list<ElectricalField*>::iterator itEF;
	list<MagneticalField*>::iterator itMF;
	list<GravityField*>::iterator itGF;
	
	list<Protone*>::iterator itP;
	list<Electrone*>::iterator itE;
	list<DefaultSphere*>::iterator itDS;

	list<Emitter*>::iterator itOE;
	list<DefaultCuboid*>::iterator itDC;

	// Deselect everything

	if(!bAddSelection) {
		for(itP = m_lpListProtone->begin(); itP!= m_lpListProtone->end(); itP++ ) {
			(*itP)->bSelected = false;
		}
	
		for(itE = m_lpListElectrone->begin(); itE!= m_lpListElectrone->end(); itE++ ) {
			(*itE)->bSelected = false;
		}

		for(itEF = m_lpListElectricalField->begin(); itEF!= m_lpListElectricalField->end(); itEF++ ) {
			(*itEF)->bSelected = false;
		}

		for(itMF = m_lpListMagneticalField->begin(); itMF!= m_lpListMagneticalField->end(); itMF++ ) {
			(*itMF)->bSelected = false;
		}

		for(itGF = m_lpListGravityField->begin(); itGF!= m_lpListGravityField->end(); itGF++ ) {
			(*itGF)->bSelected = false;
		}

		for(itOE = m_lpListEmitter->begin(); itOE!= m_lpListEmitter->end(); itOE++ ) {
			(*itOE)->bSelected = false;
		}

		for(itDC = m_lpListDefaultCuboid->begin(); itDC!= m_lpListDefaultCuboid->end(); itDC++ ) {
			(*itDC)->bSelected = false;
		}

		for(itDS = m_lpListDefaultSphere->begin(); itDS!= m_lpListDefaultSphere->end(); itDS++ ) {
			(*itDS)->bSelected = false;
		}

	}

	for(itP = m_lpListProtone->begin(); itP!= m_lpListProtone->end(); itP++ ) {
		if((*itP)->bShowSpeed){
			VectorArrow.SetPosition(ARROW_SPEED, (*itP)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEED, (*itP)->Speed * FACTOR_SPEED_ARROW);
			VectorArrow.SetMatrix(ARROW_SPEED);
			if(IsPicked(VectorArrow.Get3DObject(ARROW_SPEED))) {
				//(*itP)->bSelected = true;
				ShowArrow = true;
				Selected = (LPDEFAULTELEMENT) (*itP);
				SelectedPart = SELECTED_SPEED;
				TranslateTool.SetPosition((*itP)->Position + (*itP)->Speed * FACTOR_SPEED_ARROW);
				return true;
			}
		}
	}

	for(itE = m_lpListElectrone->begin(); itE!= m_lpListElectrone->end(); itE++ ) {
		if((*itE)->bShowSpeed){
			VectorArrow.SetPosition(ARROW_SPEED, (*itE)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEED, (*itE)->Speed * FACTOR_SPEED_ARROW);
			VectorArrow.SetMatrix(ARROW_SPEED);
			if(IsPicked(VectorArrow.Get3DObject(ARROW_SPEED))) {
				//(*itP)->bSelected = true;
				ShowArrow = true;
				Selected = (LPDEFAULTELEMENT) (*itE);
				SelectedPart = SELECTED_SPEED;
				TranslateTool.SetPosition((*itE)->Position + (*itE)->Speed * FACTOR_SPEED_ARROW);
				return true;
			}
		}
	}

	for(itDS = m_lpListDefaultSphere->begin(); itDS!= m_lpListDefaultSphere->end(); itDS++ ) {
		if((*itDS)->bShowSpeed){
			VectorArrow.SetPosition(ARROW_SPEED, (*itDS)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEED, (*itDS)->Speed * FACTOR_SPEED_ARROW);
			VectorArrow.SetMatrix(ARROW_SPEED);
			if(IsPicked(VectorArrow.Get3DObject(ARROW_SPEED))) {
				//(*itP)->bSelected = true;
				ShowArrow = true;
				Selected = (LPDEFAULTELEMENT) (*itDS);
				SelectedPart = SELECTED_SPEED;
				TranslateTool.SetPosition((*itDS)->Position + (*itDS)->Speed * FACTOR_SPEED_ARROW);
				return true;
			}
		}
	}

	for(itP = m_lpListProtone->begin(); itP!= m_lpListProtone->end(); itP++ ) {
		if((*itP)->bShowSpeedup){
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itP)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itP)->SpeedingUp * FACTOR_SPEEDUP_ARROW);
			VectorArrow.SetMatrix(ARROW_SPEEDUP);
			if(IsPicked(VectorArrow.Get3DObject(ARROW_SPEEDUP))) {
				//(*itP)->bSelected = true;
				ShowArrow = true;
				Selected = (LPDEFAULTELEMENT) (*itP);
				SelectedPart = SELECTED_SPEEDUP;
				TranslateTool.SetPosition((*itP)->Position + (*itP)->SpeedingUp * FACTOR_SPEEDUP_ARROW);
				return true;
			}
		}
	}

	for(itE = m_lpListElectrone->begin(); itE!= m_lpListElectrone->end(); itE++ ) {
		if((*itE)->bShowSpeedup){
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itE)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itE)->SpeedingUp * FACTOR_SPEEDUP_ARROW);
			VectorArrow.SetMatrix(ARROW_SPEEDUP);
			if(IsPicked(VectorArrow.Get3DObject(ARROW_SPEEDUP))) {
				//(*itP)->bSelected = true;
				ShowArrow = true;
				Selected = (LPDEFAULTELEMENT) (*itE);
				SelectedPart = SELECTED_SPEEDUP;
				TranslateTool.SetPosition((*itE)->Position + (*itE)->SpeedingUp * FACTOR_SPEEDUP_ARROW);
				return true;
			}
		}
	}

	for(itDS = m_lpListDefaultSphere->begin(); itDS!= m_lpListDefaultSphere->end(); itDS++ ) {
		if((*itDS)->bShowSpeedup){
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itDS)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itDS)->SpeedingUp * FACTOR_SPEEDUP_ARROW);
			VectorArrow.SetMatrix(ARROW_SPEEDUP);
			if(IsPicked(VectorArrow.Get3DObject(ARROW_SPEEDUP))) {
				//(*itP)->bSelected = true;
				ShowArrow = true;
				Selected = (LPDEFAULTELEMENT) (*itDS);
				SelectedPart = SELECTED_SPEEDUP;
				TranslateTool.SetPosition((*itDS)->Position + (*itDS)->SpeedingUp * FACTOR_SPEEDUP_ARROW);
				return true;
			}
		}
	}

	for(itEF = m_lpListElectricalField->begin(); itEF!= m_lpListElectricalField->end(); itEF++ ) {
		if((*itEF)->bShowVector) {
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itEF)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itEF)->ViewAt * 1.0f);
			VectorArrow.SetMatrix(ARROW_SPEEDUP);
			if(IsPicked(VectorArrow.Get3DObject(ARROW_SPEEDUP))) {
				ShowArrow = true;
				Selected = (LPDEFAULTELEMENT) (*itEF);
				SelectedPart = SELECTED_FORCE;
				TranslateTool.SetPosition((*itEF)->Position + (*itEF)->ViewAt  * 1.0f);
				return true;
			}
		}
	}

	for(itMF = m_lpListMagneticalField->begin(); itMF!= m_lpListMagneticalField->end(); itMF++ ) {
		if((*itMF)->bShowVector) {
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itMF)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itMF)->ViewAt * 1.0f);
			VectorArrow.SetMatrix(ARROW_SPEEDUP);
			if(IsPicked(VectorArrow.Get3DObject(ARROW_SPEEDUP))) {
				ShowArrow = true;
				Selected = (LPDEFAULTELEMENT) (*itMF);
				SelectedPart = SELECTED_FORCE;
				TranslateTool.SetPosition((*itMF)->Position + (*itMF)->ViewAt  * 1.0f);
				return true;
			}
		}
	}

	for(itGF = m_lpListGravityField->begin(); itGF!= m_lpListGravityField->end(); itGF++ ) {
		if((*itGF)->bShowVector) {
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itGF)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itGF)->ViewAt * 1.0f);
			VectorArrow.SetMatrix(ARROW_SPEEDUP);
			if(IsPicked(VectorArrow.Get3DObject(ARROW_SPEEDUP))) {
				ShowArrow = true;
				Selected = (LPDEFAULTELEMENT) (*itGF);
				SelectedPart = SELECTED_FORCE;
				TranslateTool.SetPosition((*itGF)->Position + (*itGF)->ViewAt  * 1.0f);
				return true;
			}
		}
	}


	for(itOE = m_lpListEmitter->begin(); itOE!= m_lpListEmitter->end(); itOE++ ) {
		if((*itOE)->bShowVector) {
			VectorArrow.SetPosition(ARROW_SPEEDUP, (*itOE)->Position);
			VectorArrow.SetPositionViewAt(ARROW_SPEEDUP, (*itOE)->ViewAt * 1.0f);
			VectorArrow.SetMatrix(ARROW_SPEEDUP);
			if(IsPicked(VectorArrow.Get3DObject(ARROW_SPEEDUP))) {
				ShowArrow = true;
				Selected = (LPDEFAULTELEMENT) (*itOE);
				SelectedPart = SELECTED_DIRECTION;
				TranslateTool.SetPosition((*itOE)->Position + (*itOE)->ViewAt  * 1.0f);
				return true;
			}
		}
	}

	for(itP = m_lpListProtone->begin(); itP!= m_lpListProtone->end(); itP++ ) {
		lpD3Object->SetPosition((*itP)->Position);
		if(IsPicked(lpD3Object)) {
			(*itP)->bSelected = true;
			ShowArrow = true;
			Selected = (LPDEFAULTELEMENT) (*itP);
			SelectedPart = SELECTED_ELEMENT;
			if(!bAddSelection) {
				TranslateTool.SetPosition((*itP)->Position);
				return true;
			}
		}
	}	

	for(itE = m_lpListElectrone->begin(); itE!= m_lpListElectrone->end(); itE++ ) {
		lpD3Object->SetPosition((*itE)->Position);
		if(IsPicked(lpD3Object)) {
			(*itE)->bSelected = true;
			ShowArrow = true;
			SelectedPart = SELECTED_ELEMENT;
			Selected = (LPDEFAULTELEMENT) (*itE);
			if(!bAddSelection) {
				TranslateTool.SetPosition((*itE)->Position);
				return true;
			}
		}
	}

	for(itEF = m_lpListElectricalField->begin(); itEF!= m_lpListElectricalField->end(); itEF++ ) {
		//FieldVector.SetMatrix((*itEF)->Position);
		//FieldVector.SetMatrix(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		lpD3Object->SetPosition((*itEF)->Position);
		if(IsPicked(lpD3Object)) {
			(*itEF)->bSelected = true;
			ShowArrow = true;
			SelectedPart = SELECTED_ELEMENT;
			Selected = (LPDEFAULTELEMENT) (*itEF);
			if(!bAddSelection) {
				TranslateTool.SetPosition((*itEF)->Position);
				return true;
			}
		}

	}

	for(itMF = m_lpListMagneticalField->begin(); itMF!= m_lpListMagneticalField->end(); itMF++ ) {
		//FieldVector.SetMatrix((*itMF)->Position);
		//FieldVector.SetMatrix(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		lpD3Object->SetPosition((*itMF)->Position);
		if(IsPicked(lpD3Object)) {
			(*itMF)->bSelected = true;
			ShowArrow = true;
			SelectedPart = SELECTED_ELEMENT;
			Selected = (LPDEFAULTELEMENT) (*itMF);
			if(!bAddSelection) {
				TranslateTool.SetPosition((*itMF)->Position);
				return true;
			}
		}

	}

	for(itGF = m_lpListGravityField->begin(); itGF!= m_lpListGravityField->end(); itGF++ ) {
		//FieldVector.SetMatrix((*itGF)->Position);
		//FieldVector.SetMatrix(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		lpD3Object->SetPosition((*itGF)->Position);
		if(IsPicked(lpD3Object)) {
			(*itGF)->bSelected = true;
			ShowArrow = true;
			SelectedPart = SELECTED_ELEMENT;
			Selected = (LPDEFAULTELEMENT) (*itGF);
			if(!bAddSelection) {
				TranslateTool.SetPosition((*itGF)->Position);
				return true;
			}
		}
	}

	for(itOE = m_lpListEmitter->begin(); itOE!= m_lpListEmitter->end(); itOE++ ) {
		EmitterObject.SetPosition((*itOE)->Position);
		EmitterObject.SetViewAt((*itOE)->ViewAt);
		EmitterObject.SetMatrix();
		lpD3Object->SetPosition((*itOE)->Position);
		if(IsPicked(EmitterObject.GetMeshPtr())) {
			(*itOE)->bSelected = true;
			ShowArrow = true;
			SelectedPart = SELECTED_ELEMENT;
			Selected = (LPDEFAULTELEMENT) (*itOE);
			if(!bAddSelection) {
				TranslateTool.SetPosition((*itOE)->Position);
				return true;
			}
		}
	}

	float fa, fb, distance;

	float oldDistance = 99999.9f;

	for(itDC = m_lpListDefaultCuboid->begin(); itDC!= m_lpListDefaultCuboid->end(); itDC++ ) {
		if(m_CuboidMeshMap[*itDC]->IsPicked(v, m_ViewMatrix, &fa, &fb, &distance)) {
			//StringCchPrintf(m_text, 64, L"Distance = %3.3f", distance);

			if(bAddSelection) {
				(*itDC)->bSelected = true;
				ShowArrow = true;
				SelectedPart = SELECTED_ELEMENT;
				Selected = (LPDEFAULTELEMENT) (*itDC);
			} else {
				if(distance < oldDistance) {
					oldDistance = distance;
					if(Selected)Selected->bSelected = false;
					(*itDC)->bSelected = true;
					ShowArrow = true;
					SelectedPart = SELECTED_ELEMENT;
					Selected = (LPDEFAULTELEMENT) (*itDC);
					TranslateTool.SetPosition((*itDC)->Position);
				}
			}
		}
	}


	//StringCchPrintf(m_text, 64, L"Distance = %3.3f", oldDistance);

	for(itDS = m_lpListDefaultSphere->begin(); itDS!= m_lpListDefaultSphere->end(); itDS++ ) {
		if(m_SphereMeshMap[*itDS]->IsPicked(v, m_ViewMatrix, &fa, &fb, &distance)) {
			if(bAddSelection) {
				(*itDS)->bSelected = true;
				ShowArrow = true;
				SelectedPart = SELECTED_ELEMENT;
				Selected = (LPDEFAULTELEMENT) (*itDS);
			} else {
				if(distance < oldDistance) {
					oldDistance = distance;
					if(Selected)Selected->bSelected = false;
					(*itDS)->bSelected = true;
					ShowArrow = true;
					SelectedPart = SELECTED_ELEMENT;
					Selected = (LPDEFAULTELEMENT) (*itDS);
					TranslateTool.SetPosition((*itDS)->Position);
				}
			}
		}
	}

	if(/*bAddSelection && */SelectedPart == SELECTED_ELEMENT) return true;

	//ShowArrow = false;

	Selected = NULL;

	return false;
	
}

BOOL CScene::IsPicked(C3DObject* lpMesh){

	D3DXVECTOR3 vPickRayDir;
    D3DXVECTOR3 vPickRayOrig;
	IDirect3DDevice9* pD3Device = m_lpD3DDevice;

	// Get the pick ray from the mouse position
    const D3DXMATRIX *pmatProj = &ProjMatrix;;

    POINT ptCursor;
    GetCursorPos( &ptCursor );
    ScreenToClient( hWnd, &ptCursor );


    D3DXVECTOR3 v;
	v.x =  ( ( ( 2.0f * ptCursor.x ) / PParams.BackBufferWidth  ) - 1 ) / pmatProj->_11;
    v.y = -( ( ( 2.0f * ptCursor.y ) / PParams.BackBufferHeight ) - 1 ) / pmatProj->_22;
    v.z =  1.0f;

    // Get the inverse view matrix
    const D3DXMATRIX matView = m_ViewMatrix;
	const D3DXMATRIX matWorld = lpMesh->GetWorldMatrix();;
    D3DXMATRIX mWorldView = matWorld * matView;
    D3DXMATRIX m;
    D3DXMatrixInverse( &m, NULL, &mWorldView );

    // Transform the screen space pick ray into 3D space
    vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig.x = m._41;
    vPickRayOrig.y = m._42;
    vPickRayOrig.z = m._43;
    

    // Get the picked triangle
    
    LPD3DXMESH              pMesh;

	lpMesh->GetMeshPtr()->CloneMeshFVF(D3DXMESH_MANAGED, 
        lpD3Object->GetFVF(), m_lpD3DDevice, &pMesh);

    LPDIRECT3DVERTEXBUFFER9 pVB;
    LPDIRECT3DINDEXBUFFER9  pIB;

    pMesh->GetVertexBuffer( &pVB );
    pMesh->GetIndexBuffer( &pIB );

    WORD*      pIndices;
    D3DVERTEX* pVertices;


    pIB->Lock( 0, 0, (void**)&pIndices, 0 );
    pVB->Lock( 0, 0, (void**)&pVertices, 0 );

       // Collect only the closest intersection
    BOOL bHit;
    DWORD dwFace;
    FLOAT fBary1, fBary2, fDist;
    D3DXIntersect(pMesh, &vPickRayOrig, &vPickRayDir, &bHit, &dwFace, &fBary1, &fBary2, &fDist, 
        NULL, NULL);
    if( bHit )
    {
        //MessageBoxW(0, L"Eine Kugel wurde getroffen", L"Info", MB_OK);
		return true;
	} 
    
	return false;
}

void CScene::CreateScreenshot() {
	//WCHAR* Filename = L"../screenshot.png";

	OPENFILENAMEW Filename; 

	WCHAR Dir[128], Name[128]; 
	ZeroMemory(Name, sizeof(Name)); 
	GetModuleFileName(NULL, Dir, 128); 

	ZeroMemory(&Filename, sizeof(OPENFILENAME)); 
	Filename.lStructSize = sizeof(OPENFILENAME); 
	Filename.lpstrInitialDir = Dir; 
	Filename.lpstrFile = Name; 
	Filename.nMaxFile = 128;
	Filename.lpstrDefExt = L"png\0bmp";
	Filename.lpstrFilter = L"PNG (Portable Network Graphics)\0*.png\0BMP (Windows Bitmap)\0*.bmp\0";

	if(!GetSaveFileName(&Filename)) return;


    // temporaere Oberflaeche fuer den Screenshot anlegen
    LPDIRECT3DSURFACE9 lpSurface;

    m_lpD3DDevice->CreateOffscreenPlainSurface(
								PParams.BackBufferWidth,
								PParams.BackBufferHeight,
                                D3DFMT_A8R8G8B8,
                                D3DPOOL_SYSTEMMEM,
                                &lpSurface, 0);

    // Inhalt des Backbuffer in die Oberflaeche kopieren
    //m_lpD3DDevice->GetFrontBufferData(0, lpSurface);
    memcpy(&lpSurface, &m_lpBackBuffer, sizeof(*m_lpBackBuffer));

	WCHAR c;

	for(int i = 0; i < 128 ; i++) {
		if(Filename.lpstrFile[i] == L'\0') break;
		c = Filename.lpstrFile[i];
	}
	
	switch(c) {
		case L'g':
				D3DXSaveSurfaceToFile(Filename.lpstrFile, D3DXIFF_PNG,
							  lpSurface, 0, NULL);
			break;
		default:
				D3DXSaveSurfaceToFile(Filename.lpstrFile, D3DXIFF_BMP,
							  lpSurface, 0, NULL);
			break;
	}

    // temporaere Oberflaeche wieder freigeben
    lpSurface->Release();
}
void CScene::MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam){

	list<Protone*>::iterator itP;
	list<Electrone*>::iterator itE;

	list<ElectricalField*>::iterator itEF;
	list<MagneticalField*>::iterator itMF;
	list<GravityField*>::iterator itGF;
	list<Emitter*>::iterator itOE;
	list<DefaultCuboid*>::iterator itDC;
	list<DefaultSphere*>::iterator itDS;


	Emitter* lpEmitter;

	switch(msg) {
		
		case WM_PAINT:
				Render();
			break;

			case WM_LBUTTONDOWN:
					Render();
				if(GetAsyncKeyState(VK_MENU)){
					IsButtonDown[0] = TRUE;
					SendMessage (m_hwndStatus, SB_SETTEXT , 0, (LPARAM)L"   Drehen");
					SetCursor(CursorRotate);
				} else {
					IsButtonDown[0] = false;
					Pick(GetAsyncKeyState(VK_LSHIFT));
					m_OnPicked((LPDEFAULTELEMENT) Selected);
					Render();
				}
			break;

		case WM_LBUTTONUP:
				IsButtonDown[0] = false;
				SendMessage (m_hwndStatus, SB_SETTEXT , 0, (LPARAM)L"   Bereit");
				
				ArrowSelected[0] = false;
				ArrowSelected[1] = false;
				ArrowSelected[2] = false;

				ScaleTool.ZeroViewAt();
				Render();
			break;
		
		case WM_MBUTTONDOWN:
				Render();
				if(GetAsyncKeyState(VK_MENU)) {
					IsButtonDown[1] = TRUE;
					SendMessage (m_hwndStatus, SB_SETTEXT , 0, (LPARAM)L"   Verschieben");
					SetCursor(CursorTrans);
				}
			break;

		case WM_MBUTTONUP:
				IsButtonDown[1] = FALSE;
				SendMessage (m_hwndStatus, SB_SETTEXT , 0, (LPARAM)L"   Bereit");
			break;

		case WM_RBUTTONDOWN:
				Render();
				if(GetAsyncKeyState(VK_MENU)) {
					IsButtonDown[2] = TRUE;
					SendMessage (m_hwndStatus, SB_SETTEXT , 0, (LPARAM)L"   Zoomen");
					SetCursor(CursorScale);
				}
			break;

		case WM_RBUTTONUP:
				IsButtonDown[2] = FALSE;
				SendMessage (m_hwndStatus, SB_SETTEXT , 0, (LPARAM)L"   Bereit");
			break;

		//case WM_MOUSEWHEEL:
		case 0x020A:
				PostQuitMessage(0);
			break;

		case WM_MOUSEMOVE:
			if(IsButtonDown[2] ) {
				SetCursor(CursorScale);
				POINT p;
				GetCursorPos(&p);
				float f = (static_cast<float>(p.x + p.y) - LastMousePosition.x - LastMousePosition.y);
				
				CameraPosition += (CameraViewAt - CameraPosition) * (f * STEP_SCALE);
				Distance -= (Distance) * (f * STEP_SCALE);

				D3DXMatrixLookAtLH(&m_ViewMatrix, 
                       &CameraPosition, 
                       &CameraViewAt, 
                       &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

				m_lpD3DDevice->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
				m_lpD3DDevice->GetLight(0, &m_Light);
				m_Light.Position = CameraPosition;
				m_Light.Position.x -= Distance * cos(CameraRotation.y) * 0.25f;
				m_Light.Position.z += Distance * sin(CameraRotation.y) * 0.25f;
				//m_Light.Position.x -= Distance * cos(CameraRotation.y) * cos(CameraRotation.x);
				//m_Light.Position.z += Distance * sin(CameraRotation.y) * cos(CameraRotation.x);
				//m_Light.Position.y -= Distance * sin(CameraRotation.x);
				m_lpD3DDevice->SetLight(0, &m_Light);

				GetCursorPos(&LastMousePosition);

				Render();
				
				
			} else if(IsButtonDown[1]) {
				SetCursor(CursorTrans);
				POINT p;
				GetCursorPos(&p);
				
				D3DXVECTOR3 Delta = D3DXVECTOR3(static_cast<float>(-p.x) + LastMousePosition.x, static_cast<float>(p.y) - LastMousePosition.y, 0.0f);

				float step = STEP_TRANSLATE * Distance;

				CameraPosition.x += Delta.x * step * cos(CameraRotation.y) - sin(CameraRotation.y) * Delta.y * step * sin(CameraRotation.x);
				CameraPosition.z -= Delta.x * step * sin(CameraRotation.y) + cos(CameraRotation.y) * Delta.y * step * sin(CameraRotation.x);
				CameraPosition.y += Delta.y * step * cos(CameraRotation.x);

				CameraViewAt.x += Delta.x * step * cos(CameraRotation.y) - sin(CameraRotation.y) * Delta.y * step * sin(CameraRotation.x);
				CameraViewAt.z -= Delta.x * step * sin(CameraRotation.y) + cos(CameraRotation.y) * Delta.y * step * sin(CameraRotation.x);
				CameraViewAt.y += Delta.y * step * cos(CameraRotation.x);

				//CameraViewAt += Delta * 0.005;

				D3DXMatrixLookAtLH(&m_ViewMatrix, 
                       &CameraPosition, 
                       &CameraViewAt, 
                       &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

				m_lpD3DDevice->SetTransform(D3DTS_VIEW, &m_ViewMatrix);

				m_lpD3DDevice->GetLight(0, &m_Light);
				m_Light.Position = CameraPosition;
				//m_Light.Position.x -= Distance * cos(CameraRotation.y) * cos(CameraRotation.x);
				//m_Light.Position.z += Distance * sin(CameraRotation.y) * cos(CameraRotation.x);
				//m_Light.Position.y -= Distance * sin(CameraRotation.x);
				m_Light.Position.x -= Distance * cos(CameraRotation.y) * 0.25f;
				m_Light.Position.z += Distance * sin(CameraRotation.y) * 0.25f;
				m_lpD3DDevice->SetLight(0, &m_Light);
			
				GetCursorPos(&LastMousePosition);

				Render();
				
				
			} else if(IsButtonDown[0]) {
				SetCursor(CursorRotate);
				POINT p;
				GetCursorPos(&p);

				D3DXVECTOR3 Delta = -D3DXVECTOR3(static_cast<float>(-p.x) + LastMousePosition.x, static_cast<float>(p.y) - LastMousePosition.y, 0.0f);

				CameraRotation.x += Delta.y * STEP_ROTATE;
				CameraRotation.y += Delta.x * STEP_ROTATE;

				if(CameraRotation.x > PI / 2 - 0.001f) CameraRotation.x = PI / 2 - 0.001f;
				else if(CameraRotation.x < -PI / 2 + 0.001f) CameraRotation.x = -PI / 2 + 0.001f;

				CameraPosition.x = CameraViewAt.x - Distance * sin(CameraRotation.y) * cos(CameraRotation.x);
				CameraPosition.z = CameraViewAt.z - Distance * cos(CameraRotation.y) * cos(CameraRotation.x);
				CameraPosition.y = CameraViewAt.y - Distance * sin(CameraRotation.x);


				D3DXMatrixLookAtLH(&m_ViewMatrix, 
                       &CameraPosition, 
                       &CameraViewAt, 
                       &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

				m_lpD3DDevice->SetTransform(D3DTS_VIEW, &m_ViewMatrix);

				m_lpD3DDevice->GetLight(0, &m_Light);
				m_Light.Position = CameraPosition;
				//m_Light.Position.x -= Distance * cos(CameraRotation.y) * cos(CameraRotation.x);
				//m_Light.Position.z += Distance * sin(CameraRotation.y) * cos(CameraRotation.x);
				//m_Light.Position.y -= Distance * sin(CameraRotation.x);
				m_Light.Position.x -= Distance * cos(CameraRotation.y) * 0.25f;
				m_Light.Position.z += Distance * sin(CameraRotation.y) * 0.25f;
	
				m_lpD3DDevice->SetLight(0, &m_Light);
				
				GetCursorPos(&LastMousePosition);

				Render();

			} else if(ArrowSelected[0]){
				POINT p;
				GetCursorPos(&p);

				float dis = (LastMousePosition.x - p.x) / size.x * -cos(CameraRotation.y);
				dis -= (LastMousePosition.y - p.y) / size.x * -sin(CameraRotation.y) / -sin(CameraRotation.x);
				dis *= TranslateTool.GetDistance();

				if(ArrowMode == TOOL_TRANSFORM) {
					TranslateTool.AddPosition(D3DXVECTOR3(dis, 0.0f, 0.0f));
					if(SelectedPart == SELECTED_ELEMENT){
						for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {
							if((*itP)->bSelected)(*itP)->Position.x += dis;
						}
						for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {
							if((*itE)->bSelected)(*itE)->Position.x += dis;
						}
						for(itEF = m_lpListElectricalField->begin(); itEF != m_lpListElectricalField->end(); itEF++) {
							if((*itEF)->bSelected)(*itEF)->Position.x += dis;
						}
						for(itMF = m_lpListMagneticalField->begin(); itMF != m_lpListMagneticalField->end(); itMF++) {
							if((*itMF)->bSelected)(*itMF)->Position.x += dis;
						}
						for(itGF = m_lpListGravityField->begin(); itGF != m_lpListGravityField->end(); itGF++) {
							if((*itGF)->bSelected)(*itGF)->Position.x += dis;
						}
						for(itOE = m_lpListEmitter->begin(); itOE != m_lpListEmitter->end(); itOE++) {
							if((*itOE)->bSelected)(*itOE)->Position.x += dis;
						}
						for(itDC = m_lpListDefaultCuboid->begin(); itDC != m_lpListDefaultCuboid->end(); itDC++) {
							if((*itDC)->bSelected)(*itDC)->Position.x += dis;
						}
						for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++) {
							if((*itDS)->bSelected)(*itDS)->Position.x += dis;
						}
						
					}
					else if(SelectedPart == SELECTED_SPEED) ((Electrone*)Selected)->Speed = ( TranslateTool.GetPosition() - ((Electrone*)Selected)->Position) / FACTOR_SPEED_ARROW;
					else if(SelectedPart == SELECTED_SPEEDUP) ((Electrone*)Selected)->SpeedingUp = ( TranslateTool.GetPosition() - ((Electrone*)Selected)->Position) / FACTOR_SPEEDUP_ARROW;
					else if(SelectedPart == SELECTED_FORCE) ((ElectricalField*)Selected)->ViewAt = TranslateTool.GetPosition() - ((ElectricalField*)Selected)->Position;
					else if(SelectedPart == SELECTED_DIRECTION) ((Emitter*)Selected)->ViewAt = TranslateTool.GetPosition() - ((Emitter*)Selected)->Position;
				} 
				else if(ArrowMode == TOOL_SCALE) {
					ScaleTool.AddViewAt(D3DXVECTOR3(dis, 0.0f, 0.0f));
					for(itDC = m_lpListDefaultCuboid->begin(); itDC != m_lpListDefaultCuboid->end(); itDC++) {
						if((*itDC)->bSelected)(*itDC)->Size.x += (dis * 2.0f);
						if((*itDC)->Size.x < 0.001) (*itDC)->Size.x = 0.001;
						m_CuboidMeshMap[(*itDC)]->SetSize((*itDC)->Size);
					}
					for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++) {
						if((*itDS)->bSelected)(*itDS)->Radius += (dis * 2.0f);
						if((*itDS)->Radius < 0.001) (*itDS)->Radius = 0.001;
						m_SphereMeshMap[(*itDS)]->SetSize((*itDS)->Radius);
					}
				} 

				m_OnArrowMoved();

				GetCursorPos(&LastMousePosition);
				Render();
			} else if(ArrowSelected[1]){
				POINT p;
				GetCursorPos(&p);

				float dis = (LastMousePosition.y - p.y) / size.x / cos(CameraRotation.x);
				//dis -= (LastMousePosition.y - p.y) / size.x * -sin(CameraRotation.y);
				dis *= TranslateTool.GetDistance();

				if(ArrowMode == TOOL_TRANSFORM) {
					TranslateTool.AddPosition(D3DXVECTOR3(0.0f, dis, 0.0f));
					if(SelectedPart == SELECTED_ELEMENT){
						for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {
							if((*itP)->bSelected)(*itP)->Position.y += dis;
						}	
						for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {
							if((*itE)->bSelected)(*itE)->Position.y += dis;
						}
						for(itEF = m_lpListElectricalField->begin(); itEF != m_lpListElectricalField->end(); itEF++) {
							if((*itEF)->bSelected)(*itEF)->Position.y += dis;
						}
						for(itMF = m_lpListMagneticalField->begin(); itMF != m_lpListMagneticalField->end(); itMF++) {
							if((*itMF)->bSelected)(*itMF)->Position.y += dis;
						}
						for(itGF = m_lpListGravityField->begin(); itGF != m_lpListGravityField->end(); itGF++) {
							if((*itGF)->bSelected)(*itGF)->Position.y += dis;
						}
						for(itOE = m_lpListEmitter->begin(); itOE != m_lpListEmitter->end(); itOE++) {
							if((*itOE)->bSelected)(*itOE)->Position.y += dis;
						}
						for(itDC = m_lpListDefaultCuboid->begin(); itDC != m_lpListDefaultCuboid->end(); itDC++) {
							if((*itDC)->bSelected)(*itDC)->Position.y += dis;
						}
						for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++) {
							if((*itDS)->bSelected)(*itDS)->Position.y += dis;
						}

					}
					else if(SelectedPart == SELECTED_SPEED) ((Electrone*)Selected)->Speed = ( TranslateTool.GetPosition() - ((Electrone*)Selected)->Position) / FACTOR_SPEED_ARROW;
					else if(SelectedPart == SELECTED_SPEEDUP) ((Electrone*)Selected)->SpeedingUp = ( TranslateTool.GetPosition() - ((Electrone*)Selected)->Position) / FACTOR_SPEEDUP_ARROW;
					else if(SelectedPart == SELECTED_FORCE) ((ElectricalField*)Selected)->ViewAt = TranslateTool.GetPosition() - ((ElectricalField*)Selected)->Position;
					else if(SelectedPart == SELECTED_DIRECTION) ((Emitter*)Selected)->ViewAt = TranslateTool.GetPosition() - ((Emitter*)Selected)->Position;
					
				}
				else if(ArrowMode == TOOL_SCALE) {
					ScaleTool.AddViewAt(D3DXVECTOR3(0.0f, dis, 0.0f));
					for(itDC = m_lpListDefaultCuboid->begin(); itDC != m_lpListDefaultCuboid->end(); itDC++) {
						if((*itDC)->bSelected)(*itDC)->Size.y += (dis * 2.0f);
						if((*itDC)->Size.y < 0.001) (*itDC)->Size.y = 0.001;
						m_CuboidMeshMap[(*itDC)]->SetSize((*itDC)->Size);
					}
					for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++) {
						if((*itDS)->bSelected)(*itDS)->Radius += (dis * 2.0f);
						if((*itDS)->Radius < 0.001) (*itDS)->Radius = 0.001;
						m_SphereMeshMap[(*itDS)]->SetSize((*itDS)->Radius);
					}
				}
				m_OnArrowMoved();

				GetCursorPos(&LastMousePosition);
				Render();
			} else if(ArrowSelected[2]){
				POINT p;
				GetCursorPos(&p);

				float dis = (LastMousePosition.x - p.x) / size.x * +sin(CameraRotation.y);
				dis -= (LastMousePosition.y - p.y) / size.x * -cos(CameraRotation.y) / -sin(CameraRotation.x);
				dis *= TranslateTool.GetDistance();

			if(ArrowMode == TOOL_TRANSFORM) {
				TranslateTool.AddPosition(D3DXVECTOR3(0.0f, 0.0f, dis));
				if(SelectedPart == SELECTED_ELEMENT){
					for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {
						if((*itP)->bSelected)(*itP)->Position.z += dis;
					}	
					for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {
						if((*itE)->bSelected)(*itE)->Position.z += dis;
					}
					for(itEF = m_lpListElectricalField->begin(); itEF != m_lpListElectricalField->end(); itEF++) {
						if((*itEF)->bSelected)(*itEF)->Position.z += dis;
					}
					for(itMF = m_lpListMagneticalField->begin(); itMF != m_lpListMagneticalField->end(); itMF++) {
						if((*itMF)->bSelected)(*itMF)->Position.z += dis;
					}
					for(itGF = m_lpListGravityField->begin(); itGF != m_lpListGravityField->end(); itGF++) {
						if((*itGF)->bSelected)(*itGF)->Position.z += dis;
					}
					for(itOE = m_lpListEmitter->begin(); itOE != m_lpListEmitter->end(); itOE++) {
						if((*itOE)->bSelected)(*itOE)->Position.z += dis;
					}
					for(itDC = m_lpListDefaultCuboid->begin(); itDC != m_lpListDefaultCuboid->end(); itDC++) {
						if((*itDC)->bSelected)(*itDC)->Position.z += dis;
					}
					for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++) {
						if((*itDS)->bSelected)(*itDS)->Position.z += dis;
					}
				}
				else if(SelectedPart == SELECTED_SPEED) ((Electrone*)Selected)->Speed = ( TranslateTool.GetPosition() - ((Electrone*)Selected)->Position) / FACTOR_SPEED_ARROW;
				else if(SelectedPart == SELECTED_SPEEDUP) ((Electrone*)Selected)->SpeedingUp = ( TranslateTool.GetPosition() - ((Electrone*)Selected)->Position) / FACTOR_SPEEDUP_ARROW;
				else if(SelectedPart == SELECTED_FORCE) ((ElectricalField*)Selected)->ViewAt = TranslateTool.GetPosition() - ((ElectricalField*)Selected)->Position;
				else if(SelectedPart == SELECTED_DIRECTION) ((Emitter*)Selected)->ViewAt = TranslateTool.GetPosition() - ((Emitter*)Selected)->Position;
			}
			else if(ArrowMode == TOOL_SCALE) {
				ScaleTool.AddViewAt(D3DXVECTOR3(0.0f, 0.0f, dis));
				for(itDC = m_lpListDefaultCuboid->begin(); itDC != m_lpListDefaultCuboid->end(); itDC++) {
					if((*itDC)->bSelected)(*itDC)->Size.z += (dis * 2.0f);
					if((*itDC)->Size.z < 0.001) (*itDC)->Size.z = 0.001;
					m_CuboidMeshMap[(*itDC)]->SetSize((*itDC)->Size);
				}
				for(itDS = m_lpListDefaultSphere->begin(); itDS != m_lpListDefaultSphere->end(); itDS++) {
					if((*itDS)->bSelected)(*itDS)->Radius += (dis * 2.0f);
					if((*itDS)->Radius < 0.001) (*itDS)->Radius = 0.001;
					m_SphereMeshMap[(*itDS)]->SetSize((*itDS)->Radius);
				}
			}

			m_OnArrowMoved();				
				
				GetCursorPos(&LastMousePosition);
				Render();
			} else {
				GetCursorPos(&LastMousePosition);
				
			}
			
			
			//GetCursorPos(&LastMousePosition);
			break;
		case WM_KEYDOWN:
			switch(wparam) {
					case VK_END: 
						Distance = 10.0f;
						
						CameraViewAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

						CameraPosition.x = CameraViewAt.x - Distance * sin(CameraRotation.y) * cos(CameraRotation.x);
						CameraPosition.z = CameraViewAt.z - Distance * cos(CameraRotation.y) * cos(CameraRotation.x);
						CameraPosition.y = CameraViewAt.y - Distance * sin(CameraRotation.x);

						D3DXMatrixLookAtLH(&m_ViewMatrix, 
						&CameraPosition, 
						&CameraViewAt, 
						&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

						m_lpD3DDevice->SetTransform(D3DTS_VIEW, &m_ViewMatrix);

						m_lpD3DDevice->GetLight(0, &m_Light);
						m_Light.Position = CameraPosition;
						m_lpD3DDevice->SetLight(0, &m_Light);
					break;
			}
			break;
	}
}

