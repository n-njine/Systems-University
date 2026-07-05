// Prints string's ASCII codes. including NUL

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Declare string
        string s = "HI!";

        // Print ASCII representation including the NUL
        printf("int: %i %i %i %i\n", s[0], s[1], s[2], s[3]);

        // Exit successfully
        return 0;
}
