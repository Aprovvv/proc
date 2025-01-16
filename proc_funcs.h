#ifndef PROC_FUNCS_H

#define PROC_FUNCS_H

#include "cmds.h"

int proc_hlt  (spu* proc);
int proc_push (spu* proc);
int proc_pop  (spu* proc);
int proc_out  (spu* proc);
int proc_in   (spu* proc);
int proc_sum  (spu* proc);
int proc_sub  (spu* proc);
int proc_mult (spu* proc);
int proc_div  (spu* proc);
int proc_lbl  (spu* proc);
int proc_jmp  (spu* proc);
int proc_ja   (spu* proc);
int proc_jae  (spu* proc);
int proc_jb   (spu* proc);
int proc_jbe  (spu* proc);
int proc_je   (spu* proc);
int proc_jme  (spu* proc);
int proc_sqrt (spu* proc);
int proc_sin  (spu* proc);
int proc_draw (spu* proc);
int proc_call (spu* proc);
int proc_ret  (spu* proc);

#endif
