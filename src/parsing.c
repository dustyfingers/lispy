#include <stdio.h>
#include <stdlib.h>

// linked libraries
#include "mpc.h"

// if we are compiling on Windows compile these functions
#ifdef _WIN32
#include <string.h>

// declare a buffer that holds user input with a length of 2048
static char input_buffer[2048];

// fake readline function
char *readline(char *prompt)
{
    fputs(prompt, stdout);
    fgets(input_buffer, 2048, stdin);
    char *cpy = malloc(strlen(input_buffer) + 1);
    strcpy(cpy, input_buffer);
    cpy[strlen(cpy) - 1] = '\0'; // null terminator for end of string' return cpy;
    return cpy;
}

// fake add_history function
void add_history(char *unused) {}

// otherwise include the editline headers
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

// Lispy Value struct
// expressions can evaluate to either values or errors
// this concept is encapsulated in the 'lispy value'
typedef struct
{
    int type;
    long num;
    int err;
} lval;

// possible lval types
enum
{
    LVAL_NUM,
    LVAL_ERR
};

// possible lval error types
enum
{
    LERR_DIV_ZERO,
    LERR_BAD_OP,
    LERR_BAD_NUM
};

// construct a new number type lval

lval lval_num(long x)
{
    lval v;
    v.type = LVAL_NUM;
    v.num = x;
    return v;
}

// construct a new error type lval
lval lval_err(int x)
{
    lval v;
    v.type = LVAL_ERR;
    v.err = x;
    return v;
}

// print an 'lval'
void lval_print(lval value)
{
    switch (value.type)
    {
    case LVAL_NUM:
        printf("%li", value.num);
        break;

    case LVAL_ERR:
        if (value.err == LERR_DIV_ZERO)
        {
            printf("Error: Division by Zero!");
        }
        if (value.err == LERR_BAD_OP)
        {
            printf("Error: Invalid Operator!");
        }
        if (value.err == LERR_BAD_NUM)
        {
            printf("Error: Invalid Number!");
        }
        break;
    }
}

void lval_println(lval value)
{
    lval_print(value);
    putchar('\n');
};

// print version and exit information
void lispy_greeting()
{

    puts("Lispy Version 0.1");
    puts("Press Ctrl+c to Exit\n");
}

// recursive number of nodes function
int number_of_nodes(mpc_ast_t *t)
{
    // base case
    if (t->children_num == 0)
    {
        return 1;
    }

    // recursive case
    if (t->children_num >= 1)
    {
        int total = 1;
        for (int i = 0; i < t->children_num; i++)
        {
            total = total + number_of_nodes(t->children[i]);
        }
        return total;
    }

    return 0;
}

// operator evaluator
lval eval_op(lval x, char *op, lval y)
{

    // if either operator has an error type, return it
    if (x.type == LVAL_ERR)
    {
        return x;
    }
    if (y.type == LVAL_ERR)
    {
        return y;
    }

    // otherwise evaluate as a valid operator
    if (strcmp(op, "+") == 0)
    {
        return lval_num(x.num + y.num);
    }
    if (strcmp(op, "-") == 0)
    {
        return lval_num(x.num - y.num);
    }
    if (strcmp(op, "*") == 0)
    {
        return lval_num(x.num * y.num);
    }
    if (strcmp(op, "/") == 0)
    {
        return y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num);
    }
    return lval_err(LERR_BAD_OP);
}

// recursive evaluation function
lval eval(mpc_ast_t *t)
{
    // if tagged as a number, this node has no children, we can return it directly
    // base case
    if (strstr(t->tag, "number"))
    {
        // check for conversion error
        errno = 0;
        long x = strtol(t->contents, NULL, 10);

        return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
    }

    // operator is always second child
    char *operator= t->children[1]->contents;

    // store third child in x
    lval x = eval(t->children[2]);

    // iterate the remaining children and combine
    int i = 3;
    while (strstr(t->children[i]->tag, "expr"))
    {
        x = eval_op(x, operator, eval(t->children[i]));
        i++;
    }

    return x;
}

int main(int argc, char **argv)
{
    // define parsers
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Operator = mpc_new("operator");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Lispy = mpc_new("lispy");

    // define grammar
    mpca_lang(
        MPCA_LANG_DEFAULT,
        // lispy represents the totality of our language
        "                                                               \
            number        : /-?[0-9]+/ ;                                \
            operator      : '+' | '-' | '*' | '/' ;                     \
            expr          : <number> | '(' < operator><expr> + ')' ;    \
            lispy         : /^/ <operator><expr>+ /$/ ;                 \
        ",
        Number, Operator, Expr, Lispy);

    lispy_greeting();

    // in a never ending loop...
    while (1)
    {

        // no matter the compilation platform, readline() will be defined!
        char *input = readline("lispy>");

        add_history(input);

        // parse input using mpc parsers
        mpc_result_t parse_result;
        if (mpc_parse("<stdin>", input, Lispy, &parse_result))
        {
            // on success, print AST (abstract syntax tree) after parse
            lval eval_result = eval(parse_result.output);
            lval_println(eval_result);
            mpc_ast_delete(parse_result.output);
        }
        else
        {
            // otherwise, handle error
            mpc_err_print(parse_result.error);
            mpc_err_delete(parse_result.error);
        }

        free(input);
    }

    // clean up parsers defined above
    mpc_cleanup(4, Number, Operator, Expr, Lispy);

    return 0;
}