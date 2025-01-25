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
    CMD_CODE_LBL,
    CMD_CODE_JMP,
    CMD_CODE_JA,
    CMD_CODE_JAE,
    CMD_CODE_JB,
    CMD_CODE_JBE,
    CMD_CODE_JE,
    CMD_CODE_JME,
    CMD_CODE_SQRT,
    CMD_CODE_SIN,
    CMD_CODE_DRAW,
    CMD_CODE_CALL,
    CMD_CODE_RET,
};

enum PUSH_MASKS
{
    MASK_NUM       = 0x1,
    MASK_REG       = 0x2,
    MASK_PLUS      = 0x4,
    MASK_MINUS     = 0x8,
    MASK_STAR      = 0x10,
    MASK_MEM       = 0x20,
};

struct command {
    const char* name;
    int (*ass_func) (stack_t* stk,
                     FILE* fp,
                     stack_t* lbl_stk);
    int (*proc_func) (spu* proc);
    int code;
};

extern const command cmd_list[];
extern const size_t cmd_count;

extern const char* reg_list[];
extern const size_t reg_count;

#endif
