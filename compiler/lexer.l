%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

int line_num = 1;

void add_token(const char *type, const char *val) {
    extern char token_output[50000];
    char msg[256];
    snprintf(msg, sizeof(msg), "%-15s : %s\n", type, val);
    strncat(token_output, msg, 50000 - strlen(token_output) - 1);
}
%}

DIGIT       [0-9]
LETTER      [a-zA-Z_]
INTEGER     {DIGIT}+
FLOAT       {DIGIT}+"."{DIGIT}+
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
WHITESPACE  [ \t]+

%%

"int"       { add_token("KEYWORD", "int");    return INT;       }
"float"     { add_token("KEYWORD", "float");  return FLOAT_TYPE;}
"if"        { add_token("KEYWORD", "if");     return IF;        }
"else"      { add_token("KEYWORD", "else");   return ELSE;      }
"while"     { add_token("KEYWORD", "while");  return WHILE;     }
"return"    { add_token("KEYWORD", "return"); return RETURN;    }
"void"      { add_token("KEYWORD", "void");   return VOID;      }


{FLOAT}     { add_token("FLOAT_NUM", yytext); yylval.fval = atof(yytext); return FLOAT_NUM; }
{INTEGER}   { add_token("NUMBER",    yytext); yylval.ival = atoi(yytext); return NUMBER;    }

{IDENTIFIER} { add_token("IDENTIFIER", yytext); yylval.sval = strdup(yytext); return IDENTIFIER; }

"+"   { add_token("PLUS",      "+"); return PLUS;      }
"-"   { add_token("MINUS",     "-"); return MINUS;     }
"*"   { add_token("MULTIPLY",  "*"); return MULTIPLY;  }
"/"   { add_token("DIVIDE",    "/"); return DIVIDE;    }
"="   { add_token("ASSIGN",    "="); return ASSIGN;    }

"==" { add_token("EQ",  "=="); return EQ;  }
"!=" { add_token("NEQ", "!="); return NEQ; }
"<"  { add_token("LT",  "<");  return LT;  }
">"  { add_token("GT",  ">");  return GT;  }
"<=" { add_token("LEQ", "<="); return LEQ; }
">=" { add_token("GEQ", ">="); return GEQ; }

"&&" { add_token("AND", "&&"); return AND; }
"||" { add_token("OR",  "||"); return OR;  }
"!"  { add_token("NOT", "!");  return NOT; }

"("  { add_token("LPAREN",    "("); return LPAREN;    }
")"  { add_token("RPAREN",    ")"); return RPAREN;    }
"{"  { add_token("LBRACE",    "{"); return LBRACE;    }
"}"  { add_token("RBRACE",    "}"); return RBRACE;    }
";"  { add_token("SEMICOLON", ";"); return SEMICOLON; }
","  { add_token("COMMA",     ","); return COMMA;     }

{WHITESPACE} { }

"\n" { line_num++; }

"//"[^\n]* { }

"/*"([^*]|\*+[^*/])*\*+"/" { 
    char *p;
    for(p = yytext; *p; p++)
        if(*p == '\n') line_num++;
}

. {
    char msg[128];
    snprintf(msg, sizeof(msg), "Unknown char '%s' line %d\n", yytext, line_num);
    extern char token_output[50000];
    strncat(token_output, msg, 50000 - strlen(token_output) - 1);
}

%%

int yywrap() {
    return 1;
}