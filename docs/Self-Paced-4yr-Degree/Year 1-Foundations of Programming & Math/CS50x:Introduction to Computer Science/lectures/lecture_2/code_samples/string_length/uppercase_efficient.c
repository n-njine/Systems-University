// Uppercase string using ctype library

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        // Prompt the user for string
        string s = get_string("Before: ");
        printf("After: ");
        for (int i = 0, n = strlen(s); i < n; i++)
        {
                printf("%c", toupper(s[i]));
        }
        printf("\n");

        // Exit successfully
        return 0;
}
