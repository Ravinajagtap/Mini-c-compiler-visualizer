

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"

/* ---- Internal storage: array of TAC instructions ---- */
static TACInstruction tac_list[MAX_TAC_INSTRUCTIONS];
static int tac_count = 0;


void reset_tac() {
    tac_count = 0;
    memset(tac_list, 0, sizeof(tac_list));
}


void emit_tac(const char *instruction) {

    /* Check if we have space */
    if (tac_count >= MAX_TAC_INSTRUCTIONS) {
        strncat(tac_output,
            "Error: Too many TAC instructions!\n",
            50000 - strlen(tac_output) - 1);
        return;
    }

    /* Store the instruction */
    strncpy(tac_list[tac_count].instruction,
            instruction,
            MAX_TAC_LINE - 1);
    tac_count++;
}


int get_tac_count() {
    return tac_count;
}


void print_tac() {
    int i;
    char line[512];

    /* Header */
    strncat(tac_output,
        "============================================\n"
        "      THREE ADDRESS CODE (TAC)\n"
        "============================================\n",
        50000 - strlen(tac_output) - 1);

    /* Column headers */
    strncat(tac_output,
        " #   | Instruction\n"
        "-----|-------------------------------------\n",
        50000 - strlen(tac_output) - 1);

    /* Each instruction row */
    if (tac_count == 0) {
        strncat(tac_output,
            "(no TAC instructions generated)\n",
            50000 - strlen(tac_output) - 1);
    } else {
        for (i = 0; i < tac_count; i++) {
            /* Labels like "L1:" get no line number indent */
            if (tac_list[i].instruction[strlen(tac_list[i].instruction)-1] == ':') {
                snprintf(line, sizeof(line),
                    " %-3d | %s\n",
                    i + 1,
                    tac_list[i].instruction);
            } else {
                snprintf(line, sizeof(line),
                    " %-3d |   %s\n",
                    i + 1,
                    tac_list[i].instruction);
            }
            strncat(tac_output, line,
                50000 - strlen(tac_output) - 1);
        }
    }

    /* Footer with count */
    snprintf(line, sizeof(line),
        "============================================\n"
        "Total: %d instruction(s)\n"
        "============================================\n",
        tac_count);
    strncat(tac_output, line,
        50000 - strlen(tac_output) - 1);
}
