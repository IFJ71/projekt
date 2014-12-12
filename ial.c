#include "ial.h"

//v v v hledani podretezce KNUTH/MORIS/PRATH algoritmus v v v
int kmp_search(string S, string W)
{
	int m = 0;
	int i = 0;
	int index = strGetLength(&W)-1;
	int T[index];
	for(int i = 0; i < index; i++)
	{
		T[i] = 0;
	}
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


//v v v MERGE SORT v v v
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


//v v v TABULKA SYMBOLU a jeji funkce v v v

void tableInit(tSymbolTable *T)
//funkce inicializuje tabulku symbolu
{
	T->first = NULL;
}

tTableItem *tableSearch(tSymbolTable *T, string *key)
// pokud se dana polozka s klicem |key| v tabulce symbolu nachazi,
// funkce vrati ukazatel na tuto polozku, jinak NULL
{
	tTableItem *ptr = T->first;
	int found = 0;
  
	while ((!found) && (ptr != NULL))	//dokud nenajde, nebo nedojde na konec stromu
	{
		if(strCmpString(key, &(ptr->key)) == 0)
		{
			found = 1;
		}
		else if(strCmpString(key, &(ptr->key)) < 0)	//hledany klic je mensi, prohledej levy podstrom
			ptr = ptr->leftptr;
		else if(strCmpString(key, &(ptr->key)) > 0)	//hledany klic je vetsi, prohledej pravy podstrom
			ptr = ptr->rightptr;
	} 
	return ptr;
}

tTableItem *tableSearchVar(tSymbolTable *T, string *key)
// pokud se promenna s nazvem |key| v tabulce symbolu nachazi,
// funkce vrati ukazatel na tuto polozku, jinak NULL
{
	tTableItem *ptr = T->first;
	int found = 0;
  
	while ((!found) && (ptr != NULL))	//dokud nenajde, nebo nedojde na konec stromu
	{
		if(strCmpString(key, &(ptr->key)) == 0)
		{
			found = 1;
			if(ptr->typ != VAR) ptr == NULL;		//pokud neni vysledek promenna (VAR), vrati NULL
		}
		else if(strCmpString(key, &(ptr->key)) < 0)	//hledany klic je mensi, prohledej levy podstrom
			ptr = ptr->leftptr;
		else if(strCmpString(key, &(ptr->key)) > 0)	//hledany klic je vetsi, prohledej pravy podstrom
			ptr = ptr->rightptr;
	} 
	return ptr;	
}

tTableItem *tableSearchFun(tSymbolTable *T, string *key)
// pokud se funkce s nazvem |key| v tabulce symbolu nachazi,
// funkce vrati ukazatel na tuto polozku, jinak NULL
{
	tTableItem *ptr = T->first;
	int found = 0;
  
	while ((!found) && (ptr != NULL))	//dokud nenajde, nebo nedojde na konec stromu
	{
		if(strCmpString(key, &(ptr->key)) == 0)
		{
			found = 1;
			if(ptr->typ != FUN) ptr == NULL;		//pokud neni vysledek funkce (FUN), vrati NULL
		}
		else if(strCmpString(key, &(ptr->key)) < 0)	//hledany klic je mensi, prohledej levy podstrom
			ptr = ptr->leftptr;
		else if(strCmpString(key, &(ptr->key)) > 0)	//hledany klic je vetsi, prohledej pravy podstrom
			ptr = ptr->rightptr;
	} 
	return ptr;
}


tTableItem *tableInsertVar(tSymbolTable *T, string *key)
// funkce vlozi do tabulky symbolu NOVOU promennou, pokud nazev (key) jiz existuje nebo nastala chyba alokace, vraci NULL
{
	tTableItem *ptr = T->first; 
	tTableItem *new = NULL;
	int found = 0;
	int end = 0;
	
	if(ptr == NULL)		//tabulka prazdna, vlozime promennou
	{
		if((new = (tTableItem*) malloc(sizeof(tTableItem))) != NULL)		//kontrola mallocu
		{
			//vlozeni typu (VAR) a nazvu (key)
			new->typ = VAR;
			strInit(&(new->key));
			strCopyString(&(new->key), key);
			T->first = new;			
		}
	}
	else	//zjistime, zda se v tabulce polozka s danym klicem jiz nenachazi a zapiseme ji
	{
		while ((!found) && (ptr != NULL) && (end == 0))	//dokud nenajde, nebo nedojde na konec stromu
		{
			if(strCmpString(key, &(ptr->key)) == 0)
			{
				found = 1;
				ptr = new = NULL;
			}
			else if(strCmpString(key, &(ptr->key)) < 0)	//hledany klic je mensi, prohledej levy podstrom
				if(ptr->leftptr != NULL) ptr = ptr->leftptr;
				else end = 1;	//koncime v levem podstromu
			else if(strCmpString(key, &(ptr->key)) > 0)	//hledany klic je vetsi, prohledej pravy podstrom
				if(ptr->rightptr != NULL) ptr = ptr->rightptr;
				else end = 2;	//koncime v pravem podstromu
		} 
		if(found==0 && end != 0)	//prvek nebyl nalezen, muzeme proto vlozit novy
		{
			if((new = (tTableItem*) malloc(sizeof(tTableItem))) != NULL)		//kontrola mallocu
			{
			//vlozeni typu (VAR) a nazvu (key)
			new->typ = VAR;
			strInit(&(new->key));
			strCopyString(&(new->key), key);		
			if(end == 1) ptr->leftptr = new;		//prvek vlozen do leveho podstromu
			if(end == 2) ptr->rightptr = new;		//prvek vlozen do praveho podstromu
			}
		}
	}
	return new;
}


tTableItem *tableInsertFun(tSymbolTable *T, string *key)
// funkce vlozi do tabulky symbolu NOVOU funkci, pokud nazev (key) jiz existuje nebo nastala chyba alokace, vraci NULL
//zaroven inicializuje lokalni tabulku symoblu T->data->localTable
{
	tTableItem *ptr = T->first; 
	tTableItem *new = NULL;
	int found = 0;
	int end = 0;
	
	if(ptr == NULL)		//tabulka prazdna, vlozime funkci
	{
		if((new = (tTableItem*) malloc(sizeof(tTableItem))) != NULL)		//kontrola mallocu
		{
			//vlozeni typu (FUN) a nazvu (key)
			new->typ = FUN;
			strInit(&(new->key));
			strCopyString(&(new->key), key);
			new->data->localTable = NULL;		//inicializace lokalni tabulky symbolu
			T->first = new;			
		}
	}
	else	//zjistime, zda se v tabulce polozka s danym klicem jiz nenachazi a zapiseme ji
	{
		while ((!found) && (ptr != NULL) && (end == 0))	//dokud nenajde, nebo nedojde na konec stromu
		{
			if(strCmpString(key, &(ptr->key)) == 0)
			{
				found = 1;
				ptr = new = NULL;
			}
			else if(strCmpString(key, &(ptr->key)) < 0)	//hledany klic je mensi, prohledej levy podstrom
				if(ptr->leftptr != NULL) ptr = ptr->leftptr;
				else end = 1;	//koncime v levem podstromu
			else if(strCmpString(key, &(ptr->key)) > 0)	//hledany klic je vetsi, prohledej pravy podstrom
				if(ptr->rightptr != NULL) ptr = ptr->rightptr;
				else end = 2;	//koncime v pravem podstromu
		} 
		if(found==0 && end != 0)	//prvek nebyl nalezen, muzeme proto vlozit novy
		{
			if((new = (tTableItem*) malloc(sizeof(tTableItem))) != NULL)		//kontrola mallocu
			{
			//vlozeni typu (FUN) a nazvu (key)
			new->typ = FUN;
			strInit(&(new->key));
			strCopyString(&(new->key), key);
			new->data->localTable = NULL;		//inicializace lokalni tabulky symbolu			
			if(end == 1) ptr->leftptr = new;		//prvek vlozen do leveho podstromu
			if(end == 2) ptr->rightptr = new;		//prvek vlozen do praveho podstromu
			}
		}
	}
	return new;
}


















































