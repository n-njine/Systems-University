// Division with ints, demonstrating truncation

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Prompt user for x
        int x = get_int("What's x: ");

        // Prompt user for y
        int y = get_int("What's y: ");

        // Print result of division
        printf("x divide y is %i\n", x / y);

        return 0;
}
