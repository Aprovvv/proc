#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <assert.h>

#include "proc_funcs.h"
#include "cmds.h"
#include "stack.h"
#include "color_print/color_print.h"

static spu proc_init (const char* filename);
static void proc_destroy (spu proc);
static int read_code (spu* proc, const char* filename);

int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED, "FILE NAME NOT SPECIFIED\n");
        exit(EXIT_FAILURE);
    }

    spu proc = proc_init(argv[1]);
    spu empty_struct = {0};
    if (memcmp (&proc, &empty_struct, sizeof (spu)) == 0)
        exit(EXIT_FAILURE);

    for (proc.ip = 0; proc.ip < proc.code_size; proc.ip++)
    {
        for (size_t i = 0; i < cmd_count; i++)
        {
            if ((int)proc.code[proc.ip] == cmd_list[i].code)
            {
                if (cmd_list[i].proc_func(&proc) != 0)
                    goto end;
                break;
            }
        }
    }
end:
    proc_destroy (proc);
}

static spu proc_init (const char* filename)
{
    spu proc = {0};

    proc.main_stk = stack_init (sizeof (proc_elem_t), 20);
    proc.funcs_stk = stack_init (sizeof (size_t), 8);
    proc.ram_size = 1024;
    proc.ram = (proc_elem_t*) calloc (sizeof (proc_elem_t), proc.ram_size);
    if (proc.ram == NULL)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "FAILED TO ALLOCATE MEMORY FOR RAM\n");
        goto init_failure;
    }
    if (read_code (&proc, filename) == EXIT_FAILURE)
        goto init_failure;

    return proc;
init_failure:
    proc_destroy (proc);
    proc = {0};
    return proc;
}

static int read_code (spu* proc, const char* filename)
{
    FILE* fp = fopen (filename, "r");
    if (fp == NULL)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "FAILED TO OPEN INPUT FILE\n");
        return EXIT_FAILURE;
    }
    struct stat filedata = {};
    stat (filename, &filedata);

    proc->code_size = (size_t) filedata.st_size;
    proc->code = (proc_elem_t*) calloc (sizeof (proc_elem_t), proc->code_size);
    if (proc->code == NULL)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "FAILED TO ALLOCATE MEMORY FOR MACHINE CODE\n");
        return EXIT_FAILURE;
    }
    fread (proc->code, sizeof (proc_elem_t), proc->code_size, fp);

    fclose (fp);
    return 0;
}

static void proc_destroy (spu proc)
{
    if (proc.main_stk)
        stack_destroy (proc.main_stk);
    if (proc.funcs_stk)
        stack_destroy (proc.funcs_stk);
    if (proc.ram)
        free (proc.ram);
    if (proc.code)
        free (proc.code);
}
