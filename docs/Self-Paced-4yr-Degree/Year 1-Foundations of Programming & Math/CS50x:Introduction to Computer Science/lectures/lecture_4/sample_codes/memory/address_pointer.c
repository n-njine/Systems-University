// Stores and prints and integer's address

#include <stdio.h>

int main(void)
{
        // Store 50 in memory and store 50's address in a pointer
        int n = 50;
        int *p = &n;

        // Print 50's address
        printf("%p\n", p);

        // Exit Successfully
        return 0;
}
