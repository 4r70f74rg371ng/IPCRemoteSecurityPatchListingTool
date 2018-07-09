#ifndef _PAINT_OBJECTS_H_
#define _PAINT_OBJECTS_H_

#include <tchar.h>

// main paint object
typedef struct _PrintObjects{
	int type;
	void* object;
	struct _PrintObjects* next;
}PrintObjects;

void PrintObjects_Add(PrintObjects** ppPOs, int type, void* object);
int PrintObjects_TestIn(PrintObjects** ppPOs, void* pointer);
void PrintObjects_Cleanup(PrintObjects** ppPOs);
void PrintObjects_CleanupFree(PrintObjects** ppPOs); // with free object allocation


// Text Paint Object

#define PRINT_OBJECT_TYPE_TEXT 0
typedef struct _PrintObjectText{
	int x;
	int y;
	int length;
	TCHAR* string;
}PrintObjectText;

int PrintObjects_AddText(PrintObjects** ppPOs, int x, int y, int length, TCHAR* string);
int PrintObjects_AddTextAutoFill(PrintObjects** ppPOs, int x, int y, TCHAR* string);


#endif //_PAINT_OBJECTS_H_

