

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"


static Symbol symbol_table[MAX_SYMBOLS];

/* How many variables are currently stored */
static int symbol_count = 0;


void reset_symbol_table() {
    symbol_count = 0;
    memset(symbol_table, 0, sizeof(symbol_table));
}


void insert_symbol(const char *name, const char *type, float value) {

    /* Step 1: Check if it already exists */
    if (lookup_symbol(name) != NULL) {
        /* Already declared — write warning to output */
        char msg[256];
        snprintf(msg, sizeof(msg),
            "Warning: Variable '%s' already declared. Ignoring redeclaration.\n",
            name);
        strncat(symtable_output, msg, 50000 - strlen(symtable_output) - 1);
        return;
    }

    /* Step 2: Check if table is full */
    if (symbol_count >= MAX_SYMBOLS) {
        strncat(symtable_output,
            "Error: Symbol table full! Too many variables.\n",
            50000 - strlen(symtable_output) - 1);
        return;
    }

    /* Step 3: Add to the next empty slot */
    strncpy(symbol_table[symbol_count].name,  name,  63);
    strncpy(symbol_table[symbol_count].type,  type,  15);
    symbol_table[symbol_count].value          = value;
    symbol_table[symbol_count].is_initialized = 0;
    symbol_count++;

    /* Log it to output */
    char msg[256];
    snprintf(msg, sizeof(msg),
        "Inserted: %-15s | type: %-8s | value: %.2f\n",
        name, type, value);
    strncat(symtable_output, msg, 50000 - strlen(symtable_output) - 1);
}


Symbol* lookup_symbol(const char *name) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return &symbol_table[i];  /* found! return its address */
        }
    }
    return NULL;  /* not found */
}


void update_symbol(const char *name, float value) {

    Symbol *s = lookup_symbol(name);

    if (s == NULL) {
        /* Variable not in table — error */
        char msg[256];
        snprintf(msg, sizeof(msg),
            "Error: Cannot update '%s' — not declared.\n", name);
        strncat(symtable_output, msg, 50000 - strlen(symtable_output) - 1);
        return;
    }

    /* Update value */
    s->value = value;
    s->is_initialized = 1;

    char msg[256];
    snprintf(msg, sizeof(msg),
        "Updated:  %-15s | value: %.2f\n", name, value);
    strncat(symtable_output, msg, 50000 - strlen(symtable_output) - 1);
}


void print_symbol_table() {
    int i;
    char line[256];

    /* Header */
    strncat(symtable_output,
        "============================================\n"
        "           SYMBOL TABLE\n"
        "============================================\n",
        50000 - strlen(symtable_output) - 1);

    /* Column headers */
    strncat(symtable_output,
        "Name            Type      Value     Init?\n"
        "--------------------------------------------\n",
        50000 - strlen(symtable_output) - 1);

    /* Each variable row */
    if (symbol_count == 0) {
        strncat(symtable_output,
            "(no variables declared)\n",
            50000 - strlen(symtable_output) - 1);
    } else {
        for (i = 0; i < symbol_count; i++) {
            snprintf(line, sizeof(line),
                "%-15s | %-8s | %-8.2f | %s\n",
                symbol_table[i].name,
                symbol_table[i].type,
                symbol_table[i].value,
                symbol_table[i].is_initialized ? "yes" : "no");
            strncat(symtable_output, line,
                50000 - strlen(symtable_output) - 1);
        }
    }

    /* Footer */
    snprintf(line, sizeof(line),
        "============================================\n"
        "Total: %d variable(s)\n"
        "============================================\n",
        symbol_count);
    strncat(symtable_output, line, 50000 - strlen(symtable_output) - 1);
}