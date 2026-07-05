#include <ctype.h>      // For islower() and isupper() functions
#include <cs50.h>       // For get_string() function
#include <stdio.h>      // For getting I/O 
#include <string.h>     // For strlen() function

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// Function prototype
int calculate_result(string word);

int main(void)
{
        // Prompt the user for two words
        string string1 = get_string("Player 1: ");
        string string2 = get_string("Player 2: ");

        // Compute the score for each word
        int score1 = calculate_result(string1);
        int score2 = calculate_result(string2);

        // Print the winner
        if (score1 > score2)
        {
                printf("Player 1 wins\n");
        }
        else if (score1 < score2)
        {
                printf("Player 2 wins\n");
        }
        else
        {
                printf("Tie\n");
        }

        // Exit successflly
        return 0;
}

// Gets final score of word
int calculate_result(string word)
{
        // Keep track of score
        int score = 0;

        // Compute score for each character
        for (int i = 0, length = strlen(word); i < length; i++)
        {
                // Check for case then compute
                if (isupper(word[i]))
                {
                        // Get point score of word
                        score += POINTS[word[i] - 'A'];
                }
                else if(islower(word[i]))
                {
                        // Get point score of word
                        score += POINTS[word[i] - 'a'];
                }
        }

        return score;
}

