#include <stdio.h>

#include "str.h"
#include "scanner.h"
#include "ial.h"

int main(int argc, char *argv[])
{
   /*
   FILE *f = fopen("zdroj.p", "r");
   setSourceFile(f);

   string str;
   strInit(&str);
   
   while (!feof(f))
   {
      int token = getNextToken(&str);
      printf("%d\t\t%s\n", token, strGetStr(&str));
   }

   strFree(&str);
   return 0;
   */
   string S,W;
   strInit(&S);
   strInit(&W);
   for(int i = 0; i < 10; i++)
   {
      strAddChar(&S, 'a');
   }

   for(int i = 0; i < 5; i++)
   {
      strAddChar(&S, 'b');
      strAddChar(&W, 'b');
   }

   for(int i = 0; i < 10; i++)
   {
      strAddChar(&S, 'a');
   }

   printf("%d", kmp_search(S, W));
   
   printf("%s", merge_sort(S).str);
   return 0;

}
