#include "LogBook.h"


CLogBook::CLogBook(LPCWSTR name, LPCWSTR version){
	WCHAR time[64];
	WCHAR date[64];
	_wstrtime_s(time, 9);
	_wstrdate_s(date, 9);

	// Delete all "/"
	for(int i = 0; i < 64; i++) {
		if(time[i] == L'/') time[i] = L'-';
		if(date[i] == L'/') date[i] = L'-';
		if(time[i] == L':') time[i] = L'-';
	}
	
	StringCchPrintf(FileName, sizeof(FileName) / sizeof(WCHAR), L"%s_%s_%s.html", LOGBOOK_FILE, date, time);

	_wstrtime_s(time, 9);
	_wstrdate_s(date, 9);
	//fopen_s(&Datei, LOGBOOK_FILE, "w");
	_wfopen_s(&Datei, FileName, L"w");
	if(Datei == NULL){
		WCHAR Text[512];
		StringCchPrintf(Text, 512, L"Logbook  %s konnte nicht geöffnet werden", FileName);
		MessageBox(0,Text, L"Fehler", MB_OK |MB_ICONEXCLAMATION); 
	}
	fwprintf(Datei, L"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n");
	fwprintf(Datei, L"   \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n");
	fwprintf(Datei, L"<html xmlns=\"http://www.w3.org/199/xhtml\"\n");
	fwprintf(Datei, L"   xml:lang=\"de\" lang= \"de\">\n");
	fwprintf(Datei, L"<head>\n"); 
    fwprintf(Datei, L"   <meta http-equiv=\"content-type\" content=\"text/html: charset=utf-8\" />\n"); 
    fwprintf(Datei, L"   <title>Logbook of %s %s </title>\n", name, version);
    fwprintf(Datei, L"   <meta name=\"author-personal\" content=\"Matthias L.\" />\n");
	fwprintf(Datei, L"   <meta name=\"author-mail\" content=\"mailto:Matthias_Lochbrunner@web.de\" />\n");
    fwprintf(Datei, L"   <meta name=\"describtion\" content=\"A Logbook of Errors and Warnings of the proceeding programm\" />\n"); 
    fwprintf(Datei, L"</head>\n");
    fwprintf(Datei, L"<body>\n");
	fwprintf(Datei, L"<table cellspacing='0' cellpading='0' width='1000%' bgcolor='#DFDFE5'><tr><br>\n");
	fwprintf(Datei, L"   <td>\n");
	fwprintf(Datei, L"   <h1>Logbook of %s<br></h1>\n", name);
	fwprintf(Datei, L"   </td>\n");
	fwprintf(Datei, L"</table>\n");
	fwprintf(Datei, L"<p>version %s</p>\n", version);

#ifdef _DEBUG
	fwprintf(Datei, L"<p>mode: debug</p>\n");
#else 
	fwprintf(Datei, L"<p>mode: release</p>\n");
#endif

	fwprintf(Datei, L"<p>systemdate: %s - %s</p>\n", date, time);
	fwprintf(Datei, L"<a href= \"mailto:vatix@fkrauthan.de?subject=Logbook\">Please send us this Bugreport</a><br><br></br></br>\n");
	fwprintf(Datei, L"<table>\n");
	fwprintf(Datei, L"<colgroup>\n");
	fwprintf(Datei, L"   <col width=\"70\" />\n");
	fwprintf(Datei, L"   <col width=\"280\" />\n");
	fwprintf(Datei, L"   <col width=\"420\" />\n");
	fwprintf(Datei, L"   <col width=\"100\" />\n");
	fwprintf(Datei, L"</colgroup\n");
	fwprintf(Datei, L"<thead>\n");
	fwprintf(Datei, L"   <tr>\n");
	fwprintf(Datei, L"      <th>Status</th>\n");
	fwprintf(Datei, L"      <th>Function/Methode</th>\n");
	fwprintf(Datei, L"      <th>Describtion</th>\n");
	fwprintf(Datei, L"      <th>Error-Code</th>\n");
	fwprintf(Datei, L"   </tr>\n");
	fwprintf(Datei, L"</thead>\n");
	fwprintf(Datei, L"<tbody>\n");
	fclose(Datei);
}


CLogBook::~CLogBook(){
	//fopen_s(&Datei, LOGBOOK_FILE, "a");
	_wfopen_s(&Datei, FileName, L"a");
	fwprintf(Datei, L"</tbody>\n");
	fwprintf(Datei, L"</table>\n");
	fwprintf(Datei, L"<table cellspacing='0' cellpading='0' width='1000%' bgcolor='#DFDFE5'><tr><br>\n");
	fwprintf(Datei, L"   <td>\n");
	fwprintf(Datei, L"   <font  size='+2' text-align: center>\n");
	fwprintf(Datei, L"      <p>Logbook closed</p><p></p>\n");
	fwprintf(Datei, L"   </td>\n");
	fwprintf(Datei, L"</table>\n");
	fwprintf(Datei, L"</body>\n");
	fwprintf(Datei, L"</html>\n");
	fclose(Datei);
};

void CLogBook::Succed(LPCWSTR location){
	//fopen_s(&Datei, LOGBOOK_FILE, "a");
	_wfopen_s(&Datei, FileName, L"a");
	fwprintf(Datei, L"   <tr>\n");
	fwprintf(Datei, L"      <th><font Color=#11ff11><b>Successful</b></font></th>\n");
	fwprintf(Datei, L"      <th><font Color=#666666> %s </font></th>\n", location);
	fwprintf(Datei, L"      <th>-------------------------</th>\n");
	fwprintf(Datei, L"      <th>-----</th>\n");
	fwprintf(Datei, L"   </tr>\n");
	fclose(Datei);
}

void CLogBook::Succed(LPCWSTR location, LPWSTR describtion) {
	//fopen_s(&Datei, LOGBOOK_FILE, "a");
	_wfopen_s(&Datei, FileName, L"a");
	fwprintf(Datei, L"   <tr>\n");
	fwprintf(Datei, L"      <th><font Color=#11ff11><b>Successful</b></font></th>\n");
	fwprintf(Datei, L"      <th><font Color=#666666> %s </font></th>\n", location);
	fwprintf(Datei, L"      <th> %s </th>\n", describtion);
	fwprintf(Datei, L"      <th>-----</th>\n");
	fwprintf(Datei, L"   </tr>\n");
	fclose(Datei);
}

void CLogBook::Succed(LPCWSTR location, LPSTR describtion) {
	//fopen_s(&Datei, LOGBOOK_FILE, "a");
	_wfopen_s(&Datei, FileName, L"a");
	fwprintf(Datei, L"   <tr>\n");
	fwprintf(Datei, L"      <th><font Color=#11ff11><b>Successful</b></font></th>\n");
	fwprintf(Datei, L"      <th><font Color=#666666> %s </font></th>\n", location);
	fprintf(Datei, "      <th> %s </th>\n", describtion);
	fwprintf(Datei, L"      <th>-----</th>\n");
	fwprintf(Datei, L"   </tr>\n");
	fclose(Datei);
}


void CLogBook::Warning(LPCWSTR location, LPCWSTR reason, const int ErrorCode){
	//fopen_s(&Datei,LOGBOOK_FILE, "a");
	_wfopen_s(&Datei, FileName, L"a");
	fwprintf(Datei, L"   <tr>\n");
	fwprintf(Datei, L"      <th><font color=#ee9900><b>Warning</b></font></th>\n");
	fwprintf(Datei, L"      <th><font color=#666666> %s </font></th>\n", location);
	fwprintf(Datei, L"      <th> %s </th>\n", reason);
	fwprintf(Datei, L"      <th> %d </th>\n", ErrorCode);
	fwprintf(Datei, L"   </tr>\n");
	fclose(Datei);
}

void CLogBook::Error(LPCWSTR location, LPCWSTR reason, const int ErrorCode){
	//fopen_s(&Datei,LOGBOOK_FILE, "a");
	_wfopen_s(&Datei, FileName, L"a");
	fwprintf(Datei, L"   <tr>\n");
	fwprintf(Datei, L"      <th><font color=#ee0000><b>Failed</b></font></th>\n");
	fwprintf(Datei, L"      <th><font color=#666666> %s </font></th>\n", location);
	fwprintf(Datei, L"      <th> %s </th>\n", reason);
	fwprintf(Datei, L"      <th> %d </th>\n", ErrorCode);
	fwprintf(Datei, L"   </tr>\n");
	fclose(Datei);
}

void CLogBook::Error(LPCWSTR location, HRESULT Error, const int ErrorCode){
	//fopen_s(&Datei,LOGBOOK_FILE, "a");
	_wfopen_s(&Datei, FileName, L"a");
	fwprintf(Datei, L"   <tr>\n");
	fwprintf(Datei, L"      <th><font color=#ee0000><b>Failed</b></font></th>\n");
	fwprintf(Datei, L"      <th><font color=#666666> %s </font></th>\n", location);
	fwprintf(Datei, L"      <th> %s - %s </th>\n", DXGetErrorString9(Error), DXGetErrorDescription9(Error));
	fwprintf(Datei, L"      <th> %d </th>\n", ErrorCode);
	fwprintf(Datei, L"   </tr>\n");
	fclose(Datei);
}

void CLogBook::Title(LPCWSTR text) {
	//fopen_s(&Datei,LOGBOOK_FILE, "a");
	_wfopen_s(&Datei, FileName, L"a");
	fwprintf(Datei, L"</tbody>\n");
	fwprintf(Datei, L"</table>\n");
	fwprintf(Datei, L"<table cellspacing='0' cellpading='0' width='1000%' bgcolor='#DFDFE5'><tr><br>\n");
	fwprintf(Datei, L"   <td>\n");
	fwprintf(Datei, L"   <font  size='+2'>\n");
	fwprintf(Datei, L"      %s\n", text);
	fwprintf(Datei, L"   </font>\n");
	fwprintf(Datei, L"   </td>\n");
	fwprintf(Datei, L"</table>\n");
	fwprintf(Datei, L"<table>\n");
	fwprintf(Datei, L"<colgroup>\n");
	fwprintf(Datei, L"   <col width=\"50\" />\n");
	fwprintf(Datei, L"   <col width=\"280\" />\n");
	fwprintf(Datei, L"   <col width=\"420\" />\n");
	fwprintf(Datei, L"   <col width=\"100\" />\n");
	fwprintf(Datei, L"</colgroup\n");
	fwprintf(Datei, L"<tbody>\n");
	fclose(Datei);
}

void CLogBook::SubTitle(LPCWSTR text) {
	//fopen_s(&Datei,LOGBOOK_FILE, "a");
	_wfopen_s(&Datei, FileName, L"a");
	fwprintf(Datei, L"</tbody>\n");
	fwprintf(Datei, L"</table>\n");
	fwprintf(Datei, L"<table cellspacing='0' cellpading='0' width='1000%' bgcolor='#DFDFE5'><tr><br>\n");
	fwprintf(Datei, L"   <td>\n");
	fwprintf(Datei, L"   <font  size='+1'>\n");
	fwprintf(Datei, L"      %s\n", text);
	fwprintf(Datei, L"   </td>\n");
	fwprintf(Datei, L"</table>\n");
	fwprintf(Datei, L"<table>\n");
	fwprintf(Datei, L"<colgroup>\n");
	fwprintf(Datei, L"   <col width=\"50\" />\n");
	fwprintf(Datei, L"   <col width=\"280\" />\n");
	fwprintf(Datei, L"   <col width=\"420\" />\n");
	fwprintf(Datei, L"   <col width=\"100\" />\n");
	fwprintf(Datei, L"</colgroup\n");
	fwprintf(Datei, L"<tbody>\n");
	fclose(Datei);
}
