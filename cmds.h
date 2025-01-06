#ifndef CMDS_H
#define CMDS_H

#include <stdio.h>
#include "stack.h"

typedef double proc_elem_t;

struct spu {
    struct stack_t* main_stk;
    struct stack_t* funcs_stk;
    proc_elem_t reg[4];

    proc_elem_t* code;
    proc_elem_t* ram;

    size_t code_size;
    size_t ram_size;
    size_t ip;
};

enum COMMAND_CODES
{
    CMD_CODE_HLT,
    CMD_CODE_PUSH,
    CMD_CODE_POP,
    CMD_CODE_OUT,
    CMD_CODE_IN,
    CMD_CODE_SUM,
    CMD_CODE_SUB,
    CMD_CODE_MULT,
    CMD_CODE_DIV,
};

struct command {
    const char* name;
    int (*ass_func) (stack_t* stk, FILE* fp);
    int (*proc_func) (spu* proc);
    int code;
};

extern const command cmd_list[];
extern const size_t cmd_count;

extern const char* reg_list[];
extern const size_t reg_count;

#endif
