// Prints two string

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Get two strings
        char *s = get_string("What's s: ");
        char *t = get_string("What's t: ");

        // Print strings
        printf("s is : %s\n", s);
        printf("t is : %s\n", t);

        // Exit Successfully
        return 0;
}
