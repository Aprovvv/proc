#include "cmds.h"
#include "ass_funcs.h"
#include "proc_funcs.h"

#define DEFCMD(name, code) {#name, ass_##name, proc_##name, code}

extern const command cmd_list[] =
{
    DEFCMD(hlt, CMD_CODE_HLT),
    DEFCMD(push, CMD_CODE_PUSH),
    DEFCMD(pop, CMD_CODE_POP),
    DEFCMD(out, CMD_CODE_OUT),
    DEFCMD(in, CMD_CODE_IN),
    DEFCMD(sum, CMD_CODE_SUM),
    DEFCMD(sub, CMD_CODE_SUB),
    DEFCMD(mult, CMD_CODE_MULT),
    DEFCMD(div, CMD_CODE_DIV)
};

extern const size_t cmd_count = sizeof(cmd_list) / sizeof(cmd_list[0]);

extern const char* reg_list[] =
{
    "ax", "bx", "cx", "dx"
};

extern const size_t reg_count = sizeof(reg_list) / sizeof(reg_list[0]);
