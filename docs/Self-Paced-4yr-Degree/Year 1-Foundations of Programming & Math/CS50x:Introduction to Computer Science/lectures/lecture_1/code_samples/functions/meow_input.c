// Meow with user input

#include <cs50.h>
#include <stdio.h>

// Meow from user input
void meow(int n);

int main(void)
{
        // Get user input and meow that number of times
        int n;
        do
        {
                n = get_int("Number: ");
        }
        while (n < 1);

        meow(n);

        // Exit successfully.
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
