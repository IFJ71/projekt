#ifndef PARSER_H
#define PARSER_H

#include "ial.h"
#include "stable.h"
#include "errors.h"
#include "scanner.h"

#define SYNTAX_OK     0
#define LEX_ERROR     LEXIKALNICHYBA

// hlavicka pro syntakticky analyzator
int parse(tSymbolTable *ST/*, tListOfInstr *instrList*/);

int start();
int prom_dekl();
int typ(/* ! */);
int prom_dekl2();
int funkce();
int sez_param(/* ! */);
int sez_param2(/* ! */);
int telofunkce(/* ! */);
int telo();
int skup_prikaz();
int prikaz();
int skup_prikaz2();
int prirazeni();
int po_prirazeni(/* ! */);
int vol_param(/* ! */);
int hodnota();
int vol_param2();

#endif
