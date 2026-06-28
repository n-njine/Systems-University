// Print 3x3 block using input from the user

#include <cs50.h>
#include <stdio.h>

// Fuction prototypes
int get_positive(void);
void print_row(int width);

// Start of execution
int main(void)
{
        const int n = get_positive();

        // Print 3x3 block
        for (int i = 0; i < n; i++)
        {
                print_row(n);
        }

        // Exit successfully
        return 0;
}

// Widht function
void print_row(int width)
{
        for (int i = 0; i < width; i++)
        {
                printf("#");
        }
        printf("\n");
}


// Get positive input from the user
int get_positive(void)
{
        int n;
        do
        {
                n = get_int("Number: ");
        }
        while (n < 1);

        return n;
}
