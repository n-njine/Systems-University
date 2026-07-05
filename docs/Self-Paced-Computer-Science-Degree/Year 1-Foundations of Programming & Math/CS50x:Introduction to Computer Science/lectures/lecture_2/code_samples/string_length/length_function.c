// Determines the length of a string using a function

#include <cs50.h>
#include <stdio.h>

// Function prototype
int string_length(string s);

int main(void)
{
        // Prompt for the user's name and print number of characters
        string name = get_string("Name: ");
        int length = string_length(name);
        printf("%s has: %i, characters\n", name, length);

        // Exit successfully
        return 0;

}

// get_string function
int string_length(string s)
{
        // Count number of characters up until '\0' aka (NULL)
        int n = 0;
        while(s[n] != '\0')
        {
                n++;
        }
        return n;
}
