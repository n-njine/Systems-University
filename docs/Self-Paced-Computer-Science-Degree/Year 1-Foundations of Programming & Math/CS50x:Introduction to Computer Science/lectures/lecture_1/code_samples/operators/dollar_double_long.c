// Long data type to store numbers

#include <cs50.h>
#include <stdio.h>

int main(void)
{
        long dollars = 1;

        // Continuously prompt user to double dollars
        while (true)
        {
                char c = get_char("Here's $%li. Doube it and give it to next person? ", dollars);
                if (c == 'Y' || c == 'y')
                {
                        dollars *= 2;
                }
                else
                {
                        break;
                }
        }
        printf("Here's $%li.\n", dollars);

        // Exit successfully
        return 0;
}
