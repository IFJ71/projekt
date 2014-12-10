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
   strClear(attr);

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
               state = 1;
            else if (isalpha(c))
            {
               strAddChar(attr, c);
               state = 2;
            }
            else if (isdigit(c))
            {
               strAddChar(attr, c);
               state = 3;
            }
            else if (c == ':')
            {
               state = 4;
            }
            else if (c == '+')
            {
               state = 5;
            }
            else if (c == '-')
               state = 6;
            else if (c == '*')
               return KRAT;
            else if (c == '/')
               return DELENO;
            else if (c == '<')
               state = 7;
            else if (c == '>')
               state = 8;
            else if (c == '\'')
               state = 10;
            else if (c == '#')
               state = 12;
            else if (c == '\"')
               return KONST_STRING;
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
            if (isalnum(c))
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
               state = 9;
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
         case 5: // inkrementace
         {
            if (c == '+')
               return INKREMENTACE;
            else
            {
               ungetc(c, source);
               return PLUS;
            }
         }
         case 6: // dekrementace
         {
            if (c == '-')
               return DEKREMENTACE;
            else
            {
               ungetc(c, source);
               return MINUS;
            }
         }
         case 7: // mensi, mensi nebo rovno, nerovno
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
         case 8:
         {
            if (c == '=')
               return VETSIROVNO;
            else
            {
               ungetc(c, source);
               return VETSI;
            }
         }
         case 9:
         {
            if (isdigit(c))
               strAddChar(attr, c);
            else
               return KONST_REAL;
            
            break;
         }
         case 10:
         {
            if (c == '\'')
               state = 11;
            else if (c == EOF || c == '\n')
               return LEXIKALNICHYBA;
            else
               strAddChar(attr, c);

            break;
         }
         case 11:
         {
            if (c == '\'')
            {
               strAddChar(attr, c);
               state = 10;
            }
            else
            {
               ungetc(c, source);
               return KONST_STRING;
            }

            break;
         }
         case 12:
         {
            if (isdigit(c))
               strAddChar(attr, c);
            else if (strGetLength(attr) == 0)
            {
               ungetc(c, source);
               return LEXIKALNICHYBA;
            }
            else
            {
               ungetc(c, source);
               return KONST_CHAR;
            }

            break;
         }         
      }
   }
}

