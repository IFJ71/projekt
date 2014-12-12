#include <stdio.h>

#include "str.h"
#include "ilist.h"
#include "parser.h"

// Globalni promenne
tSymbolTable *table;       // Tabulka symbolu
//tListOfInstr *list;      // Seznam instrukci
int token;                 // Aktualni token
string attr;               // Atribut tokenu
string *parameters = NULL; // Parametry

int counterVar = 1;


int parse(tSymbolTable *ST/*, tListOfInstr *instrList*/)
{
   int result = SYNTAX_ERROR;
   
   table = ST;
   
   strInit(&attr);
   
   token = getNextToken(&attr);
   if (token == LEX_ERROR)
      return LEX_ERROR;
   
   result = start();
   
   strFree(&attr);
   
   return result;
}

int start() // <START>
{
   int result = SYNTAX_ERROR;
   
   switch (token)
   {
      // <START> -> <PROM_DEKL> <FUNKCE> <TELO> TECKA KONECSOUBORU
      case VAR:
      case FUNCTION:
      case BEGIN:
         if ((result = prom_dekl()) != SYNTAX_OK) // <PROM_DEKL>
            return result;
         
         if ((result = funkce()) != SYNTAX_OK) // <FUNKCE>
            return result;
         
         // TODO: Testovani zda jsou funkce spravne definovany
         
         if ((result = telo()) != SYNTAX_OK) // <TELO>
            return result;
         
         if (token != TECKA) // TECKA
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != KONECSOUBORU) // KONECSOUBORU
            return SYNTAX_ERROR;
         return SYNTAX_OK;
      default: // Jestli je prvni token jiny nez povoleny, jedna se o syntaktickou chybu
         return SYNTAX_ERROR;
   }
}

int prom_dekl() // <PROM_DEKL>
{
   int result = SYNTAX_ERROR;
   // TODO: ukazatel na novou polozku
   
   switch (token)
   {
      // <PROM_DEKL> -> VAR IDENTIFIKATOR DVOJTECKA <TYP> STREDNIK <PROM_DEKL2>
      case VAR: // VAR
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != IDENTIFIKATOR) // IDENTIFIKATOR
            return SYNTAX_ERROR;
         // TODO: Vytvoreni polozky v tabulce symbolu a ziskani jejiho ukazatele
            
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != DVOJTECKA) // DVOJTECKA
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         result = typ(/*TODO: ukazatel na polozku*/); // <TYP>
         if (result != SYNTAX_OK)
            return result;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != STREDNIK) // STREDNIK
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         return prom_dekl2(); // <PROM_DEKL2>
      
      case FUNCTION:
      case BEGIN:
         return SYNTAX_OK; // <PROM_DEKL> -> (epsilon)
   }
   
   return result;
}

int typ(/*TODO: ukazatel na novou polozku*/) // <TYP>
{
   switch (token) // Vsechny moznosti jsou analogicke
   {
      case BOOLEAN: // <TYP> -> BOOLEAN
         if (parameters != NULL) // Pokud se jedna o parametr
            strAddChar(parameters, 'b'); // Vloz informaci o typu noveho parametru
         
         // TODO: nastaveni typu nove promenne
         break;
         
      case INTEGER: // <TYP> -> INTEGER
         if (parameters != NULL)
            strAddChar(parameters, 'i');
         
         // TODO
         break;
         
      case REAL: // <TYP> -> REAL
         if (parameters != NULL)
            strAddChar(parameters, 'r');
         
         // TODO
         break;
         
      case STRING: // <TYP> -> STRING
         if (parameters != NULL)
            strAddChar(parameters, 's');
         
         // TODO
         break;
      
      default: // Dalsi pravidlo pro <TYP> neexistuje, jedna se o syntaktickou chybu
         return SYNTAX_ERROR;
   }
   if ((token = getNextToken(&attr)) == LEX_ERROR)
      return LEX_ERROR;
   return SYNTAX_OK;
}

int prom_dekl2() // <PROM_DEKL2>
{
   int result = SYNTAX_ERROR;
   
   switch (token)
   {
      // <PROM_DEKL2> -> IDENTIFIKATOR DVOJTECKA <TYP> STREDNIK <PROM_DEKL2>
      case IDENTIFIKATOR: // IDENTIFIKATOR
         // TODO: Vytvoreni polozky v tabulce symbolu a ziskani jejiho ukazatele
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != DVOJTECKA) // DVOJTECKA
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         result = typ(); // <TYP>
         if (result != SYNTAX_OK)
            return result;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != STREDNIK) // STREDNIK
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         return prom_dekl2(); // <PROM_DEKL2>
         
      case FUNCTION:
      case BEGIN:
         return SYNTAX_OK; // <PROM_DEKL2> -> (epsilon)
         
      default: // Dalsi pravidlo pro <PROM_DEKL2> neexistuje, jedna se o syntaktickou chybu
         return SYNTAX_ERROR;
   }
}

int funkce() // <FUNKCE>
{
   int result = SYNTAX_ERROR;
   //TODO: ukazatele na funkci v globalni tabulce symbolu (prvni z pohledu funkce, druhy z pohledu promenne)
   
   switch (token)
   {
      // <FUNKCE> -> FUNCTION IDENTIFIKATORF LEVAZAVORKA <SEZ_PARAM> PRAVAZAVORKA DVOJTECKA <TYP> STREDNIK <TELOFUNKCE> <FUNKCE>
      case FUNCTION: // FUNCTION
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != IDENTIFIKATORF) // IDENTIFIKATORF
            return SYNTAX_ERROR;
         /* TODO: Vyhledani zda jiz funkce neexistuje v globalni tabulce symbolu.
                  Pokud existuje, a je plne deklarovana, vraci se semanticka chyba.
                  Pokud existuje, a je pouze dopredne deklarovana, nastavi se aktivni lokalni tabulka na tabulku funkce.
                  Pokud neexistuje, vytvori se lokalni tabulka funkce a do globalni tabulky se funkce prida
                  a ukazatel na ni se ulozi vyse.*/
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != LEVAZAVORKA) // LEVAZAVORKA
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         result = sez_param(/*TODO: ukazatel na funkci v globalni tabulce symbolu*/); // <SEZ_PARAM>
         if (result != SYNTAX_OK)
            return result;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != PRAVAZAVORKA) // PRAVAZAVORKA
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != DVOJTECKA) // DVOJTECKA
            return SYNTAX_ERROR;
            
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         
         // TODO: semanticka kontrola datoveho typu dopredne deklarovane funkce (vcetne nacteni dalsiho radneho tokenu)
         
         // ZACATEK CASTI KODU, KTERA BUDE V ELSE VETVI, AZ BUDE IMPLEMENTOVANA SEMANTICKA KONTROLA
         
         result = typ(/* TODO */); // <TYP>
         // TODO: nastaveni datoveho typu funkce
         
         // KONEC CASTI KODU ...
         
         if (result != SYNTAX_OK)
            return result;
         
         // dalsi token nacetla funkce typ, staci jen porovnat
         if (token != STREDNIK)
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         
         result = telofunkce(/* TODO */);
         if (result != SYNTAX_OK)
            return result;
         
         return funkce();
         
      case BEGIN:
         return SYNTAX_OK; // <FUNKCE> -> (epsilon)
         
      default: // Dalsi pravidlo pro <FUNKCE> neexistuje, jedna se o syntaktickou chybu
         return SYNTAX_ERROR;
   }
}

int sez_param(/* TODO */) // <SEZ_PARAM>
{
   int result = SYNTAX_ERROR;
   
   switch (token)
   {
      // <SEZ_PARAM> -> IDENTIFIKATOR DVOJTECKA <TYP> <SEZ_PARAM2>
      case IDENTIFIKATOR: // IDENTIFIKATOR
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != DVOJTECKA) // DVOJTECKA
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         result = typ(/* TODO */); // <TYP>
         if (result != SYNTAX_OK)
            return result;
         return sez_param2(/* TODO */); // <SEZ_PARAM2>
         
      case PRAVAZAVORKA:
         return SYNTAX_OK; // <SEZ_PARAM> -> (epsilon)
      
      default:
         return SYNTAX_ERROR;
   }
   
   // TODO: po dopredne deklaraci porovnavani jestli se retezce parametru shoduji
   //       jinak pridani parametru (do polozky pro funkci v glob. tabulce)
}

int sez_param2(/* TODO */) // <SEZ_PARAM2>
{
   int result = SYNTAX_ERROR;
   
   switch (token)
   {
      // <SEZ_PARAM2> -> STREDNIK IDENTIFIKATOR DVOJTECKA <TYP> <SEZ_PARAM2>
      case STREDNIK: // STREDNIK
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != IDENTIFIKATOR) // IDENTIFIKATOR
            return SYNTAX_ERROR;
         
         // TODO
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != DVOJTECKA) // DVOJTECKA
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         result = typ(/* TODO */);
         if (result != SYNTAX_OK)
            return result;
         return sez_param2(/* TODO */);
      
      case PRAVAZAVORKA: // <SEZ_PARAM2> -> (epsilon)
         return SYNTAX_OK;
      
      default:
         return SYNTAX_ERROR;
   }
}

int telofunkce(/* TODO */) // <TELOFUNKCE>
{
   int result = SYNTAX_ERROR;
   
   switch (token)
   {
      // <TELOFUNKCE> -> FORWARD STREDNIK
      case FORWARD: // FORWARD
         // TODO: semanticka kontrola dalsi dopredne deklarace
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != STREDNIK) // STREDNIK
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         return SYNTAX_OK;
      
      // <TELOFUNKCE> -> <PROM_DEKL> <TELO> STREDNIK
      case VAR:
      case BEGIN:
         result = prom_dekl(); // <PROM_DEKL>
         if (result != SYNTAX_OK)
            return result;
         
         result = telo(); // <TELO>
         if (result != SYNTAX_OK)
            return result;
            
         if (token != STREDNIK) // <STREDNIK>
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         return SYNTAX_OK;
      
      default:
         return SYNTAX_ERROR;
   }
}

int telo() // <TELO>
{
   int result = SYNTAX_ERROR;
   
   switch (token)
   {
      // <TELO> -> BEGIN <SKUP_PRIKAZ> END
      case BEGIN: // BEGIN
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         result = skup_prikaz(); // <SKUP_PRIKAZ>
         if (result != SYNTAX_OK)
            return result;
         
         if (token != END) // END
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         return SYNTAX_OK;
      
      default:
         return SYNTAX_ERROR;
   }
}

int skup_prikaz() // <SKUP_PRIKAZ>
{
   int result = SYNTAX_ERROR;
   
   switch (token)
   {
      // <SKUP_PRIKAZ> -> <PRIKAZ> <SKUP_PRIKAZ2>
      case ID:
      case WHILE:
      case IF:
      case WRITE:
      case READLN:
         result = prikaz(); // <PRIKAZ>
         if (result != SYNTAX_OK)
            return result;
         
         return skup_prikaz2(); // <SKUP_PRIKAZ2>
      
      case END: // <SKUP_PRIKAZ> -> (epsilon)
         return SYNTAX_OK;
      
      default:
         return SYNTAX_ERROR;
   }
}

int prikaz() // <PRIKAZ>
{
   switch (token)
   {
      case IDENTIFIKATOR: // <PRIKAZ> -> <PRIRAZENI>
         return prirazeni();
      
      case IF: // <PRIKAZ> -> <IF>
         return _if();
      
      case WHILE: // <PRIKAZ> -> <WHILE>
         return _while();
      
      case READLN: // <PRIKAZ> -> <FUNC_READLN>
         return func_readln();
      
      case WRITE: // <PRIKAZ> -> <FUNC_WRITE>
         return func_write();
      
      default:
         return SYNTAX_ERROR;
   }
}

int skup_prikaz2() // <SKUP_PRIKAZ2>
{
   int result = SYNTAX_ERROR;
   
   switch (token)
   {
      // <SKUP_PRIKAZ2> -> STREDNIK <PRIKAZ> <SKUP_PRIKAZ2>
      case STREDNIK: // STREDNIK
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         result = prikaz(); // <PRIKAZ>
         if (result != SYNTAX_OK)
            return result;
         
         return skup_prikaz2(); // <SKUP_PRIKAZ2>
      
      case END: // <SKUP_PRIKAZ2> -> (epsilon)
         return SYNTAX_OK;
      
      default:
         return SYNTAX_ERROR;
   }
}

int prirazeni() // <PRIRAZENI>
{   
   switch (token)
   {
      // <PRIRAZENI> -> IDENTIFIKATOR PRIRAZENI <PO_PRIRAZENI>
      case IDENTIFIKATOR: // IDENTIFIKATOR
         // TODO: chyba kdyz promenna neexistuje, v opacnem pripade nastaveni inicializace
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != PRIRAZENI)
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         return po_prirazeni(/* TODO: typ promenne */);
      
      default:
         return SYNTAX_ERROR;
   }
}

int po_prirazeni(/* TODO */) // <PO_PRIRAZENI>
{
   int result = SYNTAX_ERROR;
   
   switch (token)
   {
      // <PO_PRIRAZENI> -> <VYRAZ>
      case IDENTIFIKATOR:
      case BOOLEAN:
      case INTEGER:
      case REAL:
      case STRING:
         return vyraz(/* TODO: predani parametru teto funkce */); // VYRAZ
      
      // <PO_PRIRAZENI> -> IDENTIFIKATORF LEVAZAVORKA <VOL_PARAM> PRAVAZAVORKA
      case IDENTIFIKATORF: // IDENTIFIKATORF
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         if (token != LEVAZAVORKA) // LEVAZAVORKA
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         result = vol_param(/* TODO: ukazatel na funkci v glob. tab. */); // <VOL_PARAM>
         if (result != SYNTAX_OK)
            return result;
         
         if (token != PRAVAZAVORKA) // PRAVAZAVORKA
            return SYNTAX_ERROR;
         
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         
         return SYNTAX_OK;
      
      default:
         return SYNTAX_ERROR;
   }
}

int vol_param(/* TODO */) // <VOL_PARAM>
{
   int result = SYNTAX_ERROR;
   
   switch (token)
   {
      // <VOL_PARAM> -> <HODNOTA> <VOL_PARAM2>
      case IDENTIFIKATOR:
      case BOOLEAN:
      case INTEGER:
      case REAL:
      case STRING:
         result = hodnota(); // <HODNOTA>
         if (result != SYNTAX_OK)
            return result;
         
         return vol_param2(); // <VOL_PARAM2>
      
      // <VOL_PARAM> -> (epsilon)
      case PRAVAZAVORKA:
         return SYNTAX_OK;
      
      default:
         return SYNTAX_ERROR;
   }
}

int hodnota() // <HODNOTA>
{
   switch (token)
   {
      case FALSE: // <HODNOTA> -> FALSE
      case TRUE: // <HODNOTA> -> TRUE
      case KONST_INTEGER: // <HODNOTA> -> KONST_INTEGER
      case KONST_REAL: // <HODNOTA> -> KONST_REAL
      case KONST_STRING: // <HODNOTA> ->KONST_STRING
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         return SYNTAX_OK;
      
      default:
         return SYNTAX_ERROR;
   }
}

int vol_param2() // <VOL_PARAM2>
{
   int result = SYNTAX_ERROR;
   
   switch (token)
   {
      // <VOL_PARAM2> -> CARKA <HODNOTA> <VOL_PARAM2>
      case CARKA: // CARKA
         if ((token = getNextToken(&attr)) == LEX_ERROR)
            return LEX_ERROR;
         result = hodnota(); // <HODNOTA>
         if (result != SYNTAX_OK)
            return result;
         
         return vol_param2(); // <VOL_PARAM2>
         
      case PRAVAZAVORKA: // <VOL_PARAM2> -> (epsilon)
         return SYNTAX_OK;
      
      default:
         return SYNTAX_ERROR;
   }
}

/*
void generateVariable(string *var)
// generuje jedinecne nazvy identifikatoru
// nazev se sklada ze znaku $ nasledovanym cislem
// postupne se tu generuji prirozena cisla a do nazvu promenne se ukladaji
// v reverzovanem poradi - na funkcnost to nema vliv, ale je jednodussi implementace

{
  strClear(var);
  strAddChar(var, '$');
  int i;
  i = counterVar;
  while (i != 0)
  {
    strAddChar(var, (char)(i % 10 + '0'));
    i = i / 10;
  }
  counterVar ++;
} 

void generateInstruction(int instType, void *addr1, void *addr2, void *addr3)
// vlozi novou instrukci do seznamu instrukci
{
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(list, I);
}

// ==================================================================
// jednotlive funkce odpovidajici jednotlivym nonterminalum gramatiky
// ==================================================================

int stat(); // deklarace funkce, nebot bude volana drive, nez je definovana


int declrList()
{
  switch (token)
  {
    case ID:
      // pravidlo <declrList> -> "ID" ";" <declrList>

      // resime ID, ulozime do tabulky symbolu
      // pokud tento ID jiz byl deklarovan, jedna se o semantickou chybu
      if (tableInsert(table, &attr, TYPE_INT) == 1) return SEM_ERROR;
      
      // pozadame o dalsi token, kterym musi byt ";"
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      if (token != SEMICOLON) return SYNTAX_ERROR;
      
      // pozadame o dalsi token a rekurzivne zavolame funkci declrList
      // cele pravidlo probehne v poradku, pokud nam tato funkce vrati uspech
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      return declrList();    
    break;
      

    case LEFT_VINCULUM:
      // pravidlo <declrList> -> "{"  
 
      // pouze pozadame o dalsi token a vratime uspech
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;	    
      return SYNTAX_OK;
    break;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int statList()
{
  int result;
  switch (token)
  {
    case RIGHT_VINCULUM:
      // pravidlo <statList> -> "}"

      // pouze pozadame o dalsi token a vratime uspech
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      return SYNTAX_OK;
    break;    

    case WHILE:
    case READ:
    case WRITE:
    case SETZERO:
    case INC:
    case DEC:
      // pravidlo <statList> -> <stat> <statList>
      
      // nejprve zavolame funkci stat
      result = stat();
      // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
      if (result != SYNTAX_OK) return result;
      // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce statList
      return statList();
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}


int stat()
{
  tData *variableInfo;

  int result;
  int firstTermInRule;

  switch (token)
  {
    case READ:
    case WRITE:
    case SETZERO:
    case INC:
    case DEC:
      // tato pravidla maji stejny kontext, budeme je zpracovavat zaroven,
      // akorat vygenerujeme prislusnou instrukci
    
      // pravidlo <stat> -> "READ" "ID" ";"
      // pravidlo <stat> -> "WRITE" "ID" ";"
      // pravidlo <stat> -> "SETZERO" "ID" ";"
      // pravidlo <stat> -> "INC" "ID" ";"
      // pravidlo <stat> -> "DEC" "ID" ";"
      
      firstTermInRule = token;  // musime si ulozit tento token, abychom na zaver vedeli, jakou instrukci generovat
      
      // pozadame o dalsi token, kterym musi byt ID
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      // pokud token neni ID, jedna se o syntaktickou chybu
      if (token != ID) return SYNTAX_ERROR;
      // musime zkontrolovat, zda je jiz identifikator v tabulce symbolu
      // pokud ne, jedna se o semantickou chybu
      if ((variableInfo = tableSearch(table, &attr)) == NULL) return SEM_ERROR;      
      // nagenerujeme patricnou instrukci
      switch (firstTermInRule)
      {
        case READ:    generateInstruction(I_READ,    (void*) variableInfo, NULL, NULL); break;
        case WRITE:   generateInstruction(I_WRITE,   (void*) variableInfo, NULL, NULL); break;
        case SETZERO: generateInstruction(I_SETZERO, (void*) variableInfo, NULL, NULL); break;
        case INC:     generateInstruction(I_INC,     (void*) variableInfo, NULL, NULL); break;
        case DEC:     generateInstruction(I_DEC,     (void*) variableInfo, NULL, NULL); break;
      }
      
      // pozadame o dalsi token, kterym musi byt strednik
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      if (token != SEMICOLON) return SYNTAX_ERROR;
      
      // pozadame o dalsi token a vratime uspech
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      return SYNTAX_OK;
    break;	 
        
    case WHILE:
      // pravidlo <stat> -> "WHILE" "ID" "{" <statList>
      
      // nagenerujeme instrukci noveho navesti a uchovame jeji ukazatel
      generateInstruction(I_LAB, NULL, NULL, NULL);
      void *addrOfLab1;
      addrOfLab1 = listGetPointerLast(list);
      
      // pozadame o dalsi token, kterym musi byt ID
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      // pokud token neni ID, jedna se o syntaktickou chybu
      if (token != ID) return SYNTAX_ERROR;
      // musime zkontrolovat, zda je jiz identifikator v tabulce symbolu
      // pokud ne, jedna se o semantickou chybu
      if ((variableInfo = tableSearch(table, &attr)) == NULL) return SEM_ERROR;      
      
      // nagenerujeme pomocnou promennou, kterou ulozime do tabulky symbolu
      // testovat jeji existenci nemusime, nebot jeji nazev je jedinecny
      string newVar;
      strInit(&newVar);
      generateVariable(&newVar);
      tableInsert(table, &newVar, TYPE_INT);
      tData *newVariableInfo;
      newVariableInfo = tableSearch(table, &newVar);
      strFree(&newVar);
      
      // nagenerujeme instrukci, ktera provede negaci drive ziskaneho identifikatoru
      // a vysledek ulozi do nove promenne      
      generateInstruction(I_NOT, (void*) variableInfo, NULL, (void*) newVariableInfo);
      
      // nagenerujeme instrukci podmineneho skoku a ulozime jeji adresu,
      // protoze pozdeji bude potreba doplnit adresu skoku
      void *addrOfIfGoto;
      generateInstruction(I_IFGOTO, (void*) newVariableInfo, NULL, NULL); 
      addrOfIfGoto = listGetPointerLast(list);
      
      // zazadame o dalsi token, kterym musi byt slozena zavorka
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      if (token != LEFT_VINCULUM) return SYNTAX_ERROR;
      
      // zazadame o dalsi token a rekurzivne zavolame funkci stat
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      result = statList();
      if (result != SYNTAX_OK) return result;
      
      // nagenerujeme instrukci skoku
      generateInstruction(I_GOTO, NULL, NULL, (void*) addrOfLab1);

      // nagenerujeme instrukci druheho navesti
      generateInstruction(I_LAB, NULL, NULL, NULL);
      void *addrOfLab2;
      addrOfLab2 = listGetPointerLast(list);

      // jiz zname adresu druheho navesti, muzeme tedy nastavit adresu
      // do drive generovane instrukce podmineneho skoku
      listGoto(list, addrOfIfGoto);
      tInstr *data;
      data = listGetData(list);
      data->addr3 = addrOfLab2;
      
      return SYNTAX_OK;
    break;
    
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;      
}    
    
    
int program()
{
  int result;
  switch (token)
  {
    case ID:
    case LEFT_VINCULUM:    
      // pravidlo <program> -> <declrList> <statList> <END_OF_FILE>
      result = declrList();
      if (result != SYNTAX_OK) return result;
      result = statList();
      if (result != SYNTAX_OK) return result;
      
      // POZOR! Nezapomenout testovat, zda nasleduje konec souboru.
      // Pri oponenuti teto veci by zde mohly pokracovat nejake nesmysly, ktere by se
      // v ramci syntakticke analyzy jiz nezpracovavaly a program by se tvaril, ze je OK
      if (token != END_OF_FILE) return SYNTAX_ERROR;
      
      // nagenerujeme instrukci konce programu
      generateInstruction(I_STOP, NULL, NULL, NULL);
      
      return SYNTAX_OK;
    break;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}


int parse(tSymbolTable *ST, tListOfInstr *instrList)
{
  int result;
  table = ST;
  list = instrList;
  strInit(&attr);
  if ((token = getNextToken(&attr)) == LEX_ERROR)
     // nastala chyba jiz pri nacteni prvniho lexemu
     result = LEX_ERROR;
  else
     result = program();
  strFree(&attr);
  return result;
}*/
