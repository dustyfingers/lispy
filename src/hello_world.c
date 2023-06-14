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

    puts("Hello, world!");
    return 0;
}