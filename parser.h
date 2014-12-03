#include "scanner.h"
#include "stable.h"

#define SYNTAX_OK     4
#define SYNTAX_ERROR -2
#define SEM_ERROR    -3
#define LEX_ERROR     LEXIKALNICHYBA

// hlavicka pro syntakticky analyzator
int parse(tSymbolTable *ST, tListOfInstr *instrList);

int start();
int declaration();
int type(tTableItem *item);
