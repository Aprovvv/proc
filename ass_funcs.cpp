#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cmds.h"
#include "ass_funcs.h"
#include "stack.h"
#include "color_print/color_print.h"

static int get_type (char* arg_str,
                          proc_elem_t* type,
                          proc_elem_t* arg_num,
                          proc_elem_t* arg_reg);
static int universal_jmp (stack_t* stk,
                          FILE* fp,
                          stack_t* lbl_stk,
                          proc_elem_t jmp_code);

static size_t get_reg_num (const char* reg_name);

int ass_hlt (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t hlt = CMD_CODE_HLT;
    stack_push (stk, &hlt);
    return 0;
}

int ass_push (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t push = CMD_CODE_PUSH;
    stack_push (stk, &push);

    char arg_str[CMD_LEN] = "";
    read_until_space (fp, arg_str, CMD_LEN);

    proc_elem_t type = 0, arg_num = 0, arg_reg = 0;
    if (get_type (arg_str, &type, &arg_num, &arg_reg) == EXIT_FAILURE)
        return EXIT_FAILURE;

    int int_type = (int)type;
    stack_push (stk, &type);
    if (int_type & MASK_NUM)
        stack_push (stk, &arg_num);
    if (int_type & MASK_REG)
        stack_push (stk, &arg_reg);

    return 0;
}

int ass_pop (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t pop = CMD_CODE_POP;
    stack_push (stk, &pop);

    char arg_str[CMD_LEN] = "";
    read_until_space (fp, arg_str, CMD_LEN);

    proc_elem_t type = 0, arg_num = 0, arg_reg = 0;
    if (get_type (arg_str, &type, &arg_num, &arg_reg) == EXIT_FAILURE)
        return EXIT_FAILURE;

    int int_type = (int)type;
    if (!(int_type & MASK_MEM) && (int_type & MASK_NUM))
    {
        fprintf_color (stderr, CONSOLE_TEXT_RED, "UNDEFINED REGISTER %s\n", arg_str);
        return EXIT_FAILURE;
    }

    stack_push (stk, &type);
    if (int_type & MASK_NUM)
        stack_push (stk, &arg_num);
    if (int_type & MASK_REG)
        stack_push (stk, &arg_reg);

    return 0;
}

int ass_out (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t out = CMD_CODE_OUT;
    stack_push (stk, &out);

    return 0;
}

int ass_in (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t in = CMD_CODE_IN;
    stack_push (stk, &in);

    return 0;
}

int ass_sum (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t sum = CMD_CODE_SUM;
    stack_push(stk, &sum);

    return 0;
}

int ass_sub (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t sub = CMD_CODE_SUB;
    stack_push(stk, &sub);

    return 0;
}

int ass_mult (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t mult = CMD_CODE_MULT;
    stack_push(stk, &mult);

    return 0;
}

int ass_div (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t div = CMD_CODE_DIV;
    stack_push(stk, &div);

    return 0;
}

int ass_lbl (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    return 0;
}

int ass_jmp (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    universal_jmp (stk, fp, lbl_stk, CMD_CODE_JMP);

    return 0;
}

int ass_ja (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    universal_jmp (stk, fp, lbl_stk, CMD_CODE_JA);

    return 0;
}

int ass_jae (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    universal_jmp (stk, fp, lbl_stk, CMD_CODE_JAE);

    return 0;
}

int ass_jb (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    universal_jmp (stk, fp, lbl_stk, CMD_CODE_JB);

    return 0;
}

int ass_jbe (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    universal_jmp (stk, fp, lbl_stk, CMD_CODE_JBE);

    return 0;
}

int ass_je (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    universal_jmp (stk, fp, lbl_stk, CMD_CODE_JE);

    return 0;
}

int ass_jme (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    universal_jmp (stk, fp, lbl_stk, CMD_CODE_JME);

    return 0;
}

int ass_sqrt (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t code = CMD_CODE_SQRT;
    stack_push (stk, &code);

    return 0;
}

int ass_sin (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t code = CMD_CODE_SIN;
    stack_push (stk, &code);

    return 0;
}

int ass_draw (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t code = CMD_CODE_DRAW;
    proc_elem_t width = 0, height = 0;
    fscanf (fp, "%lf%lf", &width, &height);

    stack_push (stk, &code);
    stack_push (stk, &width);
    stack_push (stk, &height);

    return 0;
}

int ass_call (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    label l = {"", stack_size (stk) + 1};
    read_until_space (fp, l.name, CMD_LEN);
    stack_push (lbl_stk, &l);

    proc_elem_t call = CMD_CODE_CALL, minus_1 = -1;
    stack_push (stk, &call);
    stack_push (stk, &minus_1);

    return 0;
}

int ass_ret (stack_t* stk, FILE* fp, stack_t* lbl_stk)
{
    proc_elem_t ret = CMD_CODE_RET;
    stack_push (stk, &ret);

    return 0;
}

int read_until_space (FILE* fp, char* dest, size_t n)
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

static int universal_jmp (stack_t* stk,
                          FILE* fp,
                          stack_t* lbl_stk,
                          proc_elem_t jmp_code)
{
    label l = {"", stack_size (stk) + 1};
    read_until_space (fp, l.name, CMD_LEN);
    stack_push (lbl_stk, &l);

    proc_elem_t minus_1 = -1;
    stack_push (stk, &jmp_code);
    stack_push (stk, &minus_1);

    return 0;
}

static int get_type (char* arg_str,
                          proc_elem_t* type,
                          proc_elem_t* arg_num,
                          proc_elem_t* arg_reg)
{
    char reg_name [CMD_LEN] = "";
    if (strchr (arg_str, '[') && strchr (arg_str, ']'))
    {
        *type += MASK_MEM;
        char temp_str [CMD_LEN] = "";
        sscanf (arg_str + 1, "%[^]]", temp_str);
        strcpy (arg_str, temp_str);
    }
    if ((strchr(arg_str, '[') == NULL) != (strchr(arg_str, ']') == NULL))
    {
        fprintf_color (stderr, CONSOLE_TEXT_RED, "UNCLOSED BRACKETS IN %s\n", arg_str);
        return EXIT_FAILURE;
    }

    if (sscanf (arg_str, "%lf+%s", arg_num, reg_name) == 2)
    {
        *type += MASK_NUM + MASK_PLUS + MASK_REG;
        size_t reg_num = get_reg_num (reg_name);
        if (reg_num == (size_t)-1)
        {
            fprintf_color (stderr, CONSOLE_TEXT_RED, "UNDEFINED REGISTER NAME %s", reg_name);
            return EXIT_FAILURE;
        }
        *arg_reg = (proc_elem_t)reg_num;
        return 0;
    }
    if (sscanf (arg_str, "%lf-%s", arg_num, reg_name) == 2)
    {
        *type += MASK_NUM + MASK_MINUS + MASK_REG;
        size_t reg_num = get_reg_num (reg_name);
        if (reg_num == (size_t)-1)
        {
            fprintf_color (stderr, CONSOLE_TEXT_RED, "UNDEFINED REGISTER NAME %s", reg_name);
            return EXIT_FAILURE;
        }
        *arg_reg = (proc_elem_t)reg_num;
        return 0;
    }
    if (sscanf (arg_str, "%lf*%s", arg_num, reg_name) == 2)
    {
        *type += MASK_NUM + MASK_STAR + MASK_REG;
        size_t reg_num = get_reg_num (reg_name);
        if (reg_num == (size_t)-1)
        {
            fprintf_color (stderr, CONSOLE_TEXT_RED, "UNDEFINED REGISTER NAME %s", reg_name);
            return EXIT_FAILURE;
        }
        *arg_reg = (proc_elem_t)reg_num;
        return 0;
    }

    if (sscanf (arg_str, "%lf", arg_num) == 1)
    {
        *type += MASK_NUM;
        return 0;
    }

    sscanf (arg_str, "%s", reg_name);
    *type += MASK_REG;
    size_t reg_num = get_reg_num (reg_name);
    if (reg_num == (size_t)-1)
    {
        fprintf_color (stderr, CONSOLE_TEXT_RED, "UNDEFINED REGISTER NAME %s", reg_name);
        return EXIT_FAILURE;
    }
    *arg_reg = (proc_elem_t)reg_num;
    return 0;
}

static size_t get_reg_num (const char* reg_name)
{
    size_t result;
    size_t i = 0;
    for (i = 0; i < reg_count; i++)
    {
        if (strcmp(reg_name, reg_list[i]) == 0)
        {
            result = i;
            break;
        }
    }
    if (i == reg_count)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "UNDEFINED REGISTER NAME %s\n", reg_name);
        return (size_t)-1;
    }
    return result;
}
