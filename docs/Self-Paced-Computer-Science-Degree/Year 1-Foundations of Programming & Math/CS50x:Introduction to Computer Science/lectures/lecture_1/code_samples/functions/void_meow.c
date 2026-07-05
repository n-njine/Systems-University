// Abstration with void functions

#include <stdio.h>

// meow function prototype
void meow(void);

int main(void)
{
        // Print meow n times
        for (int i = 0; i < 3; i++)
        {
                meow();
        }

        return 0;
}

// Meow once
void meow(void)
{
        printf("meow\n");
}
