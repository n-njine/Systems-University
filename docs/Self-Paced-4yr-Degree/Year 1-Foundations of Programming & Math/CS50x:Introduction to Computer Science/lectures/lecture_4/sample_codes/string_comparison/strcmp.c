// Compares two strings uing strcmp

#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        // Get two strings
        char *s = get_string("What's s: ");
        char *t = get_string("What's t: ");

        // Compare strings
        if (strcmp(s, t) == 0)
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
