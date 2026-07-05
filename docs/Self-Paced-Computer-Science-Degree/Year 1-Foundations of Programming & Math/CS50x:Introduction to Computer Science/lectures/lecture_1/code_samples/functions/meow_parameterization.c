// Abstration with parameterization

#include <stdio.h>

// meow prototype
void meow(int n);

int main(void)
{
        meow(3);

        return 0;
}

// Meow some number of times
void meow(int n)
{
        for (int i = 0; i < n; i++)
        {
                printf("meow\n");
        }
}
