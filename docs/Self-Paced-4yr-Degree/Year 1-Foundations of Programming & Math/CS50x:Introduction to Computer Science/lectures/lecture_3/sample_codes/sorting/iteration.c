// Draws a pyramid using iteration

#include <cs50.h>
#include <stdio.h>

void draw(int n);

int main(void)
{
        // Get height of pyramid
        int height = get_int("Height: ");

        // Draw pyramid
        draw(height);

        // Exit successfully
        return 0;
}

// Draw height function
void draw(int n)
{
        // Draw pyramid of height n
        for (int i = 0; i < n; i++)
        {
                // Draw width of pyramid
                for (int j = 0; j < i + 1; j++)
                {
                        printf("#");
                }
                printf("\n");
        }
}
