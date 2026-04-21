

#ifndef TAC_H
#define TAC_H

/* ---- Max number of TAC instructions we can store ---- */
#define MAX_TAC_INSTRUCTIONS 1000

/* ---- Max length of one TAC instruction line ---- */
#define MAX_TAC_LINE 256


typedef struct {
    char instruction[MAX_TAC_LINE];  /* the full TAC line as text */
} TACInstruction;

/* ---- Function Declarations ---- */

/* Store one TAC instruction line */
void emit_tac(const char *instruction);

/* Print all stored TAC instructions to tac_output buffer */
void print_tac(void);

/* Clear all TAC instructions — called before each compile */
void reset_tac(void);

/* Get total number of TAC instructions generated */
int get_tac_count(void);

/* External buffer declared in parser.y */
extern char tac_output[50000];

#endif /* TAC_H */