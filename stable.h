// definice jednotlivych typu
#define TYPE_INT     1
#define FCE 201
#define IDENT 202
#include "str.h"
typedef struct
{
  int varType;  // typ dane promenne
  union varValue hodnota; // pro ruzne typy nutnost resit pomoci unie
} tData;

union varValue
{	
	int integer;
	bool boolean;
	double real;
	string str;
	
}varValue;
  
typedef struct tableItem
{
  int typ;
  string key;                  // klic, podle ktereho se bude vyhledavat = nazev identifikatoru
  tData data;                  // data, ktera jsou ke klici pridruzena
  struct tableItem *nextItem;  // ukazatel na dalsi prvek tabulky symbolu
  
} tTableItem;

typedef struct
{
  struct tableItem *first;
} tSymbolTable;



void tableInit(tSymbolTable *T);
int tableInsert(tSymbolTable *T, string *key, int varType);
tData *tableSearch(tSymbolTable *T, string *key);
void tableFree(tSymbolTable *T);
