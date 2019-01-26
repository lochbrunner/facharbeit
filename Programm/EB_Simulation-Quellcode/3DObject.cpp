
#include "3DObject.h"


C3DObject::C3DObject()
{
    // das Direct3D-Device
    m_lpD3DDevice = NULL;

    // Zeiger auf den ResourceManager
    m_lpResManager = NULL;

    // Anzahl der Materialien
    m_nNumMaterials = 0;

    // das Array der Materialien
    m_lpMeshMaterials = NULL;

    // das Array mit den Texturnamen
    m_lpMeshTextures = NULL;

    // Zeiger auf den Mesh
    m_lpPMesh = NULL;

    // Position des Meshes in der 3D-Welt
    m_vPosition.x = 0.0f;
    m_vPosition.y = 0.0f;
    m_vPosition.z = 0.0f;

    // Rotation des Meshes um die Achsen
    m_RotX = 0.0f;
    m_RotY = 0.0f;
    m_RotZ = 0.0f;

	FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
}

// im Destruktor werden alle Objekte geloescht
C3DObject::~C3DObject()
{
    if(m_lpMeshMaterials)
    {
        delete[] m_lpMeshMaterials;
    }

	

    if(m_lpMeshTextures)
    {
        for(DWORD i=0;i<m_nNumMaterials;i++)
        {
            delete[] m_lpMeshTextures[i];
        }

        delete[] m_lpMeshTextures;
    }

	

    if(m_lpPMesh != NULL)
    {
        m_lpPMesh->Release();
        m_lpPMesh = NULL;
    }
}

// Laden des Meshes
BOOL C3DObject::Load(WCHAR* xfile) {

	// First check if the file exists

	errno_t err;
	FILE* File = NULL;
	WCHAR text[256];

	if( (err  = _wfopen_s( &File, xfile, L"r" )) != 0  && File == NULL) {
		StringCchPrintf(text, 512, L"Die Datei namens \"%s\" konnte nicht gefunden werden.", xfile);
		m_lpLogBook->Error(L"C3DObject::Load", text, 0);
		return false;
	} else {
		StringCchPrintf(text, 512, L"Die Datei namens \" %s \" wurde gefunden."	, xfile);
		m_lpLogBook->Succed(L"C3DObject::Load", text);
		fclose(File);
	}


    LPD3DXMESH lpMesh = NULL;

    LPD3DXBUFFER lpMaterialBuffer;

    if(FAILED(D3DXLoadMeshFromX(xfile, 
                      D3DXMESH_MANAGED, 
                      m_lpD3DDevice, 
                      NULL, 
                      &lpMaterialBuffer, 
                      NULL, 
                      &m_nNumMaterials, 
                      &lpMesh)))   {
        MessageBox(0, L"Mesh konnte nicht geladen werden.", L"Fehler", MB_OK);
    }

    // Zeiger auf die verwendeten Materialien
    D3DXMATERIAL* lpMaterials = (D3DXMATERIAL*)lpMaterialBuffer->GetBufferPointer();

    // Fehler fuer Materialien und Texturen anlegen
    m_lpMeshMaterials = new D3DMATERIAL9[m_nNumMaterials];
    m_lpMeshTextures = new WCHAR*[m_nNumMaterials];

    // Materialien und Texturen kopieren
    for(DWORD i=0;i<m_nNumMaterials;i++)
    {

        m_lpMeshMaterials[i] = lpMaterials[i].MatD3D;
        m_lpMeshMaterials[i].Ambient = m_lpMeshMaterials[i].Diffuse;

        // wenn Textur vorhanden
        if(NULL != lpMaterials[i].pTextureFilename && false)
        {

            m_lpMeshTextures[i] = new WCHAR[strlen(lpMaterials[i].pTextureFilename)+14+1];

            wcscpy(m_lpMeshTextures[i], L"../data/");
            wcscat(m_lpMeshTextures[i], reinterpret_cast<WCHAR*>(lpMaterials[i].pTextureFilename));
			
            m_lpResManager->AddTexture(m_lpMeshTextures[i]);
        }
        else
        {
            m_lpMeshTextures[i] = NULL;
        }
    }

    // Erzeugung des Progressive Mesh
    LPD3DXBUFFER pBuffer = NULL;
    LPD3DXMESH pMeshTemp = NULL;

    // Anzahl der Faces (Dreiecke) ermitteln
    int nNumFaces = lpMesh->GetNumFaces();

    // Adjazenz Informationen generieren
    DWORD *pAdjacency1 = new DWORD[nNumFaces * 3];
    DWORD *pAdjacency2 = new DWORD[nNumFaces * 3];

    lpMesh->GenerateAdjacency(0.0f, pAdjacency1);

    // Mesh Bereinigen - fuehrt Operationen aus, wie Umsortieren und
    // Zusammenfassen von Vertices. Diese Funktion muss aufgerufen werden
    // damit der Mesh vereinfacht werden kann
    D3DXCleanMesh(D3DXCLEAN_SIMPLIFICATION, lpMesh, pAdjacency1, &pMeshTemp, pAdjacency2, &pBuffer);

    // Pruefen, ob der Mesh gueltig ist
    if(FAILED(D3DXValidMesh(pMeshTemp, pAdjacency2, &pBuffer)))
    {
        MessageBox(0, L"Mesh konnte nicht validiert werden.", L"Fehler", MB_OK);
    }

    lpMesh->Release();
    lpMesh = pMeshTemp;


    // Progressive Mesh anlegen - dadurch kann der Mesh in verschiedenen Detailstufen
    // gerendert werden
    D3DXGeneratePMesh(lpMesh, pAdjacency2, NULL, NULL, 300, D3DXMESHSIMP_FACE, &m_lpPMesh);

    // temporaere Objekte freigeben
    delete[] pAdjacency1;
    delete[] pAdjacency2;

    lpMesh->Release();

    return TRUE;
}

// Position des Objekts setzen
void C3DObject::SetPosition(D3DXVECTOR3 vPos)
{
    m_vPosition = vPos;

	D3DXMATRIX TransMatrix;
    D3DXMATRIX RotMatrix;
    D3DXMATRIX RotMatrixX, RotMatrixY, RotMatrixZ;
    
    // Rotationsmatrizen anlegen
    D3DXMatrixRotationX(&RotMatrixX, m_RotX);
    D3DXMatrixRotationY(&RotMatrixY, m_RotY);
    D3DXMatrixRotationZ(&RotMatrixZ, m_RotZ);

    D3DXMatrixIdentity(&RotMatrix);

    // und miteinander multiplizieren
    D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixX);
    D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixY);
    D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixZ);


    // Transformations und Rotationsmatrix zur Weltmatrix zusammenfassen
    D3DXMatrixMultiply(&WorldMatrix, &RotMatrix, &TransMatrix);

    D3DXMatrixTranslation(&TransMatrix, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    D3DXMatrixMultiply(&WorldMatrix, &RotMatrix, &TransMatrix);
}


// Rotation des Objekts setzen
void C3DObject::SetRotation(float RotX, float RotY, float RotZ)
{
    m_RotX = RotX;
    m_RotY = RotY;
    m_RotZ = RotZ;
}


// hier geben Sie die Adresse des ResourceManager an
void C3DObject::Init(LPDIRECT3DDEVICE9 lpD3DDevice, CResourceManager* ResManager)
{
    m_lpD3DDevice  = lpD3DDevice;
    m_lpResManager = ResManager;
}

// Anzahl der Vertices zurueckgeben
DWORD C3DObject::GetMaxFaces(void)
{
    return m_lpPMesh->GetMaxFaces();
}



// Rendern des Meshes
void C3DObject::Render(DWORD nFaces)
{
    //D3DXMATRIX TransMatrix;
    //D3DXMATRIX RotMatrix;
    //D3DXMATRIX RotMatrixX, RotMatrixY, RotMatrixZ;
    //
    //// Rotationsmatrizen anlegen
    //D3DXMatrixRotationX(&RotMatrixX, m_RotX);
    //D3DXMatrixRotationY(&RotMatrixY, m_RotY);
    //D3DXMatrixRotationZ(&RotMatrixZ, m_RotZ);

    //D3DXMatrixIdentity(&RotMatrix);

    //// und miteinander multiplizieren
    //D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixX);
    //D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixY);
    //D3DXMatrixMultiply(&RotMatrix, &RotMatrix, &RotMatrixZ);


    //// Transformations und Rotationsmatrix zur Weltmatrix zusammenfassen
    //D3DXMatrixMultiply(&WorldMatrix, &RotMatrix, &TransMatrix);

    //D3DXMatrixTranslation(&TransMatrix, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    //D3DXMatrixMultiply(&WorldMatrix, &RotMatrix, &TransMatrix);

	m_lpD3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

    // durch alle Materialien/Texturen laufen
    for(DWORD i=0;i<m_nNumMaterials;i++)
    {
        // Material setzen
        m_lpD3DDevice->SetMaterial(&m_lpMeshMaterials[i]);

        // Textur setzen
        if(NULL != m_lpMeshTextures[i])
        {
            m_lpD3DDevice->SetTexture(0, m_lpResManager->GetTexture(m_lpMeshTextures[i]));
        }
        else
        {
            m_lpD3DDevice->SetTexture(0, NULL);
        }

        // Mesh mit moeglichst genau nFaces Faces darstellen
        m_lpPMesh->SetNumFaces(nFaces);

        // Mesh rendern
        m_lpPMesh->DrawSubset(i);
    }

    m_lpD3DDevice->SetTexture(0, NULL);
}