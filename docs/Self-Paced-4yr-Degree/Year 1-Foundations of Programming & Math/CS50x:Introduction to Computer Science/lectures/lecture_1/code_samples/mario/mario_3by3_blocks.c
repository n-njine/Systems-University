// Prints a 3-by-3 grid of blocks with nested loops

#include <stdio.h>

int main(void)
{
        for (int i = 0; i < 3; i++)
        {
                for (int j = 0; j < 3; j++)
                {
                        printf("#");
                }
                printf("\n");
        }

        return 0;
}
