// Get name from the user and greet them

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Get name from the user
        string name = get_string("What's your name? ");

        // greet user
        printf("Hello, %s\n", name);

        // exit successfully
        return 0;
}
