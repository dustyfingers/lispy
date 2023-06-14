// reptile is the repl in the lispy ecosystem

#include <stdio.h>

// declare a buffer that holds user input with a length of 2048
static char input[2048];

int main(int argc, char **argv)
{
    // print version and exit information
    puts("Lispy Version 0.1");
    puts("Press Ctrl+c to Exit\n");

    // in a never ending loop...
    while (1)
    {
        // output out prompt
        fputs("lispy>", stdout);

        // read a line of user input of max size 2048
        fgets(input, 2048, stdin);

        // echo input back to user (for now....)
        printf("No, you %s", input);
    }

    return 0;
}