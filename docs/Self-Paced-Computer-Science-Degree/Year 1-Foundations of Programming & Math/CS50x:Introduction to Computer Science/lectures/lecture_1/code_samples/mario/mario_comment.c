// Helper function

#include <stdio.h>

// print_row function prototype
void print_row(int width);

// Execution starts here
int main(void)
{
        const int n = 3;

        // Print n rows
        for (int i = 0; i < n; i++)
        {
                print_row(n);
        }
}

// print_row function
void print_row(int width)
{
        // Print "###" blocks
        for (int i = 0; i < width; i++)
        {
                printf("#");
        }
        printf("\n");
}
