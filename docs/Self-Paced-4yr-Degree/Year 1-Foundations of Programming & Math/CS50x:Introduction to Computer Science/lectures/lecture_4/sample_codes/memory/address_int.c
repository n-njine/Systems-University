// Stores and prints an integer via its address

#include <stdio.h>

int main(void)
{
        // Store 50 in memory and store its address in a pointer
        int n = 50;
        int *p = &n;

        // Print n via its address
        printf("%i\n", *p);

        // Exit successfully
        return 0;
}
