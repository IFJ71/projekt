// scanner.h

enum TokenyScanneru
{
   IDENTIFIKATOR =      0,
   LEXIKALNICHYBA =     1,

   // klicova slova
   BEGIN =              100,
   BOOLEAN =            101,
   COPY =               102,
   DO =                 103,
   ELSE =               104,
   END =                105,
   FALSE =              106,
   FIND =               107,
   FORWARD =            108,
   FUNCTION =           109,
   IF =                 110,
   INTEGER =            111,
   LENGTH =             112,
   READLN =             113,
   REAL =               114,
   SORT =               115,
   STRING =             116,
   THEN =               117,
   TRUE =               118,
   VAR =                119,
   WHILE =              120,
   WRITE =              121,

   // konstanty
   KONST_INTEGER =      130,
   KONST_REAL =         131,
   KONST_STRING =       132,

   // aritmeticke operatory
   INKREMENTACE =       140,
   DEKREMENTACE =       141,
   PLUS =               142,
   MINUS =              143,
   KRAT =               144,
   DELENO =             145,
   PRIRAZENI =          146,

   // porovnavaci operatory
   MENSI =              150,
   VETSI =              151,
   MENSIROVNO =         152,
   VETSIROVNO =         153,
   ROVNO =              154,
   NEROVNO =            155,

   // logicke operatory
   AND =                160,
   OR =                 161,
   NOT =                162,
   XOR =                163,

   // znaky
   STREDNIK =           170,
   TECKA =              171,
   DVOJTECKA =          172,
   LEVAZAVORKA =        173,
   PRAVAZAVORKA =       174,
   KONECSOUBORU =       175
};

int is_keyword(string *str);
void setSourceFile(FILE *file);
int getNextToken(string *attr);
