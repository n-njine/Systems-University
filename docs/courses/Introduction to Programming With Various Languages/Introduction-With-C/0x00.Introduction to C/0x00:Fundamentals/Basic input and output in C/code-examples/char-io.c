/*
   C program to demonstrate character input and output using getchar() and putchar() function.
*/

#include <stdio.h>

const char *get_score(char grade);
int main(void)
{
    // Input character using getchar() and output using putchar
    char grade;
    printf("Enter student grade: ");
    grade = getchar();

    // Print output
    printf("The grade of student is [%c], and has thus [%s]\n", grade, get_score(grade));

    return 0;
}

// Student check function
const char *get_score(char grade)
{
    if (grade == 'A')
    {
       return "Passed"; 
    }
    else
    {
        return "Failed";
    }
}

