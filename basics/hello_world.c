#include <stdio.h>

// struct declaration
typedef struct
{
    float x;
    float y;
} point2D;

// function declaration
int add_together(int x, int y)
{
    int result = x + y;
    return result;
}

int main(int argc, char **argv)
{
    int count = 100;
    double precise_count = 100.517553;

    // call user written function
    int added = add_together(count, 5);

    // use custom type
    point2D point;
    point.x = 5.0;
    point.y = -0.03;

    float length = sqrt(point.x * point.x + point.y * point.y);

    // conditionals
    if (count > 10 && count < 200)
    {
        puts("x is juuuuuuuust right.....");
    }
    else
    {
        puts("x is out of the proper range! oh no!");
    }

    // loops

    int i = 10;
    while (i > 0)
    {
        puts("while loop iteration...");
        i = i - 1;
    }

    for (int j = 0; j < 10; j++)
    {
        puts("for loop iteration...");
    }

    puts("Hello, world!");
    return 0;
}