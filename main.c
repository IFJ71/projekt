#include <stdio.h>

#include "errors.h"
#include "str.h"
#include "parser.h"
#include "ial.h"

int main(int argc, char *argv[])
{
   int result;
   FILE *f;
   
   if (argc == 1)
   {
      fprintf(stderr, "Nebyl zadan vstupni soubor!\n");
      return INTERNAL_ERROR;
   }
   if (argc > 2)
   {
      fprintf(stderr, "Bylo zadano prilis mnoho parametru!\n");
      return INTERNAL_ERROR;
   }
   
   if (!(f = fopen(argv[1], "r")))
   {
      fprintf(stderr, "Soubor %s se nepodarilo otevrit!\n", argv[1]);
      return INTERNAL_ERROR;
   }
   
   setSourceFile(f);

   tSymbolTable table;
   tableInit(&table);
   
   result = parse(&table);
   
   tableFree(&table);
   
   if (result != SYNTAX_OK)
      return result;
   
   // Pozdeji zde bude interpretace...
   
   return result;
}
