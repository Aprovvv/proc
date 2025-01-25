#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ass_funcs.h"
#include "cmds.h"
#include "stack.h"
#include "color_print/color_print.h"

int run_ass (stack_t* cmd_stk, FILE* fp);

int main (int argc, char* argv[])
{
    const char* output_name = "src.bin";
    if (argc < 2)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED, "FILE NAME NOT SPECIFIED\n");
        return EXIT_FAILURE;
    }
    if (argc == 4)
    {
        if (strcmp (argv[2], "-o") != 0)
        {
            fprintf_color(stderr, CONSOLE_TEXT_RED, "UNDEFINED FLAG %s\n", argv[2]);
            return EXIT_FAILURE;
        }
        output_name = argv[3];
    }

    FILE* fp = fopen (argv[1], "r");
    if (fp == NULL)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED, "FAILED TO OPEN INPUT FILE\n");
        return EXIT_FAILURE;
    }

    stack_t* cmd_stk = stack_init (sizeof(proc_elem_t), 10);
    if (run_ass (cmd_stk, fp) == EXIT_FAILURE)
        return EXIT_FAILURE;

    void* ptr = stack_data (cmd_stk);
    FILE* fout = fopen (output_name, "wb");
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

int run_ass (stack_t* cmd_stk, FILE* fp)
{
    char cmd[CMD_LEN] = "";
    stack_t* lbl_stk = stack_init (sizeof (label), 4);
    stack_t* defined_lbl_stk = stack_init (sizeof (label), 4);

    proc_elem_t zero = 0;
    stack_push (cmd_stk, &zero);
    while (read_until_space (fp, cmd, CMD_LEN) != EOF)
    {
        //1 проход
        size_t i = 0;
        for (i = 0; i < cmd_count; i++)
        {
            if (strcmp(cmd, cmd_list[i].name) == 0)
            {
                cmd_list[i].ass_func(cmd_stk, fp, lbl_stk);
                break;
            }
        }
        if (i == cmd_count)
        {
            if (cmd[strlen(cmd) - 1] == ':')
            {
                label l = {"", stack_size (cmd_stk) - 1};
                strncpy (l.name, cmd, strlen(cmd) - 1);
                proc_elem_t lbl_code = CMD_CODE_LBL;
                stack_push (defined_lbl_stk, &l);
            }
            else
            {
                fprintf_color (stderr, CONSOLE_TEXT_RED, "UNDEFINED COMMAND: %s\n", cmd);
                stack_destroy (defined_lbl_stk);
                stack_destroy (lbl_stk);
                stack_destroy (cmd_stk);
                return EXIT_FAILURE;
            }
        }
    }
    //проставляем адреса в джампах
    size_t jmp_count = stack_size (lbl_stk);
    size_t lbl_count = stack_size (defined_lbl_stk);
    label* lbl_ptr = (label*) stack_data (defined_lbl_stk);
    proc_elem_t* cmd_ptr = (proc_elem_t*) stack_data (cmd_stk);
    for (size_t j = 0; j < jmp_count; j++)
    {
        label lbl = {};
        stack_pop (lbl_stk, &lbl);
        size_t i = 0;
        for (i = 0; i < lbl_count; i++)
        {
            if (strcmp (lbl.name, lbl_ptr[i].name) == 0)
            {
                cmd_ptr[lbl.ip] = (proc_elem_t) lbl_ptr[i].ip;
                break;
            }
        }
        if (i == lbl_count)
        {
            fprintf_color (stderr, CONSOLE_TEXT_RED, "UNDEFINED LABEL: %s\n", lbl.name);
            stack_destroy (defined_lbl_stk);
            stack_destroy (lbl_stk);
            stack_destroy (cmd_stk);
            return EXIT_FAILURE;
        }
    }

    stack_destroy (defined_lbl_stk);
    stack_destroy (lbl_stk);
    return 0;
}
