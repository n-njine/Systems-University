// Program to convert plaintext to cipher text
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
bool only_digits(string s);
char rotate(char c, int n);

int main(int argc, string argv[])
{
    // Check that commandline argument is one
    if (argc != 2)
    {
        printf("Error: Check comand-line arguments!!\n");
        printf("Usage: ./caesar key\n");

        return 1;
    }

    // Check that argument is a digit
    if ( only_digits(argv[1]) != true)
    {
        printf("Error: Command-line argument not a digit!\n");
        printf("Usage: ./caesar key\n");
    }

    // convert argument from string to int
    int key = atoi(argv[1]);

    // Prompt user for plaintext
    string plaintext = get_string("Plaintext: ");

    // Print cipher text
    printf("Ciphertext: ");
    // For each character in the plaintext:
    for (int i = 0, length = strlen(plaintext); i < length; i++)
    {
        // Rotate the character if it's a letter
        printf("%c", rotate(plaintext[i], key));
    }
    printf("\n");
        
    return 0;

}

// check for digits in string
bool only_digits(string s)
{
    // For every chararacter in s
    for (int i = 0, len = strlen(s); i < len; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

// Convert plaintext to ciphertext
char rotate(char c, int n)
{
    // Keep track of shift and position of alphabet and offset
    int shift = 0, position = 0, offset;

    // Check if c is char
    if (!isalpha(c))
    {
        return c;
    }
    else
    {
        // Rotate for lowercase
        if (islower(c))
        {
            offset = 'a';       // Lowercase alphabet starts
        }
        // Rotate for uppercase
        else if (isupper(c))
        {
            offset = 'A';       // Uppercase aphabet starts
        }
    }

    // Convert to 0-25 aphabet index
    position = c - offset;

    // Apply rotation
    shift = position + n;

    // Wrap around using modulo
    shift = shift % 26;

    // Fix negative wrap
    if (shift < 0)
    {
        shift += 26;
    }

    // Convert back to actual ASCII character
    return (char)(shift + offset);
}


