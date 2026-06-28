// An array of strings

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Declare a string of words
        string words[2];

        // Assign values to string array
        words[0] = "HI!";
        words[1] = "BYE!";

        // Print as array of strings
        printf("Array of chars1: %c%c%c\n", words[0][0], words[0][1], words[0][2]);
        printf("Array of chars2: %c%c%c%c\n", words[1][0], words[1][1], words[1][2], words[1][3]);

        // Exit successfully
        return 0;

}
