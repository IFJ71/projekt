#define ID        0        // identifikator
#define LEX_ERROR 1        // lexikální chyba

// klicova slova
#define BEGIN     100
#define BOOLEAN   101
#define COPY      102
#define DO        103
#define ELSE      104
#define END       105
#define FALSE     106
#define FIND      107
#define FORWARD   108
#define FUNCTION  109
#define IF        110
#define INTEGER   111
#define LENGTH    112
#define READLN    113
#define REAL      114
#define SORT      115
#define STRING    116
#define THEN      117
#define TRUE      118
#define VAR       119
#define WHILE     120
#define WRITE     121

// aritmeticke operatory
#define INK       130
#define DEK       131
#define PLUS      132
#define MINUS     133
#define KRAT      134
#define DELENO    135
#define PRIRAZENI 136

// porovnavaci operatory
#define MENSI        140
#define VETSI        141
#define MENSIROVNO   142
#define VETSIROVNO   143
#define ROVNO        144
#define NEROVNO      145

// logicke operatory
#define AND          150
#define OR           151
#define NOT          152
#define XOR          153

// znaky
#define STREDNIK     160
#define TECKA        161
#define DVOJTECKA    162
#define LZAVORKA     163
#define PZAVORKA     164
#define KONECSOUBORU 165

int is_keyword(string *str);
void setSourceFile(FILE *file);
int getNextToken(string *attr);
