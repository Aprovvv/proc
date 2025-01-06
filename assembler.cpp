#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ass_funcs.h"
#include "cmds.h"
#include "stack.h"
#include "color_print/color_print.h"

int pr (const void* a);

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

    stack_t* cmd_stk = stack_init (sizeof(proc_elem_t), 10);
    char cmd[CMD_LEN] = "";

    while (read_until_space (fp, cmd, CMD_LEN) != EOF)
    {
        for (size_t i = 0; i < cmd_count; i++)
        {
            if (strcmp(cmd, cmd_list[i].name) == 0)
            {
                cmd_list[i].ass_func(cmd_stk, fp);
                continue;
            }
        }
    }

    stack_printf (cmd_stk, pr);
    void* ptr = stack_data (cmd_stk);
    FILE* fout = fopen ("src.bin", "wb");
    if (fout == NULL)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED, "FAILED TO OPEN OUTPUT FILE\n");
        return EXIT_FAILURE;
    }
    fwrite (ptr, sizeof (proc_elem_t), stack_size (cmd_stk), fout);
    fclose (fout);
    fclose (fp);
    stack_destroy (cmd_stk);
}

int pr (const void* a)
{
    return fprintf(stderr, "%f", *((const proc_elem_t*)a));
}
