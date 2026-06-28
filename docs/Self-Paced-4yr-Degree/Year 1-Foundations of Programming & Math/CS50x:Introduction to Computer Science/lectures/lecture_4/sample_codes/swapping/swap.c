// Fails to swap two integers

#include <stdio.h>

void swap(int a, int b);

int main(void)
{
        // Set x to 1 and y to 2
        int x = 1;
        int y = 2;

        // Print x is {value} and y is {value}
        printf("x is %i, y is %i\n", x, y);

        // Call swap
        swap(x, y);

        // Print values of x and y after swapping
        printf("x is %i, y is %i\n", x, y);

        // Exit successfully
        return 0;
}

// Swap two values
void swap(int a, int b)
{
        int tmp = a;
        a = b;
        b = tmp;
}
