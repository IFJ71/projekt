// scanner.c

#include <stdio.h>
#include <ctype.h>

#include "str.h"
#include "scanner.h"

FILE *source;

void setSourceFile(FILE *file)
{
   source = file;
}

int is_keyword(string *str, int fce)
{
   if (strCmpConstStr(str, "begin") == 0)
      return BEGIN;
   if (strCmpConstStr(str, "boolean") == 0)
      return BOOLEAN;
   if (strCmpConstStr(str, "copy") == 0)
      return COPY;
   if (strCmpConstStr(str, "do") == 0)
      return DO;
   if (strCmpConstStr(str, "else") == 0)
      return ELSE;
   if (strCmpConstStr(str, "end") == 0)
      return END;
   if (strCmpConstStr(str, "false") == 0)
      return FALSE;
   if (strCmpConstStr(str, "find") == 0)
      return FIND;
   if (strCmpConstStr(str, "forward") == 0)
      return FORWARD;
   if (strCmpConstStr(str, "function") == 0)
      return FUNCTION;
   if (strCmpConstStr(str, "if") == 0)
      return IF;
   if (strCmpConstStr(str, "integer") == 0)
      return INTEGER;
   if (strCmpConstStr(str, "length") == 0)
      return LENGTH;
   if (strCmpConstStr(str, "readln") == 0)
      return READLN;
   if (strCmpConstStr(str, "real") == 0)
      return REAL;
   if (strCmpConstStr(str, "sort") == 0)
      return SORT;
   if (strCmpConstStr(str, "string") == 0)
      return STRING;
   if (strCmpConstStr(str, "then") == 0)
      return THEN;
   if (strCmpConstStr(str, "true") == 0)
      return TRUE;
   if (strCmpConstStr(str, "var") == 0)
      return VAR;
   if (strCmpConstStr(str, "while") == 0)
      return WHILE;
   if (strCmpConstStr(str, "write") == 0)
      return WRITE;
   
   if (fce)
      return IDENTIFIKATORF;
   return IDENTIFIKATOR;
}

int getNextToken(string *attr)
{
   int c, state = 0;
   string pomEsc;
   strClear(attr);
   strClear(&pomEsc);

   while (1)
   {
      c = getc(source);

      switch (state)
      {
         case 0: // pocatecni stav
         {
            if (isspace(c))
               state = 0; // ignorovani bilych mist
            else if (c == '{')
               state = 1; // komentar
            else if (isalpha(c))
            {
               strAddChar(attr, c);
               state = 2; // zacatek identifikatoru nebo klic. slova
            }
            else if (isdigit(c))
            {
               strAddChar(attr, c);
               state = 3; // cislo int ci double
            }
            else if (c == ':')
            {
               state = 4; // samostatna dvojtecka nebo operator prirazeni
            }
            else if (c == '<')
               state = 5; // mensi, mensi nebo rovno
            else if (c == '>')
               state = 6; // vetsi, vetsi nebo rovno
            else if (c == '\'')
               state = 8; // zacatek retezce
            else if (c == '+')
               return PLUS;
            else if (c == '-')
               return MINUS;
            else if (c == '*')
               return KRAT;
            else if (c == '/')
               return DELENO;
            else if (c == '=')
               return ROVNO;
            else if (c == ';')
               return STREDNIK;
            else if (c == ',')
               return CARKA;
            else if (c == '.')
               return TECKA;
            else if (c == '(')
               return LEVAZAVORKA;
            else if (c == ')')
               return PRAVAZAVORKA;
            else if (c == EOF)
               return KONECSOUBORU;
            else
               return LEXIKALNICHYBA;

            break;
         }
         case 1: // komentar
         {
            if (c == '}')
               state = 0;
            else if (c == EOF)
               return LEXIKALNICHYBA;
            
            break;
         }
         case 2: // alfanumericky znak (id nebo klic. slovo)
         {
            if (isalnum(c) || c == '_')
               strAddChar(attr, c);
            else
            {
               ungetc(c, source);
               return is_keyword(attr, c == '(');
            }

            break;
         }
         case 3: // libovolne cislo
         {
            if (isdigit(c))
               strAddChar(attr, c);
            else if (c == '.')
            {
               strAddChar(attr, c);
               state = 7;
            }
            else
            {
               ungetc(c, source);
               return KONST_INTEGER;
            }

            break;
         }
         case 4: // prirazeni
         {
            if (c == '=')
               return PRIRAZENI;
            else
            {
               ungetc(c, source);
               return DVOJTECKA;
            }
         }
         case 5: // mensi, mensi nebo rovno, nerovno
         {
            if (c == '=')
               return MENSIROVNO;
            else if (c == '>')
               return NEROVNO;
            else
            {
               ungetc(c, source);
               return MENSI;
            }
         }
         case 6: // vetsi, vetsi nebo rovno
         {
            if (c == '=')
               return VETSIROVNO;
            else
            {
               ungetc(c, source);
               return VETSI;
            }
         }
         case 7: // zakladni realne cislo
         {
            if (isdigit(c))
               strAddChar(attr, c);
            else
               return KONST_REAL;
            
            break;
         }
         case 8: // retezec
         {
            if (c == '\'')
               state = 9;
            else if (c == EOF || c == '\n')
               return LEXIKALNICHYBA;
            else
               strAddChar(attr, c);

            break;
         }

         case 9: // apostrof v retezci, escape sekvence nebo konec retezce
         {
            if (c == '\'')
            {
               strAddChar(attr, c);
               state = 8; // apostrof v retezci
            }
            else if (c == '#')
               state = 10; // escape sekvence
            else
               return KONST_STRING; // konec retezce

            break;
         }

         case 10: // escape sekvence
         {
            if (isdigit(c))
               strAddChar(&pomEsc, c);
            else if (c == '\'')
            {
               if (strlen(pomEsc.str) == 0)
                  return LEXIKALNICHYBA; // Zadne cislo escape sekvence

               int escape = atoi(pomEsc.str);
               if (esc < 1 || esc > 255)
                  return LEXIKALNICHYBA; // Escape sekvence neni znak
               
               strAddChar(attr, escape);
               strClear(&pomEsc);
               state = 8; // pokracovani retezce
            }
            else
               return LEXIKALNICHYBA;
            
            break;
         }
      }
   }
}

