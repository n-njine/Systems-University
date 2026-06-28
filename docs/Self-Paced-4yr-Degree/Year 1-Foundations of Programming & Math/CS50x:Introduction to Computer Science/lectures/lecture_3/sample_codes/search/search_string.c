// Implements linear search for strings

#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        // An array of strings
        string strings[] = {"battleship", "boot", "cannon", "iron", "thimble", "topmark"};

        // Search for string
        string s = get_string("String: ");
        for (int i = 0; i < 6; i++)
        {
                // Check if i has string
                if (strcmp(strings[i], s) == 0)
                {
                        printf("Found\n");
                        return 0;
                }
        }
        printf("Not found\n");
        return 1;
}
