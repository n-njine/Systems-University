// Capitalizes a string, checking length first

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        // Get a string
        string s = get_string("What's s: ");

        // Copy string's address
        string t = s;

        // Capitalize first letter in string checking length first
        if (strlen(t) > 0)
        {
                t[0] = toupper(t[0]);
        }

        // Print string twice
        printf("s: %s\n", s);
        printf("t: %s\n", t);

        // Eixt successfully
        return 0;
}
/* strlen is used to make sure "string t" exits. If it does not, nothing will be copied */
