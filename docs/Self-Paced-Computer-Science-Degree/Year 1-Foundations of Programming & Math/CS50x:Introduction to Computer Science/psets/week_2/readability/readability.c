// A program that calculates the reading level of some text

#include <ctype.h>
#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Function prototypes
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int compute_index(int letters, int words, int sentences);

int main (void)
{
        // Prompt the user for some text
        string text = get_string("Text : ");

        // Count the number of letters, words, and sentences in the text
        int letters = count_letters(text);
        int words = count_words(text);
        int sentences = count_sentences(text);

        // Compute the Coleman-Liau index
        int index = compute_index(letters, words, sentences);

        // Print the grade level
        if (index < 1)
        {
                printf("Before Grade 1\n");
        }
        else if (index >= 16)
        {
                printf("Grade 16+\n");
        }
        else
        {
                printf("Grade %i\n", index);
        }

        return 0;
}

// Count number of letters in text
int count_letters(string text)
{
        // Return the number of letters in text
        // Keep track of letters
        int letters = 0;

        // Count letters in text
        for (int i = 0, length = strlen(text); i < length; i++)
        {
                if ((isupper(text[i])) || (islower(text[i])) && text[i] != ' ')
                {
                        letters ++;
                }
        }

        return letters;
}

// Count number of words in text
int count_words(string text)
{
        // Return the number of words in text
        
        // Keep track of words
        int words = 0;

        // Count words in a string
        for (int i = 0, length = strlen(text); i < length; i++)
        {
                if (text[i] == ' ')
                {
                        words ++;
                }
        }
        words ++;

        // Return count of words
        return words;
}

// Count number of sentences in text
int count_sentences(string text)
{
        // Return the number of sentences in text

        // Keep track of sentences in string
        int sentences = 0;

        // Count sentences in text
        for (int i = 0, length = strlen(text); i < length; i++)
        {
                if (text[i] == '.' || text[i] == '!' || text[i] == '?')
                {
                        sentences ++;
                }
        }
        
        // Return count of sentences
        return sentences;
}

// Compute the Coleman-Liau index
int compute_index(int letters, int words, int sentences)
{
        // Calculate the values of L and S
        float L = (float)letters / (float)words * 100;

        float S = (float)sentences / (float)words * 100;

        // Compute Coleman-Liau index
        float index = (0.0588 * L) - (0.296 * S) - 15.0;

        // Return rounded index
        return (int)round(index);
}
