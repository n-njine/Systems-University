// Compares two string's addresses

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Get two string
        char *s = get_string("What's s: ");
        char *t = get_string("What's t: ");

        // Compare strings' addresses
        if ( s == t)
        {
                printf("Same\n");
        }
        else
        {
                printf("Different\n");
        }

        // Exit successfully
        return 0;
}
