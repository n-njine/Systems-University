// Array of strings

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Declare an array of strings
        string words[2];

        // Insert words into the array
        words[0] = "HI!";
        words[1] = "BYE!";

        // Print words of the array
        printf("String1: %s\n", words[0]);
        printf("String2: %s\n", words[1]);

        // Exit successfully
        return 0;

}
