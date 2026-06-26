// Program to Demonstrate input output using scanf() and printf()

#include <stdio.h>

int main(void)
{
    // Prompt for two intergers from the user and print their sim
    int num_1, num_2, sum;

    // Prompt user for 2 integers
    printf("Enter two integers: ");
    scanf("%d%d", &num_1, &num_2);

    // Compute sum of two integers
    sum = num_1 + num_2;

    // Print results to user
    printf("Sum of %d and %d is %d\n", num_1, num_2, sum);

    return 0;
}
