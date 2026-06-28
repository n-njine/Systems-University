// Prints command-line arguements

#include <cs50.h>
#include <stdio.h>

int main(int argc, string argv[])
{
        printf("Printing commandline arguments!\n");

        // Print the number of commandline arguments
        for (int i = 0; i < argc; i++)
        {
                printf("%s\n", argv[i]);
        }

        // Exit successfully
        return 0;
}
