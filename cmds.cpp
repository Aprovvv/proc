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
    DEFCMD(div, CMD_CODE_DIV),
    {"", ass_lbl, proc_lbl, CMD_CODE_LBL},
    DEFCMD(jmp, CMD_CODE_JMP),
    DEFCMD(ja, CMD_CODE_JA),
    DEFCMD(jae, CMD_CODE_JAE),
    DEFCMD(jb, CMD_CODE_JB),
    DEFCMD(jbe, CMD_CODE_JBE),
    DEFCMD(je, CMD_CODE_JE),
    DEFCMD(jme, CMD_CODE_JME),
};

extern const size_t cmd_count = sizeof(cmd_list) / sizeof(cmd_list[0]);

extern const char* reg_list[] =
{
    "ax", "bx", "cx", "dx"
};

extern const size_t reg_count = sizeof(reg_list) / sizeof(reg_list[0]);
