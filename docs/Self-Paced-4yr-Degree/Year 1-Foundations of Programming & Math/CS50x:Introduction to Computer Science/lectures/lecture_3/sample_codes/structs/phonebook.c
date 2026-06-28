// Implements a phone book with structs

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Create a struct called person with name and number in it.
typedef struct
{
        string name;
        string number;
}person;

int main(void)
{
        // Create array of own struct
        person people[3];

        // Assign values to own struct
        people[0].name = "Yuliia";
        people[0].number = "+1-617-495-1000";

        people[1].name = "David";
        people[1].number = "+1-617-495-1000";

        people[2].name = "John";
        people[2].number = "+1-949-468-2750";

        // Prompt user for name
        string name = get_string("Name: ");

        // Search for name in own struct
        for (int i = 0; i < 3; i++)
        {
                if (strcmp(people[i].name, name) == 0)
                {
                        printf("Found: %s\n", people[i].number);
                        return 0;
                }
        }
        printf("Not found\n");
        return 1;
}
