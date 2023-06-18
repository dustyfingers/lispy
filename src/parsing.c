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

// print version and exit information
void lispy_greeting()
{

    puts("Lispy Version 0.1");
    puts("Press Ctrl+c to Exit\n");
}

int main(int argc, char **argv)
{

    // TODO: functionize this?

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
        mpc_result_t result;
        if (mpc_parse("<stdin>", input, Lispy, &result))
        {
            // on success, print AST (what is that??)
            mpc_ast_print(result.output);
            mpc_ast_delete(result.output);
        }
        else
        {
            // otherwise, handle error
            mpc_err_print(result.error);
            mpc_err_delete(result.error);
        }

        free(input);
    }

    // clean up parsers defined above
    mpc_cleanup(4, Number, Operator, Expr, Lispy);

    return 0;
}