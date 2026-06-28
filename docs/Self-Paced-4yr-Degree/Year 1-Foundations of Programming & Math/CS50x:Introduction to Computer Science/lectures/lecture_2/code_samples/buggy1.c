// Improved buggy with abstraction

#include <cs50.h>
#include <stdio.h>

// Function protopyes
void print_column(int height);

int main(void)
{
        // Get height from the user
        int h = get_int("Height: ");
        print_column(h);

        return 0;
}

void print_column(int height)
{
        for (int i = 0; i < height; i++)
        {
                printf("#\n");
        }
}
