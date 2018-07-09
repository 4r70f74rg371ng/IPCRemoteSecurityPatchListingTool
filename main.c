#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <msi.h>

#include "PaintObjects.h"
#include "PsToolBin.h"
#include "GetLog.h"

static TCHAR szWindowClass[] = _T("win32app");
//static TCHAR szTitle[] = _T("Win32 Guided Tour Application"); 
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);  

PrintObjects* g_pPO = NULL;

#define IDC_EDIT 0x8001
#define IDC_TEXTHOST 0x8002
#define IDC_TEXTUSER 0x8003
#define IDC_TEXTPASS 0x8004
#define IDC_BUTTON 0x8005
#define IDC_BUTTONTEST 0x8006
char textbuffer[1024000] = "";

#define PRINT_OBJECT_TYPE_FONT 1
typedef struct _PrintObjectFont{
	int x;
	int y;
	int length;
	TCHAR* string;
}PrintObjectFont;

void EnumerationWindowsUpdates(HWND hWnd, char* ip){
	int now = 0;
	char szPatchCode[10240] = "";
	char szTargetProductCode[10240] = "";
	DWORD pcchTargetUserSid[10240] = { 0 };
	DWORD dwRet = 0;
	char szDisplayName[10240] = "";
	MSIINSTALLCONTEXT  dwTargetProductContext;
	UINT uintRet = ERROR_SUCCESS;
	HWND hedit;
	char cmd [1024] = "";
	
	textbuffer[0] = '\0';

	for(now = 0; uintRet == ERROR_SUCCESS;now++) {
		uintRet = MsiEnumPatchesExA(
			NULL, // If NULL, the patches for all products under the specified context are enumerated.
			NULL, // This parameter can be set to NULL to specify the current user.
			MSIINSTALLCONTEXT_USERMANAGED | MSIINSTALLCONTEXT_USERUNMANAGED | MSIINSTALLCONTEXT_MACHINE,
			MSIPATCHSTATE_ALL,
			now,
			szPatchCode,
			szTargetProductCode,
			&dwTargetProductContext,
			NULL,
			pcchTargetUserSid);
		if (uintRet == ERROR_SUCCESS) {
			dwRet = 10240;
			//printf("\"%s\" \"%s\" \"%s\" \n", szPatchCode, szTargetProductCode, pcchTargetUserSid);
			MsiGetPatchInfoExA(szPatchCode, szTargetProductCode, NULL, (MSIINSTALLCONTEXT)4, "DisplayName", szDisplayName, &dwRet);
			//printf("\"%s\" \"%s\" \"%s\" \"%s\"\n", szPatchCode, szTargetProductCode, pcchTargetUserSid, szDisplayName);
			sprintf(cmd, "\"%s\" \"%s\" \"%s\" \"%s\"\r\n", szPatchCode, szTargetProductCode, pcchTargetUserSid, szDisplayName);
			strcat(textbuffer,cmd);
		}
	}
	hedit = GetDlgItem(hWnd, IDC_EDIT);
	SetWindowTextA(hedit, textbuffer);
}

void ExtractGetLog(){
	FILE* fp = NULL;
	long long i;
	
	fp = fopen("Z:\\getlog.exe", "wb");
	
	for(i=0;i<getlog_len;i++){
		fputc(getlog_exe[i], fp);
	}
	
	fclose(fp);
}

void TestIPC(HWND hWnd, char* host, char* user, char* pass){
	char cmd [4000] = "";
	HWND hedit;
	FILE* readlog = NULL;
	
	sprintf(cmd, "net use \\\\%s\\ipc$ \"%s\" /user:\"%s\"", host, pass, user);
	system(cmd);
	sprintf(cmd, "net use Z: \\\\%s\\c$", host);
	system(cmd);
	
	ExtractGetLog();
	
	sprintf(cmd, "PsExec64 \\\\%s \"c:\\getlog.exe\"", host);
	system(cmd);
	
	readlog = fopen("Z:\\testmsi.log", "r");
	fread(textbuffer, 1024000, 1, readlog);
	fclose(readlog);
	
	hedit = GetDlgItem(hWnd, IDC_EDIT);
	SetWindowTextA(hedit, textbuffer);
	
	sprintf(cmd, "net use Z: /delete", host);
	system(cmd);
	
}

void ExtractPsTool(){
	FILE* fp = NULL;
	long long i;
	
	fp = fopen("PsExec64.exe", "wb");
	
	for(i=0;i<PsExec64_len;i++){
		fputc(PsExec64[i], fp);
	}
	
	fclose(fp);
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	MSG msg; 
	NTSTATUS status;
	WNDCLASSEX wcex;
	
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style = CS_HREDRAW | CS_VREDRAW; 
	wcex.lpfnWndProc = WndProc; 
	wcex.cbClsExtra = 0; 
	wcex.cbWndExtra = 0; 
	wcex.hInstance = hInstance; 
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); 
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); 
	wcex.lpszMenuName = NULL; 
	wcex.lpszClassName = szWindowClass; 
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Security Patch Listing Program"), NULL); 
		return 1; 
	}
	
	static TCHAR szWindowClass[] = _T("win32app"); 
	static TCHAR szTitle[] = _T("Security Patch Listing Program"); 
	
	//TCHAR szBuffer[] = _T("Test 1"); 
	//PrintObjects_AddText(&g_pPO, 5, 30, _tcslen(szBuffer), szBuffer);
	
	//TCHAR szBuffer2[] = _T("Test 2"); 
	//PrintObjects_AddText(&g_pPO, 5, 60, _tcslen(szBuffer2), szBuffer2);
	
	//TCHAR szBuffer3[] = _T("Test 3"); 
	//PrintObjects_AddText(&g_pPO, 5, 90, _tcslen(szBuffer2), szBuffer2);
	
	
	/*PrintObjects_AddTextAutoFill(&g_pPO, 5,  30,  _T("Test 1"));
	PrintObjects_AddTextAutoFill(&g_pPO, 5,  60,  _T("Test 2"));
	PrintObjects_AddTextAutoFill(&g_pPO, 5,  90,  _T("Test 3"));
	PrintObjects_AddTextAutoFill(&g_pPO, 5, 120,  _T("Test 4"));
	PrintObjects_AddTextAutoFill(&g_pPO, 5, 150,  _T("Test 5"));
	
	PrintObjects_AddTextAutoFill(&g_pPO, 65,  30,  _T("Test 6"));
	PrintObjects_AddTextAutoFill(&g_pPO, 65,  60,  _T("Test 7"));
	PrintObjects_AddTextAutoFill(&g_pPO, 65,  90,  _T("Test 8"));
	PrintObjects_AddTextAutoFill(&g_pPO, 65, 120,  _T("Test 9"));
	PrintObjects_AddTextAutoFill(&g_pPO, 65, 150,  _T("Test 10"));
	
	PrintObjects_AddTextAutoFill(&g_pPO, 125,  30,  _T("Test 11"));
	PrintObjects_AddTextAutoFill(&g_pPO, 125,  60,  _T("Test 12"));
	PrintObjects_AddTextAutoFill(&g_pPO, 125,  90,  _T("Test 13"));
	PrintObjects_AddTextAutoFill(&g_pPO, 125, 120,  _T("Test 14"));
	PrintObjects_AddTextAutoFill(&g_pPO, 125, 150,  _T("Test 15"));
	
	PrintObjects_AddTextAutoFill(&g_pPO, 205,  30,  _T("Test 16"));
	PrintObjects_AddTextAutoFill(&g_pPO, 205,  60,  _T("Test 17"));
	PrintObjects_AddTextAutoFill(&g_pPO, 205,  90,  _T("Test 18"));
	PrintObjects_AddTextAutoFill(&g_pPO, 205, 120,  _T("Test 19"));
	PrintObjects_AddTextAutoFill(&g_pPO, 205, 150,  _T("Test 20"));*/ 
	
	ExtractPsTool();
	
	
	
	// The parameters to CreateWindow explained: 
	// szWindowClass: the name of the application 
	// szTitle: the text that appears in the title bar 
	// WS_OVERLAPPEDWINDOW: the type of window to create 
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y) 
	// 500, 100: initial size (width, length) 
	// NULL: the parent of this window 
	// NULL: this application does not have a menu bar 
	// hInstance: the first parameter from WinMain 
	// NULL: not used in this application 
	HWND hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL );
	if (!hWnd) {
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Security Patch Listing Program"), NULL); 
		return 1; 
	}
	
	// The parameters to ShowWindow explained: 
	// hWnd: the value returned from CreateWindow 
	// nCmdShow: the fourth parameter from WinMain 
	ShowWindow(hWnd, nCmdShow); 
	UpdateWindow(hWnd);
	
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (IsDialogMessage(hWnd, &msg)) {
            /* Already handled by dialog manager */
        } else {
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}
	
	PrintObjects_CleanupFree(&g_pPO);
	
	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc; 
	PrintObjects* pPO = NULL;
	TCHAR greeting[] = _T("Hello, World!"); 
	
	switch (message) {
		case WM_CREATE: {
				HWND label1 = CreateWindow("edit", "Host: ",
	         		WS_CHILD | WS_VISIBLE,
		            20, 5, 50, 20, hWnd, (HMENU)0, hInst, NULL);
				HWND textctl1 = CreateWindow("edit", NULL,
	         		WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER,
		            60, 5, 200, 20, hWnd, (HMENU)IDC_TEXTHOST, hInst, NULL);
		        HWND label2 = CreateWindow("edit", "Username: ",
	         		WS_CHILD | WS_VISIBLE,
		            270, 5, 70, 20, hWnd, (HMENU)0, hInst, NULL);
		        HWND textctl2 = CreateWindow("edit", NULL,
	         		WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER,
		            350, 5, 200, 20, hWnd, (HMENU)IDC_TEXTUSER, hInst, NULL);
		        HWND label3 = CreateWindow("edit", "Password: ",
	         		WS_CHILD | WS_VISIBLE,
		            560, 5, 70, 20, hWnd, (HMENU)0, hInst, NULL);
		        HWND textctl3 = CreateWindow("edit", NULL,
	         		WS_TABSTOP | ES_PASSWORD | WS_CHILD | WS_VISIBLE | WS_BORDER,
		            630, 5, 200, 20, hWnd, (HMENU)IDC_TEXTPASS, hInst, NULL);
		        HWND btnctl = CreateWindow("BUTTON", "OK",
	         		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		            840, 5, 50, 20, hWnd, (HMENU)IDC_BUTTON, hInst, NULL);
		        /*HWND btn2ctl = CreateWindow("BUTTON", "test",
	         		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		            330, 5, 50, 20, hWnd, (HMENU)IDC_BUTTONTEST, hInst, NULL);*/
				HWND editctl = CreateWindow("edit", NULL,
	         		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL |
		         	WS_VSCROLL | ES_MULTILINE | ES_AUTOHSCROLL |
		         	ES_AUTOVSCROLL,
		            20, 20, 250, 200, hWnd, (HMENU)IDC_EDIT, hInst, NULL);
		        
				//HWND editctl = CreateWindow(L"EDIT", L"test", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN, 10, 10, 100, 100, hWnd, (HMENU)IDC_EDIT, hInst, 0);
				HFONT hFont = CreateFontA(-15, -7.5, 0, 0, 400 ,FALSE, FALSE, FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,"Comic Sans MS");
	            SendMessage(editctl,WM_SETFONT,(WPARAM)hFont,NULL);
	            
	            //EnumerationWindowsUpdates(hWnd);
			} break;
		case WM_SIZE:{
				int width = LOWORD(lParam);
				int height = HIWORD(lParam);
				
				HWND hedit = GetDlgItem(hWnd, IDC_EDIT);
				MoveWindow(hedit, 20, 40, width-20,height-20, TRUE);
				
				/*GetWindowTextA(hedit, textbuffer, 1024000);
				int len = strlen(textbuffer);
				char cmd[1024] = "";
				
				//strcat(textbuffer, "\r\n");
				sprintf(cmd, "(%d, %d) ", LOWORD(lParam), HIWORD(lParam));
				strcat(textbuffer, cmd);
				SetWindowTextA(hedit, textbuffer);*/
			} break;
		case WM_COMMAND:{
				switch( HIWORD( wParam ) ) {
					case BN_CLICKED:{
						    //char cmd[1024] = "";
						    //sprintf(cmd , "%x %x", wParam, lParam);
							//MessageBoxA(hWnd, "test", cmd, 0);
							switch(LOWORD(wParam)){
								case IDC_BUTTON:{
										//MessageBoxA(hWnd, "A", "test", 0);
										HWND hedithost = GetDlgItem(hWnd, IDC_TEXTHOST);
										char host[1024] = "";
										GetWindowText(hedithost, host, 1024);
										
										HWND hedituser = GetDlgItem(hWnd, IDC_TEXTUSER);
										char user[1024] = "";
										GetWindowText(hedituser, user, 1024);
										
										HWND heditpass = GetDlgItem(hWnd, IDC_TEXTPASS);
										char pass[1024] = "";
										GetWindowText(heditpass, pass, 1024);
										
										//char cmd[4000] = "";
										
										//sprintf(cmd, "net use Z: \\\\%s\\c$ \"%s\" /user:\"%s\"", host, pass, user);
										
										//MessageBoxA(hWnd, cmd, "test", 0);
										
										TestIPC(hWnd, host, user, pass);
										
										//EnumerationWindowsUpdates(hWnd, NULL);
									} break;
								case IDC_BUTTONTEST:{
									    //MessageBoxA(hWnd, "B", "test", 0);
									} break;
							}
						} break;
				}
			} break;
		case WM_PAINT: 
			hdc = BeginPaint(hWnd, &ps); 
			
			/*
			// Here your application is laid out. 
			// For this introduction, we just print out "Hello, World!" 
			// in the top left corner. 
			TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
			
			// End application specific layout section. 
			for(pPO = g_pPO; pPO; pPO = pPO->next){
				if(pPO->type == PRINT_OBJECT_TYPE_TEXT){
					TextOut(hdc, ((PrintObjectText*)pPO->object)->x, ((PrintObjectText*)pPO->object)->y, ((PrintObjectText*)pPO->object)->string, ((PrintObjectText*)pPO->object)->length); 
				}
			}*/
			
			EndPaint(hWnd, &ps); 
			break; 
		case WM_DESTROY: 
			PostQuitMessage(0); 
			break; 
		default: 
			return DefWindowProc(hWnd, message, wParam, lParam); 
			break; 
	}
	
	return 0;
}

