#include <stdio.h>
#include <stdlib.h>

#include "cmds.h"
#include "proc_funcs.h"
#include "stack.h"
#include "color_print/color_print.h"

int proc_hlt (spu* proc)
{
    return 1;
}

int proc_push (spu* proc)
{
    proc->ip++;
    stack_push (proc->main_stk, proc->code + proc->ip);

    return 0;
}

int proc_pop (spu* proc)
{
    proc->ip++;
    size_t reg_num = (size_t) proc->code[proc->ip];
    stack_pop (proc->main_stk, &proc->reg[reg_num]);

    return 0;
}

int proc_out (spu* proc)
{
    proc_elem_t val = 0;
    stack_pop (proc->main_stk, &val);
    printf("%f\n", val);

    return 0;
}

int proc_in (spu* proc)
{
    proc_elem_t val = 0;
    scanf("%lf", &val);
    stack_push(proc->main_stk, &val);

    return 0;
}
