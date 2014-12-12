#include "str.h"
#include <stdbool.h>

#define FUN 201
#define VAR 202


int kmp_search(string S, string W);		//Knuth-Moris-Prath algrotimus
void kmp_table(string W, int *T);		//Knuth-Moris-Prath algrotimus

string merge_sort(string m);			//merge sort
string merge(string left, string right);//merge sort

void tableInit(tSymbolTable *T)											//inicializace tabulky symbolu
tTableItem *tableSearch(tSymbolTable *T, string *key)			//hledani polozky v tabulce, vraci odkaz
tTableItem *tableSearchVar(tSymbolTable *T, string *key)		//hledani promenne v tabulce
tTableItem *tableSearchFun(tSymbolTable *T, string *key)		//heldani funkce v tabulce
tTableItem *tableInsertVar(tSymbolTable *T, string *key)		//vlozeni NOVE promenne do tabulky
tTableItem *tableInsertFun(tSymbolTable *T, string *key)		//vlozeni NOVE funkce do tabulky, inicializuje lokalni tabulku dane funkce


union varValue	//unie pro ukladani hodnot promennych v TS
{	
	int integer;
	bool boolean;
	double real;
	string str;
	
}varValue;

typedef struct
{
  int varType;  // typ dane promenne - integer, bool, ...
  union varValue hodnota; // hodnota promenne pomoci unie
  /* tList neco	//seznam promennych */
  tSymbolTable *localTable;	//lokalni tabulka symbolu
} tData;

typedef struct tableItem
{
  int typ;						//funkce, nebo promenna
  string key;                  // klic, podle ktereho se bude vyhledavat == jmeno promenne/funkce
  tData data;                  // data, ktera jsou ke klici pridruzena
  struct tableItem *rightptr;  // ukazatel na pravy podstorm
  struct tableItem *leftptr;	//ukazatel na levy podstrom
} tTableItem;

typedef struct
{
  struct tableItem *first;
} tSymbolTable;