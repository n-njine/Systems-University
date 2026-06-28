// Demonstrates memory errors via valgrind

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
        // Declare pointer x and allocate memory of size 3
        int *x = malloc(3 * sizeof(int));

        // Assign values to memory allocated
        x[1] = 72;
        x[2] = 73;
        x[3] = 33;

        // Free memory
        free(x);
}
