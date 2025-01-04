#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "proc_funcs.h"
#include "cmds.h"
#include "stack.h"
#include "color_print/color_print.h"

struct spu {
    struct stack_t* main_stk;
    struct stack_t* funcs_stk;
    proc_elem_t reg[4];

    proc_elem_t* code;
    proc_elem_t* ram;

    size_t code_size;
    size_t ram_size;
    size_t ip;
};

int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED, "FILE NAME NOT SPECIFIED\n");
        return EXIT_FAILURE;
    }
    FILE* fp = fopen (argv[1], "r");
    if (fp == NULL)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED, "FAILED TO OPEN INPUT FILE\n");
        return EXIT_FAILURE;
    }
    struct stat filedata = {};
    stat (argv[1], &filedata);

    spu proc = proc_init((size_t)filedata.st_size);

    for (proc.ip = 0; proc.ip < proc.code_size; proc.ip++)
        for (int i = 0; i < cmd_count; i++)
            if ((int)proc.code[i] == cmd_list[i].code)
                if (cmd_list[i].proc_func(&proc) != 0)
                    break;

    proc_destroy (proc);
    fclose fp;
}

int proc_hlt (spu* proc)
{
    return 1;
}

int proc_push (spu* proc)
{
    proc.ip++;
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
    scanf("%f", &val);
    stack_push(proc->main_stk, &val);

    return 0;
}

spu proc_init (size_t code_size)
{
    spu proc = {};
    proc.main_stk = stack_init (sizeof (proc_elem_t), 64);
    proc.funcs_stk = stack_init (sizeof (size_t), 8);
    proc.ram_size = 1024;
    proc.ram = (proc_elem_t*) calloc (sizeof(proc_elem_t), proc.ram_size);
    proc.code_size = code_size;
    proc.code = (proc_elem_t*) calloc (sizeof(proc_elem_t), proc.code_size);
}

spu proc_destroy (spu proc)
{
    stack_destroy (proc.main_stk);
    stack_destroy (proc.funcs_stk);
    free(proc.ram);
}
