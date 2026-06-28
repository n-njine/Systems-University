// Ask for name and greet using name

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Ask for name and greet
        string name = get_string("What's your name? ");
        printf("Hello, %s!\n", name);

        // Exit successfully
        return 0;
}
