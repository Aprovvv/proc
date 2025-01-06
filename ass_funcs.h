#ifndef ASS_FUNCS_H

#define ASS_FUNCS_H

#include "stack.h"

#define CMD_LEN 32

int ass_hlt (stack_t* stk, FILE* fp);
int ass_push (stack_t* stk, FILE* fp);
int ass_pop (stack_t* stk, FILE* fp);
int ass_out (stack_t* stk, FILE* fp);
int ass_in (stack_t* stk, FILE* fp);

int read_until_space (FILE* fp, char* dest, size_t n);
#endif
