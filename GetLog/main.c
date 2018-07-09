#include <windows.h>
#include <stdio.h>
#include <msi.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int now = 0;
	char szPatchCode[10240] = "";
	char szTargetProductCode[10240] = "";
	DWORD pcchTargetUserSid[10240] = { 0 };
	DWORD dwRet = 0;
	char szDisplayName[10240] = "";
	MSIINSTALLCONTEXT  dwTargetProductContext;
	UINT uintRet = ERROR_SUCCESS;
	FILE * fp = fopen("testmsi.log", "w");
	
	//textbuffer[0] = '\0';

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
			fprintf(fp, "\"%s\" \"%s\" \"%s\" \"%s\"\r\n", szPatchCode, szTargetProductCode, pcchTargetUserSid, szDisplayName);
			//strcat(textbuffer,cmd);
		}
	}
	
	fclose(fp);
	
	return 0;
}

