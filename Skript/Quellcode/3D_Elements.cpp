#include "3D_Elements.h"

//----------------------------------------------------------------------------------------------------
// The CGrid class
//----------------------------------------------------------------------------------------------------

CGrid::CGrid(void){
	distance = 8.0f;
	count = 84;
}

CGrid::~CGrid(void) {
	DeInit();

}


void CGrid::Init(){

	GridVertices = new CustomVertex[count];

	FLOAT x = distance * (count / 4 - 1) / -2;
	FLOAT y = 0.0f;
	FLOAT z = distance * (count / 4 - 1) / -2;

	for(UINT i = 0; i < count / 2;) {
		if(x != 0.0f && true){
			GridVertices[i] = MakeCustomVertex(x, y, z, 0xff707070);
			i++;
			z = -z;
			GridVertices[i] = MakeCustomVertex(x, y, z, 0xff707070);
			i++;
			z = -z;
			x += distance;
		} else {
			GridVertices[i] = MakeCustomVertex(x, y, z, 0xff000000);
			i++;
			z = -z;
			GridVertices[i] = MakeCustomVertex(x, y, z, 0xff000000);
			i++;
			z = -z;
			x += distance;
		}
	}

	x = distance * (count / 4 - 1) / -2;
	z = distance * (count / 4 - 1) / -2;
		
	for(UINT i = count / 2; i < count;) {
		if(z != 0.0f && true) {
			GridVertices[i] = MakeCustomVertex(x, y, z, 0xff707070);
			i++;
			x = -x;
			GridVertices[i] = MakeCustomVertex(x, y, z, 0xff707070);
			i++;
			x = -x;
			z += distance;
		} else {
			GridVertices[i] = MakeCustomVertex(x, y, z, 0xff000000);
			i++;
			x = -x;
			GridVertices[i] = MakeCustomVertex(x, y, z, 0xff000000);
			i++;
			x = -x;
			z += distance;
		}
	}
		
	
	// Zeiger, der auf den Vertex Buffer gesetzt wird
    BYTE* VertexBufferStart;

    // Vertex Buffer fuer das Dreieck anlegen
    // die Groesse des Vertex Buffer muss mit
    // der Groesse der Struktur, in der das Dreieck
    // gespeichert ist, uebereinsimmen
    m_lpD3DDevice->CreateVertexBuffer(count * sizeof(CustomVertex), 
                                    D3DUSAGE_WRITEONLY, 
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE, 
                                    D3DPOOL_MANAGED, 
                                    &VB_Grid, 
                                    NULL);

    // Vertex Buffer sperren
    VB_Grid->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

    // Vertices des Dreiecks in den Puffer kopieren
    memcpy(VertexBufferStart, GridVertices, count * sizeof(CustomVertex));

    // Vertex Buffer wieder freigeben
    VB_Grid->Unlock();

	delete[] GridVertices;

}

void CGrid::Render() {

	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Das Vertex-Format festlegen
    m_lpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);


	D3DXMATRIX WorldMatrix;
    D3DXMatrixTranslation(&WorldMatrix, 0.0f, 0.0f, 0.0f);

	m_lpD3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	// die Quelle fuer das Grid setzen
    m_lpD3DDevice->SetStreamSource(0, VB_Grid, 0, sizeof(CustomVertex));

	//m_lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
	m_lpD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, count / 2);

	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CGrid::DeInit(){
	if(NULL != VB_Grid)
    {
        VB_Grid->Release();
        VB_Grid = NULL;
    }
}

//----------------------------------------------------------------------------------------------------
// The CTranslateTool class
//----------------------------------------------------------------------------------------------------

CTranslateTool::CTranslateTool(){
	m_Position = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

CTranslateTool::~CTranslateTool(){
	DeInit();
}

void CTranslateTool::Init() {
	lpD3Elements = new C3DObject[3];
	lpD3Elements[0].SetLogBookPtr(m_lpLogBook);
	lpD3Elements[1].SetLogBookPtr(m_lpLogBook);
	lpD3Elements[2].SetLogBookPtr(m_lpLogBook);
	lpD3Elements[0].Init(m_lpD3DDevice, m_lpResmanager);
	lpD3Elements[1].Init(m_lpD3DDevice, m_lpResmanager);
	lpD3Elements[2].Init(m_lpD3DDevice, m_lpResmanager);
	lpD3Elements[0].Load(L"../data/arrow_red_high.x");
	lpD3Elements[1].Load(L"../data/arrow_green_high.x");
	lpD3Elements[2].Load(L"../data/arrow_blue_high.x");
	lpD3Elements[0].SetRotation(0.0f, 0.0f, PI_F / 2 * 3);
	lpD3Elements[1].SetRotation(0.0f, 0.0f, 0.0f);
	lpD3Elements[2].SetRotation(PI_F / 2, 0.0f, 0.0f);


	LinesVertices = new CustomVertex[6];

	LinesVertices[0] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xffFF0000);
	LinesVertices[1] = MakeCustomVertex(2.0f, 0.0f, 0.0f, 0xffFF0000);
	LinesVertices[2] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xff00DF00);
	LinesVertices[3] = MakeCustomVertex(0.0f, 2.0f, 0.0f, 0xff00DF00);
	LinesVertices[4] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xff0000FF);
	LinesVertices[5] = MakeCustomVertex(0.0f, 0.0f, 2.0f, 0xff0000FF);

	// Zeiger, der auf den Vertex Buffer gesetzt wird
    BYTE* VertexBufferStart;

    // Vertex Buffer fuer das Dreieck anlegen
    // die Groesse des Vertex Buffer muss mit
    // der Groesse der Struktur, in der das Dreieck
    // gespeichert ist, uebereinsimmen
    m_lpD3DDevice->CreateVertexBuffer(6 *  sizeof(CustomVertex), 
                                    D3DUSAGE_WRITEONLY, 
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE, 
                                    D3DPOOL_MANAGED, 
                                    &VB_Lines, 
                                    NULL);

    // Vertex Buffer sperren
    VB_Lines->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

    // Vertices des Dreiecks in den Puffer kopieren
    memcpy(VertexBufferStart, LinesVertices, 6 * sizeof(CustomVertex));

    // Vertex Buffer wieder freigeben
    VB_Lines->Unlock();

	delete[] LinesVertices;

}

void CTranslateTool::DeInit() {
	
	delete[] lpD3Elements;

	lpD3Elements = NULL;

	if(NULL != VB_Lines) {
        VB_Lines->Release();
        VB_Lines = NULL;
    }
	
}

void CTranslateTool::Render(D3DXVECTOR3 PositionCamera){

	D3DXVECTOR3 pos = m_Position - PositionCamera;

	distance = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);

	pos *= 32.0f;
	pos /= distance;
	pos +=  PositionCamera;

	// Render the meshes

	lpD3Elements[0].SetPosition(D3DXVECTOR3(2.0f, 0.0f, 0.0f) + pos);
	lpD3Elements[0].Render(lpD3Elements->GetMaxFaces());

	lpD3Elements[1].SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f) + pos);
	lpD3Elements[1].Render(lpD3Elements->GetMaxFaces());

	lpD3Elements[2].SetPosition(D3DXVECTOR3(0.0f, 0.0f, 2.0f) + pos);
	lpD3Elements[2].Render(lpD3Elements->GetMaxFaces());

	// Render the lines

	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Das Vertex-Format festlegen
    m_lpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);


	D3DXMATRIX WorldMatrix;
	D3DXMatrixTranslation(&WorldMatrix, pos.x, pos.y, pos.z);

	m_lpD3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	// die Quelle fuer das Grid setzen
    m_lpD3DDevice->SetStreamSource(0, VB_Lines, 0, sizeof(CustomVertex));

	m_lpD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);

	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}


//----------------------------------------------------------------------------------------------------
// The CScaleTool class
//----------------------------------------------------------------------------------------------------

CScaleTool::CScaleTool() {
	m_Position = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

CScaleTool::~CScaleTool() {
	DeInit();
}
void CScaleTool::ZeroViewAt(void) {
	m_ViewAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	LinesVertices = new CustomVertex[6];

	LinesVertices[0] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xffFF0000);
	LinesVertices[1] = MakeCustomVertex(2.0f, 0.0f, 0.0f, 0xffFF0000);
	LinesVertices[2] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xff00DF00);
	LinesVertices[3] = MakeCustomVertex(0.0f, 2.0f, 0.0f, 0xff00DF00);
	LinesVertices[4] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xff0000FF);
	LinesVertices[5] = MakeCustomVertex(0.0f, 0.0f, 2.0f, 0xff0000FF);

	// Zeiger, der auf den Vertex Buffer gesetzt wird
    BYTE* VertexBufferStart;

    // Vertex Buffer fuer das Dreieck anlegen
    // die Groesse des Vertex Buffer muss mit
    // der Groesse der Struktur, in der das Dreieck
    // gespeichert ist, uebereinsimmen
    m_lpD3DDevice->CreateVertexBuffer(6 *  sizeof(CustomVertex), 
                                    D3DUSAGE_WRITEONLY, 
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE, 
                                    D3DPOOL_MANAGED, 
                                    &VB_Lines, 
                                    NULL);

    // Vertex Buffer sperren
    VB_Lines->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

    // Vertices des Dreiecks in den Puffer kopieren
    memcpy(VertexBufferStart, LinesVertices, 6 * sizeof(CustomVertex));

    // Vertex Buffer wieder freigeben
    VB_Lines->Unlock();

	delete[] LinesVertices;
}

void CScaleTool::AddViewAt(D3DXVECTOR3 deltaViewAt){
	m_ViewAt += (deltaViewAt * 1.0f);
	LinesVertices = new CustomVertex[6];

	LinesVertices[0] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xffFF0000);
	LinesVertices[1] = MakeCustomVertex(2.0f + m_ViewAt.x, 0.0f, 0.0f, 0xffFF0000);
	LinesVertices[2] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xff00DF00);
	LinesVertices[3] = MakeCustomVertex(0.0f, 2.0f + m_ViewAt.y, 0.0f, 0xff00DF00);
	LinesVertices[4] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xff0000FF);
	LinesVertices[5] = MakeCustomVertex(0.0f, 0.0f, 2.0f + m_ViewAt.z, 0xff0000FF);

	// Zeiger, der auf den Vertex Buffer gesetzt wird
    BYTE* VertexBufferStart;

    // Vertex Buffer fuer das Dreieck anlegen
    // die Groesse des Vertex Buffer muss mit
    // der Groesse der Struktur, in der das Dreieck
    // gespeichert ist, uebereinsimmen
    m_lpD3DDevice->CreateVertexBuffer(6 *  sizeof(CustomVertex), 
                                    D3DUSAGE_WRITEONLY, 
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE, 
                                    D3DPOOL_MANAGED, 
                                    &VB_Lines, 
                                    NULL);

    // Vertex Buffer sperren
    VB_Lines->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

    // Vertices des Dreiecks in den Puffer kopieren
    memcpy(VertexBufferStart, LinesVertices, 6 * sizeof(CustomVertex));

    // Vertex Buffer wieder freigeben
    VB_Lines->Unlock();

	delete[] LinesVertices;
}

void CScaleTool::Init() {
	lpD3Elements = new C3DObject[3];
	lpD3Elements[0].SetLogBookPtr(m_lpLogBook);
	lpD3Elements[1].SetLogBookPtr(m_lpLogBook);
	lpD3Elements[2].SetLogBookPtr(m_lpLogBook);
	lpD3Elements[0].Init(m_lpD3DDevice, m_lpResmanager);
	lpD3Elements[1].Init(m_lpD3DDevice, m_lpResmanager);
	lpD3Elements[2].Init(m_lpD3DDevice, m_lpResmanager);
	lpD3Elements[0].Load(L"../data/cube_red.x");
	lpD3Elements[1].Load(L"../data/cube_green.x");
	lpD3Elements[2].Load(L"../data/cube_blue.x");
	lpD3Elements[0].SetRotation(0.0f, 0.0f, PI_F / 2 * 3);
	lpD3Elements[1].SetRotation(0.0f, 0.0f, 0.0f);
	lpD3Elements[2].SetRotation(PI_F / 2, 0.0f, 0.0f);


	LinesVertices = new CustomVertex[6];

	LinesVertices[0] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xffFF0000);
	LinesVertices[1] = MakeCustomVertex(2.0f, 0.0f, 0.0f, 0xffFF0000);
	LinesVertices[2] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xff00DF00);
	LinesVertices[3] = MakeCustomVertex(0.0f, 2.0f, 0.0f, 0xff00DF00);
	LinesVertices[4] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xff0000FF);
	LinesVertices[5] = MakeCustomVertex(0.0f, 0.0f, 2.0f, 0xff0000FF);

	// Zeiger, der auf den Vertex Buffer gesetzt wird
    BYTE* VertexBufferStart;

    // Vertex Buffer fuer das Dreieck anlegen
    // die Groesse des Vertex Buffer muss mit
    // der Groesse der Struktur, in der das Dreieck
    // gespeichert ist, uebereinsimmen
    m_lpD3DDevice->CreateVertexBuffer(6 *  sizeof(CustomVertex), 
                                    D3DUSAGE_WRITEONLY, 
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE, 
                                    D3DPOOL_MANAGED, 
                                    &VB_Lines, 
                                    NULL);

    // Vertex Buffer sperren
    VB_Lines->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

    // Vertices des Dreiecks in den Puffer kopieren
    memcpy(VertexBufferStart, LinesVertices, 6 * sizeof(CustomVertex));

    // Vertex Buffer wieder freigeben
    VB_Lines->Unlock();

	delete[] LinesVertices;

}

void CScaleTool::DeInit() {
	
	delete[] lpD3Elements;

	lpD3Elements = NULL;

	if(NULL != VB_Lines) {
        VB_Lines->Release();
        VB_Lines = NULL;
    }
	
}

void CScaleTool::Render(D3DXVECTOR3 PositionCamera){

	D3DXVECTOR3 pos = m_Position - PositionCamera;

	distance = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);

	pos *= 32.0f;
	pos /= distance;
	pos +=  PositionCamera;

	// Render the meshes

	lpD3Elements[0].SetPosition(D3DXVECTOR3(2.0f + m_ViewAt.x, 0.0f, 0.0f) + pos);
	lpD3Elements[0].Render(lpD3Elements->GetMaxFaces());

	lpD3Elements[1].SetPosition(D3DXVECTOR3(0.0f, 2.0f + m_ViewAt.y, 0.0f) + pos);
	lpD3Elements[1].Render(lpD3Elements->GetMaxFaces());

	lpD3Elements[2].SetPosition(D3DXVECTOR3(0.0f, 0.0f, 2.0f + m_ViewAt.z) + pos);
	lpD3Elements[2].Render(lpD3Elements->GetMaxFaces());

	// Render the lines

	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Das Vertex-Format festlegen
    m_lpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);


	D3DXMATRIX WorldMatrix;
	D3DXMatrixTranslation(&WorldMatrix, pos.x, pos.y, pos.z);

	m_lpD3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	// die Quelle fuer das Grid setzen
    m_lpD3DDevice->SetStreamSource(0, VB_Lines, 0, sizeof(CustomVertex));

	m_lpD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);

	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}


//----------------------------------------------------------------------------------------------------
// The CVectorArrow class
//----------------------------------------------------------------------------------------------------

CVectorArrow::CVectorArrow(){
	
}

CVectorArrow::~CVectorArrow() {
	DeInit();
}



void CVectorArrow::Init() {

	lpD3Elements = new C3DObject[2];
	lpD3Elements[0].SetLogBookPtr(m_lpLogBook);
	lpD3Elements[1].SetLogBookPtr(m_lpLogBook);
	lpD3Elements[0].Init(m_lpD3DDevice, m_lpResmanager);
	lpD3Elements[1].Init(m_lpD3DDevice, m_lpResmanager);
	lpD3Elements[0].Load(L"../data/arrow_green_high.x");
	lpD3Elements[1].Load(L"../data/arrow_orange_high.x");


	float RotateX, RotateY;

	D3DXVECTOR3 v2 = m_PositionViewAt[0];

	RotateX  = atan(sqrt(v2.x * v2.x + v2.z * v2.z) / v2.y);

	if(v2.y < 0.0f) RotateX += PI_F;

	if(v2.z < 0.0f)RotateY = atan(v2.x / v2.z) + PI_F;

	else RotateY = atan(v2.x / v2.z);

	lpD3Elements[0].SetRotation(RotateX, RotateY, 0.0f);

	
	v2 = m_PositionViewAt[1];

	RotateX  = atan(sqrt(v2.x * v2.x + v2.z * v2.z) / v2.y);

	if(v2.y < 0.0f) RotateX += PI_F;

	if(v2.z < 0.0f)RotateY = atan(v2.x / v2.z) + PI_F;

	else RotateY = atan(v2.x / v2.z);


	lpD3Elements[1].SetRotation(RotateX, RotateY, 0.0f);


	LineVertices = new CustomVertex[2];
	LineVertices[0] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xffFF0000);
	LineVertices[1] = MakeCustomVertex(m_PositionViewAt[0].x, m_PositionViewAt[0].y, m_PositionViewAt[0].z , 0xffFF0000);

	// Zeiger, der auf den Vertex Buffer gesetzt wird
    BYTE* VertexBufferStart;

    // Vertex Buffer fuer das Dreieck anlegen
    // die Groesse des Vertex Buffer muss mit
    // der Groesse der Struktur, in der das Dreieck
    // gespeichert ist, uebereinsimmen
    m_lpD3DDevice->CreateVertexBuffer(2 *  sizeof(CustomVertex), 
                                    D3DUSAGE_WRITEONLY, 
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE, 
                                    D3DPOOL_MANAGED, 
                                    &VB_Line[0], 
                                    NULL);

    // Vertex Buffer sperren
    VB_Line[0]->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

    // Vertices des Dreiecks in den Puffer kopieren
    memcpy(VertexBufferStart, LineVertices, 2 * sizeof(CustomVertex));

    // Vertex Buffer wieder freigeben
    VB_Line[0]->Unlock();

	delete[] LineVertices;

	LineVertices = new CustomVertex[2];
	LineVertices[0] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xff00FF00);
	LineVertices[1] = MakeCustomVertex(m_PositionViewAt[1].x, m_PositionViewAt[1].y, m_PositionViewAt[1].z , 0xff00FF00);

	// Vertex Buffer fuer das Dreieck anlegen
    // die Groesse des Vertex Buffer muss mit
    // der Groesse der Struktur, in der das Dreieck
    // gespeichert ist, uebereinsimmen
    m_lpD3DDevice->CreateVertexBuffer(2 *  sizeof(CustomVertex), 
                                    D3DUSAGE_WRITEONLY, 
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE, 
                                    D3DPOOL_MANAGED, 
                                    &VB_Line[1], 
                                    NULL);

    // Vertex Buffer sperren
    VB_Line[1]->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

    // Vertices des Dreiecks in den Puffer kopieren
    memcpy(VertexBufferStart, LineVertices, 2 * sizeof(CustomVertex));

    // Vertex Buffer wieder freigeben
    VB_Line[1]->Unlock();

	delete[] LineVertices;


}

void CVectorArrow::DeInit() {
	if(NULL != VB_Line[0]) {
        VB_Line[0]->Release();
        VB_Line[0] = NULL;
    }

	if(NULL != VB_Line[1]) {
        VB_Line[1]->Release();
        VB_Line[1] = NULL;
    }

	delete[] lpD3Elements;

	lpD3Elements = NULL;
}

void CVectorArrow::SetPositionViewAt(UINT ID, D3DXVECTOR3 ViewAt) {

	m_PositionViewAt[ID] = ViewAt;

	float RotateX, RotateY;

	D3DXVECTOR3 v2 = m_PositionViewAt[ID];
	
	RotateX  = atan(sqrt(v2.x * v2.x + v2.z * v2.z) / v2.y);

	if(v2.y < 0.0f) RotateX += PI_F;

	if(v2.z < 0.0f)RotateY = atan(v2.x / v2.z) + PI_F;

	else RotateY = atan(v2.x / v2.z);

	lpD3Elements[ID].SetRotation(RotateX, RotateY, 0.0f);
	//lpD3Elements[1].SetRotation(RotateX, RotateY, 0.0f);


	if(NULL != VB_Line[0]) {
        VB_Line[0]->Release();
        VB_Line[0] = NULL;
    }	

	if(NULL != VB_Line[1]) {
        VB_Line[1]->Release();
        VB_Line[1] = NULL;
    }

	LineVertices = new CustomVertex[2];
	if(ID == 1) {
		LineVertices[0] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xffFF0000);
		LineVertices[1] = MakeCustomVertex(m_PositionViewAt[ID].x, m_PositionViewAt[ID].y, m_PositionViewAt[ID].z , 0xffFF0000);
	} else {
		LineVertices[0] = MakeCustomVertex(0.0f, 0.0f, 0.0f, 0xff00FF00);
		LineVertices[1] = MakeCustomVertex(m_PositionViewAt[ID].x, m_PositionViewAt[ID].y, m_PositionViewAt[ID].z , 0xff00FF00);
	}

	// Zeiger, der auf den Vertex Buffer gesetzt wird
    BYTE* VertexBufferStart;

    // Vertex Buffer fuer das Dreieck anlegen
    // die Groesse des Vertex Buffer muss mit
    // der Groesse der Struktur, in der das Dreieck
    // gespeichert ist, uebereinsimmen
    m_lpD3DDevice->CreateVertexBuffer(2 *  sizeof(CustomVertex), 
                                    D3DUSAGE_WRITEONLY, 
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE, 
                                    D3DPOOL_MANAGED, 
                                    &VB_Line[ID], 
                                    NULL);

    // Vertex Buffer sperren
    VB_Line[ID]->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

    // Vertices des Dreiecks in den Puffer kopieren
    memcpy(VertexBufferStart, LineVertices, 2 * sizeof(CustomVertex));

    // Vertex Buffer wieder freigeben
    VB_Line[ID]->Unlock();

	delete[] LineVertices;
}

void CVectorArrow::SetMatrix(UINT ID) {

	lpD3Elements[ID].SetPosition(m_Position[ID] + m_PositionViewAt[ID]);
}

void CVectorArrow::Render(UINT ID) {

	// Render the Mesh
	
	lpD3Elements[ID].SetPosition(m_Position[ID] + m_PositionViewAt[ID]);
	lpD3Elements[ID].Render(lpD3Elements->GetMaxFaces());


	// Render the line

	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Das Vertex-Format festlegen
    m_lpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);


	D3DXMATRIX WorldMatrix;
	D3DXMatrixTranslation(&WorldMatrix, m_Position[ID].x, m_Position[ID].y, m_Position[ID].z);

	m_lpD3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	// die Quelle fuer das Grid setzen
    m_lpD3DDevice->SetStreamSource(0, VB_Line[ID], 0, sizeof(CustomVertex));

	m_lpD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}


//----------------------------------------------------------------------------------------------------
// The CFieldVector class
//----------------------------------------------------------------------------------------------------

CFieldVector::CFieldVector() {

}

CFieldVector::~CFieldVector() {
	DeInit();
}

void CFieldVector::Init() {
	lpD3Elements = new C3DObject;
	lpD3Elements->SetLogBookPtr(m_lpLogBook);
	lpD3Elements->Init(m_lpD3DDevice, m_lpResmanager);
	lpD3Elements->Load(L"../data/sphere_low.x");
	lpD3Elements->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	lpD3Elements->SetRotation(0.0f, 0.0f, 0.0f);

	m_lpLogBook->Succed(L"C3DObject hinzugefuegt", L"../data/sphere_low.x");

	// Create the Lines
	BYTE* VertexBufferStart;

	for(int i = 0; i < 6; i++) {
	
		DWORD Color = NULL;
		if(i < 3) Color = D3DCOLOR_ARGB(0xFF, 0xEF * (i == 0), 0xCF * (i == 1), 0xEF * (i == 2));
		else Color = D3DCOLOR_ARGB(0xFF, 0x80 * (i == 3) + 0x7F, 0x80 * (i == 4) + 0x7F, 0x80 * (i == 5) + 0x7F);

		LineVertices = new CustomVertex[2];
		LineVertices[0] = MakeCustomVertex(0.0f, 0.0f, 0.0f, Color);
		LineVertices[1] = MakeCustomVertex(m_PositionViewAt[0].x, m_PositionViewAt[0].y, m_PositionViewAt[0].z , Color);
		
		m_lpD3DDevice->CreateVertexBuffer(2 *  sizeof(CustomVertex), 
										D3DUSAGE_WRITEONLY, 
										D3DFVF_XYZ | D3DFVF_DIFFUSE, 
										D3DPOOL_MANAGED, 
										&VB_Line[i], 
										NULL);

		// Vertex Buffer sperren
		VB_Line[i]->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

		// Vertices des Dreiecks in den Puffer kopieren
		memcpy(VertexBufferStart, LineVertices, 2 * sizeof(CustomVertex));

		// Vertex Buffer wieder freigeben
		VB_Line[i]->Unlock();

		delete[] LineVertices;

	}

}

void CFieldVector::DeInit() {
	if(NULL != VB_Line[0]) {
        VB_Line[0]->Release();
        VB_Line[0] = NULL;
    }

	if(NULL != VB_Line[1]) {
        VB_Line[1]->Release();
        VB_Line[1] = NULL;
    }
	
	if(NULL != VB_Line[2]) {
        VB_Line[2]->Release();
        VB_Line[2] = NULL;
    }
	
	if(NULL != VB_Line[3]) {
        VB_Line[3]->Release();
        VB_Line[3] = NULL;
    }
	
	if(NULL != VB_Line[4]) {
        VB_Line[4]->Release();
        VB_Line[4] = NULL;
    }
	
	if(NULL != VB_Line[5]) {
        VB_Line[5]->Release();
        VB_Line[5] = NULL;
    }

	delete lpD3Elements;

	lpD3Elements = NULL;
}

void CFieldVector::SetPositionViewAt(UINT ID, D3DXVECTOR3 ViewAt) {
	m_PositionViewAt[ID] = ViewAt;

	D3DXVECTOR3 v2 = m_PositionViewAt[ID];

	RotateX  = atan(sqrt(v2.x * v2.x + v2.z * v2.z) / v2.y);

	if(v2.y < 0.0f) RotateX += PI_F;

	if(v2.z < 0.0f)RotateY = atan(v2.x / v2.z) + PI_F;

	else RotateY = atan(v2.x / v2.z);

	lpD3Elements[ID].SetRotation(RotateX, RotateY, 0.0f);

	v2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	v2.y  = sqrt(m_PositionViewAt[ID].x * m_PositionViewAt[ID].x + 
		m_PositionViewAt[ID].y * m_PositionViewAt[ID].y + 
		m_PositionViewAt[ID].z * m_PositionViewAt[ID].z ) * 0.5f;
	

	if(NULL != VB_Line[ID]) {
        VB_Line[ID]->Release();
        VB_Line[ID] = NULL;
    }	

	DWORD Color = NULL;
	if(ID < 3) Color = D3DCOLOR_ARGB(0xFF, 0xEF * (ID == 0), 0xCF * (ID == 1), 0xEF * (ID == 2));
	else Color = D3DCOLOR_ARGB(0xFF, 0x80 * (ID == 3) + 0x7F, 0x80 * (ID == 4) + 0x7F, 0x80 * (ID == 5) + 0x7F);

	
	LineVertices = new CustomVertex[40];

	LineVertices[0] = MakeCustomVertex(-v2.x, -v2.y, -v2.z, Color);
	LineVertices[1] = MakeCustomVertex(v2.x, v2.y, v2.z , Color);
	LineVertices[2] = MakeCustomVertex(v2.x, v2.y, v2.z , Color);
	LineVertices[3] = MakeCustomVertex(v2.x - 0.1f, v2.y - 0.4f, v2.z + 0.2f, Color);
	LineVertices[4] = MakeCustomVertex(v2.x, v2.y, v2.z , Color);
	LineVertices[5] = MakeCustomVertex(v2.x - 0.1f, v2.y - 0.4f, v2.z - 0.2f, Color);
	LineVertices[6] = MakeCustomVertex(v2.x, v2.y, v2.z , Color);
	LineVertices[7] = MakeCustomVertex(v2.x + 0.2f, v2.y - 0.4f, v2.z , Color);

	LineVertices[8] = MakeCustomVertex(-v2.x + 1, -v2.y, -v2.z  + 1, Color);
	LineVertices[9] = MakeCustomVertex(v2.x + 1, v2.y, v2.z  + 1, Color);
	LineVertices[10] = MakeCustomVertex(v2.x  + 1, v2.y, v2.z   + 1, Color);
	LineVertices[11] = MakeCustomVertex(v2.x - 0.1f  + 1, v2.y - 0.4f, v2.z + 0.2f  + 1, Color);
	LineVertices[12] = MakeCustomVertex(v2.x  + 1, v2.y, v2.z   + 1, Color);
	LineVertices[13] = MakeCustomVertex(v2.x - 0.1f  + 1, v2.y - 0.4f, v2.z - 0.2f  + 1, Color);
	LineVertices[14] = MakeCustomVertex(v2.x  + 1, v2.y, v2.z   + 1, Color);
	LineVertices[15] = MakeCustomVertex(v2.x + 0.2f  + 1, v2.y - 0.4f, v2.z   + 1, Color);

	LineVertices[16] = MakeCustomVertex(-v2.x + 1, -v2.y, -v2.z  - 1, Color);
	LineVertices[17] = MakeCustomVertex(v2.x + 1, v2.y, v2.z  - 1, Color);
	LineVertices[18] = MakeCustomVertex(v2.x  + 1, v2.y, v2.z   - 1, Color);
	LineVertices[19] = MakeCustomVertex(v2.x - 0.1f  + 1, v2.y - 0.4f, v2.z + 0.2f  - 1, Color);
	LineVertices[20] = MakeCustomVertex(v2.x  + 1, v2.y, v2.z   - 1, Color);
	LineVertices[21] = MakeCustomVertex(v2.x - 0.1f  + 1, v2.y - 0.4f, v2.z - 0.2f  - 1, Color);
	LineVertices[22] = MakeCustomVertex(v2.x  + 1, v2.y, v2.z   - 1, Color);
	LineVertices[23] = MakeCustomVertex(v2.x + 0.2f  + 1, v2.y - 0.4f, v2.z   - 1, Color);

	LineVertices[24] = MakeCustomVertex(-v2.x - 1, -v2.y, -v2.z  + 1, Color);
	LineVertices[25] = MakeCustomVertex(v2.x - 1, v2.y, v2.z  + 1, Color);
	LineVertices[26] = MakeCustomVertex(v2.x  - 1, v2.y, v2.z   + 1, Color);
	LineVertices[27] = MakeCustomVertex(v2.x - 0.1f  - 1, v2.y - 0.4f, v2.z + 0.2f  + 1, Color);
	LineVertices[28] = MakeCustomVertex(v2.x  - 1, v2.y, v2.z   + 1, Color);
	LineVertices[29] = MakeCustomVertex(v2.x - 0.1f  - 1, v2.y - 0.4f, v2.z - 0.2f  + 1, Color);
	LineVertices[30] = MakeCustomVertex(v2.x  - 1, v2.y, v2.z   + 1, Color);
	LineVertices[31] = MakeCustomVertex(v2.x + 0.2f  - 1, v2.y - 0.4f, v2.z   + 1, Color);

	LineVertices[32] = MakeCustomVertex(-v2.x - 1, -v2.y, -v2.z  - 1, Color);
	LineVertices[33] = MakeCustomVertex(v2.x - 1, v2.y, v2.z  - 1, Color);
	LineVertices[34] = MakeCustomVertex(v2.x  - 1, v2.y, v2.z   - 1, Color);
	LineVertices[35] = MakeCustomVertex(v2.x - 0.1f  - 1, v2.y - 0.4f, v2.z + 0.2f  - 1, Color);
	LineVertices[36] = MakeCustomVertex(v2.x  - 1, v2.y, v2.z   - 1, Color);
	LineVertices[37] = MakeCustomVertex(v2.x - 0.1f  - 1, v2.y - 0.4f, v2.z - 0.2f  - 1, Color);
	LineVertices[38] = MakeCustomVertex(v2.x  - 1, v2.y, v2.z   - 1, Color);
	LineVertices[39] = MakeCustomVertex(v2.x + 0.2f  - 1, v2.y - 0.4f, v2.z   - 1, Color);


	
	// Zeiger, der auf den Vertex Buffer gesetzt wird
    BYTE* VertexBufferStart;

    // Vertex Buffer fuer das Dreieck anlegen
    // die Groesse des Vertex Buffer muss mit
    // der Groesse der Struktur, in der das Dreieck
    // gespeichert ist, uebereinsimmen
    m_lpD3DDevice->CreateVertexBuffer(40 *  sizeof(CustomVertex), 
                                    D3DUSAGE_WRITEONLY, 
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE, 
                                    D3DPOOL_MANAGED, 
                                    &VB_Line[ID], 
                                    NULL);

    // Vertex Buffer sperren
    VB_Line[ID]->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

    // Vertices des Dreiecks in den Puffer kopieren
    memcpy(VertexBufferStart, LineVertices, 40 * sizeof(CustomVertex));

    // Vertex Buffer wieder freigeben
    VB_Line[ID]->Unlock();

	delete[] LineVertices;
}


void CFieldVector::SetMatrix(D3DXVECTOR3 Position) {
	lpD3Elements->SetPosition(Position);
}

void CFieldVector::Render(UINT ID) {
	
	// Render the line

	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Das Vertex-Format festlegen
    m_lpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	D3DXMATRIX RotationMatrixY;
	D3DXMATRIX RotationMatrixX;
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX TranslationMatrix;
	D3DXMATRIX WorldMatrix;

	D3DXMatrixTranslation(&TranslationMatrix, m_Position[ID].x, m_Position[ID].y, m_Position[ID].z);
	
	D3DXMatrixRotationY(&RotationMatrixY, RotateY);
	D3DXMatrixRotationX(&RotationMatrixX, RotateX);

	D3DXMatrixIdentity(&RotationMatrix);

	D3DXMatrixMultiply(&RotationMatrix, &RotationMatrix, &RotationMatrixX);
	D3DXMatrixMultiply(&RotationMatrix, &RotationMatrix, &RotationMatrixY);

	D3DXMatrixMultiply(&WorldMatrix, &RotationMatrix, &TranslationMatrix);


	m_lpD3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	// die Quelle fuer das Grid setzen
    m_lpD3DDevice->SetStreamSource(0, VB_Line[ID], 0, sizeof(CustomVertex));

	m_lpD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, 20);

	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}


//----------------------------------------------------------------------------------------------------
// The CDefaultSphere class
//----------------------------------------------------------------------------------------------------

CDefaultSphere::CDefaultSphere() {
	VB_Sphere = NULL;
	IB_Sphere = NULL;

}

CDefaultSphere::~CDefaultSphere() {
	if(VB_Sphere) {
		VB_Sphere->Release();
		VB_Sphere = NULL;
	}

	if(IB_Sphere) {
		IB_Sphere->Release();
		IB_Sphere = NULL;
	}

	delete[] m_lpVerticies;
	delete[] m_lpIndicies;

}

BOOL CDefaultSphere::CreateSphere(float Radius, UINT resolutionVertical, UINT resolutionHorizontal) {

	m_Radius = Radius;
	// local value to check the D3DX-Functions
	HRESULT hr;
	
	FVF_FORMAT = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL;
	DWORD m_Color = 0xFF7C7C7C;

	
	// Calculate the number of verticies and faces
	NumFaces = ( resolutionVertical - 1) * resolutionHorizontal * 2;
	NumVerticies = 2 + (resolutionVertical - 1) * resolutionHorizontal;
	

	// Create the arrays
	m_lpVerticies = new VertexNorm[NumVerticies];
	m_lpIndicies = new DWORD[NumFaces * 3];


	// Generate the Vericies and Indicies

	float deltaH = 2 * PI / resolutionHorizontal;
	float deltaV = PI / resolutionVertical;

	float posH = 0.0f;
	float posV = PI / 2;
	
	UINT iVer = 0;
	UINT iInd = 0;

	float x, y ,z;


	for(int i = 0; i < resolutionVertical + 1; i++) {

		// start vertex at the top
		if(i == 0) {
			m_lpVerticies[iVer] = MakeLightVertex(0.0f, Radius, 0.0f, 0.0f, 1.0f, 0.0f, m_Color);
			iVer++;

			posV -=deltaV;
		}
		
		// second verticies must be treaten specially
		else if(i == 1){
			posH = 0.0f;
			for(int j = 0; j < resolutionHorizontal; j++) {
				x = sin(posH) * cos(posV);
				z = cos(posH) * cos(posV);
				y = sin(posV);

				m_lpVerticies[iVer] = MakeLightVertex(x * Radius, y * Radius, z * Radius, x, y, z, m_Color); iVer++;
				
				posH += deltaH;

				// Indicies
				m_lpIndicies[iInd] = 0;										iInd++;
				m_lpIndicies[iInd] = (iVer - 1);							iInd++;
				if(j == resolutionHorizontal - 1) {
					m_lpIndicies[iInd] = 1;									iInd++;
				} else {
					m_lpIndicies[iInd] = iVer;								iInd++;
				}


			}
			posV -=deltaV;
		}

		// last Vertex at the bottom
		else if(i == resolutionVertical) {
			m_lpVerticies[iVer] = MakeLightVertex(0.0f, -Radius, 0.0f, 0.0f, -1.0f, 0.0f, m_Color);

			// set the Indicies
			for(int j = 0; j < resolutionHorizontal; j++) {
				m_lpIndicies[iInd] = iVer;										iInd++;			// bottom vertex

				if(j == resolutionHorizontal - 1 ) {
					m_lpIndicies[iInd] = iVer - j - 1;							iInd++;	
					m_lpIndicies[iInd] = iVer - j - 2 + resolutionHorizontal;	iInd++;	
				} else {
					m_lpIndicies[iInd] = iVer - j - 1;							iInd++;	
					m_lpIndicies[iInd] = iVer - j - 2;							iInd++;	
				}
			}

		}

		// verticies in the middle
		else {
			posH = 0.0f;
			for(int j = 0; j < resolutionHorizontal; j++) {
				x = sin(posH) * cos(posV);
				z = cos(posH) * cos(posV);
				y = sin(posV);

				m_lpVerticies[iVer] = MakeLightVertex(x * Radius, y * Radius, z * Radius, x, y, z, m_Color); iVer++;
				
				posH += deltaH;


				// Idndicies
				m_lpIndicies[iInd] = (iVer - 1) -  resolutionHorizontal;				iInd++;
				m_lpIndicies[iInd] = (iVer - 1);										iInd++;
				if(j == resolutionHorizontal - 1) {
					m_lpIndicies[iInd] = 1 +  resolutionHorizontal * (i - 2);			iInd++;
				} else {
					m_lpIndicies[iInd] = iVer;											iInd++;
				}


				if(j == resolutionHorizontal - 1) {
					m_lpIndicies[iInd] = (iVer - 1);									iInd++;
					m_lpIndicies[iInd] = 1 +  resolutionHorizontal * (i - 1);			iInd++;
					m_lpIndicies[iInd] = 1 + (iVer - 1) - resolutionHorizontal * 2;		iInd++;
				} else {
					m_lpIndicies[iInd] = (iVer - 1) -  resolutionHorizontal;			iInd++;
					m_lpIndicies[iInd] = iVer;											iInd++;
					m_lpIndicies[iInd] = 1 + (iVer - 1) - resolutionHorizontal;			iInd++;
				}


			}
			posV -=deltaV;
		}
	}
	
	// Create the Vertexbuffer

	hr = m_lpD3DDevice->CreateVertexBuffer(NumVerticies *  sizeof(VertexNorm), 
										D3DUSAGE_WRITEONLY, 
										FVF_FORMAT, 
										D3DPOOL_MANAGED, 
										&VB_Sphere, 
										NULL);

	BYTE* VertexBufferStart;

	 // lock Vertexbuffer
	VB_Sphere->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

	 // copy the Verticies into the buffer
	memcpy(VertexBufferStart, m_lpVerticies, NumVerticies *  sizeof(VertexNorm));

	 // unlock Vertexbuffer
	VB_Sphere->Unlock();

	
	// Create the Indexbuffer

	hr = m_lpD3DDevice->CreateIndexBuffer(3 * NumFaces * sizeof(DWORD), D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										D3DFMT_INDEX32, D3DPOOL_MANAGED, 
										&IB_Sphere, NULL);


	BYTE* IndexBufferStart;

	// lock Index Buffer
	IB_Sphere->Lock(0, 0, reinterpret_cast<void**>(&IndexBufferStart), 0);

	// copy the Indices into the buffer
	memcpy(IndexBufferStart, m_lpIndicies, 3 * NumFaces * sizeof(DWORD));

	// unlock Indexbuffer 
	IB_Sphere->Unlock();
	
	return true;

}

void CDefaultSphere::SetWorldMatrix() {
	D3DXMATRIX TransMatrix;
    D3DXMATRIX RotMatrix;
    D3DXMATRIX RotMatrixX, RotMatrixY, RotMatrixZ;
    
    // Rotationsmatrizen anlegen
	D3DXMatrixRotationX(&RotMatrixX, m_vRotation.x);
    D3DXMatrixRotationY(&RotMatrixY, m_vRotation.y);
    D3DXMatrixRotationZ(&RotMatrixZ, m_vRotation.z);

    D3DXMatrixIdentity(&RotMatrix);

    // und miteinander multiplizieren
    D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixX);
    D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixY);
    D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixZ);

	D3DXMatrixTranslation(&TransMatrix, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    // Transformations und Rotationsmatrix zur Weltmatrix zusammenfassen
    D3DXMatrixMultiply(&WorldMatrix, &RotMatrix, &TransMatrix);
    

    D3DXMatrixMultiply(&WorldMatrix, &RotMatrix, &TransMatrix);
}

void CDefaultSphere::Render() {

	m_lpD3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	m_lpD3DDevice->SetFVF(FVF_FORMAT);
	
	 //die Quelle setzen
    m_lpD3DDevice->SetStreamSource(0, VB_Sphere, 0, sizeof(VertexNorm));

	m_lpD3DDevice->SetIndices(IB_Sphere);
	
	m_lpD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NumVerticies, 0, NumFaces);

}

BOOL CDefaultSphere::IsPicked(D3DXVECTOR3 v, D3DXMATRIX ViewMatrix, FLOAT *pU, FLOAT *pV, FLOAT *pDist) {
	BOOL bHit = false;

	D3DXVECTOR3 vPickRayDir;
    D3DXVECTOR3 vPickRayOrig;


	const D3DXMATRIX matWorld = WorldMatrix;
    D3DXMATRIX mWorldView = matWorld * ViewMatrix;
    D3DXMATRIX m;
    D3DXMatrixInverse( &m, NULL, &mWorldView );

    // Transform the screen space pick ray into 3D space
    vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig.x = m._41;
    vPickRayOrig.y = m._42;
    vPickRayOrig.z = m._43;

	
	for(int i = 0; i < NumFaces; i++) {
				
		bHit = bHit || D3DXIntersectTri(&D3DXVECTOR3(m_lpVerticies[m_lpIndicies[3*i]].x, m_lpVerticies[m_lpIndicies[3*i]].y,m_lpVerticies[m_lpIndicies[3*i]].z), 
							&D3DXVECTOR3(m_lpVerticies[m_lpIndicies[3*i+1]].x, m_lpVerticies[m_lpIndicies[3*i+1]].y,m_lpVerticies[m_lpIndicies[3*i+1]].z),
							&D3DXVECTOR3(m_lpVerticies[m_lpIndicies[3*i+2]].x, m_lpVerticies[m_lpIndicies[3*i+2]].y,m_lpVerticies[m_lpIndicies[3*i+2]].z),
							&vPickRayOrig, &vPickRayDir, pU, pV, pDist);
	}

	return bHit;
}

void CDefaultSphere::SetSize(float Radius) {
	if(m_lpVerticies == 0) return;

	
	for(int i = 0; i < NumVerticies; i++) {
		m_lpVerticies[i].x *= Radius / m_Radius;
		m_lpVerticies[i].y *= Radius / m_Radius;
		m_lpVerticies[i].z *= Radius / m_Radius;
	}

	BYTE* VertexBufferStart;

	 //Vertex Buffer sperren
	VB_Sphere->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

	 //Vertices des Dreiecks in den Puffer kopieren
	memcpy(VertexBufferStart, m_lpVerticies, NumVerticies *  sizeof(VertexNorm));

	 //Vertex Buffer wieder freigeben
	VB_Sphere->Unlock();

	m_Radius = Radius;
}


//----------------------------------------------------------------------------------------------------
// The CDefaultCuboid class
//----------------------------------------------------------------------------------------------------

CDefaultCuboid::CDefaultCuboid() {
	VB_Cuboid = NULL;
	IB_Cuboid = NULL;
}

CDefaultCuboid::~CDefaultCuboid() {

	if(VB_Cuboid) {
		VB_Cuboid->Release();
		VB_Cuboid = NULL;
	}

	if(IB_Cuboid) {
		IB_Cuboid->Release();
		IB_Cuboid = NULL;
	}

	delete[] m_lpVerticies;
	delete[] m_lpIndicies;
}


BOOL CDefaultCuboid::CreateCuboid(D3DXVECTOR3 Size) {

	m_Size = Size;

	Size *= 0.5f;

	FVF_FORMAT = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL /*| D3DFVF_TEX1*/;
	DWORD m_Color = 0xFF7C7C7C;
	
	FaceCount = 12;
	VertexCount = 24;

		// Generate the Verticies

	m_lpVerticies = new VertexNorm[VertexCount];
	m_lpIndicies = new DWORD[FaceCount * 3];



	// back face
	m_lpIndicies[0] = 2;		m_lpVerticies[0] = MakeLightVertex(-Size.x, -Size.y, -Size.z, 0.0f, 0.0f, -1.0f, m_Color);
	m_lpIndicies[1] = 1;		m_lpVerticies[1] = MakeLightVertex(Size.x, -Size.y, -Size.z, 0.0f, 0.0f, -1.0f, m_Color);
	m_lpIndicies[2] = 0;		m_lpVerticies[2] = MakeLightVertex(Size.x, Size.y, -Size.z, 0.0f, 0.0f, -1.0f, m_Color);
	m_lpIndicies[3] = 2;		m_lpVerticies[3] = MakeLightVertex(-Size.x, Size.y, -Size.z, 0.0f, 0.0f, -1.0f, m_Color);
	m_lpIndicies[4] = 0;
	m_lpIndicies[5] = 3;

		// front face
	m_lpIndicies[6] = 4;		m_lpVerticies[4] = MakeLightVertex(-Size.x, -Size.y, Size.z, 0.0f, 0.0f, 1.0f, m_Color);
	m_lpIndicies[7] = 5;		m_lpVerticies[5] = MakeLightVertex(Size.x, -Size.y, Size.z, 0.0f, 0.0f, 1.0f, m_Color);
	m_lpIndicies[8] = 6;		m_lpVerticies[6] = MakeLightVertex(Size.x, Size.y, Size.z, 0.0f, 0.0f, 1.0f, m_Color);
	m_lpIndicies[9] = 7;		m_lpVerticies[7] = MakeLightVertex(-Size.x, Size.y, Size.z, 0.0f, 0.0f, 1.0f, m_Color);
	m_lpIndicies[10] = 4;
	m_lpIndicies[11] = 6;

		// bottom face
	m_lpIndicies[12] = 8;		m_lpVerticies[8] = MakeLightVertex(-Size.x, -Size.y, -Size.z, 0.0f, -1.0f, 0.0f, m_Color);
	m_lpIndicies[13] = 9;		m_lpVerticies[9] = MakeLightVertex(Size.x, -Size.y, -Size.z, 0.0f, -1.0f, 0.0f, m_Color);
	m_lpIndicies[14] = 10;		m_lpVerticies[10] = MakeLightVertex(Size.x, -Size.y, Size.z, 0.0f, -1.0f, 0.0f, m_Color);
	m_lpIndicies[15] = 11;		m_lpVerticies[11] = MakeLightVertex(-Size.x, -Size.y, Size.z, 0.0f, -1.0f, 0.0f, m_Color);
	m_lpIndicies[16] = 8;
	m_lpIndicies[17] = 10;

		// top face
	m_lpIndicies[18] = 12;		m_lpVerticies[12] = MakeLightVertex(-Size.x, Size.y, Size.z, 0.0f, 1.0f, 0.0f, m_Color);
	m_lpIndicies[19] = 13;		m_lpVerticies[13] = MakeLightVertex(Size.x, Size.y, Size.z, 0.0f, 1.0f, 0.0f, m_Color);
	m_lpIndicies[20] = 14;		m_lpVerticies[14] = MakeLightVertex(Size.x, Size.y, -Size.z, 0.0f, 1.0f, 0.0f, m_Color);
	m_lpIndicies[21] = 15;		m_lpVerticies[15] = MakeLightVertex(-Size.x, Size.y, -Size.z, 0.0f, 1.0f, 0.0f, m_Color);
	m_lpIndicies[22] = 12;
	m_lpIndicies[23] = 14;

		// left face
	m_lpIndicies[24] = 18;		m_lpVerticies[16] = MakeLightVertex(-Size.x, -Size.y, -Size.z, -1.0f, 0.0f, 0.0f, m_Color);
	m_lpIndicies[25] = 17;		m_lpVerticies[17] = MakeLightVertex(-Size.x, Size.y, -Size.z, -1.0f, 0.0f, 0.0f, m_Color);
	m_lpIndicies[26] = 16;		m_lpVerticies[18] = MakeLightVertex(-Size.x, Size.y, Size.z, -1.0f, 0.0f, 0.0f, m_Color);
	m_lpIndicies[27] = 18;		m_lpVerticies[19] = MakeLightVertex(-Size.x, -Size.y, Size.z, -1.0f, 0.0f, 0.0f, m_Color);
	m_lpIndicies[28] = 16;
	m_lpIndicies[29] = 19;

		// right face
	m_lpIndicies[30] = 22;		m_lpVerticies[20] = MakeLightVertex(Size.x, -Size.y, Size.z, 1.0f, 0.0f, 0.0f, m_Color);
	m_lpIndicies[31] = 21;		m_lpVerticies[21] = MakeLightVertex(Size.x, Size.y, Size.z, 1.0f, 0.0f, 0.0f, m_Color);
	m_lpIndicies[32] = 20;		m_lpVerticies[22] = MakeLightVertex(Size.x, Size.y, -Size.z, 1.0f, 0.0f, 0.0f, m_Color);
	m_lpIndicies[33] = 22;		m_lpVerticies[23] = MakeLightVertex(Size.x, -Size.y, -Size.z, 1.0f, 0.0f, 0.0f, m_Color);
	m_lpIndicies[34] = 20;
	m_lpIndicies[35] = 23;

	// Generate the Vertexbuffer

	m_lpD3DDevice->CreateVertexBuffer(VertexCount *  sizeof(VertexNorm), 
										D3DUSAGE_WRITEONLY, 
										FVF_FORMAT, 
										D3DPOOL_MANAGED, 
										&VB_Cuboid, 
										NULL);

	BYTE* VertexBufferStart;

	 //Vertex Buffer sperren
	VB_Cuboid->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

	 //Vertices des Dreiecks in den Puffer kopieren
	memcpy(VertexBufferStart, m_lpVerticies, VertexCount *  sizeof(VertexNorm));

	 //Vertex Buffer wieder freigeben
	VB_Cuboid->Unlock();

	//delete[] lpVertices;
	

	// Generate the Indexbuffer

	m_lpD3DDevice->CreateIndexBuffer(3 * FaceCount * sizeof(DWORD), D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										D3DFMT_INDEX32, D3DPOOL_MANAGED, 
										&IB_Cuboid, NULL);


	BYTE* IndexBufferStart;

	// Index Buffer sperren
	IB_Cuboid->Lock(0, 0, reinterpret_cast<void**>(&IndexBufferStart), 0);

	// Indices des Dreiecks in den Puffer kopieren
	memcpy(IndexBufferStart, m_lpIndicies, 3 * FaceCount * sizeof(DWORD));

	// Index Buffer wieder freigeben
	IB_Cuboid->Unlock();
	
	return true;
}

void CDefaultCuboid::SetWorldMatrix() {
	D3DXMATRIX TransMatrix;
    D3DXMATRIX RotMatrix;
    D3DXMATRIX RotMatrixX, RotMatrixY, RotMatrixZ;
    
    // Rotationsmatrizen anlegen
	D3DXMatrixRotationX(&RotMatrixX, m_vRotation.x);
    D3DXMatrixRotationY(&RotMatrixY, m_vRotation.y);
    D3DXMatrixRotationZ(&RotMatrixZ, m_vRotation.z);

    D3DXMatrixIdentity(&RotMatrix);

    // und miteinander multiplizieren
    D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixX);
    D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixY);
    D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixZ);

	D3DXMatrixTranslation(&TransMatrix, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    // Transformations und Rotationsmatrix zur Weltmatrix zusammenfassen
    D3DXMatrixMultiply(&WorldMatrix, &RotMatrix, &TransMatrix);
    

    D3DXMatrixMultiply(&WorldMatrix, &RotMatrix, &TransMatrix);
}

void CDefaultCuboid::Render() {

	//m_lpD3DDevice->SetTexture(0,0);
	//m_lpD3DDevice->SetMaterial(&m_lpMaterial);
	
	m_lpD3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	m_lpD3DDevice->SetFVF(FVF_FORMAT);
	
	 //die Quelle setzen
    m_lpD3DDevice->SetStreamSource(0, VB_Cuboid, 0, sizeof(VertexNorm));

	m_lpD3DDevice->SetIndices(IB_Cuboid);
	
	m_lpD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCount, 0, FaceCount);


}

BOOL CDefaultCuboid::IsPicked(D3DXVECTOR3 v, D3DXMATRIX ViewMatrix, FLOAT *pU, FLOAT *pV, FLOAT *pDist) {
	BOOL bHit = false;

	D3DXVECTOR3 vPickRayDir;
    D3DXVECTOR3 vPickRayOrig;


	const D3DXMATRIX matWorld = WorldMatrix;
    D3DXMATRIX mWorldView = matWorld * ViewMatrix;
    D3DXMATRIX m;
    D3DXMatrixInverse( &m, NULL, &mWorldView );

    // Transform the screen space pick ray into 3D space
    vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig.x = m._41;
    vPickRayOrig.y = m._42;
    vPickRayOrig.z = m._43;

	
	for(int i = 0; i < FaceCount; i++) {
		
		
		bHit = bHit || D3DXIntersectTri(&D3DXVECTOR3(m_lpVerticies[m_lpIndicies[3*i]].x, m_lpVerticies[m_lpIndicies[3*i]].y,m_lpVerticies[m_lpIndicies[3*i]].z), 
							&D3DXVECTOR3(m_lpVerticies[m_lpIndicies[3*i+1]].x, m_lpVerticies[m_lpIndicies[3*i+1]].y,m_lpVerticies[m_lpIndicies[3*i+1]].z),
							&D3DXVECTOR3(m_lpVerticies[m_lpIndicies[3*i+2]].x, m_lpVerticies[m_lpIndicies[3*i+2]].y,m_lpVerticies[m_lpIndicies[3*i+2]].z),
							&vPickRayOrig, &vPickRayDir, pU, pV, pDist);
	}
	
	return bHit;
}

void CDefaultCuboid::SetSize(D3DXVECTOR3 Size) {
	if(m_lpVerticies == 0) return;

	m_Size = Size;

	Size *= 0.5f;

	for(int i = 0; i < VertexCount; i++) {
		m_lpVerticies[i].x = abs(m_lpVerticies[i].x) / m_lpVerticies[i].x * Size.x;
		m_lpVerticies[i].y = abs(m_lpVerticies[i].y) / m_lpVerticies[i].y * Size.y;
		m_lpVerticies[i].z = abs(m_lpVerticies[i].z) / m_lpVerticies[i].z * Size.z;
	}

	BYTE* VertexBufferStart;

	 //Vertex Buffer sperren
	VB_Cuboid->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

	 //Vertices des Dreiecks in den Puffer kopieren
	memcpy(VertexBufferStart, m_lpVerticies, VertexCount *  sizeof(VertexNorm));

	 //Vertex Buffer wieder freigeben
	VB_Cuboid->Unlock();
}


//----------------------------------------------------------------------------------------------------
// The CTrace class
//----------------------------------------------------------------------------------------------------

CTrace::CTrace() {
	numPoints = 0;
	TraceVertices = NULL;
	VB_Trace = NULL;

	m_lpLogBook = NULL;
	m_lpD3DDevice = NULL,
	m_lpResmanager = NULL;
}

CTrace::~CTrace() {
	if(TraceVertices != NULL) {
		delete[] TraceVertices;
		TraceVertices = NULL;
	}

	if(VB_Trace != NULL) {
		VB_Trace->Release();
		VB_Trace = NULL;
	}
}

void CTrace::AddPoint(D3DXVECTOR3 position, DWORD Color) {
	numPoints++;


	CustomVertex* tempVertices = new CustomVertex[numPoints];

	if(m_lpLogBook) m_lpLogBook->Succed(L"TraceVertex hinzugefuegt");

	if(TraceVertices) {
		memcpy(tempVertices, TraceVertices, sizeof(CustomVertex) * (numPoints - 1));
		if(m_lpLogBook) m_lpLogBook->Succed(L"memcpy(tempVertices, TraceVertices, sizeof(CustomVertex) * (numPoints - 1));");
	}

	tempVertices[numPoints - 1] = MakeCustomVertex(position.x, position.y, position.z , Color);

	if(m_lpLogBook) m_lpLogBook->Succed(L"tempVertices[numPoints] = MakeCustomVertex(..);");

	if(TraceVertices) delete[] TraceVertices;
	TraceVertices = tempVertices;

	if(m_lpLogBook) m_lpLogBook->Succed(L"TraceVertex hinzugefuegt");
	
	// Zeiger, der auf den Vertex Buffer gesetzt wird
    BYTE* VertexBufferStart;

	if(VB_Trace != NULL) {
		VB_Trace->Release();
		VB_Trace = NULL;
	}

	if(m_lpLogBook) m_lpLogBook->Succed(L"VB_Trace->Release()");


	/*--------------------------------------------------------------------------------------------------*/
	//	Todo:	To get a faster application, try to use one vertexbuffer with a constant size.
	//			Try to add the new verticies with the flag D3DLOCK_NOOVERWRITE to the old vertexbuffer
	//			to minimaze the data stream from system-memory to graphics-memory	
	/*--------------------------------------------------------------------------------------------------*/

 //   // Vertex Buffer fuer das Dreieck anlegen
 //   // die Groesse des Vertex Buffer muss mit
 //   // der Groesse der Struktur, in der das Dreieck
 //   // gespeichert ist, uebereinsimmen
    m_lpD3DDevice->CreateVertexBuffer(numPoints * sizeof(CustomVertex), 
                                    D3DUSAGE_WRITEONLY, 
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE, 
                                    D3DPOOL_MANAGED, 
                                    &VB_Trace, 
                                    NULL);

	if(m_lpLogBook) m_lpLogBook->Succed(L"Vertexbuffer erstellt");

    // Vertex Buffer sperren
    VB_Trace->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), D3DLOCK_DISCARD);		// using a new buffer

    // Vertices des Dreiecks in den Puffer kopieren
    memcpy(VertexBufferStart, TraceVertices, numPoints * sizeof(CustomVertex));

	if(m_lpLogBook) m_lpLogBook->Succed(L"Vertices in Vertexbuffer kopiert");

    // Vertex Buffer wieder freigeben
    VB_Trace->Unlock();
	
}

void CTrace::Render(void) {
	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Das Vertex-Format festlegen
    m_lpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);


	D3DXMATRIX WorldMatrix;
    D3DXMatrixTranslation(&WorldMatrix, 0.0f, 0.0f, 0.0f);

	m_lpD3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	// die Quelle fuer das Grid setzen
    m_lpD3DDevice->SetStreamSource(0, VB_Trace, 0, sizeof(CustomVertex));

	m_lpD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, numPoints - 1);

	m_lpD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//----------------------------------------------------------------------------------------------------
// The CEmitter class
//----------------------------------------------------------------------------------------------------

CEmitter::CEmitter() {

}

CEmitter::~CEmitter() {

}

void CEmitter::Init(void) {
	EmitterMesh.SetLogBookPtr(m_lpLogBook);
	EmitterMesh.Init(m_lpD3DDevice, m_lpResmanager);
	EmitterMesh.Load(L"../data/emitter.x");

	maxFaces = EmitterMesh.GetMaxFaces();

}

void CEmitter::Render() {
	EmitterMesh.SetPosition(m_Position);
	EmitterMesh.SetRotation(m_Rotation.x, m_Rotation.y, m_Rotation.z);

	EmitterMesh.Render(maxFaces);
}

void CEmitter::SetMatrix(void) {
	EmitterMesh.SetPosition(m_Position);
	EmitterMesh.SetRotation(m_Rotation.x, m_Rotation.y, m_Rotation.z);
}

void CEmitter::SetViewAt(D3DXVECTOR3 ViewAt) {
	
	m_Rotation.x  = atan(sqrt(ViewAt.x * ViewAt.x + ViewAt.z * ViewAt.z) / ViewAt.y);

	if(ViewAt.y < 0.0f) m_Rotation.x += PI_F;

	if(ViewAt.z < 0.0f) m_Rotation.y = atan(ViewAt.x / ViewAt.z) + PI_F;

	else m_Rotation.y = atan(ViewAt.x / ViewAt.z);
}


//----------------------------------------------------------------------------------------------------
// Global functions
//----------------------------------------------------------------------------------------------------

CustomVertex MakeCustomVertex(FLOAT cx, FLOAT cy, FLOAT cz, DWORD ccolor){
	CustomVertex v;
	v.x = cx;
	v.y = cy;
	v.z = cz;
	v.color = ccolor;
	return v;
}

CustomVertex MakeCustomVertex(D3DXVECTOR3 position, DWORD Color) {
	CustomVertex v;
	v.x = position.x;
	v.y = position.y;
	v.z = position.z;
	v.color = Color;
	return v;
}

Vertex_XYZ_Nor_Diff MakeLightVertex(FLOAT vx, FLOAT vy, FLOAT vz, FLOAT nx, FLOAT ny, FLOAT nz, DWORD ccolor){
	Vertex_XYZ_Nor_Diff v;
	
	v.x = vx;
	v.y = vy;
	v.z = vz;
	v.nx = nx;
	v.ny = ny;
	v.nz = nz;
	v.color = ccolor;
	
	return v;
}

Vertex_XYZ_Nor_Diff MakeLightVertex(D3DXVECTOR3 position, D3DXVECTOR3 normal, DWORD ccolor) {
	Vertex_XYZ_Nor_Diff v;

	v.x = position.x;
	v.y = position.y;
	v.z = position.z;
	v.nx = normal.x;
	v.ny = normal.y;
	v.nz = normal.z;
	v.color = ccolor;

	return v;
}

VertexTex MakeVertexTex(FLOAT cx, FLOAT cy, FLOAT cz, FLOAT nx, FLOAT ny, FLOAT nz, DWORD ccolor) {

	
	VertexTex v;

	v.x = cx;
	v.y = cy;
	v.z = cz;
	v.nx = nx;
	v.ny = ny;
	v.nz = nz;
	//v.tu = 0.0f;
	//v.tv = 0.0f;
	v.color = ccolor;

	return v;
}

//--------------------------------------------------------------------------------------
// Given a ray origin (orig) and direction (dir), and three vertices of a triangle, this
// function returns TRUE and the interpolated texture coordinates if the ray intersects 
// the triangle
//--------------------------------------------------------------------------------------
BOOL IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, 
                        D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, 
                        FLOAT* t, FLOAT* u, FLOAT* v )
{
    // Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &dir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );

    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = orig - v0;
    }
    else
    {
        tvec = v0 - orig;
        det = -det;
    }

    if( det < 0.0001f )
        return FALSE;

    // Calculate U parameter and test bounds
    *u = D3DXVec3Dot( &tvec, &pvec );
    if( *u < 0.0f || *u > det )
        return FALSE;

    // Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );

    // Calculate V parameter and test bounds
    *v = D3DXVec3Dot( &dir, &qvec );
    if( *v < 0.0f || *u + *v > det )
        return FALSE;

    // Calculate t, scale parameters, ray intersects triangle
    *t = D3DXVec3Dot( &edge2, &qvec );
    FLOAT fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

    return TRUE;
}