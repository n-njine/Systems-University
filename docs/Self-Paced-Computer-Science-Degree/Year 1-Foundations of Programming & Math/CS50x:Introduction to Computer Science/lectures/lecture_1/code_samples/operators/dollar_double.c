// Double dollar inputed

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        // Set number of dollars
        int dollars = 1;

        while (true)
        {
                char c = get_char("Here is $%i. Double it and give it to next person? ", dollars);
                if (c == 'y' || c == 'Y')
                {
                        dollars *= 2;
                }
                else
                {
                        break;
                }
        }
        printf("Here's $%i.\n", dollars);

        return 0;
}
