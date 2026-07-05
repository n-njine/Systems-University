// Using floats in division

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Prompt the user for x
        float x = get_float("What's x: ");

        // Prompt the user for y
        float y = get_float("What's y: ");

        // Divide x by y
        printf("x divide y is: %.50f\n", x / y);

        // Exit successfully
        return 0;
}
