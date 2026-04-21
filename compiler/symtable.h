/* ============================================================
   symtable.h — Phase 3: Symbol Table Header
   
   This is the HEADER FILE.
   It only DECLARES things — no actual code here.
   Other files (#include "symtable.h") to know what's available.
   ============================================================ */

#ifndef SYMTABLE_H   /* Include guard — prevents double inclusion */
#define SYMTABLE_H

/* ---- How many variables can we store? ---- */
#define MAX_SYMBOLS 100

/* ---- The Symbol struct ---- */
/*
   This is like a row in a table:
   | name  | type  | value |
   | "x"   | "int" |  5    |
   | "y"   | "float"| 3.14 |
*/
typedef struct {
    char name[64];    /* variable name, e.g. "x", "myCounter" */
    char type[16];    /* data type,     e.g. "int", "float"   */
    float value;      /* current value, e.g. 5, 3.14          */
    int is_initialized; /* 1 = has a value, 0 = just declared */
} Symbol;

/* ---- Function Declarations ---- */
/* These are defined in symtable.c */

/* Add a new variable to the table */
void insert_symbol(const char *name, const char *type, float value);

/* Find a variable — returns pointer to it, or NULL if not found */
Symbol* lookup_symbol(const char *name);

/* Change a variable's value (for assignments like x = 10) */
void update_symbol(const char *name, float value);

/* Print the whole table — output goes to symtable_output buffer */
void print_symbol_table(void);

/* Clear everything — called before each new compilation */
void reset_symbol_table(void);

/* 
   This is the actual output buffer — declared in parser.y
   but used here to write symbol table output into it 
*/
extern char symtable_output[50000];

#endif /* SYMTABLE_H */