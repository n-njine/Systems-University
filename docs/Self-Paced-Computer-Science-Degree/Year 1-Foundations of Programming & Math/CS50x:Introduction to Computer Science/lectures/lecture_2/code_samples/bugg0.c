// Buggy example for printf

#include <stdio.h>

int main(void)
{
        for (int i = 0; i <= 3; i++)
        {
                // Printf debugging
                printf("i is %i\n", i);
                printf("#\n");
        }

        return 0;
}
