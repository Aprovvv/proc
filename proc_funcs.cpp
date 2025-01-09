#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cmds.h"
#include "proc_funcs.h"
#include "stack.h"
#include "color_print/color_print.h"

static int pr_double (const void* a);
static int dblcmp (proc_elem_t a, proc_elem_t b);

static int pr_double (const void* p)
{
    return fprintf (stderr, "%f", *((const double*)p));
}

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

int proc_sum (spu* proc)
{
    proc_elem_t a = 0, b = 0;
    stack_pop (proc->main_stk, &a);
    stack_pop (proc->main_stk, &b);
    a = b + a;
    stack_push(proc->main_stk, &a);

    return 0;
}

int proc_sub (spu* proc)
{
    proc_elem_t a = 0, b = 0;
    stack_pop (proc->main_stk, &a);
    stack_pop (proc->main_stk, &b);
    a = b - a;
    stack_push(proc->main_stk, &a);

    return 0;
}
int proc_mult (spu* proc)
{
    proc_elem_t a = 0, b = 0;
    stack_pop (proc->main_stk, &a);
    stack_pop (proc->main_stk, &b);
    a = b * a;
    stack_push(proc->main_stk, &a);

    return 0;
}

int proc_div (spu* proc)
{
    proc_elem_t a = 0, b = 0;
    stack_pop (proc->main_stk, &a);
    stack_pop (proc->main_stk, &b);
    a = b / a;
    stack_push(proc->main_stk, &a);

    return 0;
}

int proc_lbl (spu* proc)
{
    return 0;
}

int proc_jmp (spu* proc)
{
    proc->ip++;
    proc->ip = (size_t)proc->code[proc->ip];
    return 0;
}

int proc_jae (spu* proc)
{
    proc_elem_t a = 0, b = 0;
    stack_pop (proc->main_stk, &b);
    stack_pop (proc->main_stk, &a);
    proc->ip++;
    if (dblcmp (a, b) >= 0)
        proc->ip = (size_t)proc->code[proc->ip];

    return 0;
}

int proc_ja (spu* proc)
{
    proc_elem_t a = 0, b = 0;
    stack_pop (proc->main_stk, &b);
    stack_pop (proc->main_stk, &a);
    proc->ip++;
    if (dblcmp (a, b) > 0)
        proc->ip = (size_t)proc->code[proc->ip];

    return 0;
}

int proc_jb (spu* proc)
{
    proc_elem_t a = 0, b = 0;
    stack_pop (proc->main_stk, &b);
    stack_pop (proc->main_stk, &a);
    proc->ip++;
    if (dblcmp (a, b) < 0)
        proc->ip = (size_t)proc->code[proc->ip];

    return 0;
}

int proc_jbe (spu* proc)
{
    proc_elem_t a = 0, b = 0;
    stack_pop (proc->main_stk, &b);
    stack_pop (proc->main_stk, &a);
    proc->ip++;
    if (dblcmp (a, b) <= 0)
        proc->ip = (size_t)proc->code[proc->ip];

    return 0;
}

int proc_je (spu* proc)
{
    proc_elem_t a = 0, b = 0;
    stack_pop (proc->main_stk, &b);
    stack_pop (proc->main_stk, &a);
    proc->ip++;
    if (dblcmp (a, b) == 0)
        proc->ip = (size_t)proc->code[proc->ip];

    return 0;
}

int proc_jme (spu* proc)
{
    proc_elem_t a = 0, b = 0;
    stack_pop (proc->main_stk, &b);
    stack_pop (proc->main_stk, &a);
    proc->ip++;
    if (dblcmp (a, b) != 0)
        proc->ip = (size_t)proc->code[proc->ip];

    return 0;
}

int proc_sqrt (spu* proc)
{
    proc_elem_t val = 0;
    stack_pop (proc->main_stk, &val);
    val = sqrt (val);
    stack_push (proc->main_stk, &val);

    return 0;
}

int proc_sin (spu* proc)
{
    proc_elem_t val = 0;
    stack_pop (proc->main_stk, &val);
    val = sin (val);
    stack_push (proc->main_stk, &val);

    return 0;
}

static int dblcmp (proc_elem_t a, proc_elem_t b)
{
    const double EPS = 10e-6;
    if (abs (a - b) < EPS)
        return 0;
    if (a > b)
        return 1;
    return -1;
}
