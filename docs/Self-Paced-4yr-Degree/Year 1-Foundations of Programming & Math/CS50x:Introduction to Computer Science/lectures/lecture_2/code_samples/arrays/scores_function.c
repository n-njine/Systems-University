// Averages thee numbers using an array, a constant, and a helper function

#include <cs50.h>
#include <stdio.h>

// constant
const int N = 3;

// Prototype
float average(int length, int array[]);

int main(void)
{
        // get scores
        int scores[N];
        for (int i = 0; i < N; i++)
        {
                scores[i] = get_int("Score: ");
        }

        // Print average
        printf("Average: %f\n", average(N, scores));

        // Exit successfully
        return 0;
}

// Get average function
float average(int length, int array[])
{
        // Calculate average
        int sum = 0;
        for (int i = 0; i < length; i++)
        {
                sum += array[i];
        }
        return sum / (float) length;
}
