#ifndef ASS_FUNCS_H

#define ASS_FUNCS_H

#include "stack.h"

#define CMD_LEN 32
#define DEF_ASS_FUNC(name) int ass_##name \
    (stack_t* stk, FILE* fp, stack_t* lbl_stk)

DEF_ASS_FUNC (hlt);
DEF_ASS_FUNC (push);
DEF_ASS_FUNC (pop);
DEF_ASS_FUNC (out);
DEF_ASS_FUNC (in);
DEF_ASS_FUNC (sum);
DEF_ASS_FUNC (sub);
DEF_ASS_FUNC (mult);
DEF_ASS_FUNC (div);
DEF_ASS_FUNC (lbl);
DEF_ASS_FUNC (jmp);

int read_until_space (FILE* fp, char* dest, size_t n);

struct label
{
    char name[CMD_LEN];
    size_t ip;
};
#endif
