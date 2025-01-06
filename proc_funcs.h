#ifndef PROC_FUNCS_H

#define PROC_FUNCS_H

#include "cmds.h"

int proc_hlt (spu* proc);
int proc_push (spu* proc);
int proc_pop (spu* proc);
int proc_out (spu* proc);
int proc_in (spu* proc);

#endif
