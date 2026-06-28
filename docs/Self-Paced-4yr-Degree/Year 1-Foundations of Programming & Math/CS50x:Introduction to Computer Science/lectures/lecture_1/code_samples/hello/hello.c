// get_string and printf with %s

// Include necessary header files, cs50 and stdio.h
#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Get string from the user and greet them
        string answer = get_string("What's your name? ");
        printf("hello, %s\n", answer);

        // Exit successfully
        return 0;
}
