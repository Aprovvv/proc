#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cmds.h"
#include "stack.h"
#include "color_print/color_print.h"

#define CMD_LEN 32
extern const command cmd_list[];
extern const size_t cmd_count;

static int read_until_space (FILE* fp, char* dest, size_t n);
int pr (const void* a);

int ass_hlt (stack_t* stk, FILE* fp);
int ass_push (stack_t* stk, FILE* fp);

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
        fprintf_color(stderr, CONSOLE_TEXT_RED, "FAILED TO OPEN ASSEMBLER FILE\n");
        return EXIT_FAILURE;
    }

    stack_t* cmd_stk = stack_init (sizeof(double), 10);
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

    stack_printf(cmd_stk, pr);
    stack_destroy(cmd_stk);
}

int pr (const void* a)
{
    return fprintf(stderr, "%f", *((const double*)a));
}

int ass_hlt (stack_t* stk, FILE* fp)
{
    double hlt = CMD_CODE_HLT;
    stack_push (stk, &hlt);
    return 0;
}

int ass_push (stack_t* stk, FILE* fp)
{
    double push = CMD_CODE_PUSH;
    stack_push (stk, &push);

    char arg_str[CMD_LEN] = "";
    read_until_space (fp, arg_str, CMD_LEN);

    double arg_double = 0;
    sscanf (arg_str, "%lf", &arg_double);
    stack_push (stk, &arg_double);

    return 0;
}

static int read_until_space (FILE* fp, char* dest, size_t n)
{
    size_t i = 0;
    int ch = getc(fp);
    while (!isspace(ch) && i < n && ch != EOF)
    {
        dest[i++] = ch;
        ch = getc(fp);
    }
    dest[i] = '\0';
    return ch;
}
