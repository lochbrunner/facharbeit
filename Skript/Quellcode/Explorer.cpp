#include "Explorer.h"

CExplorerWindow::CExplorerWindow() {

}

CExplorerWindow::~CExplorerWindow() {
	m_lpPhysics = NULL;

}

BOOL CExplorerWindow::Create(HINSTANCE hInstance, HWND parentHWnd, UINT x, UINT y, UINT width, UINT height) {
	m_hWnd = CreateWindowEx(NULL,                   // Keine erweiterten Stile nutzen
                          L"WindowClass",           // Klassenname
                          L"Explorer",				// Fenstertitel
                          WS_NOMAXIMIZEBOX |		// Fenster
						  WS_VISIBLE,				// Eigenschaften
                          x, y,						// Anfangsposition
                          width, height,			// und Groesse des Fensters
                          parentHWnd,               // Handle des Elternfensters
                          NULL,						// Handle des Menues
                          hInstance,				// Anwendungsinstanz
                          NULL);					// wird nicht benoetigt

	if(!m_hWnd) return FALSE;


	// Tab erstellen

	//INITCOMMONCONTROLSEX ictrlex;
	//ictrlex.dwSize	= sizeof(INITCOMMONCONTROLSEX);
	//ictrlex.dwICC	= ICC_TAB_CLASSES ;				
	//InitCommonControlsEx(&ictrlex);

	//hwndTab = CreateWindowExW(WS_EX_CLIENTEDGE,WC_TABCONTROL, L"",
	//	WS_CHILD |WS_BORDER| WS_VISIBLE /*| WS_VSCROLL*/,
	//	0, 0, width - 10, height - 10,
	//	m_hWnd, NULL , hInstance, NULL);

	//TabCtrl_InsertItem(hwndTab, 0, 


	// Treeview erstellen

	INITCOMMONCONTROLSEX ictrlex;
	ictrlex.dwSize	= sizeof(INITCOMMONCONTROLSEX);
	ictrlex.dwICC	= ICC_TREEVIEW_CLASSES ;				//wir wollen ja dir Treeviewklassen
	InitCommonControlsEx(&ictrlex);		

	hwndTree = CreateWindowExW(WS_EX_CLIENTEDGE,WC_TREEVIEW, L"",
		WS_CHILD |WS_BORDER| WS_VISIBLE /*| WS_VSCROLL*/,
		0, height - 10, width - 10, 100,
		m_hWnd, NULL , hInstance, NULL);

	/*hwndList = CreateWindow(L"listbox", L"Mein erster Button",WS_CHILD | WS_VISIBLE | ES_LEFT,
	0, 0, width - 10, height - 100, m_hWnd, NULL,
	hInstance, NULL);*/

	//SendMessage(hwndList, 

	UpdateList();

	return TRUE;
}

void CExplorerWindow::UpdateList(void) {

	TreeView_DeleteAllItems(hwndTree);

	TVITEM Item;
	TVINSERTSTRUCT istr;
	HTREEITEM par;

	Item.mask=TVIF_TEXT|TVIF_CHILDREN;
	Item.pszText = L"Scene";
	Item.cchTextMax = 99;
	Item.cChildren = 3;

	istr.hParent = NULL;
	istr.hInsertAfter = TVI_LAST;
	istr.item = Item;
	par = TreeView_InsertItem(hwndTree,&istr);					//und rein mit unserem Item

	Item.pszText = L"Protonen";
	istr.item = Item;
	istr.hParent = par;
	ProtoneList = TreeView_InsertItem(hwndTree,&istr);

	Item.pszText = L"Elektron";
	istr.item = Item;
	ElectroneList = TreeView_InsertItem(hwndTree,&istr);

	
	list<Protone*>::iterator itP;

	for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {
		Item.pszText = (*itP)->Name;
		istr.hParent = ProtoneList;
		istr.item = Item;
		TreeView_InsertItem(hwndTree,&istr);
	}
	

	list<Electrone*>::iterator itE;

	for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {
		Item.pszText = (*itE)->Name;
		istr.hParent = ElectroneList;
		istr.item = Item;
		TreeView_InsertItem(hwndTree,&istr);
	}

}

void CExplorerWindow::SetCPhysicsPtr(CPhysics* lpPhysics) {
	m_lpPhysics = lpPhysics; 

	m_lpListProtone = m_lpPhysics->GetListProtonePtr();
	m_lpListElectrone = m_lpPhysics->GetListElectronePtr();

}

BOOL CExplorerWindow::IsHwndMemberOfThis(HWND hwnd) {
	return (hwnd == m_hWnd) || (hwnd == hwndTree);
}

void CExplorerWindow::MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam){
	switch(msg) {
		case WM_CLOSE:
				m_OnWindowClosed(m_ID);
			break;

		case WM_NOTIFY: 
				switch (((LPNMHDR) lparam)->code) { //welche Message will uns unser Treeviewelement mitteilen?
					case TVN_SELCHANGED:				//aha die auswahl hat sich geändert
						TVITEM* item = &((LPNMTREEVIEW) lparam)->itemNew;

						SendDlgItemMessage(hwndTree, 0, TVM_GETNEXTITEM,TVGN_CARET,(LPARAM)item);

						//TVITEM* item = &((LPNMTREEVIEW) lparam)->itemNew;

						
						NMTREEVIEW current_selection; 

					    //Markierung des Treeviews geändert 
					    if(reinterpret_cast<LPNMHDR>(lparam)->code==TVN_SELCHANGED) 
					    { 
						   //Neue Markierung speichern 
						   current_selection=*reinterpret_cast<LPNMTREEVIEW>(lparam); 
					    }

						item = &current_selection.itemNew;

						//item = TreeView_GetNextItem(hwndTree, NULL,  TVGN_CARET);
	
						
						SendDlgItemMessage(hwndTree, 0, TVM_GETITEM, TVGN_CARET,(LPARAM)item);

						(item->pszText);
						//MessageBoxW(0, item->pszText, L"Info", MB_OK);

						list<Protone*>::iterator itP;

						for(itP = m_lpListProtone->begin(); itP != m_lpListProtone->end(); itP++) {
							if(wcscmp(item->pszText, (*itP)->Name) == 0) {
								(*itP)->Position.y += 1;
								PostQuitMessage(0);
								
							}

						}
						

						list<Electrone*>::iterator itE;

						for(itE = m_lpListElectrone->begin(); itE != m_lpListElectrone->end(); itE++) {
							if(item->pszText == (*itE)->Name){
								(*itE)->Position.y += 1;
							}
						}
						
						break;
					}
			break;
	}
}

