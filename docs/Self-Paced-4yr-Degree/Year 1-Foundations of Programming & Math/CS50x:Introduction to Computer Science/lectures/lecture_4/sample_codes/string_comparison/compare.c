// Compares two integers

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Get two integers
        int i = get_int("What's i: ");
        int j = get_int("What's j: ");

        // Compare integers
        if ( i == j)
        {
                printf("same\n");
        }
        else
        {
                printf("Different\n");
        }

        // Exit successfully
        return 0;
}
