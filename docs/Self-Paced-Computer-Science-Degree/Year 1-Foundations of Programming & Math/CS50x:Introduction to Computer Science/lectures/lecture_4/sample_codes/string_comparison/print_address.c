// Prints two strings' addresses

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Get two strings
        char *s = get_string("What's s: ");
        char *t = get_string("What's t: ");

        // Print strings' addresses
        printf("The address of s is: %p\n", s);
        printf("The address of t is: %p\n", t);

        // Exit successfully
        return 0;

}
