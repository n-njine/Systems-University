// Helper function for printing width

#include <stdio.h>

// Width prototype
void print_row(int width);

// Start of execution
int main(void)
{
        const int n = 3;

        // Print 3x3 block
        for (int i = 0; i < n; i++)
        {
                print_row(n);
        }

        // Exit successfully
        return 0;
}

// Width function 
void print_row(int width)
{
        for (int i = 0; i < width; i++)
        {
                printf("#");
        }
        printf("\n");
}
