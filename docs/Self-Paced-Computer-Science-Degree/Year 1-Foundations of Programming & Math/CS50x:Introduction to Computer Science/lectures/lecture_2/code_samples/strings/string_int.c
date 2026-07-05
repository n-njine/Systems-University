// Print strings ASCII codes, including a null.

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Declare and print a string's ASCII codes
        string s = "HI!";
        printf("ASCII: %i %i %i %i\n", s[0], s[1], s[2], s[3]);

        // Exit successfully
        return 0;

}
