// Function with a return value

// Header files
#include <cs50.h>
#include <stdio.h>

// Function prototypes
int get_positive_int(void);
void meow(int n);

// Exection starts here
int main(void)
{
        int n = get_positive_int();
        meow(n);

        return 0;
}

// Get number of meows
int get_positive_int(void)
{
        int n;
        do
        {
                n = get_int("Number: ");
        }
        while (n < 1);

        return n;
}

// Meow some number of times
void meow(int n)
{
        for (int i = 0; i < n; i++)
        {
                printf("meow\n");
        }
}

