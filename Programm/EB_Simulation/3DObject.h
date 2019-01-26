#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <strsafe.h>

#include "ResourceManager.h"
#include "LogBook.h"

#pragma warning (disable : 4996 )
#pragma warning (disable : 4995 )


#pragma once

// Klasse fuer die Speicherung von 3D-Objekten (auf der Basis von X-Files)
class C3DObject

{

protected:

		// Zeiger auf den Progressive-Mesh
        LPD3DXPMESH m_lpPMesh;
		D3DXMATRIX WorldMatrix;
		DWORD FVF;

		CLogBook* m_lpLogBook;

    public:

        C3DObject();
        virtual ~C3DObject();

        // Laden des Meshes
        BOOL Load(WCHAR* xfile);

        // Initialisierung der  Klasse
        void Init(LPDIRECT3DDEVICE9 lpD3DDevice, CResourceManager* Manager);

        // Rendern des Meshes - mit moeglichst nFaces
        void Render(DWORD nFaces);

        // Position des Objekts setzen
        void SetPosition(D3DXVECTOR3 vPos);

        // Rotation des Objekts setzen
        void SetRotation(float RotX, float RotY, float RotZ);

		LPD3DXPMESH GetMeshPtr(void) {return m_lpPMesh; }

        // Anzahl der Faces zurueckgeben die maximal dargestellt werden koennen
        DWORD GetMaxFaces(void);

		D3DXMATRIX GetWorldMatrix(void) { return WorldMatrix; }

		DWORD GetFVF(void) { return FVF; }

		void SetLogBookPtr(CLogBook* lpLogBook) { m_lpLogBook = lpLogBook; }

    protected:

        // das Direct3D Device;
        LPDIRECT3DDEVICE9 m_lpD3DDevice;

        // Zeiger auf den ResourceManager
        CResourceManager* m_lpResManager;

        // Anzahl der Materialien
        unsigned long m_nNumMaterials;

        // das Array der Materialien
        D3DMATERIAL9* m_lpMeshMaterials;

        // das Array mit den Texturnamen
        WCHAR** m_lpMeshTextures;

        // Position des Meshes in der 3D-Welt
        D3DXVECTOR3 m_vPosition;

        // Rotation des Meshes um die Achsen
        float m_RotX;
        float m_RotY;
        float m_RotZ;
};