// Implements a phone book without structs

#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        // Array of strings of names and numbers
        string names[] = {"Yuliia", "David", "John"};
        string numbers[] = {"+1-617-495-1000", "+1-617-495-1000", "+1-949-468-2750"};

        // Prompt user for name and search for name
        string name = get_string("Name: ");
        for (int i = 0; i < 3; i++)
        {
                // Check if input string is equal to name
                if (strcmp(names[i], name) == 0)
                {
                        printf("Found: %s\n", numbers[i]);
                        return 0;
                }
        }
        printf("Not found\n");
        return 1;
}
