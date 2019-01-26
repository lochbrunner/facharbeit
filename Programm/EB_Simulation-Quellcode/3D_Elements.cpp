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
		if(x != 0.0f){
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
		if(z != 0.0f) {
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
// The CArrow class
//----------------------------------------------------------------------------------------------------

CArrow::CArrow(){
	m_Position = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

CArrow::~CArrow(){
	DeInit();
}

void CArrow::Init() {
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

void CArrow::DeInit() {
	
	delete[] lpD3Elements;

	lpD3Elements = NULL;

	if(NULL != VB_Lines) {
        VB_Lines->Release();
        VB_Lines = NULL;
    }
	
}

void CArrow::Render(D3DXVECTOR3 PositionCamera){

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

}

CDefaultSphere::~CDefaultSphere() {

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
    VB_Trace->Lock(0, 0, reinterpret_cast<void**>(&VertexBufferStart), 0);

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