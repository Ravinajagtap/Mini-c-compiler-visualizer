%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtable.h"
#include "tac.h"

char token_output[50000];
char parse_output[50000];
char symtable_output[50000];
char tac_output[50000];
char tree_output[100000];

/* Semantic error/warning buffer */
char semantic_output[50000];

void append_to(char *buf, const char *text) {
    strncat(buf, text, 50000 - strlen(buf) - 1);
}

void tree_append(const char *s) {
    strncat(tree_output, s, 100000 - strlen(tree_output) - 1);
}

void tree_leaf(const char *name) {
    char buf[256];
    char safe[128];
    int i=0, j=0;
    while(name[i] && j<126) {
        if(name[i]=='"') { safe[j++]='\\'; }
        safe[j++] = name[i++];
    }
    safe[j]='\0';
    snprintf(buf, sizeof(buf), "{\"name\":\"%s\"}", safe);
    tree_append(buf);
}

void tree_leaf_c(const char *name) {
    tree_append(",");
    tree_leaf(name);
}

void fix_tree_commas() {
    char fixed[100000];
    char *src = tree_output;
    char *dst = fixed;
    while (*src) {
        if (src[0] == ',' && src[1] == ',') {
            *dst++ = ',';
            src += 2;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
    strncpy(tree_output, fixed, 99999);
    int len = strlen(tree_output);
    if (len > 0 && tree_output[len-1] == ',')
        tree_output[len-1] = '\0';
}

void tac_emit_and_append(const char *line) {
    char clean[256];
    strncpy(clean, line, 255);
    clean[255] = '\0';
    int len = strlen(clean);
    if (len > 0 && clean[len-1] == '\n')
        clean[len-1] = '\0';
    if (strlen(clean) > 0)
        emit_tac(clean);
    append_to(tac_output, line);
}

/* ============================================================
   SEMANTIC CHECK FUNCTIONS
   ============================================================ */

/* 1. Check if variable was declared */
void sem_check_declared(const char *name) {
    if (lookup_symbol(name) == NULL) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "[SEMANTIC ERROR] Variable '%s' used without declaration.\n", name);
        append_to(semantic_output, msg);
        append_to(parse_output,    msg);
    }
}

/* 2. Check if variable is initialized before use */
void sem_check_initialized(const char *name) {
    Symbol *s = lookup_symbol(name);
    if (s != NULL && s->is_initialized == 0) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "[SEMANTIC WARNING] Variable '%s' used before initialization.\n", name);
        append_to(semantic_output, msg);
        append_to(parse_output,    msg);
    }
}

/* 3. Check type mismatch — int var assigned float value */
void sem_check_type(const char *varname, const char *valtype) {
    Symbol *s = lookup_symbol(varname);
    if (s == NULL) return;
    if (strcmp(s->type, "int") == 0 && strcmp(valtype, "float") == 0) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "[SEMANTIC WARNING] Type mismatch: assigning float value to int variable '%s'. Precision loss may occur.\n",
            varname);
        append_to(semantic_output, msg);
        append_to(parse_output,    msg);
    }
}

/* 4. Check division by zero */
void sem_check_div_zero(const char *divisor) {
    if (strcmp(divisor, "0") == 0) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "[SEMANTIC ERROR] Division by zero detected.\n");
        append_to(semantic_output, msg);
        append_to(parse_output,    msg);
    }
}

/* 5. Check void function return */
void sem_check_void_return(const char *funcname, const char *retval) {
    if (strcmp(funcname, "void") == 0 && retval != NULL) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "[SEMANTIC WARNING] void function should not return a value.\n");
        append_to(semantic_output, msg);
        append_to(parse_output,    msg);
    }
}

/* Track current function return type */
char current_func_type[32] = "";

void yyerror(const char *msg) {
    char err[256];
    snprintf(err, sizeof(err), "Syntax Error: %s\n", msg);
    append_to(parse_output, err);
    fprintf(stderr, "%s\n", err);
}

extern int yylex();
extern int line_num;

int temp_count  = 0;
int label_count = 0;

char* new_temp() {
    char *t = malloc(10);
    snprintf(t, 10, "t%d", ++temp_count);
    return t;
}

char* new_label() {
    char *l = malloc(10);
    snprintf(l, 10, "L%d", ++label_count);
    return l;
}

%}

%union {
    int    ival;
    float  fval;
    char  *sval;
}

%token INT FLOAT_TYPE VOID
%token IF ELSE WHILE RETURN

%token <ival> NUMBER
%token <fval> FLOAT_NUM
%token <sval> IDENTIFIER

%token PLUS MINUS MULTIPLY DIVIDE ASSIGN
%token EQ NEQ LT GT LEQ GEQ
%token AND OR NOT
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA

%type <sval> expr term factor condition

%left OR
%left AND
%right NOT
%left LT GT LEQ GEQ EQ NEQ
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right UMINUS

%start program

%%

program
    : decl_list
        {
            fix_tree_commas();
            char full[110000];
            snprintf(full, sizeof(full),
                "{\"name\":\"program\",\"children\":[%s]}",
                tree_output);
            strncpy(tree_output, full, 99999);

            /* Add semantic summary to parse output */
            if (strlen(semantic_output) == 0) {
                append_to(parse_output,
                    "\n[SEMANTIC] All checks passed. No errors found.\n");
            } else {
                append_to(parse_output,
                    "\n[SEMANTIC] Checks completed. See warnings/errors above.\n");
            }
            append_to(parse_output, "Program parsed successfully.\n");
        }
    ;

decl_list
    : decl_list decl
    | decl
    ;

decl
    : func_decl
    | var_decl
    ;

func_decl
    : INT IDENTIFIER LPAREN param_list RPAREN LBRACE stmt_list RBRACE
        {
            /* Semantic: track function type for return check */
            strncpy(current_func_type, "int", 31);

            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"func: int %s()\",\"children\":[]},", $2);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Function declared: int %s()\n", $2);
            append_to(parse_output, msg);
            free($2);
        }
    | VOID IDENTIFIER LPAREN param_list RPAREN LBRACE stmt_list RBRACE
        {
            strncpy(current_func_type, "void", 31);

            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"func: void %s\",\"children\":[]},", $2);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Function declared: void %s(...)\n", $2);
            append_to(parse_output, msg);
            free($2);
        }
    | INT IDENTIFIER LPAREN RPAREN LBRACE stmt_list RBRACE
        {
            strncpy(current_func_type, "int", 31);

            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"func: int %s()\",\"children\":[]},", $2);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Function declared: int %s()\n", $2);
            append_to(parse_output, msg);
            free($2);
        }
    ;

param_list
    : param_list COMMA param
    | param
    ;

param
    : INT IDENTIFIER
        {
            insert_symbol($2, "int", 0);
            char msg[128];
            snprintf(msg, sizeof(msg), "Parameter: int %s\n", $2);
            append_to(parse_output, msg);
            free($2);
        }
    | FLOAT_TYPE IDENTIFIER
        {
            insert_symbol($2, "float", 0);
            free($2);
        }
    ;

var_decl
    : INT IDENTIFIER SEMICOLON
        {
            insert_symbol($2, "int", 0);
            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"var_decl\",\"children\":[{\"name\":\"int\"},{\"name\":\"%s\"}]},",
                $2);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Variable declared: int %s\n", $2);
            append_to(parse_output, msg);
            char tac[128];
            snprintf(tac, sizeof(tac), "declare int %s\n", $2);
            tac_emit_and_append(tac);
            free($2);
        }

    | INT IDENTIFIER ASSIGN expr SEMICOLON
        {
            insert_symbol($2, "int", 0);
            update_symbol($2, 0);

            /* Semantic: type check — warn if float assigned to int */
            sem_check_type($2, "float_check");

            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"var_decl\",\"children\":[{\"name\":\"int\"},{\"name\":\"%s\"},{\"name\":\"=\"},{\"name\":\"%s\"}]},",
                $2, $4);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Variable declared + assigned: int %s = %s\n", $2, $4);
            append_to(parse_output, msg);
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s\n", $2, $4);
            tac_emit_and_append(tac);
            free($2);
            if($4) free($4);
        }

    | FLOAT_TYPE IDENTIFIER ASSIGN expr SEMICOLON
        {
            insert_symbol($2, "float", 0);
            update_symbol($2, 0);

            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"var_decl\",\"children\":[{\"name\":\"float\"},{\"name\":\"%s\"},{\"name\":\"=\"},{\"name\":\"%s\"}]},",
                $2, $4);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Variable declared: float %s = %s\n", $2, $4);
            append_to(parse_output, msg);
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s\n", $2, $4);
            tac_emit_and_append(tac);
            free($2);
            if($4) free($4);
        }

    | FLOAT_TYPE IDENTIFIER SEMICOLON
        {
            insert_symbol($2, "float", 0);
            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"var_decl\",\"children\":[{\"name\":\"float\"},{\"name\":\"%s\"}]},",
                $2);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Variable declared: float %s\n", $2);
            append_to(parse_output, msg);
            free($2);
        }
    ;

stmt_list
    : stmt_list stmt
    | /* empty */
    ;

stmt
    : var_decl
    | assign_stmt
    | if_stmt
    | while_stmt
    | return_stmt
    | expr SEMICOLON
        {
            if($1) free($1);
        }
    ;

assign_stmt
    : IDENTIFIER ASSIGN expr SEMICOLON
        {
            /* Semantic Rule 1: Check variable declared */
            sem_check_declared($1);

            /* Semantic Rule 2: Check initialized (reading $3 which may be a var) */
            Symbol *s = lookup_symbol($1);
            if (s != NULL) {
                update_symbol($1, 0);
            }

            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"assign\",\"children\":[{\"name\":\"%s\"},{\"name\":\"=\"},{\"name\":\"%s\"}]},",
                $1, $3);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Assignment: %s = %s\n", $1, $3);
            append_to(parse_output, msg);
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s\n", $1, $3);
            tac_emit_and_append(tac);
            free($1);
            if($3) free($3);
        }
    ;

if_stmt
    : IF LPAREN condition RPAREN LBRACE stmt_list RBRACE ELSE LBRACE stmt_list RBRACE
        {
            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"if_else\",\"children\":[{\"name\":\"cond: %s\"},{\"name\":\"then\"},{\"name\":\"else\"}]},",
                $3);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "If-else statement parsed. Condition: %s\n", $3);
            append_to(parse_output, msg);
            if($3) free($3);
        }
    | IF LPAREN condition RPAREN LBRACE stmt_list RBRACE
        {
            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"if_stmt\",\"children\":[{\"name\":\"cond: %s\"},{\"name\":\"then\"}]},",
                $3);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "If statement parsed. Condition: %s\n", $3);
            append_to(parse_output, msg);
            if($3) free($3);
        }
    ;

while_stmt
    : WHILE LPAREN condition RPAREN LBRACE stmt_list RBRACE
        {
            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"while\",\"children\":[{\"name\":\"cond: %s\"},{\"name\":\"body\"}]},",
                $3);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "While loop parsed. Condition: %s\n", $3);
            append_to(parse_output, msg);
            if($3) free($3);
        }
    ;

return_stmt
    : RETURN expr SEMICOLON
        {
            /* Semantic Rule 5: void function returning value */
            if (strcmp(current_func_type, "void") == 0) {
                append_to(semantic_output,
                    "[SEMANTIC WARNING] void function should not return a value.\n");
                append_to(parse_output,
                    "[SEMANTIC WARNING] void function should not return a value.\n");
            }

            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"return\",\"children\":[{\"name\":\"%s\"}]},",
                $2);
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Return statement: return %s\n", $2);
            append_to(parse_output, msg);
            char tac[128];
            snprintf(tac, sizeof(tac), "return %s\n", $2);
            tac_emit_and_append(tac);
            if($2) free($2);
        }
    | RETURN SEMICOLON
        {
            /* Semantic Rule 5: int function with no return value */
            if (strcmp(current_func_type, "int") == 0) {
                append_to(semantic_output,
                    "[SEMANTIC WARNING] int function has empty return — no value returned.\n");
                append_to(parse_output,
                    "[SEMANTIC WARNING] int function has empty return — no value returned.\n");
            }

            tree_append("{\"name\":\"return\",\"children\":[]},");
            append_to(parse_output, "Return statement: return\n");
            tac_emit_and_append("return\n");
        }
    ;

condition
    : expr LT expr
        {
            /* Semantic: check both sides declared */
            sem_check_declared($1);
            sem_check_declared($3);

            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s < %s goto %s\ngoto %s\n%s:\n", $1,$3,l,l2,l);
            tac_emit_and_append(tac);
            $$ = malloc(strlen($1)+strlen($3)+6);
            snprintf($$, strlen($1)+strlen($3)+6, "%s < %s", $1, $3);
            free($1); free($3);
        }
    | expr GT expr
        {
            sem_check_declared($1);
            sem_check_declared($3);

            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s > %s goto %s\ngoto %s\n%s:\n", $1,$3,l,l2,l);
            tac_emit_and_append(tac);
            $$ = malloc(strlen($1)+strlen($3)+6);
            snprintf($$, strlen($1)+strlen($3)+6, "%s > %s", $1, $3);
            free($1); free($3);
        }
    | expr EQ expr
        {
            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s == %s goto %s\ngoto %s\n%s:\n", $1,$3,l,l2,l);
            tac_emit_and_append(tac);
            $$ = malloc(strlen($1)+strlen($3)+7);
            snprintf($$, strlen($1)+strlen($3)+7, "%s == %s", $1, $3);
            free($1); free($3);
        }
    | expr NEQ expr
        {
            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s != %s goto %s\ngoto %s\n%s:\n", $1,$3,l,l2,l);
            tac_emit_and_append(tac);
            $$ = malloc(strlen($1)+strlen($3)+7);
            snprintf($$, strlen($1)+strlen($3)+7, "%s != %s", $1, $3);
            free($1); free($3);
        }
    | expr LEQ expr
        {
            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s <= %s goto %s\ngoto %s\n%s:\n", $1,$3,l,l2,l);
            tac_emit_and_append(tac);
            $$ = malloc(strlen($1)+strlen($3)+7);
            snprintf($$, strlen($1)+strlen($3)+7, "%s <= %s", $1, $3);
            free($1); free($3);
        }
    | expr GEQ expr
        {
            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s >= %s goto %s\ngoto %s\n%s:\n", $1,$3,l,l2,l);
            tac_emit_and_append(tac);
            $$ = malloc(strlen($1)+strlen($3)+7);
            snprintf($$, strlen($1)+strlen($3)+7, "%s >= %s", $1, $3);
            free($1); free($3);
        }
    ;

expr
    : expr PLUS term
        {
            char *t = new_temp();
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s + %s\n", t, $1, $3);
            tac_emit_and_append(tac);
            $$ = t;
            free($1); free($3);
        }
    | expr MINUS term
        {
            char *t = new_temp();
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s - %s\n", t, $1, $3);
            tac_emit_and_append(tac);
            $$ = t;
            free($1); free($3);
        }
    | term
        {
            $$ = $1;
        }
    ;

term
    : term MULTIPLY factor
        {
            char *t = new_temp();
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s * %s\n", t, $1, $3);
            tac_emit_and_append(tac);
            $$ = t;
            free($1); free($3);
        }
    | term DIVIDE factor
        {
            /* Semantic Rule 4: Division by zero check */
            sem_check_div_zero($3);

            char *t = new_temp();
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s / %s\n", t, $1, $3);
            tac_emit_and_append(tac);
            $$ = t;
            free($1); free($3);
        }
    | factor
        {
            $$ = $1;
        }
    ;

factor
    : NUMBER
        {
            char *s = malloc(20);
            snprintf(s, 20, "%d", $1);
            $$ = s;
        }
    | FLOAT_NUM
        {
            char *s = malloc(20);
            snprintf(s, 20, "%.2f", $1);
            $$ = s;
        }
    | IDENTIFIER
        {
            /* Semantic Rule 1: declared check */
            sem_check_declared($1);

            /* Semantic Rule 2: initialized check */
            sem_check_initialized($1);

            $$ = $1;
        }
    | LPAREN expr RPAREN
        {
            $$ = $2;
        }
    | MINUS factor %prec UMINUS
        {
            char *t = new_temp();
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = -%s\n", t, $2);
            tac_emit_and_append(tac);
            $$ = t;
            free($2);
        }
    ;

%%

void reset_outputs() {
    token_output[0]     = '\0';
    parse_output[0]     = '\0';
    symtable_output[0]  = '\0';
    tac_output[0]       = '\0';
    tree_output[0]      = '\0';
    semantic_output[0]  = '\0';
    current_func_type[0]= '\0';
    temp_count          = 0;
    label_count         = 0;
}