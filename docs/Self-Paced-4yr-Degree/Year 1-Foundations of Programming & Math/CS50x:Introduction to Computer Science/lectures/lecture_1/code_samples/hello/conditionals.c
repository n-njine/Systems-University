// Compare two integers

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Get two integers from users
        int x = get_int("What's x? ");
        int y = get_int("What's y? ");

        // Compare which is greater
        if (x > y)
        {
                printf("x is greater than y\n");
        }
        else if (x < y)
        {
                printf("x is less than y\n");
        }
        else
        {
                printf("x is equal to y\n");
        }

        // Exit successfully
        return 0;
}
