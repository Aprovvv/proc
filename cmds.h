#ifndef CMDS_H
#define CMDS_H

#include <stdio.h>
#include "stack.h"

enum COMMAND_CODES
{
    CMD_CODE_HLT,
    CMD_CODE_PUSH,
};

struct command {
    const char* name;
    int (*ass_func) (stack_t* stk, FILE* fp);
};

extern const command cmd_list[];
extern const size_t cmd_count;

#endif
