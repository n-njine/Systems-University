// Determine the length of a string using a function 

#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        // Prompt for user's name and print length of name
        string name = get_string("Name: ");
        int length = strlen(name);
        printf("[%s] has: %i, characters.\n", name, length);

        // Return 0
        return 0;
}
