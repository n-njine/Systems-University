// Uppercases string using ctype librayr ( and an unnecessary condition)

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        // Prompt for string from the user
        string s = get_string("Before: ");
        printf("After: ");

        // Iterate through string replacing lowercase character with uppercase
        for (int i = 0, n = strlen(s); i < n; i++)
        {
                if (islower(s[i]))
                {
                        printf("%c", toupper(s[i]));
                }
                else
                {
                        printf("%c", s[i]);
                }
        }
        printf("\n");

        // Return 0
        return 0;
}


