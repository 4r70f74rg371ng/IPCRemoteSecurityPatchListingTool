#include "PaintObjects.h"


void PrintObjects_Add(PrintObjects** ppPOs, int type, void* object){
	PrintObjects* pPO = NULL;
	if(ppPOs){
		pPO = (PrintObjects*)malloc(sizeof(PrintObjects));
		pPO->type = type;
		pPO->object = object;
		pPO->next = (*ppPOs);
		(*ppPOs) = pPO;
	}
}

int PrintObjects_TestIn(PrintObjects** ppPOs, void* pointer){
	PrintObjects* pPO = NULL;
	
	if(ppPOs){
		for(pPO = *ppPOs; pPO; pPO = pPO->next){
			if(pointer == pPO->object){
				return 1;
			}
		}
	}
	return 0;
}

void PrintObjects_Cleanup(PrintObjects** ppPOs){
	PrintObjects* pPO = NULL;
	PrintObjects* ptmp = NULL;
	
	if(ppPOs){
		for(pPO = *ppPOs; pPO; ){
			ptmp = pPO;
			pPO = pPO->next;
			free(ptmp);
		}
	}
}

void PrintObjects_CleanupFree(PrintObjects** ppPOs){
	PrintObjects* pPO = NULL;
	if(ppPOs){
		for(pPO = *ppPOs; pPO; pPO = pPO->next){
			free(pPO->object);
		}
		PrintObjects_Cleanup(ppPOs);
	}
}


// =====================================================================================================================================
int PrintObjects_AddText(PrintObjects** ppPOs, int x, int y, int length, TCHAR* string){
	PrintObjectText* text = malloc(sizeof(PrintObjectText));
	
	text->string = (TCHAR*)malloc(sizeof(TCHAR)*(length+1));
	_tcsncpy(text->string, string, length+1);
	text->length = length;
	text->x = x;
	text->y = y;
	
	PrintObjects_Add(ppPOs, PRINT_OBJECT_TYPE_TEXT, text);
	
	if(PrintObjects_TestIn(ppPOs, text) == 0){
		free(text);
		return 0;
	}
	
	return 1;
}

int PrintObjects_AddTextAutoFill(PrintObjects** ppPOs, int x, int y, TCHAR* string){
	PrintObjectText* text = malloc(sizeof(PrintObjectText));
	
	text->length = _tcslen(string);
	text->string = (TCHAR*)malloc(sizeof(TCHAR)*(text->length+1));
	_tcsncpy(text->string, string, text->length+1);
	text->x = x;
	text->y = y;
	
	PrintObjects_Add(ppPOs, PRINT_OBJECT_TYPE_TEXT, text);
	
	if(PrintObjects_TestIn(ppPOs, text) == 0){
		free(text);
		return 0;
	}
	
	return 1;
}




