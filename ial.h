#ifndef IAL_H
#define IAL_H

#include "str.h"
#include "scanner.h"

#include <stdbool.h>
#include <malloc.h>

#define FUN    301
#define PROM   302

int kmp_search(string S, string W);       // Knuth-Moris-Prath algoritmus
void kmp_table(string W, int *T);         // Knuth-Moris-Prath algoritmus

string merge_sort(string m);              // merge sort
string merge(string left, string right);  // merge sort

union tValue // unie pro ukladani hodnot promennych v TS
{
   int integer;
   bool boolean;
   double real;
   string str;
};

typedef struct symbolTable
{
   struct tableItem *first;
}tSymbolTable;

typedef struct
{
   int varType;
   union tValue value;
   struct symbolTable localTable;
   int fwd;
   int initialized;
   int index;
   string params;
   string parTypes;
}tData;

typedef struct tableItem
{
   int typ;
   string key;
   tData data;
   struct tableItem *rightptr;
   struct tableItem *leftptr;
}tTableItem;

int tableInit(tSymbolTable *T, int global); // inicializace TS
tTableItem *tableSearch(tSymbolTable *T, string *key);
tTableItem *tableSearchVar(tSymbolTable *T, string *key);
tTableItem *tableSearchFun(tSymbolTable *T, string *key);
tTableItem *tableInsertVar(tSymbolTable *T, string *key);
tTableItem *tableInsertFun(tSymbolTable *T, string *key);
int functionsDefined(tSymbolTable *T);
void tableFree(tTableItem *T);	//nebere ukazatel na tabulku, ale na jeji prvni prvek (T->first)

#endif

