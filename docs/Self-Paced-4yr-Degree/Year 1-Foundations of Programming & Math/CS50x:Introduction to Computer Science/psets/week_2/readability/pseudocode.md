# Problem: 
- Program to implement:
        - Calculate approximate grade level needed to comprehend some text.
        - Output:"Grade X" where "X" is grade level computed, rounded to the nearest integer.
        - If level is 16 or higher, print "Grade 16+" instead of exact index number.
        - If level is less than 1, program should output "Before Grade 1"

# Background
- Use Coleman-Liau index:
        index = 0.0588 * L -0.296 * S - 15.8

        where:
                L = average number of letters per 100 words
                S = average number of sentences per 100 words.

# Specification
- word: Any sequence of characters separated by a space to be a word
- sentence:
        - will contain at least one word
        - will not start or end with a space; and
        - will not have multiple spaces in a row

# Advice
> Write some code that you know will compile
```
#include <ctype.h>
#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{

}
```

> Write some pseudocode before writting more code
```text

1. Prompt the user for some text
2. Count the number of letters, words, and sentences in the text
3. Compute the Coleman-Liau index
4. Print the grade level
```
> Write pseudocode as comments in code
```
#include <ctype.h>
#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        // Prompt the user for some text

        // Count the number of letters, words, and sentences in the text

        // Compute the Coleman-Liau index

        // Print the grade level
}
```
> Convert the pseudocode to code
        1. Get input
        2. Analyse the input: Count letters in the text, consider letters to be uppercase or lowercase character, not punctuation, digits or other symbols
                - Create function `count_letters()` takes string of text and return number of letters.
        3.Write function to count the number of sentences. Consider any sequence of characters that ends with a `.` or a `!` or a `?` to be a sentence.
```
#include <ctype.h>
#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        // Prompt the user for some text
        string text = get_string("Sentence ");

        // Count the number of letter, words, and sentences in the text

        // Compute the Coleman-Liau index

        // Print the grade level
}
