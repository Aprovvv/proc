#include "cmds.h"

int ass_hlt (stack_t* stk, FILE* fp);
int ass_push (stack_t* stk, FILE* fp);

extern const command cmd_list[] =
{
    {"hlt", ass_hlt},
    {"push", ass_push},
};

extern const size_t cmd_count = sizeof(cmd_list) / sizeof(cmd_list[0]);
