/* ============================================================
   main.c — Entry Point: ties all 4 phases together
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtable.h"
#include "tac.h"

extern char token_output[50000];
extern char parse_output[50000];
extern char symtable_output[50000];
extern char tac_output[50000];
extern char tree_output[100000];

extern void reset_outputs();
extern int  yyparse();
extern FILE *yyin;
extern int   line_num;

int main(int argc, char *argv[]) {

    /* Step 1: Reset everything */
    reset_outputs();
    reset_symbol_table();
    reset_tac();
    line_num = 1;

    /* Step 2: Create temp file to store input */
    char TEMP_FILE[] = "mini_c_input_temp.c";

    FILE *temp_file = fopen(TEMP_FILE, "w");
    if (!temp_file) {
        fprintf(stderr, "Error: Cannot create temp file.\n");
        return 1;
    }

    /* Read from stdin and write to temp file */
    char buffer[65536];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer)-1, stdin)) > 0) {
        buffer[bytes_read] = '\0';
        fwrite(buffer, 1, bytes_read, temp_file);
    }
    fclose(temp_file);

    /* Step 3: Point lexer at temp file */
    yyin = fopen(TEMP_FILE, "r");
    if (!yyin) {
        fprintf(stderr, "Error: Cannot open temp file for reading.\n");
        return 1;
    }

    /* Step 4: Run parser — triggers all 4 phases */
    int parse_result = yyparse();
    fclose(yyin);

    /* Step 5: Format symbol table and TAC output */
    print_symbol_table();
    print_tac();

    /* Step 6: Print all sections with markers for Flask */
    printf("===TOKENS===\n");
    if (strlen(token_output) > 0)
        printf("%s", token_output);
    else
        printf("(No token output)\n");

    printf("\n===PARSE===\n");
    if (parse_result == 0)
        printf("Parsing completed successfully.\n\n");
    else
        printf("Parsing completed with errors.\n\n");
    printf("%s", parse_output);

    printf("\n===SYMTABLE===\n");
    printf("%s", symtable_output);

    printf("\n===TAC===\n");
    if (strlen(tac_output) > 0)
        printf("%s", tac_output);
    else
        printf("(No TAC generated)\n");

    printf("\n===TREE===\n");
    if (strlen(tree_output) > 0)
        printf("%s", tree_output);
    else
        printf("{}");

    /* Cleanup */
    remove(TEMP_FILE);

    return (parse_result == 0) ? 0 : 1;
}