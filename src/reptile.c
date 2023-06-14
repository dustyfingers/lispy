// reptile is the repl in the lispy ecosystem

#include <stdio.h>
#include <stdlib.h>

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
    lispy_greeting();

    // in a never ending loop...
    while (1)
    {

        // no matter the compilation platform, readline() will be defined!
        char *input = readline("lispy>");

        add_history(input);

        // echo input back to user (for now....)
        printf("No, you %s\n", input);
        free(input);
    }

    return 0;
}