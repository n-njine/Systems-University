// Uppercases a string

#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        string s = get_string("Before: ");
        printf("After: ");

        // Iterate through string and uppercase any lower case characters.
        for (int i = 0, n = strlen(s); i < n; i++)
        {
                if (s[i] >= 'a' && s[i] <= 'z')
                {
                        printf("%c", s[i] - 32);
                }
                else
                {
                        printf("%c", s[i]);
                }
        }
        printf("\n");

        // Exit successfully
        return 0;
}
