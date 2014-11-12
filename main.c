/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  5.11.2014 15:05:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dr. Fritz Mehner (mn), mehner@fh-swf.de
 *        Company:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */

#include <stdio.h>

#include "str.h"
#include "scanner.h"

int main(int argc, char *argv[])
{
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

}
