#include "ial.h"

// vyhledani podretezce - KNUTH-MORIS-PRATH algoritmus
int kmp_search(string S, string W)
{
	int m = 0;
	int i = 0;
	int index = strGetLength(&W)-1;
	int T[index];
    memset(T, 0, sizeof(int) * index); // Vsechny hodnoty pole budou 0
	kmp_table(W, T);
	while((m+i) < strGetLength(&S))
	{
		if(W.str[i] == S.str[m + i])
		{
			if(i == strGetLength(&W) - 1)
			{
				return m;
			}
			i++;
		}
		else
		{
			if(T[i] > -1)
			{
				m = m + i - T[i];
				i = T[i];
			}
			else
			{
				i = 0;
				m++;
			}
		}
	}
	return -20;
}


void kmp_table(string W, int *T)
{
	int pos = 2;
	int cnd = 0;

	T[0] = -1;
	T[1] = 0;

	while(pos < strGetLength(&W))
	{
		if(W.str[pos-1] == W.str[cnd])
		{
			T[pos] = ++cnd;
			pos++;
		}
		else if(cnd > 0)
		{
			cnd = T[cnd];
		}
		else
		{
			T[pos++] = 0;
		}
	}
}

// MERGE SORT
string merge_sort(string m)
{
	int length = strGetLength(&m);
	if(length <= 1)
	{
		return m;
	}
	string left, right;
	strInit(&left);
	strInit(&right);
	int middle = length/2;
	for(int i = 0; i < middle; i++)
	{
		strAddChar(&left, m.str[i]);
	}
	for(int i = middle; i < length; i++)
	{
		strAddChar(&right, m.str[i]);
	}
	left = merge_sort(left);
	right = merge_sort(right);

	return merge(left, right);
}

string merge(string left, string right)
{
	string result;
	strInit(&result);
	int x = 0, y = 0;
	int length_L = strGetLength(&left);
	int length_R = strGetLength(&right);
	while((length_L-x > 0) || (length_R-y > 0))
	{
		if((length_L-x > 0) && (length_R-y > 0))
		{
			if(left.str[x] <= right.str[y])
			{
				strAddChar(&result, left.str[x++]);
			}
			else
			{
				strAddChar(&result, right.str[y++]);
			}
		}
		else if(length_L-x > 0)
		{
			strAddChar(&result, left.str[x++]);
		}
		else if (length_R-y > 0)
		{
			strAddChar(&result, right.str[y++]);
		}
	}
	return result;
}

// TABULKA SYMBOLU a jeji funkce

int tableInit(tSymbolTable *T, int global)
// funkce inicializuje TS
{
   T->first = NULL;

   if (!global)
      return 1;

   tSymbolTable *localTable = NULL;
   tTableItem *newBuiltInVar = NULL;
   tTableItem *newBuiltInFunc = NULL;

   string strName;
   strInit(&strName);
   
   // Pridani vestavene funkce length(s:string):integer
   strAddConstStr(&strName, "length");

   newBuiltInFunc = tableInsertFun(T, &strName);
   if (newBuiltInFunc == NULL)
      return 0;

   localTable = &newBuiltInFunc->data.localTable;
   newBuiltInFunc->data.varType = INTEGER; // typ vracene hodnoty
   newBuiltInFunc->data.fwd = 1;
   newBuiltInFunc->data.initialized = 1;
   strAddChar(&newBuiltInFunc->data.params, 's'); // informace o parametru s
   strAddChar(&newBuiltInFunc->data.parTypes, 's');

   // Pridani parametru s:string
   strClear(&strName);
   strAddChar(&strName, 's');
   newBuiltInVar = tableInsertVar(localTable, &strName);
   if (newBuiltInVar == NULL)
      return 0;

   newBuiltInVar->data.varType = STRING;
   newBuiltInVar->data.initialized = 1;
   newBuiltInVar->data.index = 1;

   // Pridani vestavene funkce copy(s:string, i:integer, n:integer):string
   strClear(&strName);
   strAddConstStr(&strName, "copy");
   
   newBuiltInFunc = tableInsertFun(T, &strName);
   if (newBuiltInFunc == NULL)
      return 0;

   localTable = &newBuiltInFunc->data.localTable;
   newBuiltInFunc->data.varType = STRING;
   newBuiltInFunc->data.fwd = 1;
   newBuiltInFunc->data.initialized = 1;
   strAddConstStr(&newBuiltInFunc->data.params, "s,i,n"); // informace o parametrech
   strAddConstStr(&newBuiltInFunc->data.parTypes, "sii");

   // Pridani parametru n:integer
   strClear(&strName);
   strAddChar(&strName, 'n');
   newBuiltInVar = tableInsertVar(localTable, &strName);
   if (newBuiltInVar == NULL)
      return 0;

   newBuiltInVar->data.varType = INTEGER;
   newBuiltInVar->data.initialized = 1;
   newBuiltInVar->data.index = 3;

   // Pridani parametru s:string
   strClear(&strName);
   strAddChar(&strName, 's');
   newBuiltInVar = tableInsertVar(localTable, &strName);
   if (newBuiltInVar == NULL)
      return 0;

   newBuiltInVar->data.varType = STRING;
   newBuiltInVar->data.initialized = 1;
   newBuiltInVar->data.index = 1;

   // Pridani parametru i:integer
   strClear(&strName);
   strAddChar(&strName, 'i');
   newBuiltInVar = tableInsertVar(localTable, &strName);
   if (newBuiltInVar == NULL)
      return 0;

   newBuiltInVar->data.varType = INTEGER;
   newBuiltInVar->data.initialized = 1;
   newBuiltInVar->data.index = 2;

   // Pridani vestavene funkce find(s:string, search:string):integer
   strClear(&strName);
   strAddConstStr(&strName, "find");
   newBuiltInFunc = tableInsertFun(T, &strName);
   if (newBuiltInFunc == NULL)
      return 0;

   localTable = &newBuiltInFunc->data.localTable;
   newBuiltInFunc->data.varType = INTEGER;
   newBuiltInFunc->data.fwd = 1;
   newBuiltInFunc->data.initialized = 1;
   strAddConstStr(&newBuiltInFunc->data.params, "s,search");
   strAddConstStr(&newBuiltInFunc->data.parTypes, "ss");

   // Pridani parametru s:string
   strClear(&strName);
   strAddChar(&strName, 's');
   newBuiltInVar = tableInsertVar(localTable, &strName);
   if (newBuiltInVar == NULL)
      return 0;

   newBuiltInVar->data.varType = STRING;
   newBuiltInVar->data.initialized = 1;
   newBuiltInVar->data.index = 1;

   // Pridani parametru search:string
   strAddConstStr(&strName, "earch");
   newBuiltInVar = tableInsertVar(localTable, &strName);
   if (newBuiltInVar == NULL)
      return 0;

   newBuiltInVar->data.varType = STRING;
   newBuiltInVar->data.initialized = 1;
   newBuiltInVar->data.index = 2;

   // Pridani vestavene funkce sort(s:string):string
   strClear(&strName);
   strAddConstStr(&strName, "sort");
   newBuiltInFunc = tableInsertFun(T, &strName);
   if (newBuiltInFunc == NULL)
      return 0;

   localTable = &newBuiltInFunc->data.localTable;
   newBuiltInFunc->data.varType = STRING;
   newBuiltInFunc->data.fwd = 1;
   newBuiltInFunc->data.initialized = 1;
   strAddChar(&newBuiltInFunc->data.params, 's');
   strAddChar(&newBuiltInFunc->data.parTypes, 's');

   // Pridani parametru s:string
   strClear(&strName);
   strAddChar(&strName, 's');
   newBuiltInVar = tableInsertVar(localTable, &strName);
   if (newBuiltInVar == NULL)
      return 0;

   newBuiltInVar->data.varType = STRING;
   newBuiltInVar->data.initialized = 1;
   newBuiltInVar->data.index = 1;

   strFree(&strName);

   return 1;
}

tTableItem *tableSearch(tSymbolTable *T, string *key)
// pokud se dana polozka s klicem |key| v tabulce symbolu nachazi,
// funkce vrati ukazatel na tuto polozku, jinak vrati NULL
{
   tTableItem *ptr = T->first;
   int found = 0;

   while (!found && ptr != NULL) // dokud nenajde, nebo nedojde na konec stromu
   {
      if (strCmpString(key, &ptr->key) == 0)
         found = 1;
      else if (strCmpString(key, &ptr->key) < 0)
         ptr = ptr->leftptr; // hledany klic je mensi, prohledej levy podstrom
      else
         ptr = ptr->rightptr; // hledany klic je vetsi, prohledej pravy podstrom
   }
   return ptr;
}

tTableItem *tableSearchVar(tSymbolTable *T, string *key)
// pokud se promenna s nazvem |key| v tabulce symbolu nenachazi,
// funkce vrati ukazatel na tuto polozku, jinak vraci NULL
{
   tTableItem *ptr = T->first;
   int found = 0;

   while (!found && ptr != NULL) // dokud nenajde, nebo nedojde na konec stromu
   {
      if (strCmpString(key, &ptr->key) == 0)
      {
         found = 1;
         if (ptr->typ != PROM)
            ptr = NULL; // pokud neni vysledek promenna (PROM), vrati NULL
      }
      else if (strCmpString(key, &ptr->key) < 0)
         ptr = ptr->leftptr; // hledany klic je mensi, prohledej levy podstrom
      else
         ptr = ptr->rightptr; // hledany klic je vetsi, prohledej pravy podstrom

   }
   return ptr;
}

tTableItem *tableSearchFun(tSymbolTable *T, string *key)
// pokud se funkce s nazevm |key| v tabulce symbolu nachazi,
// funkce vrati ukazatel na tuto polozku, jinak vraci NULL
{
   tTableItem *ptr = T->first;
   int found = 0;

   while (!found && ptr != NULL) // dokud nenajde, nebo nedojde na konec stromu
   {
      if (strCmpString(key, &ptr->key) == 0)
      {
         found = 1;
         if (ptr->typ != FUN)
            ptr = NULL; // pokud neni vysledek funkce (FUN), vrati NULL
      }
      else if (strCmpString(key, &ptr->key) < 0)
         ptr = ptr->leftptr; // hledany klic je mensi, prohledej levy podstrom
      else
         ptr = ptr->rightptr; // hledany klic je vetsi, prohledej pravy podstrom
   }
   return ptr;
}

tTableItem *tableInsertVar(tSymbolTable *T, string *key)
// funkce vlozi do tabulky symbolu NOVOU promennou,
// pokud nazev (key) jiz existuje, nebo nastala chyba alokace, vraci NULL
{
   tTableItem *ptr = T->first;
   tTableItem *new = NULL;
   int found = 0;
   int end = 0;

   if (ptr == NULL) // tabulka prazdna, vlozime promennou
   {
      if ((new = (tTableItem*) malloc(sizeof(tTableItem))) != NULL)
      {
         // vlozeni typu a nazvu
         new->typ = PROM;
         strInit(&new->key);
         strCopyString(&new->key, key);
         T->first = new;
      }
   }
   else // zjistime, zda se v tabulce polozka s danym klicem jiz nenachazi a zapiseme ji
   {
      while (!found && ptr != NULL && end == 0) // dokud nenajde, nebpo nedojde na konec stromu
      {
         if (strCmpString(key, &ptr->key) == 0)
         {
            found = 1;
            ptr = new = NULL;
         }
         else if (strCmpString(key, &ptr->key) < 0) // hledany klic je mensi, prohledej levy podstrom
         {
            if (ptr->leftptr != NULL)
               ptr = ptr->leftptr;
            else
               end = 1; // koncime v levem podstromu
         }
         else // hledany klic je vetsi, prohledej pravy podstrom
         {
            if (ptr->rightptr != NULL)
               ptr = ptr->rightptr;
            else
               end = 2; // koncime v pravem podstromu
         }
      }
      if (found == 0 && end != 0) // prvek nebyl nalezen, muzeme proto vlozit novy
      {
         if ((new = (tTableItem*) malloc(sizeof(tTableItem))) != NULL)
         {
            // vlozeni typu a nazvu
            new->typ = PROM;
            strInit(&new->key);
            strCopyString(&new->key, key);
            if (end == 1)
               ptr->leftptr = new;  // prvek vlozen do leveho podstromu
            if (end == 2)
               ptr->rightptr = new; // prvek vlozen do praveho podstromu
         }
      }
   }
   return new;
}

tTableItem *tableInsertFun(tSymbolTable *T, string *key)
// funkce vlozi do tabulky symbolu NOVOU funkci, pokud nazev (key) jiz existuje nebo nastala chyba alokace, vraci NULL
// zaroven inicializuje lokalni tabulku symoblu T->data->localTable
{
   tTableItem *ptr = T->first; 
   tTableItem *new = NULL;
   int found = 0;
   int end = 0;

   if (ptr == NULL) //tabulka prazdna, vlozime funkci
   {
      if ((new = (tTableItem*) malloc(sizeof(tTableItem))) != NULL)      //kontrola mallocu
      {
         //vlozeni typu a nazvu
         new->typ = FUN;
         strInit(&new->key);
         strInit(&new->data.params);
         strInit(&new->data.parTypes);
         strCopyString(&new->key, key);
         tableInit(&new->data.localTable, 0); //inicializace lokalni tabulky symbolu
         T->first = new;
      }
   }
   else //zjistime, zda se v tabulce polozka s danym klicem jiz nenachazi a zapiseme ji
   {
      while (!found && ptr != NULL && end == 0) //dokud nenajde, nebo nedojde na konec stromu
      {
         if (strCmpString(key, &ptr->key) == 0)
         {
            found = 1;
            ptr = new = NULL;
         }
         else if (strCmpString(key, &ptr->key) < 0) //hledany klic je mensi, prohledej levy podstrom
         {
            if (ptr->leftptr != NULL)
               ptr = ptr->leftptr;
            else
               end = 1; //koncime v levem podstromu
         }
         else if (strCmpString(key, &ptr->key) > 0) //hledany klic je vetsi, prohledej pravy podstrom
         {
            if (ptr->rightptr != NULL)
               ptr = ptr->rightptr;
            else
               end = 2; //koncime v pravem podstromu
         }
      } 
      if (found == 0 && end != 0) //prvek nebyl nalezen, muzeme proto vlozit novy
      {
         if ((new = (tTableItem*) malloc(sizeof(tTableItem))) != NULL)      //kontrola mallocu
         {
            //vlozeni typu a nazvu
            new->typ = FUN;
            strInit(&new->key);
            strInit(&new->data.params);
            strInit(&new->data.parTypes);
            strCopyString(&new->key, key);
            tableInit(&new->data.localTable, 0); //inicializace lokalni tabulky symbolu       
            if (end == 1)
               ptr->leftptr = new; //prvek vlozen do leveho podstromu
            if (end == 2)
               ptr->rightptr = new; //prvek vlozen do praveho podstromu
         }
      }
   }
   return new;
}

int functionDefined(tTableItem *ptr)
// pomocna rekurzivni funkce kontrolujici vsechny funkce v TS,
// nasledujici funkce vraci hodnotu teto funkce
{
   int result = 1;

   if (ptr != NULL)
   {
      if (ptr->typ == FUN && ptr->data.fwd == 1) // Jedna se o dopredne deklarovanou funkci?
      {
         if (ptr->data.initialized == 1) // Je i definovana?
            result = 1;
         else
            result = 0;
      } // Poznamka: prohledavat funkce, kde data.fwd == 0 neni potreba
      else if (ptr->typ == PROM)
         result = 1;
   }

   if (ptr->leftptr != NULL)
      result = result && functionDefined(ptr->leftptr); // prohledani leveho podstromu
   if (ptr->rightptr != NULL)
      result = result && functionDefined(ptr->rightptr); // prohledani praveho podtstromu

   return result;
}

int functionsDefined(tSymbolTable *T)
// funkce projde celou tabulku symbolu a kdyz jsou vsechny funkce
// definovane (kdyz ptr->data.fwd == 1, tak i ptr->data.initialized == 1)
// nebo kdyz neexistuje zadna funkce, tak vraci 1, jinak vraci 0
{
   return functionDefined(T->first);
}

void tableFree(tTableItem *node)
//zrusi binarni strom a uvolni pamet, tabulku navrati do stavu po inicializaci (jestli staci T->first = NULL)
{ 
	if (node != NULL)
	{
		tableFree(node->leftrptr);		// uvolni leve uzly
		tableFree(node->rightptr);		// uvolni prave uzly
		free (node);							// uvolni koren
		node = NULL;						//inicializace
	}
}