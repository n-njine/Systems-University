// Returns explicit value from main

#include <cs50.h>
#include <stdio.h>

int main(int argc, string argv[])
{
        // Check for commandline arguments
        if ( argc != 2)
        {
                printf("Missing command-line arguments\n");

                // Exit errorneously
                return 1;
        }
        printf("hello, %s\n", argv[1]);

        // Exit successfully
        return 0;
}
