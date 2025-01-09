#ifndef PROC_FUNCS_H

#define PROC_FUNCS_H

#include "cmds.h"

int proc_hlt (spu* proc);
int proc_push (spu* proc);
int proc_pop (spu* proc);
int proc_out (spu* proc);
int proc_in (spu* proc);
int proc_sum (spu* proc);
int proc_sub (spu* proc);
int proc_mult (spu* proc);
int proc_div (spu* proc);
int proc_lbl (spu* proc);
int proc_jmp (spu* proc);

#endif
